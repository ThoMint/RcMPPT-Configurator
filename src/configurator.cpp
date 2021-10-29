#include <QtDebug>

#include "configurator.h"
#include "ui_configurator.h"

configurator::configurator(QSerialPort* port, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::configurator)
{
    ui->setupUi(this);
    serialPort = port;
}

configurator::~configurator()
{
    delete ui;
}

void configurator::on_horizontalSlider_Vout_valueChanged(int value)
{
    configState.targetOutputVoltage = value * 100.0;
    this->ui->doubleSpinBox_Vout->setValue(value/10.0);

    emit configChanged(configState);
}


void configurator::on_doubleSpinBox_Vout_valueChanged(double arg1)
{
    configState.targetOutputVoltage = arg1 * 1000.0;
    this->ui->horizontalSlider_Vout->setValue(arg1 * 10.0);

    //emit configChanged(configState);
}

void configurator::sampleRateSliderChanged(int sliderPos)
{
    switch (sliderPos)
    {
    case 1:
        configState.targetSampleRate = 10;
        break;
    case 2:
        configState.targetSampleRate = 100;
        break;
    case 3:
        configState.targetSampleRate = 1000;
        break;
    case 4:
        configState.targetSampleRate = 10000;
        break;
    }

    emit configChanged(configState);
}
