/*
  Copyright © 2018 Hasan Yavuz Özderya and Thomas Hofmann

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

#include <QtGlobal>
#include "sink.h"

void Sink::connectFollower(Sink* sink)
{
    Q_ASSERT(!followers.contains(sink));

    followers.append(sink);
    sink->setNumChannels(_numChannels, _hasX);
}

void Sink::disconnectFollower(Sink* sink)
{
    Q_ASSERT(followers.contains(sink));

    followers.removeOne(sink);
}

void Sink::feedIn(const SamplePack& data)
{
    for (auto sink : followers)
    {
        sink->feedIn(data);
    }
}

void Sink::setNumChannels(unsigned nc, bool x)
{
    _numChannels = nc;
    _hasX = x;
    for (auto sink : followers)
    {
        sink->setNumChannels(nc, x);
    }
}

void Sink::setSource(Source* s)
{
    Q_ASSERT((source == nullptr) != (s == nullptr));
    source = s;
}

const Source* Sink::connectedSource() const
{
    return source;
}

Source* Sink::connectedSource()
{
    return const_cast<Source*>(static_cast<const Sink&>(*this).connectedSource());
}
