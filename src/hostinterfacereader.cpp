/*
  Copyright © 2021 Hasan Yavuz Özderya and Thomas Hofmann

  This file is part of RcMPPT-Configurator.

  RcMPPT-Configurator is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  RcMPPT-Configurator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with RcMPPT-Configurator.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtDebug>
#include "hostinterfacereader.h"

/// If set to this value number of channels is determined from input
#define NUMOFCHANNELS_AUTO   (0)

hostInterfaceReader::hostInterfaceReader(QSerialPort* device, PortControl* portControl, configurator* configurator, QObject* parent) :
    AbstractReader(device, parent),
    _settingsWidget(portControl)
{
    connect(configurator, &configurator::configChanged, this, &hostInterfaceReader::configChanged);
    connect(&_settingsWidget, &hostInterfaceReaderSettings::sampleRateSliderChanged, configurator, &configurator::sampleRateSliderChanged);

    _serialDevice = device;
    //_serialDevice->setReadBufferSize(8);

    //my_serial = new serial::Serial("COM3", 115200, serial::Timeout::simpleTimeout(1000));

    //qDebug() << my_serial->isOpen();

    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(readData()));
    //timer->start(0);

    paused = false;

    currenSampleIndex = 0;

    /*
    currentSampleState.channelNames->append("Vout");
    currentSampleState.channelSamples->append(0);

    currentSampleState.channelNames->append("Vin");
    currentSampleState.channelSamples->append(0);

    currentSampleState.channelNames->append("Iout");
    currentSampleState.channelSamples->append(0);
    */

    for(int i = 0; i< PLOT_MAX_NUM_CHANNELS; i++)
    {
        currentSampleState.channelSamples[i] = 0;
        currentSampleState.channelNames[i] = "Ch";
    }

    _numChannels = PLOT_MAX_NUM_CHANNELS;
    updateNumChannels();
    emit numOfChannelsChanged(_numChannels);
}

void hostInterfaceReader::configChanged(RcMPPTConfiguration configState)
{
    hostInterfaceQueueDeviceCMDExpl(OPSTATUS_OK, OPCODE_SET_SETTING, OPTYPE_TARGET_OUTPUT_VOLTAGE, configState.targetOutputVoltage);
    hostInterfaceQueueDeviceCMDExpl(OPSTATUS_OK, OPCODE_SET_SETTING, OPTYPE_TARGET_OUTPUT_CURRENT, configState.targetOutputCurrent);
    hostInterfaceQueueDeviceCMDExpl(OPSTATUS_OK, OPCODE_SET_SETTING, OPTYPE_TARGET_SAMPLE_RATE, configState.targetSampleRate);
}

QWidget* hostInterfaceReader::settingsWidget()
{
    return &_settingsWidget;
}

unsigned hostInterfaceReader::numChannels() const
{
    return _numChannels;
}

void hostInterfaceReader::enable(bool enabled)
{
    if (enabled)
    {
        firstReadAfterEnable = true;
    }

    AbstractReader::enable(enabled);
}

unsigned int hostInterfaceReader::readData()
{
    unsigned int numBytesRead = 0;

    //if(!my_serial->isOpen())
    if(!_serialDevice->isOpen())
    {
        //qDebug() << "Error processing Data, Port not open";
        return numBytesRead;
    }

    // discard only once when we just started reading
    if (firstReadAfterEnable)
    {
        firstReadAfterEnable = false;
        //numBytesRead += my_serial->read(my_serial->available()).size();
        numBytesRead += _serialDevice->readAll().size();
        return numBytesRead;
    }

    // discard data if paused
    if (paused)
    {
        return numBytesRead;
    }

    //qDebug() << my_serial->available();
    qDebug() << _serialDevice->bytesAvailable();

    numBytesRead += hostInterfaceProcessCommand();

    return numBytesRead;
}

void hostInterfaceReader::hostInterfaceExecuteActualCommand()
{
    const SamplePack* samples;

    switch (ActualHostCMD.Opcode)
    {
    case OPCODE_REPORT_LIVE_VALUE:
        switch (ActualHostCMD.Type)
        {
        case OPTYPE_OUTPUT_VOLTAGE:
            currentSampleState.channelSamples[0] = (ActualHostCMD.Value.Int32);
            currentSampleState.channelNames[0] = "Output Voltage";
            break;
        case OPTYPE_INPUT_VOLTAGE:
            currentSampleState.channelSamples[1] = (ActualHostCMD.Value.Int32);
            currentSampleState.channelNames[1] = "Input Voltage";
            break;
        }
        break;
    case OPCODE_FINISHED_SAMPLE:
        samples = new SamplePack(1, PLOT_MAX_NUM_CHANNELS);

        for(int i = 0; i < PLOT_MAX_NUM_CHANNELS; i++)
        {
            samples->data(i)[0] = currentSampleState.channelSamples[i];
        }

        feedOut(*samples);
        delete samples;
        break;
    default:
        break;
    }
}

void hostInterfaceReader::hostInterfaceQueueDeviceCMDExpl(uint8_t Status, uint8_t Opcode, uint8_t Type, uint32_t Int32)
{
    DeviceCommand cmd;
    cmd.Status = Status;
    cmd.Opcode = Opcode;
    cmd.Type = Type;
    cmd.Value.Int32 = Int32;
    hostInterfaceQueueDeviceCMD(cmd);
}

