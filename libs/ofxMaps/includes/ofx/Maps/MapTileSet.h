//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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
