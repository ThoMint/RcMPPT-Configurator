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

#ifndef LININDEXBUFFER_H
#define LININDEXBUFFER_H

#include "framebuffer.h"

/// A dynamic frame buffer that start and end values can be set and
/// intermediate values are calculated linearly.
///
/// @note This buffer isn't for storing data.
class LinIndexBuffer : public XFrameBuffer
{
public:
    LinIndexBuffer(unsigned n, Range lim);
    LinIndexBuffer(unsigned n, double min, double max) :
        LinIndexBuffer(n, {min, max}) {};

    unsigned size() const override;
    double sample(unsigned i) const override;
    Range limits() const override;
    void resize(unsigned n) override;
    int findIndex(double value) const override;

    /// Sets minimum and maximum sample values of the buffer.
    void setLimits(Range lim);

private:
    unsigned _size;
    Range _limits;
    double _step;
};

#endif
