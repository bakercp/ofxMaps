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


#pragma once


#include "ofx/Maps/TileCoordinate.h"
#include "ofx/Maps/Tile.h"
#include "Poco/Task.h"


namespace ofx {


namespace Geo {
    class Coordinate;
}

namespace Maps {


class TileCoordinate;


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
    virtual std::string getID() const = 0;

    /// \brief Get a load task for the given TileCoordinate.
    /// \param coordinate The tile coordinate of the requested tile.
    /// \returns A task that will load the given tile when submitted.
    virtual Poco::Task* requestTile(const TileCoordinate& coordinate) const = 0;

    /// \brief Get the attribution string for the current tile provider.
    /// \returns An attribution string.
    virtual const std::string& getAttribution() const = 0;

    /// \brief Get the minimum zoom level for this provider.
    /// \returns the minimum zoom level;
    virtual int getMinZoom() const = 0;

    /// \brief Get the maximum zoom level for this provider.
    /// \returns the maximum zoom level.
    virtual int getMaxZoom() const = 0;

    /// \brief Get the tile width.
    /// \returns the tile width;
    virtual int getTileWidth() const = 0;

    /// \brief Get the tile height;
    /// \returns the tile height;
    virtual int getTileHeight() const = 0;
    
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


} } // namespace ofx::Maps
