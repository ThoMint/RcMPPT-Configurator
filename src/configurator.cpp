#include <QtDebug>

#include "configurator.h"
#include "ui_configurator.h"

configurator::configurator(QSerialPort* port, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::configurator)
{
    ui->setupUi(this);
    serialPort = port;
    configState.outputVoltage = 0;
}

configurator::~configurator()
{
    delete ui;
}

void configurator::on_horizontalSlider_Vout_sliderMoved(int position)
{
    this->ui->doubleSpinBox_Vout->setValue(position/10);
    configState.outputVoltage = position * 100;
    emit configChanged(configState);
}


void configurator::on_doubleSpinBox_Vout_valueChanged(double arg1)
{
    this->ui->horizontalSlider_Vout->setValue(arg1 * 10);
}


void configurator::on_pushButton_toggled(bool checked)
{
    qDebug() << checked;
}

