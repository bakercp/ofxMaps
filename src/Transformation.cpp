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


#include "Transformation.h"
#include "Point2d.h"


Transformation::Transformation():
    ax(1),
    bx(0),
    cx(0),
    ay(0),
    by(1),
    cy(0)
{
}


Transformation::Transformation(float _ax,
                               float _bx,
                               float _cx,
                               float _ay,
                               float _by,
                               float _cy):
    ax(_ax),
    bx(_bx),
    cx(_cx),
    ay(_ay),
    by(_by),
    cy(_cy)
{
}


Point2d Transformation::transform(const Point2d& point) const
{
	return Point2d(ax * point.x + bx * point.y + cx,
                   ay * point.x + by * point.y + cy);
}


Point2d Transformation::untransform(const Point2d& point) const
{
	return Point2d((point.x * by - point.y * bx - cx * by + cy * bx) / (ax * by - ay * bx),
                   (point.x * ay - point.y * ax - cx * ay + cy * ax) / (bx * ay - by * ax));
}
