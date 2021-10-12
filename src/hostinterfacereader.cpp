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

    _serialDevice = device;

    paused = false;

    _numChannels = 1;
    updateNumChannels();
    emit numOfChannelsChanged(_numChannels);
}

void hostInterfaceReader::configChanged(RcMPPTConfiguration configState)
{
    hostInterfaceQueueDeviceCMDExpl(OPSTATUS_OK, OPCODE_REPORT_AVERAGE_VALUE, OPTYPE_OUTPUT_VOLTAGE, configState.outputVoltage);
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

unsigned hostInterfaceReader::readData()
{
    unsigned int numBytesRead = 0;

    // discard only once when we just started reading
    if (firstReadAfterEnable)
    {
        firstReadAfterEnable = false;
        numBytesRead += _serialDevice->readAll().size();
        return numBytesRead;
    }

    // discard data if paused
    if (paused)
    {
        return numBytesRead;
    }

    //qDebug() << _serialDevice->bytesAvailable();

    numBytesRead += hostInterfaceProcessCommand();

    return numBytesRead;
}

void hostInterfaceReader::hostInterfaceExecuteActualCommand()
{
    const SamplePack* samples;

    switch (ActualHostCMD.Opcode)
    {
    case OPCODE_REPORT_LIVE_VALUE:
        switch (ActualHostCMD.Type) {
        case OPTYPE_OUTPUT_VOLTAGE:
            samples = new SamplePack(1, _numChannels);
            for (unsigned ci = 0; ci < samples->numChannels(); ci++)
            {
                //TODO: Parse actual Data
                samples->data(ci)[0] = static_cast<double>(ActualHostCMD.Value.Int32);
            }
            feedOut(*samples);
            delete samples;
            break;
        default:
            break;
        }
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
    /*
    cmd.Value.Byte[0] = Int32 & 0xFF;
    cmd.Value.Byte[1] = (Int32>>8) & 0xFF;
    cmd.Value.Byte[2] = (Int32>>16) & 0xFF;
    cmd.Value.Byte[3] = (Int32>>24) & 0xFF;
    */
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
    uint32_t numBytesRead = 0;
    uint8_t USBDeviceCMD[8];

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

        if(!_serialDevice->isOpen())
        {
            qDebug() << "Error sending Data, device not open";
        }

        _serialDevice->write((const char *) USBDeviceCMD, 8);
        _serialDevice->flush();
        _serialDevice->waitForBytesWritten(100);
    }

    if((_serialDevice->bytesAvailable() > 0) && ((_serialDevice->bytesAvailable() % 8)==0))
    {
        USBHostCMD = _serialDevice->readAll();
        if(USBHostCMD.size()>0)
        {
            numBytesRead += USBHostCMD.size();
            for(int i=0; i<(USBHostCMD.size()/8); i++)
            {
                uint8_t checksum = 0;
                for (i = 0; i < 7; i++)
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
        }
    }
    else if((_serialDevice->bytesAvailable() > 0) && ((_serialDevice->bytesAvailable() % 8)!=0))
    {
        numBytesRead += _serialDevice->readAll().size();
        qDebug() << "Host Interface out of sync, flushed " << numBytesRead << "Bytes from Buffer!\n";
    }
    return numBytesRead;
}
