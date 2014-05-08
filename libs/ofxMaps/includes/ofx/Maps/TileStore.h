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
#include "ofURLFileLoader.h"
#include "ofx/Maps/TileCoordinate.h"
#include "ofx/Maps/lrucache.h"


namespace ofx {
namespace Maps {


class BaseTileStore
{
public:
    typedef std::shared_ptr<ofImage> SharedImagePtr;

    BaseTileStore(std::size_t cacheSize = DEFAULT_CACHE_SIZE);

    virtual ~BaseTileStore();

    SharedImagePtr getTile(const TileCoordinate& coordinate);

    void urlResponse(ofHttpResponse& args);

    void queueTile(const TileCoordinate& coord);

    enum
    {
        DEFAULT_CACHE_SIZE = 64
    };

private:
    typedef std::shared_ptr<ofURLFileLoader> SharedLoaderPtr;

    /// \brief Image store.
    cache::lru_cache<TileCoordinate, SharedImagePtr> _images;

    std::map<int, std::shared_ptr<ofURLFileLoader> > _loaders;

    /// \brief Tiles waiting to load.
    std::map<TileCoordinate, int> _pending;

    mutable Poco::FastMutex _mutex;

};


} } // namespace ofx::Maps
