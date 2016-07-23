// =============================================================================
//
// Copyright (c) 2014-2016 Christopher Baker <http://christopherbaker.net>
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
#include "ofx/Maps/AbstractMapTypes.h"
#include "ofx/Maps/BaseProjection.h"
#include "ofx/Maps/SphericalMercatorProjection.h"
#include "ofx/Maps/TileCoordinate.h"


namespace ofx {
namespace Maps {


/// \brief A simplified BaseTileProvider.
class BaseTileProvider: public AbstractTileProvider
{
public:
    /// \brief Create a BaseTileProvider.
    /// \param attribution The attribution string for the tile source.
    /// \param minZoom The minimum zoom level supported by the provider.
    /// \param maxZoom The maximum zoom level supported by the provider.
    /// \param tileWidth The width of the provider's tiles in pixels.
    /// \param tileHeight The height of the provider's tiles in pixels.
    /// \param projection The projection used by the provider.
    BaseTileProvider(const std::string& attribution,
                     int minZoom,
                     int maxZoom,
                     int tileWidth,
                     int tileHeight,
                     const BaseProjection& projection);

    /// \brief Destroy the BaseMapProvider.
    virtual ~BaseTileProvider();

    const std::string& getAttribution() const;

    int getMinZoom() const;

    int getMaxZoom() const;

	int getTileWidth() const;

    int getTileHeight() const;

    glm::dvec2 getTileSize() const;

    virtual double zoomForScale(double scale) const;

    TileCoordinate geoToTile(const Geo::Coordinate& location) const;
    
    Geo::Coordinate tileToGeo(const TileCoordinate& coordinate) const;

    enum
    {
        /// \brief The minimum zoom level supported by most map tile providers.
        DEFAULT_MIN_ZOOM = 0,
        /// \brief The maximum zoom level supported by most map tile providers.
        DEFAULT_MAX_ZOOM = 19,
        /// \brief The default tile width supported by most map tile providers.
        DEFAULT_TILE_WIDTH = 256,
        /// \brief The default tile height supported by most map tile providers.
        DEFAULT_TILE_HEIGHT = 256
    };

    /// \brief The default projection used by most map tile providers.
    static const SperhicalMercatorProjection DEFAULT_PROJECTION;

protected:
    /// \brief A string containing this provider's attribution.
    std::string _attribution;

    /// \brief The minimum zoom level for this provider.
    int _minZoom;

    /// \brief The maximum zoom level for this provider.
    int _maxZoom;
    
    /// \brief The tile width used by this provider.
    int _tileWidth;

    /// \brief The tile height used by this provider.
    int _tileHeight;

    /// \brief A reference to this provider's projection.
    const BaseProjection& _projection;

    /// \brief The log of 2 (i.e. log(2)).
    static const double LOG_2;

};


} } // namespace ofx::Maps
