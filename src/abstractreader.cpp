/*
  Copyright © 2019 Hasan Yavuz Özderya and Thomas Hofmann

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

#include "abstractreader.h"

AbstractReader::AbstractReader(QIODevice* device, QObject* parent) :
    QObject(parent)
{
    _device = device;
    timer = new QTimer(this);
    bytesRead = 0;
}

void AbstractReader::pause(bool enabled)
{
    paused = enabled;
}

void AbstractReader::enable(bool enabled)
{
    if (enabled)
    {
        //QObject::connect(_device, &QIODevice::readyRead, this, &AbstractReader::onDataReady);

        connect(timer, &QTimer::timeout, this, &AbstractReader::onDataReady);
        timer->start(0);
    }
    else
    {
        //QObject::disconnect(_device, 0, this, 0);
        timer->stop();
        disconnectSinks();
    }
}

void AbstractReader::onDataReady()
{
    bytesRead += readData();
}

unsigned AbstractReader::getBytesRead()
{
    unsigned r = bytesRead;
    bytesRead = 0;
    return r;
}
