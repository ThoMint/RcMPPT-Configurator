#include "hostinterfacereadersettings.h"
#include "ui_hostinterfacereadersettings.h"

hostInterfaceReaderSettings::hostInterfaceReaderSettings(PortControl* portControl, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hostInterfaceReaderSettings)
{
    ui->setupUi(this);
    connect(portControl, &PortControl::portToggled, this, &hostInterfaceReaderSettings::onPortToggled);
}

hostInterfaceReaderSettings::~hostInterfaceReaderSettings()
{
    delete ui;
}

void hostInterfaceReaderSettings::onPortToggled(bool open)
{
    this->ui->progressBar_hifConEstablished->setValue(open);
}


void hostInterfaceReaderSettings::on_horizontalSlider_hifSpeed_valueChanged(int value)
{
    emit sampleRateSliderChanged(value);
}

