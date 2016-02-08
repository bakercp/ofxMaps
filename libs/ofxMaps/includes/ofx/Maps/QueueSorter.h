// =============================================================================
//
// Copyright (c) 2014-2016 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/Maps/TileCoordinate.h"


namespace ofx {
namespace Maps {
        

class QueueSorter
{
public:
	QueueSorter(const TileCoordinate& center): _center(center)
    {
    }
	
    // TODO: simplify these calculations by using internal vec calculations
	bool operator () (const TileCoordinate& c0, const TileCoordinate& c1) const
    {
		if (c0.getZoom() == _center.getZoom())
        {
			if (c1.getZoom() == _center.getZoom())
            {
                ofVec2d offset(0.5, 0.5);

                ofVec2d center2d(_center);
                ofVec2d c02d(c0);
                ofVec2d c12d(c1);

                double d0 = center2d.distanceSquared(c02d + offset);
                double d1 = center2d.distanceSquared(c12d + offset);

//
//				double d0 = dist(_center.getColumn(),
//                                 _center.getRow(),
//                                 c0.getColumn() + 0.5,
//                                 c0.getRow() + 0.5);
//
//				double d1 = dist(_center.getColumn(),
//                                 _center.getRow(),
//                                 c1.getColumn() + 0.5,
//                                 c1.getRow() + 0.5);

				return d0 < d1;
			}
		}
		else if (c1.getZoom() == _center.getZoom())
        {
			return false;
		}
		else
        {
			double d0 = fabs(c0.getZoom() - _center.getZoom());
			double d1 = fabs(c1.getZoom() - _center.getZoom());
			return d0 < d1;
		}

		return false;
	}

protected:
    TileCoordinate _center;

};


} } // namespace ofx::Maps
