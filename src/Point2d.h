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


#include "ofMain.h"

// TODO: This should really be replaced with ofVec2d ... but that requires
// a templated ofVec series.
class Point2d
{
public:
	Point2d():
        x(0),
        y(0)
    {
    }

	Point2d(double _x, double _y):
        x(_x),
        y(_y)
    {
    }

	Point2d(const Point2d &point):
        x(point.x),
        y(point.y)
    {
    }

	friend std::ostream& operator << (std::ostream& stream, const Point2d& p) {
		// TODO: number format
		stream << "(" << p.x << ", " << p.y << ")";
		return stream;
	}

    bool operator == (const Point2d &p) const
    {
        return p.x == x && p.y == y;
    }

    double x;
	double y;



};
