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


#include "ofx/Geo/Coordinate.h"
#include "ofx/Maps/BaseProjection.h"
#include "ofx/Maps/TileCoordinate.h"


namespace ofx {
namespace Maps {


class BaseMapProvider
{
public:
    typedef std::shared_ptr<BaseMapProvider> SharedPtr;

    BaseMapProvider(BaseProjection::SharedPtr projection,
                    int tileWidth,
                    int tileHeight,
                    int minZoom,
                    int maxZoom);

    virtual ~BaseMapProvider();

	virtual std::vector<std::string> getTileUrls(const TileCoordinate& coordinate) const = 0;

	int getTileWidth() const;
	int getTileHeight() const;

    ofVec2d getTileSize() const;

    /// \brief Get the minimum zoom level for this provider.
    /// \returns the minimum zoom level;
    int getMinZoom() const;

    /// \brief Get the maximum zoom level for this provider.
    /// \returns the maximum zoom level.
    int getMaxZoom() const;

    /// \brief Get the zoom level for a given map scale.
    /// \param scale The scale to calculate.
    /// \returns the zoom level for the given scale.
    double zoomForScale(double scale) const;

	TileCoordinate geoCoordinateToTileCoordinate(const Geo::Coordinate& location) const;

    Geo::Coordinate tileCoordinateToGeoCoordinate(const TileCoordinate& coordinate) const;

protected:
    BaseProjection::SharedPtr _projection;

    int _tileWidth;
    int _tileHeight;
    int _minZoom;
    int _maxZoom;
	
};


} } // namespace ofx::Maps
