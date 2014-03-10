// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
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


#include "GeoLocation.h"


GeoLocation::GeoLocation():
    ofVec2d(0, 0),
    longitude(x),
    latitude(y)
{
}


GeoLocation::GeoLocation(const GeoLocation& location):
    ofVec2d(location),
    longitude(x),
    latitude(y)
{
}


GeoLocation::GeoLocation(double _latitude, double _longitude):
    ofVec2d(_longitude, _latitude),
    longitude(x),
    latitude(y)
{
}


double GeoLocation::getLatitude() const
{
    return latitude;
}


double GeoLocation::getLongitude() const
{
    return longitude;
}


void GeoLocation::setLatitude(double _latitude)
{
    latitude = _latitude;
}


void GeoLocation::setLongitude(double _longitude)
{
    longitude = _longitude;
}


GeoLocation& GeoLocation::operator = (const GeoLocation& location)
{
    set(location);
    return *this;
}
