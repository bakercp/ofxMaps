//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/Maps/MapTileSet.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/MediaType.h"
#include "ofx/HTTP/Client.h"
#include "ofx/HTTP/GetRequest.h"
#include "ofx/Maps/MBTilesCache.h"


namespace ofx {
namespace Maps {


const std::string MapTileSet::DEFAULT_BUFFER_CACHE_LOCATION = "cache";


MapTileSet::MapTileSet(std::size_t cacheSize,
                       std::shared_ptr<MapTileProvider> provider,
                       std::shared_ptr<TileBufferCache> bufferCache):
//    Cache::BaseResourceCache<TileKey, Tile>(cacheSize, taskQueue),
    _provider(provider),
    _bufferCache(bufferCache),
    _onAddListener(this->onAdd.newListener(this, &MapTileSet::_onAdd))
{
    if (_bufferCache == nullptr && _provider->isCacheable())
    {
        _bufferCache = std::make_shared<MBTilesCache>(*_provider, DEFAULT_BUFFER_CACHE_LOCATION);
    }
}


std::shared_ptr<Tile> MapTileSet::load(Cache::CacheRequestTask<TileKey, Tile>& task)
{
    std::shared_ptr<ofBuffer> buffer = _tryLoadFromCache(task);

    bool isCached = (buffer != nullptr);

    if (!isCached)
    {
        buffer = _tryLoadFromURI(task);
    }

    if (buffer != nullptr)
    {
        ofPixels pixels;

        if (!ofLoadImage(pixels, *buffer))
        {
            ofLogError("TileStore::load") << "Failure to load pixels.";
            return nullptr;
        }
        else if (!isCached && _bufferCache != nullptr && _provider->isCacheable())
        {
            _bufferCache->add(task.key(), buffer);
        }

        return std::make_shared<Tile>(pixels);
    }
    else return nullptr;
}


std::string MapTileSet::toTaskId(const TileKey& key) const
{
    // Create a unique task id.
    return _provider->id() + "_" + key.toString();
}


std::shared_ptr<ofBuffer> MapTileSet::_tryLoadFromCache(Cache::CacheRequestTask<TileKey, Tile>& task)
{
    if (_bufferCache != nullptr)
    {
        return _bufferCache->get(task.key());
    }
    else return nullptr;
}


std::shared_ptr<MapTileProvider> MapTileSet::provider() const
{
    return _provider;
}


std::shared_ptr<ofBuffer> MapTileSet::_tryLoadFromURI(Cache::CacheRequestTask<TileKey, Tile>& task)
{
    std::shared_ptr<ofBuffer> buffer = nullptr;

    // Launch a thread to go get it!
    Poco::URI uri = Poco::URI(_provider->getTileURI(task.key()));

    if (uri.getScheme() == "http" || uri.getScheme() == "https")
    {
        HTTP::Client client;
        HTTP::Context context;
        HTTP::GetRequest request(uri.toString());

        auto listener = context.events.onHTTPClientResponseProgress.newListener([&task](HTTP::ClientResponseProgressEventArgs& args)
                                                                                {
                                                                                    task.setProgress(args.progress().progress());
                                                                                });

        auto response = client.execute(context, request);

        if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
            Poco::Net::MediaType mediaType(response->getContentType());

            if (mediaType.matches("image"))
            {
                buffer = std::make_shared<ofBuffer>(response->stream());
            }
            else
            {
                ofLogError("TileStore::_tryLoadFromURI") << "Unsupported media type: " << mediaType.toString();
            }
        }
        else
        {
            ofLogError("TileStore::_tryLoadFromURI") << "Invalid response: " << response->getReason() << ": " << uri.toString();
        }
    }
    else
    {
        ofLogError("TileStore::_tryLoadFromURI") << "Unsupported URI scheme: " << uri.toString();
    }

    return buffer;
}


void MapTileSet::_onAdd(const std::pair<TileKey, std::shared_ptr<Tile>>& args)
{
    // We get a callback when it's cached (in the main thread), so we load it.
    args.second->loadTexture();
}


} } // namespace ofx::Maps
