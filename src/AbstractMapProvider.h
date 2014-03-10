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


#include "AbstractProjection.h"
#include "TileCoordinate.h"
#include "GeoLocation.h"


class AbstractMapProvider
{
public:
    typedef std::shared_ptr<AbstractMapProvider> SharedPtr;

    AbstractMapProvider(AbstractProjection::SharedPtr projection):
        _projection(projection)
    {
    }
	
	virtual std::vector<std::string> getTileUrls(const TileCoordinate& coordinate) const = 0;

	virtual int getTileWidth() const = 0;
	virtual int getTileHeight() const = 0;

//    virtual int getMinZoom() const = 0;
//    virtual int getMaxZoom() const = 0;

	TileCoordinate geoLocationToTileCoordinate(const GeoLocation& location) const
    {
		return _projection->geoLocationToTileCoordinate(location);
	}
	

    GeoLocation tileCoordinateToGeoLocation(const TileCoordinate& coordinate) const
    {
		return _projection->tileCoordinateToGeoLocation(coordinate);
	}


    TileCoordinate normalizeTileCoordinate(const TileCoordinate& coordinate) const
    {
		double gridSize = pow(2.0, coordinate.getZoom());

		double wrappedColumn = fmod(coordinate.getColumn(), gridSize);

		while (wrappedColumn < 0)
        {
			wrappedColumn += gridSize;
		}

		double wrappedRow = fmod(coordinate.getRow(), gridSize);

		while (wrappedRow < 0)
        {
			wrappedRow += gridSize;
		}

		return TileCoordinate(wrappedRow, wrappedColumn, coordinate.getZoom());
	}

    double scaleForZoom(int zoom) const
    {
        return pow(2.0, zoom);
    }

    double zoomForScale(double scale) const
    {
        return log(scale) / log(2);
    }

    int bestZoomForScale(double scale) const
    {
        // TODO get min/max zoom level from provider?
        return std::min(20,
                        std::max(1, (int)round(log(scale) / log(2)))
                        );
    }


protected:
    AbstractProjection::SharedPtr _projection;
	
};
