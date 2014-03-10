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


#include "MercatorProjection.h"


const Transformation MercatorProjection::DEFAULT_TRANSFORMATION = Transformation(1.068070779e7,
                                                                                 0.0,
                                                                                 3.355443185e7,
                                                                                 0.0,
                                                                                 -1.068070890e7,
                                                                                 3.355443057e7);
MercatorProjection::MercatorProjection(double zoom,
                                       Transformation t):
    AbstractProjection(zoom, t)
{
}


MercatorProjection::~MercatorProjection()
{
}


ofVec2d MercatorProjection::rawProject(const ofVec2d& point) const
{
	return ofVec2d(point.x,
                   log(tan(0.25 * PI + 0.5 * point.y))
                   );
}


ofVec2d MercatorProjection::rawUnproject(const ofVec2d& point) const
{
	return ofVec2d(point.x,
                   2.0 * atan(pow(M_E, (double)point.y)) - 0.5 * PI);
}
