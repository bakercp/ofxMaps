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


#include <string>
#include "ofx/Geo/Coordinate.h"
#include "ofx/Geo/CoordinateBounds.h"
#include "ofx/Maps/Tile.h"
#include "ofx/Maps/TileCoordinate.h"


namespace ofx {
namespace Maps {


/// \brief An abstract class representing a Map Tile Provider.
class AbstractTileProvider
{
public:
    /// \brief Destroy the AbstractMapTileProvider.
    virtual ~AbstractTileProvider()
    {
    }

    /// \brief Get the provider's unique ID.
    ///
    /// This unique provider id is primarily used for caching purposes.
    ///
    /// \returns a unique provider ID.
    virtual std::string id() const = 0;

    /// \brief Get the name of this provider if available.
    /// \returns the name of this provider.
    virtual std::string name() const = 0;

    /// \brief Get the description of this provider if available.
    /// \returns the description of this provider.
    virtual std::string description() const = 0;

    /// \brief Get the attribution string for the current tile provider.
    /// \returns An attribution string.
    virtual std::string attribution() const = 0;

    /// \brief Get the version of this provider data if available.
    /// \returns the version of this provider data.
    virtual std::string version() const = 0;

    /// \brief Get the minimum zoom level for this provider.
    /// \returns the minimum zoom level.
    virtual int minZoom() const = 0;

    /// \brief Get the maximum zoom level for this provider.
    /// \returns the maximum zoom level.
    virtual int maxZoom() const = 0;

    /// \brief Get the tile width.
    /// \returns the tile width;
    virtual float tileWidth() const = 0;

    /// \brief Get the tile height;
    /// \returns the tile height;
    virtual float tileHeight() const = 0;

    /// \brief Get the tile size.
    /// \returns the tile size.
    virtual glm::vec2 tileSize() const = 0;

    /// \returns the bounds for this provider.
    virtual Geo::CoordinateBounds bounds() const = 0;

    /// \returns the initial center for this provider.
    virtual TileCoordinate center() const = 0;

    /// \brief Get the zoom level for a given map scale.
    /// \param scale The scale to calculate.
    /// \returns the zoom level for the given scale.
    virtual double zoomForScale(double scale) const = 0;

    /// \brief Get the TileCoordinate from the given Geo::Coordinate at the default zoom.
    /// \param location The the Geo::Coordinate at the default zoom level.
    /// \returns the TileCoordinate at the default zoom level.
    virtual TileCoordinate geoToWorld(const Geo::Coordinate& location) const = 0;

    /// \brief Get the GeoCoordinate from the given TileCoordinate.
    /// \param coordinate The TileCoordinate to transform to a Geo::Coordinate.
    /// \returns the GeoCoordinate corresponding to the TileCoordinate.
    virtual Geo::Coordinate tileToGeo(const TileCoordinate& coordinate) const = 0;

};


class AbstractURITileProvider: public AbstractTileProvider
{
public:
    virtual ~AbstractURITileProvider()
    {
    }

    /// \brief Get a URI for the given tile coordinate.
    /// \param coordinate The TileCoordinate to get a URI for.
    /// \returns the URI for the given TileCoordinate.
    virtual std::string getTileURI(const TileCoordinateKey& key) const = 0;

};


} } // namespace ofx::Maps
