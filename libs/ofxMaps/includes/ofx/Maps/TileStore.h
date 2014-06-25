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


#include <map>
#include "ofImage.h"
#include "ofx/Maps/TileCoordinate.h"


namespace ofx {
namespace Maps {


class BaseTileStore
{
public:
    typedef std::shared_ptr<ofImage> Tile;

    BaseTileStore(std::size_t cacheSize = DEFAULT_CACHE_SIZE);

    virtual ~BaseTileStore();

    /// \brief Get a shared pointer to a Tile.
    ///
    /// If the tile is not yet available, the BaseTileStore will choose an
    /// alternate or return a null pointer if nothing is available.
    ///
    /// \param coordinate The TileCoordinate that describes the tile
    Tile getTile(const TileCoordinate& coordinate);

    enum
    {
        /// \brief The maximum number of items cached.
        DEFAULT_CACHE_SIZE = 128
    };

private:
    /// \brief Image store.
//    cache::lru_cache<TileCoordinate, Tile> _cache;

};


} } // namespace ofx::Maps
