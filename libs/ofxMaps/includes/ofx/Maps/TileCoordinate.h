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


#include <math.h>
#include "ofx/Geo/Utils.h"


namespace ofx {
namespace Maps {


/// \brief A tile coordinate in a tiled mapping system.
class TileCoordinate: public ofVec3d
{
public:
	TileCoordinate();
    TileCoordinate(const TileCoordinate& coordinate);
	TileCoordinate(double _row, double _column, double _zoom);

    double getColumn() const;
    double getRow() const;
    double getZoom() const;

	TileCoordinate getFloored() const;
	
	TileCoordinate zoomTo(double destination) const;
	TileCoordinate zoomBy(double distance) const;
	
	TileCoordinate up(double distance = 1) const;
	TileCoordinate right(double distance = 1) const;
	TileCoordinate down(double distance = 1) const;
	TileCoordinate left(double distance = 1) const;

	bool operator < (const TileCoordinate& c) const;
    TileCoordinate& operator = (const TileCoordinate& rect);

    static TileCoordinate normalizeTileCoordinate(const TileCoordinate& coordinate);
    static double scaleForZoom(int zoom);

    // TODO: These should be doubles in the future.
    double& column;
	double& row;
	double& zoom;

};


} } // namespace ofx::Maps
