/*
  Copyright © 2016 Hasan Yavuz Özderya and Thomas Hofmann

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

#ifndef COMMANDPANEL_H
#define COMMANDPANEL_H

#include <QWidget>
#include <QSerialPort>
#include <QByteArray>
#include <QList>
#include <QMenu>
#include <QAction>
#include <QSettings>

#include "commandwidget.h"

namespace Ui {
class CommandPanel;
}

class CommandPanel : public QWidget
{
    Q_OBJECT

public:
    explicit CommandPanel(QSerialPort* port, QWidget *parent = 0);
    ~CommandPanel();

    QMenu* menu();
    /// Action for creating a new command.
    QAction* newCommandAction();
    /// Stores commands into a `QSettings`
    void saveSettings(QSettings* settings);
    /// Loads commands from a `QSettings`.
    void loadSettings(QSettings* settings);
    /// Number of commands
    unsigned numOfCommands();

signals:
    // emitted when user tries to send an empty command
    void focusRequested();

private:
    Ui::CommandPanel *ui;
    QSerialPort* serialPort;
    QMenu _menu;
    QAction _newCommandAction;
    QList<CommandWidget*> commands;

    unsigned command_name_counter;

private slots:
    CommandWidget* newCommand();
    void sendCommand(QByteArray command);
};

#endif // COMMANDPANEL_H
