#
# Copyright © 2019 Hasan Yavuz Özderya and Thomas Hofmann
#
# This file is part of RcMPPT-Configurator.
#
# RcMPPT-Configurator is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# RcMPPT-Configurator is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with RcMPPT-Configurator.  If not, see <http://www.gnu.org/licenses/>.
#

#-------------------------------------------------
#
# Project created by QtCreator 2015-03-04T08:20:06
#
#-------------------------------------------------

QT       += core gui serialport network svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RcMPPT-Configurator
TEMPLATE = app

CONFIG += qwt
# LIBS += -lqwt # enable this line if qwt pri files aren't installed

DEFINES += PROGRAM_NAME="\\\"RcMPPT-Configurator\\\""

DEFINES += VERSION_MAJOR=10 VERSION_MINOR=0 VERSION_PATCH=0 VERSION_STRING=\\\"10.0.0\\\" VERSION_REVISION=\\\"10.0.0\\\"

SOURCES += \
    src/DarkStyle.cpp \
    src/configurator.cpp \
    src/hostinterfacereader.cpp \
    src/hostinterfacereadersettings.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/portcontrol.cpp \
    src/plot.cpp \
    src/serial/src/impl/unix.cc \
    src/serial/src/impl/win.cc \
    src/serial/src/serial.cc \
    src/zoomer.cpp \
    src/scrollzoomer.cpp \
    src/scrollbar.cpp \
    src/hidabletabwidget.cpp \
    src/scalepicker.cpp \
    src/scalezoomer.cpp \
    src/portlist.cpp \
    src/snapshot.cpp \
    src/snapshotview.cpp \
    src/snapshotmanager.cpp \
    src/plotsnapshotoverlay.cpp \
    src/commandpanel.cpp \
    src/commandwidget.cpp \
    src/commandedit.cpp \
    src/dataformatpanel.cpp \
    src/plotcontrolpanel.cpp \
    src/recordpanel.cpp \
    src/datarecorder.cpp \
    src/tooltipfilter.cpp \
    src/sneakylineedit.cpp \
    src/stream.cpp \
    src/streamchannel.cpp \
    src/channelinfomodel.cpp \
    src/ringbuffer.cpp \
    src/indexbuffer.cpp \
    src/linindexbuffer.cpp \
    src/readonlybuffer.cpp \
    src/framebufferseries.cpp \
    src/abstractreader.cpp \
    src/demoreader.cpp \
    src/demoreadersettings.cpp \
    src/plotmanager.cpp \
    src/plotmenu.cpp \
    src/barplot.cpp \
    src/barchart.cpp \
    src/barscaledraw.cpp \
    src/updatechecker.cpp \
    src/versionnumber.cpp \
    src/updatecheckdialog.cpp \
    src/samplepack.cpp \
    src/source.cpp \
    src/sink.cpp \
    src/samplecounter.cpp \
    src/ledwidget.cpp \
    src/datatextview.cpp \
    src/bpslabel.cpp

HEADERS += \
    src/DarkStyle.h \
    src/configurator.h \
    src/hostInterfaceDefines.h \
    src/hostInterfaceVariables.h \
    src/hostinterfacereader.h \
    src/hostinterfacereadersettings.h \
    src/mainwindow.h \
    src/serial/include/serial/impl/unix.h \
    src/serial/include/serial/impl/win.h \
    src/serial/include/serial/serial.h \
    src/serial/include/serial/v8stdint.h \
    src/utils.h \
    src/portcontrol.h \
    src/byteswap.h \
    src/plot.h \
    src/hidabletabwidget.h \
    src/framebuffer.h \
    src/scalepicker.h \
    src/scalezoomer.h \
    src/portlist.h \
    src/snapshotview.h \
    src/snapshotmanager.h \
    src/snapshot.h \
    src/plotsnapshotoverlay.h \
    src/commandpanel.h \
    src/commandwidget.h \
    src/commandedit.h \
    src/dataformatpanel.h \
    src/tooltipfilter.h \
    src/sneakylineedit.h \
    src/framebufferseries.h \
    src/plotcontrolpanel.h \
    src/abstractreader.h \
    src/demoreader.h \
    src/plotmanager.h \
    src/setting_defines.h \
    src/recordpanel.h \
    src/updatechecker.h \
    src/updatecheckdialog.h \
    src/demoreadersettings.h \
    src/datatextview.h \
    src/bpslabel.h \
    src/barchart.h \
    src/barplot.h \
    src/barscaledraw.h \
    src/channelinfomodel.h \
    src/datarecorder.h \
    src/defines.h \
    src/indexbuffer.h \
    src/ledwidget.h \
    src/linindexbuffer.h \
    src/plotmenu.h \
    src/readonlybuffer.h \
    src/ringbuffer.h \
    src/samplecounter.h \
    src/samplepack.h \
    src/scrollbar.h \
    src/scrollzoomer.h \
    src/sink.h \
    src/source.h \
    src/streamchannel.h \
    src/stream.h \
    src/version.h \
    src/versionnumber.h \
    src/zoomer.h

FORMS += \
    src/configurator.ui \
    src/hostinterfacereadersettings.ui \
    src/mainwindow.ui \
    src/about_dialog.ui \
    src/portcontrol.ui \
    src/snapshotview.ui \
    src/commandpanel.ui \
    src/commandwidget.ui \
    src/dataformatpanel.ui \
    src/plotcontrolpanel.ui \
    src/recordpanel.ui \
    src/updatecheckdialog.ui \
    src/demoreadersettings.ui \
    src/datatextview.ui

INCLUDEPATH += qmake/ src/

CONFIG += c++11

RC_FILE = misc/windows_icon.rc

RESOURCES += misc/icons.qrc
RESOURCES += darkstyle.qrc



win32 {
    RESOURCES += misc/winicons.qrc
}
