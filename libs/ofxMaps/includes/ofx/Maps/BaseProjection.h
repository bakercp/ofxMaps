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


#include "ofTypes.h"
#include "ofx/Geo/Coordinate.h"
#include "ofx/Maps/Transformation.h"
#include "ofx/Maps/TileCoordinate.h"


namespace ofx {
namespace Maps {


class BaseProjection
{
public:
    typedef std::shared_ptr<BaseProjection> SharedPtr;

    BaseProjection(double zoom, const Transformation& transformation);
    virtual ~BaseProjection();

    TileCoordinate geoCoordinateToTileCoordinate(const Geo::Coordinate& location) const;
    Geo::Coordinate tileCoordinateToGeoCoordinate(const TileCoordinate& coordinate) const;

protected:
	virtual ofVec2d rawProject(const ofVec2d& point) const = 0;
	virtual ofVec2d rawUnproject(const ofVec2d& point) const = 0;

	ofVec2d project(const ofVec2d& point) const;
	ofVec2d unproject(const ofVec2d& point) const;
    
	double _zoom;
	Transformation _transformation;

};


} } // namespace ofx::Maps
