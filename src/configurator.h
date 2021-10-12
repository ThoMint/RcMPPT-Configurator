#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QWidget>
#include <QSerialPort>
#include <QByteArray>
#include <QList>
#include <QMenu>
#include <QAction>
#include <QSettings>

namespace Ui {
class configurator;
}

typedef struct RcMPPTConfigurationStruct
{
    uint32_t outputVoltage = 0;
}RcMPPTConfiguration;

class configurator : public QWidget
{
    Q_OBJECT

public:
    explicit configurator(QSerialPort* port, QWidget *parent = nullptr);
    ~configurator();

private slots:
    void on_horizontalSlider_Vout_sliderMoved(int position);

    void on_doubleSpinBox_Vout_valueChanged(double arg1);

    void on_pushButton_toggled(bool checked);

signals:
    void configChanged(RcMPPTConfiguration configState);

private:
    Ui::configurator *ui;
    QSerialPort* serialPort;
    RcMPPTConfiguration configState;
};

#endif // CONFIGURATOR_H
