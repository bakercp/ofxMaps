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


#include "ofx/Maps/Transformation.h"


namespace ofx {
namespace Maps {


Transformation::Transformation():
    ax(1),
    bx(0),
    cx(0),
    ay(0),
    by(1),
    cy(0)
{
}


Transformation::Transformation(double _ax,
                               double _bx,
                               double _cx,
                               double _ay,
                               double _by,
                               double _cy):
    ax(_ax),
    bx(_bx),
    cx(_cx),
    ay(_ay),
    by(_by),
    cy(_cy)
{
}


Transformation::Transformation(double a1x, double a1y,
                               double a2x, double a2y,
                               double b1x, double b1y,
                               double b2x, double b2y,
                               double c1x, double c1y,
                               double c2x, double c2y):
    ax(1),
    bx(0),
    cx(0),
    ay(0),
    by(1),
    cy(0)
{
    linearSolution(a1x, a1y, a2x,
                   b1x, b1y, b2x,
                   c1x, c1y, c2x,
                   ax,  bx,  cx);

    linearSolution(a1x, a1y, a2y,
                   b1x, b1y, b2y,
                   c1x, c1y, c2y,
                   ay,  by,  cy);
}


ofVec2d Transformation::transform(const ofVec2d& point) const
{
    double x = ax * point.x + bx * point.y + cx;
    double y = ay * point.x + by * point.y + cy;

    return ofVec2d(x, y);
}


ofVec2d Transformation::untransform(const ofVec2d& point) const
{
    double x = (point.x * by - point.y * bx - cx * by + cy * bx) / (ax * by - ay * bx);
    double y = (point.x * ay - point.y * ax - cx * ay + cy * ax) / (bx * ay - by * ax);

    return ofVec2d(x, y);
}


bool Transformation::linearSolution(double r1, double s1, double t1,
                                    double r2, double s2, double t2,
                                    double r3, double s3, double t3,
                                    double& a, double& b, double& c)
{
    a = (((t2 - t3) * (s1 - s2)) - ((t1 - t2) * (s2 - s3)))
      / (((r2 - r3) * (s1 - s2)) - ((r1 - r2) * (s2 - s3)));

    b = (((t2 - t3) * (r1 - r2)) - ((t1 - t2) * (r2 - r3)))
      / (((s2 - s3) * (r1 - r2)) - ((s1 - s2) * (r2 - r3)));

    c = t1 - (r1 * a) - (s1 * b);

    return true;
};


} } // namespace ofx::Maps
