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


#include "ofx/Geo/Utils.h"
#include "ofx/Maps/BaseProjection.h"


namespace ofx {
namespace Maps {


//EPSG:3857

class Transformation;


class SperhicalMercatorProjection: public BaseProjection
{
public:	
    SperhicalMercatorProjection(double zoom = 0,
                                Transformation t = DEFAULT_SPHERICAL_MERCATOR_TRANSFORMATION);

    virtual ~SperhicalMercatorProjection();

    static const Transformation DEFAULT_SPHERICAL_MERCATOR_TRANSFORMATION;

    static const double MINIMUM_LATITUDE;
	static const double MAXIMUM_LATITUDE;
	static const double MINIMUM_LONGITUDE;
	static const double MAXIMUM_LONGITUDE;

protected:
	ofVec2d rawProject(const ofVec2d& point) const;
	ofVec2d rawUnproject(const ofVec2d& point) const;

};

    
} } // namespace ofx::Maps
