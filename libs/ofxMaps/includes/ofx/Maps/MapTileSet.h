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


#include "Poco/Task.h"
#include "Poco/TaskNotification.h"
#include "ofImage.h"
#include "ofx/TaskQueue.h"
#include "ofx/LRUCache.h"
#include "ofx/Cache/BaseHTTPStore.h"
#include "ofx/Cache/ResourceLoader.h"
#include "ofx/Maps/AbstractMapTypes.h"
#include "ofx/Maps/TileCoordinate.h"
#include "ofx/Maps/MapTileProvider.h"
#include "ofx/Maps/TileKey.h"
#include "ofx/HTTP/ClientEvents.h"
#include "ofx/HTTP/Client.h"
#include "ofx/HTTP/ThreadSettings.h"


namespace ofx {
namespace Maps {



class MapTileSet: public Cache::BaseResourceCache<TileKey, Tile>
{
public:
    typedef Cache::BaseCache<TileKey, ofBuffer> TileBufferCache;

    MapTileSet(std::size_t cacheSize,
               std::shared_ptr<MapTileProvider> provider,
               std::shared_ptr<TileBufferCache> bufferCache = nullptr);

    std::shared_ptr<Tile> load(Cache::CacheRequestTask<TileKey, Tile>& task) override;
    std::string toTaskId(const TileKey& key) const override;

    std::shared_ptr<MapTileProvider> provider() const;

    static const std::string DEFAULT_BUFFER_CACHE_LOCATION;

protected:
    std::shared_ptr<ofBuffer> _tryLoadFromCache(Cache::CacheRequestTask<TileKey, Tile>& task);
    std::shared_ptr<ofBuffer> _tryLoadFromURI(Cache::CacheRequestTask<TileKey, Tile>& task);

    void _onAdd(const std::pair<TileKey, std::shared_ptr<Tile>>& args);

    std::shared_ptr<TileBufferCache> _bufferCache;

private:
    /// \brief The tile provider associated with this loader.
    std::shared_ptr<MapTileProvider> _provider;

    /// \brief The onPut event listener used to load texture in the main thread.
    ofEventListener _onAddListener;

//    /// \brief The store mutex.
//    mutable std::mutex _mutex;
};


} } // namespace ofx::Maps
