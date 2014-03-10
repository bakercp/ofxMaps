// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
// Copyright (c) -2014 Tom Carden <https://github.com/RandomEtc>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================

#pragma once


#include "TileCoordinate.h"


class QueueSorter
{
public:
	QueueSorter(const TileCoordinate& center): _center(center)
    {
    }
	
	static double dist(double x1, double y1, double x2, double y2)
    {
		double dx = x2 - x1;
		double dy = y2 - y1;

		return sqrt(dx * dx + dy * dy);
	}

	bool operator () (const TileCoordinate& c1, const TileCoordinate& c2) const
    {
		if (c1.zoom == _center.zoom)
        {
			if (c2.zoom == _center.zoom)
            {
				double d1 = dist(_center.column, _center.row, c1.column + 0.5, c1.row + 0.5);
				double d2 = dist(_center.column, _center.row, c2.column + 0.5, c2.row + 0.5);
				return d1 < d2;
			}
		}
		else if (c2.zoom == _center.zoom)
        {
			return false;
		}
		else
        {
			double d1 = fabs(c1.zoom - _center.zoom);
			double d2 = fabs(c2.zoom - _center.zoom);
			return d1 < d2;
		}

		return false;
	}

protected:
    TileCoordinate _center;

};
