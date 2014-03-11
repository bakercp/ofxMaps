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


#include "BaseProjection.h"
#include "TileCoordinate.h"
#include "GeoLocation.h"


class BaseMapProvider
{
public:
    typedef std::shared_ptr<BaseMapProvider> SharedPtr;

    BaseMapProvider(BaseProjection::SharedPtr projection):
        _projection(projection)
    {
    }
	
	virtual std::vector<std::string> getTileUrls(const TileCoordinate& coordinate) const = 0;

	virtual int getTileWidth() const = 0;
	virtual int getTileHeight() const = 0;

    virtual int getMinZoom() const = 0;
    virtual int getMaxZoom() const = 0;

    
	TileCoordinate geoLocationToTileCoordinate(const GeoLocation& location) const
    {
		return _projection->geoLocationToTileCoordinate(location);
	}
	

    GeoLocation tileCoordinateToGeoLocation(const TileCoordinate& coordinate) const
    {
		return _projection->tileCoordinateToGeoLocation(coordinate);
	}



    double zoomForScale(double scale) const
    {
        return log(scale) / log(2);
    }


    int bestZoomForScale(double scale) const
    {
        int zoom = round(zoomForScale(scale));

        int maxMinZoom = std::max(getMinZoom(), zoom);
        int minMaxZoom = std::min(getMaxZoom(), maxMinZoom);

        return minMaxZoom;
    }


protected:
    BaseProjection::SharedPtr _projection;
	
};
