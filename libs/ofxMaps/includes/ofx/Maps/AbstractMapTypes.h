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


#include "ofx/Maps/MapTile.h"
#include "ofx/IO/ByteBuffer.h"
#include "Poco/URI.h"
#include "Poco/UUID.h"
#include "ofVec2d.h"


namespace ofx {


namespace Geo {
    class Coordinate;
}

namespace Maps {


class TileCoordinate;


/// \brief An abstract map tile source.
class AbstractTileSource
{
public:
    /// \brief Destroy the AbstractTileSource.
    virtual ~AbstractTileSource()
    {
    }

    /// \brief Get a map tile for a given TileCoordinate.
    /// \param coordinate The tile coordinate.
    /// \returns A unique tile request id.
    virtual SharedTile getTile(const TileCoordinate& coordinate) = 0;

};


/// \brief An abstract map tile source.
class AbstractAsyncTileSource
{
public:
    /// \brief Destroy the AbstractMapTileSource.
    virtual ~AbstractAsyncTileSource()
    {
    }

    /// \brief Get a map tile for a given TileCoordinate.
    /// \param coordinate The tile coordinate.
    /// \returns A unique tile request id.
    virtual Poco::UUID requestTile(const TileCoordinate& coordinate) const = 0;
};


/// \brief An abstract Tile source.
class AbstractURITileSource: public AbstractTileSource
{
public:
    /// \brief Destroy the AbstractMapTileSource.
    virtual ~AbstractURITileSource()
    {
    }

    /// \brief Get a map tile for a given TileCoordinate.
    /// \param coordinate The tile coordinate.
    /// \returns A tile of type TileType.
    virtual Poco::URI getURI(const TileCoordinate& coordinate) const = 0;

};


/// \brief An abstract map tile cache.
class AbstractTileSink
{
public:
    /// \brief Destroy the AbstractMapTileCache.
    virtual ~AbstractTileSink()
    {
    }

    /// \brief Cache a map tile for a given TileCoordinate.
    /// \param coordinate The tile coordinate.
    /// \param tile The MapTile to be cached.
    /// \returns True if the cache was successful.
    virtual bool putTile(const TileCoordinate& coordinate,
                         const IO::ByteBuffer& tile) = 0;

    /// \brief Cache a map tile for a given TileCoordinate.
    /// \param coordinate The tile coordinate.
    /// \param tile The MapTile to be cached.
    /// \returns True if the cache was successful.
    virtual bool putTile(const TileCoordinate& coordinate,
                         const SharedTile& tile) = 0;

    /// \brief Clear the cache.
    virtual void clear() = 0;

};


/// \brief An abstract class representing a Map Tile Provider.
class AbstractTileProvider: public AbstractTileSource
{
public:
    /// \brief Destroy the AbstractMapTileProvider.
    virtual ~AbstractTileProvider()
    {
    }

    /// \brief Get the tile width.
    /// \returns the tile width;
    virtual int getTileWidth() const = 0;

    /// \brief Get the tile height;
    /// \returns the tile height;
    virtual int getTileHeight() const = 0;

    /// \brief Get the minimum zoom level for this provider.
    /// \returns the minimum zoom level;
    virtual int getMinZoom() const = 0;

    /// \brief Get the maximum zoom level for this provider.
    /// \returns the maximum zoom level.
    virtual int getMaxZoom() const = 0;

    /// \brief Get the zoom level for a given map scale.
    /// \param scale The scale to calculate.
    /// \returns the zoom level for the given scale.
    virtual double zoomForScale(double scale) const = 0;

    /// \brief Get the TileCoordinate from the given Geo::Coordinate at the default zoom.
    /// \param location The the Geo::Coordinate at the default zoom level.
    /// \returns the TileCoordinate at the default zoom level.
    virtual TileCoordinate geoToTile(const Geo::Coordinate& location) const = 0;

    /// \brief Get the GeoCoordinate from the given TileCoordinate.
    /// \param coordinate The TileCoordinate to transform to a Geo::Coordinate.
    /// \returns the GeoCoordinate corresponding to the TileCoordinate.
    virtual Geo::Coordinate tileToGeo(const TileCoordinate& coordinate) const = 0;

};


/// \brief An abstract class representing a Map Tile Layer.
class AbstractTileLayer
{
public:
    /// \brief Destroy the AbstractTileLayer.
    virtual ~AbstractTileLayer()
    {
    }
    
};



} } // namespace ofx::Maps
