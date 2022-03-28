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

#ifndef HOSTINTERFACEREADER_H
#define HOSTINTERFACEREADER_H

#include <QSettings>
#include <QString>
#include <QTimer>
#include <QList>

#include "samplepack.h"
#include "abstractreader.h"
#include "configurator.h"
#include "hostinterfacereadersettings.h"
#include "hostInterfaceDefines.h"
#include "serial/include/serial/serial.h"

typedef struct sampleState
{
    QString channelNames[PLOT_MAX_NUM_CHANNELS];
    int channelSamples[PLOT_MAX_NUM_CHANNELS];
} deviceSampleState;

class hostInterfaceReader : public AbstractReader
{
    Q_OBJECT

public:
    explicit hostInterfaceReader(QSerialPort* device, PortControl* portControl, configurator* configurator, QObject *parent = 0);
    QWidget* settingsWidget() override;
    unsigned numChannels() const override;
    void enable(bool enabled) override;
    /// Stores settings into a `QSettings`
    void saveSettings(QSettings* settings);
    /// Loads settings from a `QSettings`.
    void loadSettings(QSettings* settings);

private:
    hostInterfaceReaderSettings _settingsWidget;
    unsigned _numChannels;

    bool firstReadAfterEnable = false;

    HostCommand ActualHostCMD;
    DeviceCommand ActualDeviceCMD;

    QSerialPort* _serialDevice;
    serial::Serial* my_serial;

    DeviceCommand DeviceCMDQueue[DEVICE_CMD_QUEUE_SIZE];
    uint8_t deviceCMDsToProcess = 0;
    int deviceCMDProcessPos = 0, deviceCMDPopulatePos = 0;

    deviceSampleState currentSampleState;
    int currenSampleIndex;

private slots:
    unsigned int readData() override;
    void hostInterfaceExecuteActualCommand();
    unsigned int hostInterfaceProcessCommand();
    void hostInterfaceQueueDeviceCMD(DeviceCommand cmd);
    void hostInterfaceQueueDeviceCMDExpl(uint8_t Status, uint8_t Opcode, uint8_t Type, uint32_t Int32);
    void configChanged(RcMPPTConfiguration configState);

signals:
    void cellVoltageUpdate(int cellIndex, int cellVoltage);
/*
    SamplePack* parseLine(const QString& line) const;
*/
};

#endif // HOSTINTERFACEREADER_H
