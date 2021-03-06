/*
  Copyright © 2020 Hasan Yavuz Özderya and Thomas Hofmann

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

#include <QApplication>
#include <QtGlobal>
#include <QIcon>
#include <iostream>
#include <QFile>

#include "mainwindow.h"
#include "tooltipfilter.h"
#include "version.h"
#include "DarkStyle.h"

MainWindow* pMainWindow = nullptr;

void messageHandler(QtMsgType type, const QMessageLogContext &context,
                    const QString &msg)
{
    QString logString;

    switch (type)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
        case QtInfoMsg:
            logString = "[Info] " + msg;
            break;
#endif
        case QtDebugMsg:
            logString = "[Debug] " + msg;
            break;
        case QtWarningMsg:
            logString = "[Warning] " + msg;
            break;
        case QtCriticalMsg:
            logString = "[Error] " + msg;
            break;
        case QtFatalMsg:
            logString = "[Fatal] " + msg;
            break;
    }

    std::cerr << logString.toStdString() << std::endl;

    if (pMainWindow != nullptr)
    {
        // TODO: don't call MainWindow::messageHandler if window is destroyed
        pMainWindow->messageHandler(type, logString, msg);
    }

    if (type == QtFatalMsg)
    {
        __builtin_trap();
    }
}

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
    QApplication::setApplicationName(PROGRAM_NAME);
    QApplication::setApplicationVersion(VERSION_STRING);
    QApplication::setStyle(new DarkStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());

//#ifdef Q_OS_WIN
    QIcon::setFallbackSearchPaths(QIcon::fallbackSearchPaths() << ":icons");
    QIcon::setFallbackSearchPaths(QIcon::fallbackSearchPaths() << ":winicons");
    QIcon::setThemeName("material");
//#endif

    //FramelessWindow framelessWindow;

    qInstallMessageHandler(messageHandler);
    MainWindow w;
    pMainWindow = &w;

    ToolTipFilter ttf;
    a.installEventFilter(&ttf);

    // log application information
    qDebug() << "RcMPPT-Configurator" << VERSION_STRING;
    qDebug() << "Revision" << VERSION_REVISION;

    //framelessWindow.setContent(pMainWindow);

    w.show();

    return a.exec();
}
