#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QWidget>
#include <QSerialPort>
#include <QByteArray>
#include <QList>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QListWidgetItem>

#include "hostinterfacereadersettings.h"

namespace Ui {
class configurator;
}

typedef struct RcMPPTConfigurationStruct
{
    uint32_t targetOutputVoltage = 0;
    uint32_t targetOutputCurrent = 0;
    uint32_t targetSampleRate = 100;
}RcMPPTConfiguration;

class configurator : public QWidget
{
    Q_OBJECT

public:
    explicit configurator(QSerialPort* port, QWidget *parent = nullptr);
    ~configurator();

signals:
    void configChanged(RcMPPTConfiguration configState);

private slots:
    void on_horizontalSlider_Vout_valueChanged(int value);

    void on_doubleSpinBox_Vout_valueChanged(double arg1);

    void on_horizontalSlider_Iout_valueChanged(int value);

    void on_doubleSpinBox_Iout_valueChanged(double arg1);

public slots:
    void sampleRateSliderChanged(int sliderPos);
    void cellVoltageUpdate(int cellIndex, int cellVoltage);

private:
    Ui::configurator *ui;
    QSerialPort* serialPort;
    RcMPPTConfiguration configState;
    hostInterfaceReaderSettings* _deviceSettings;
    QList<QListWidgetItem*> items;

};

#endif // CONFIGURATOR_H