void hostInterfaceReader::hostInterfaceQueueDeviceCMD(DeviceCommand cmd)
{
    deviceCMDsToProcess = 1;
    DeviceCMDQueue[deviceCMDPopulatePos] = cmd;
    deviceCMDPopulatePos++;
    if (deviceCMDPopulatePos >= DEVICE_CMD_QUEUE_SIZE)
    {
        deviceCMDPopulatePos = 0;
    }
}

unsigned int hostInterfaceReader::hostInterfaceProcessCommand()
{
    uint8_t USBDeviceCMD[8] = {0};

    while (deviceCMDsToProcess)
    {
        int index = deviceCMDProcessPos;
        ActualDeviceCMD = DeviceCMDQueue[index];
        deviceCMDProcessPos++;
        if (deviceCMDProcessPos >= DEVICE_CMD_QUEUE_SIZE)
        {
            deviceCMDProcessPos = 0;
        }

        if (deviceCMDProcessPos == deviceCMDPopulatePos)
        {
            deviceCMDsToProcess = 0;
        }

        uint8_t Checksum = ActualDeviceCMD.Status + ActualDeviceCMD.Opcode + ActualDeviceCMD.Type + ActualDeviceCMD.Value.Byte[0] + ActualDeviceCMD.Value.Byte[1] + ActualDeviceCMD.Value.Byte[2] + ActualDeviceCMD.Value.Byte[3];

        USBDeviceCMD[0] = ActualDeviceCMD.Status;
        USBDeviceCMD[1] = ActualDeviceCMD.Opcode;
        USBDeviceCMD[2] = ActualDeviceCMD.Type;
        USBDeviceCMD[3] = ActualDeviceCMD.Value.Byte[0];
        USBDeviceCMD[4] = ActualDeviceCMD.Value.Byte[1];
        USBDeviceCMD[5] = ActualDeviceCMD.Value.Byte[2];
        USBDeviceCMD[6] = ActualDeviceCMD.Value.Byte[3];
        USBDeviceCMD[7] = Checksum;

        _serialDevice->write((const char *) USBDeviceCMD, 8);
        _serialDevice->flush();
        //my_serial->write(USBDeviceCMD, 8);
        //my_serial->flushOutput();
        if(_serialDevice->waitForBytesWritten(1000))
        {
            //qDebug() << "Sent data";
        }
        else
        {
            qDebug() << "Sent data failed, time out waiting for buffer";
        }
    }

    int numBytesRead = 0;
    //uint32_t numBytesAvailable = my_serial->available();

    //if((numBytesAvailable > 0) && ((numBytesAvailable % 8)==0))
    if((_serialDevice->bytesAvailable() > 0) && ((_serialDevice->bytesAvailable() % 8)==0) && (_serialDevice->isReadable()))
    {
        QByteArray USBHostCMD;
        USBHostCMD = _serialDevice->readAll();
        //int readBytes = my_serial->read((uint8_t*) USBHostCMD.data(), numBytesAvailable);
        int readBytes = USBHostCMD.size();
        //_serialDevice->clear(QSerialPort::Input);
        if(readBytes>0)
        {
            numBytesRead += readBytes;
            for(int j=0; j<(readBytes/8); j++)
            {
                uint8_t checksum = 0;
                for (int i = 0; i < 7; i++)
                    checksum += (uint8_t) USBHostCMD[i];

                if(checksum == ((uint8_t) USBHostCMD[7]))
                {
                    ActualHostCMD.Status = (uint8_t) USBHostCMD[0];
                    ActualHostCMD.Opcode = (uint8_t) USBHostCMD[1];
                    ActualHostCMD.Type = (uint8_t) USBHostCMD[2];
                    ActualHostCMD.Value.Byte[0] = (uint8_t) USBHostCMD[3];
                    ActualHostCMD.Value.Byte[1] = (uint8_t) USBHostCMD[4];
                    ActualHostCMD.Value.Byte[2] = (uint8_t) USBHostCMD[5];
                    ActualHostCMD.Value.Byte[3] = (uint8_t) USBHostCMD[6];
                    //ActualHostCMD.Value.Int32 = (uint32_t) (((uint8_t) USBHostCMD[3]) | (((uint8_t) USBHostCMD[4])<<8) | (((uint8_t) USBHostCMD[5])<<16) | (((uint8_t) USBHostCMD[6])<<24));
                    hostInterfaceExecuteActualCommand();
                }
                else
                {
                    qDebug() << "Checksum Error";
                }

                USBHostCMD.remove(0,8);
            }
        }
        else
        {
            qDebug() << "Error reading Data, clearing all input buffers";
            _serialDevice->clear(QSerialPort::Direction::Input);
            //my_serial->flushInput();
        }
    }
    else if((_serialDevice->bytesAvailable() > 0) && ((_serialDevice->bytesAvailable() % 8)!=0) && (_serialDevice->isReadable()))
    //else if((numBytesAvailable > 0) && ((numBytesAvailable % 8)!=0))
    {
        numBytesRead += _serialDevice->readAll().size();
        //numBytesRead += my_serial->read(numBytesAvailable).size();
        //my_serial->flushInput();
        qDebug() << "Host Interface out of sync, flushed " << numBytesRead << "Bytes from Buffer!\n";
    }

    return numBytesRead;
}
