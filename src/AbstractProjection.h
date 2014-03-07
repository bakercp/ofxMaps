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


#include "Transformation.h"
#include "Point2d.h"
#include "Coordinate.h"
#include "Location.h"

class AbstractProjection {

public:
    typedef std::shared_ptr<AbstractProjection> SharedPtr;

	AbstractProjection(double zoom):
        _zoom(zoom),
        _transformation(Transformation())
    {
    }

    AbstractProjection(double zoom, Transformation transformation):
        _zoom(zoom),
        _transformation(transformation)
    {
    }

	virtual Point2d rawProject(const Point2d& point) const = 0;
	virtual Point2d rawUnproject(const Point2d& point) const = 0;

	Point2d project(const Point2d& point) const
    {
		return _transformation.transform(rawProject(point));
	}

	Point2d unproject(const Point2d& point) const
    {
		return rawUnproject(_transformation.untransform(point));
	}

	Coordinate locationCoordinate(const Location& location) const
    {
		Point2d point = project(Point2d(DEG_TO_RAD * location.lon, DEG_TO_RAD * location.lat));

        return Coordinate(point.y, point.x, _zoom);
	}

	Location coordinateLocation(const Coordinate& coordinate) const
    {
        Coordinate newCoordinate = coordinate.zoomTo(_zoom);

		Point2d point = unproject(Point2d(newCoordinate.column, newCoordinate.row));

		return Location(RAD_TO_DEG * point.x, RAD_TO_DEG * point.x);
	}

protected:
	double _zoom;
	Transformation _transformation;

};

