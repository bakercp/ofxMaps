//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>
#include "ofx/Geo/Coordinate.h"
#include "ofx/Geo/CoordinateBounds.h"
#include "ofx/Maps/Tile.h"


namespace ofx {
namespace Maps {


class TileCoordinate;
class TileKey;


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

    /// \brief Get the zoom level for a given map scale.
    /// \param scale The scale to calculate.
    /// \returns the zoom level for the given scale.
    virtual double zoomForScale(double scale) const = 0;

    /// \brief Determine if this provider is cacheable.
    ///
    /// Some providers, such as local databases are write only and should not
    /// be cached.
    ///
    /// \returns true if this provider is cachable.
    virtual bool isCacheable() const = 0;

    /// \brief Get a URI for the given tile coordinate.
    /// \param coordinate The TileCoordinate to get a URI for.
    /// \returns the URI for the given TileCoordinate.
    virtual std::string getTileURI(const TileKey& key) const = 0;

};


class AbstractMapTileProvider: public AbstractTileProvider
{
public:
    /// \brief Destroy the AbstractMapTileProvider.
    virtual ~AbstractMapTileProvider()
    {
    }

    /// \returns the bounds for this provider.
    virtual Geo::CoordinateBounds bounds() const = 0;

    /// \returns the initial center for this provider.
    virtual TileCoordinate center() const = 0;

    /// \brief Get the TileCoordinate from the given Geo::Coordinate at the default zoom.
    /// \param location The the Geo::Coordinate at the default zoom level.
    /// \returns the TileCoordinate at the default zoom level.
    virtual TileCoordinate geoToWorld(const Geo::Coordinate& location) const = 0;

    /// \brief Get the GeoCoordinate from the given TileCoordinate.
    /// \param coordinate The TileCoordinate to transform to a Geo::Coordinate.
    /// \returns the GeoCoordinate corresponding to the TileCoordinate.
    virtual Geo::Coordinate tileToGeo(const TileCoordinate& coordinate) const = 0;

};


} } // namespace ofx::Maps
