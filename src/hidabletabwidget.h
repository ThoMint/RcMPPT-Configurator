/*
  Copyright © 2017 Hasan Yavuz Özderya and Thomas Hofmann

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

#ifndef HIDABLETABWIDGET_H
#define HIDABLETABWIDGET_H

#include <QTabWidget>
#include <QAction>

class HidableTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit HidableTabWidget(QWidget *parent = 0);
    QAction hideAction;

public slots:
    void showTabs();

private slots:
    void onHideAction(bool checked);
    void onTabBarClicked();
    void onTabBarDoubleClicked();

    void connectSignals();
};

#endif // HIDABLETABWIDGET_H
