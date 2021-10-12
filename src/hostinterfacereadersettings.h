#ifndef HOSTINTERFACEREADERSETTINGS_H
#define HOSTINTERFACEREADERSETTINGS_H

#include <QWidget>
#include "portcontrol.h"

namespace Ui {
class hostInterfaceReaderSettings;
}

class hostInterfaceReaderSettings : public QWidget
{
    Q_OBJECT

public:
    explicit hostInterfaceReaderSettings(PortControl* portControl, QWidget *parent = nullptr);
    ~hostInterfaceReaderSettings();

private slots:
    void on_horizontalSlider_hifSpeed_valueChanged(int value);
    void onPortToggled(bool open);

signals:
    void sampleRateSliderChanged(int sliderPos);

private:
    Ui::hostInterfaceReaderSettings *ui;
};

#endif // HOSTINTERFACEREADERSETTINGS_H
