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


#include "Point2d.h"


class Transformation
{
public:
	
	double ax, bx, cx, ay, by, cy;

	Transformation() : ax(1), bx(0), cx(0), ay(0), by(1), cy(0) {}
	
	Transformation(float _ax, float _bx, float _cx, float _ay, float _by, float _cy) : ax(_ax), bx(_bx), cx(_cx), ay(_ay), by(_by), cy(_cy) {}
	
	Transformation(const Transformation& t) : ax(t.ax), bx(t.bx), cx(t.cx), ay(t.ay), by(t.by), cy(t.cy) {}
	
	Point2d transform(const Point2d& point) const;
	
	Point2d untransform(const Point2d& point) const;
	
};
