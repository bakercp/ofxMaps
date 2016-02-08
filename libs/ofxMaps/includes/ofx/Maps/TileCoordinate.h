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


#include <math.h>
#include <string>
#include "ofVec3d.h"


namespace ofx {
namespace Maps {


/// \brief A tile coordinate in a tiled image system.
class TileCoordinate: public ofVec3d
{
public:
	TileCoordinate();
    TileCoordinate(const TileCoordinate& coordinate);
	TileCoordinate(double row,
                   double column,
                   double zoom,
                   const std::string& id = DEFAULT_SET_ID);

    double getColumn() const;
    double getRow() const;
    double getZoom() const;
    const std::string& getId() const;

    void setColumn(double column);
    void setRow(double row);
    void setZoom(double zoom);
    void setId(const std::string& id);

	TileCoordinate getFloored() const;
    TileCoordinate getClamped() const;

	TileCoordinate zoomTo(double destination) const;
	TileCoordinate zoomBy(double distance) const;
	
	TileCoordinate up(double distance = DEFAULT_STEP) const;
	TileCoordinate right(double distance = DEFAULT_STEP) const;
	TileCoordinate down(double distance = DEFAULT_STEP) const;
	TileCoordinate left(double distance = DEFAULT_STEP) const;

	bool operator < (const TileCoordinate& coordiante) const;
    TileCoordinate& operator = (const TileCoordinate& coordiante);

    static TileCoordinate normalizeTileCoordinate(const TileCoordinate& coordinate);
    static double scaleForZoom(int zoom);

    static const std::string DEFAULT_SET_ID;

    enum
    {
        DEFAULT_STEP = 1
    };

private:
    double& _column;
	double& _row;
	double& _zoom;

    std::string _id;

};


} } // namespace ofx::Maps
