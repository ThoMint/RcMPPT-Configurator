#include <QtDebug>

#include "configurator.h"
#include "ui_configurator.h"

configurator::configurator(QSerialPort* port, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::configurator)
{
    ui->setupUi(this);
    serialPort = port;

    for(int i=0; i < 12; i++)
    {
        QListWidgetItem* item = new QListWidgetItem(QString(""), ui->listWidget);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        items.append(item);
    }
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

void configurator::on_horizontalSlider_Iout_valueChanged(int value)
{
    configState.targetOutputCurrent = value *100.0;
    this->ui->doubleSpinBox_Iout->setValue(value/10.0);

    emit configChanged(configState);
}


void configurator::on_doubleSpinBox_Vout_valueChanged(double arg1)
{
    configState.targetOutputVoltage = arg1 * 1000.0;
    this->ui->horizontalSlider_Vout->setValue(arg1 * 10.0);

    //emit configChanged(configState);
}

void configurator::on_doubleSpinBox_Iout_valueChanged(double arg1)
{
    configState.targetOutputCurrent = arg1 * 1000.0;
    this->ui->horizontalSlider_Iout->setValue(arg1 * 10.0);
}

void configurator::cellVoltageUpdate(int cellIndex, int cellVoltage)
{
    qDebug() << "Cell" << cellIndex << ": " << cellVoltage;
    items.at(cellIndex)->setText(QLatin1String("Cell-") + QString::number(cellIndex) + QLatin1String(": ") + QString::number(cellVoltage) + QLatin1String(" mV"));
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
