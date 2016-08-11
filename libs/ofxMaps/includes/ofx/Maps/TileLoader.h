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


#include "ofImage.h"
#include "ofx/TaskQueue.h"
#include "ofx/LRUCache.h"
#include "ofx/Maps/AbstractMapTypes.h"
#include "ofx/Maps/TileCoordinate.h"


#include "Poco/Task.h"
#include "Poco/TaskNotification.h"
#include "ofx/HTTP/ClientEvents.h"
#include "ofx/HTTP/Client.h"
#include "ofx/HTTP/ThreadSettings.h"


namespace ofx {
namespace Maps {



class TileLoaderRequest
{
public:
    TileLoaderRequest();

    TileLoaderRequest(const TileCoordinateKey& key,
                      const std::string& uri);

    virtual ~TileLoaderRequest();

    TileCoordinateKey key() const;
    std::string uri() const;

private:
    TileCoordinateKey _key;
    std::string _uri;

};


class TileLoaderResponse: public TileLoaderRequest
{
public:
    enum class CacheStatus
    {
        NONE,
        CACHE_MODULE_RESPONSE,
        CACHE_HIT,
        CACHE_MISS,
        VALIDATED
    };

    TileLoaderResponse();

    TileLoaderResponse(const TileLoaderRequest& request,
                       std::shared_ptr<Tile> tile,
                       const ofBuffer& buffer,
                       const Poco::Net::NameValueCollection& responseHeaders,
                       CacheStatus cacheStatus);

    virtual ~TileLoaderResponse();

    std::shared_ptr<Tile> tile() const;
    ofBuffer buffer() const;
    Poco::Net::NameValueCollection responseHeaders() const;
    CacheStatus cacheStatus() const;

private:
    /// \brief The tile.
    std::shared_ptr<Tile> _tile;

    /// \brief The raw buffer data.
    ofBuffer _buffer;

    /// \brief The response headers.
    Poco::Net::NameValueCollection _responseHeaders;

    /// \brief
    CacheStatus _cacheStatus = CacheStatus::NONE;


};


class TileLoaderTask: public Poco::Task
{
public:
    TileLoaderTask(const TileLoaderRequest& request);

    virtual ~TileLoaderTask();

    void runTask();

    bool onHTTPClientStateChange(HTTP::ClientStateChangeEventArgs& args);

    bool onHTTPClientRequestProgress(HTTP::ClientRequestProgressEventArgs& args);
    bool onHTTPClientResponseProgress(HTTP::ClientResponseProgressEventArgs& args);
    
    bool onHTTPClientResponseStream(HTTP::ClientResponseStreamEventArgs& args);
    bool onHTTPClientError(HTTP::ClientErrorEventArgs& args);
    
private:
    std::string cachePathForTile(const TileCoordinateKey& key) const;

    void loadCacheAndNotify(const ofBuffer& buffer,
                            const Poco::Net::NameValueCollection& headers,
                            TileLoaderResponse::CacheStatus cacheStatus);

    TileLoaderRequest _request;
    
};


///// \brief A persistent cache that does not share memory.
//template<typename KeyType, typename ValueType>
//class BaseCache
//{
//public:
//    virtual ~BaseCache()
//    {
//    }
//
//    bool has(const KeyType& key) const
//    {
//        return _has(key);
//    }
//
//    std::shared_ptr<ValueType> get(const KeyType& key) const
//    {
//        return _get(key);
//    }
//
//    bool put(const KeyType& key, const std::shared_ptr<ValueType>& value)
//    {
//        return put(key, value);
//    }
//
//protected:
//    virtual bool _has(const KeyType& key) const = 0;
//    virtual std::shared_ptr<ValueType> _get(const KeyType& key) const = 0;
//    virtual bool _put(const KeyType& key, const std::shared_ptr<ValueType>& value) = 0;
//
//};
//
//
//template<typename KeyType, typename ValueType>
//class DiskCache: public BaseCache<KeyType, ofBuffer>
//{
//public:
//    DiskCache(const std::string& cacheLocation,
//              const std::string& pathTemplate);
//
//    virtual ~DiskCache()
//    {
//    }
//
//    std::string cacheLocation() const
//    {
//        return _cacheLocation;
//    }
//
//
//    std::string pathTemplate() const
//    {
//        return _pathTemplate;
//    }
//
////    constexpr auto DEFAULT_CACHE_LOCATION = "";
////    constexpr auto DEFAULT_CACHE_PATH_TEMPLATE = "{id}/{set_id}/";
////    static const std::string DEFAULT_CACHE_LOCATION;
////    static const std::string DEFAULT_CACHE_PATH_TEMPLATE;
//
//
//private:
//    std::string _cacheLocation;
//    std::string _pathTemplate;
//
//};
//
//
//
////    template<typename KeyType, typename ValueType>
////    class DiskCache: public BaseCache<KeyType, ofBuffer>
////
////
////
////    std::shared_ptr<ofBuffer> _get(const KeyType& key) const override
////    {
////        return nullptr;
////    }
////
////    std::shared_ptr<ofBuffer> _get(const KeyType& key) const override
////    {
////        return nullptr;
////    }
////
////
////    bool _put(const KeyType& key, const std::shared_ptr<ofBuffer>& value) override
////    {
////        return false;
////    }
//
//














class TileLoader
{
public:
    TileLoader();

    TileLoader(std::shared_ptr<TaskQueue> taskQueue);

    TileLoader(std::size_t cacheSize,
               std::shared_ptr<TaskQueue> taskQueue);

    virtual ~TileLoader();

    bool has(const TileCoordinateKey& key) const;

    std::shared_ptr<Tile> get(const TileCoordinateKey& key) const;

    std::string request(const TileLoaderRequest& request);

    bool cancelQueuedRequest(const std::string& requestId);

    bool onTaskCustomNotification(const TaskCustomNotificationEventArgs& args);
    bool onTaskCancelledNotification(const TaskQueueEventArgs& args);

    static const std::size_t DEFAULT_CACHE_SIZE;

    ofEvent<const TileCoordinateKey> onTileCached;
    ofEvent<const TileCoordinateKey> onTileUncached;
    ofEvent<const std::string> onTaskCancelled;

private:
    void onAdd(const Poco::KeyValueArgs<TileCoordinateKey, Tile>& args);
    void onUpdate(const Poco::KeyValueArgs<TileCoordinateKey, Tile>& args);
    void onRemove(const TileCoordinateKey& args);
    void onGet(const TileCoordinateKey& args);
    void onClear(const Poco::EventArgs& args);

    /// \brief The ram cache.
    mutable LRUCache<TileCoordinateKey, Tile> _cache;

//    mutable DiskCache<

    /// \brief The shared task queue.
    std::shared_ptr<TaskQueue> _taskQueue;

    /// \brief The task event listener.
    ofEventListener _onTaskCustomNotificationListener;
    ofEventListener _onTaskCancelledListener;
};




class QueueSorter
{
public:
    QueueSorter(const TileCoordinate& center): _center(center)
    {
    }

    // TODO: simplify these calculations by using internal vec calculations
    bool operator () (const TileCoordinate& c0, const TileCoordinate& c1) const
    {
        if (ofIsFloatEqual(c0.getZoom(), _center.getZoom()))
        {
            if (ofIsFloatEqual(c1.getZoom(), _center.getZoom()))
            {
                glm::dvec2 offset(0.5, 0.5);

                glm::dvec2 center2d(_center.getColumn(),
                                    _center.getRow());

                glm::dvec2 c02d(c0.getColumn(),
                                c0.getRow());

                glm::dvec2 c12d(c1.getColumn(),
                                c1.getRow());

                double d0 = glm::distance2(center2d, c02d + offset);
                double d1 = glm::distance2(center2d, c12d + offset);

                //                double d0 = center2d.distanceSquared(c02d + offset);
                //                double d1 = center2d.distanceSquared(c12d + offset);

                //
                //				double d0 = dist(_center.getColumn(),
                //                                 _center.getRow(),
                //                                 c0.getColumn() + 0.5,
                //                                 c0.getRow() + 0.5);
                //
                //				double d1 = dist(_center.getColumn(),
                //                                 _center.getRow(),
                //                                 c1.getColumn() + 0.5,
                //                                 c1.getRow() + 0.5);

                return d0 < d1;
            }
        }
        else if (ofIsFloatEqual(c1.getZoom(), _center.getZoom()))
        {
            return false;
        }
        else
        {
            double d0 = std::fabs(c0.getZoom() - _center.getZoom());
            double d1 = std::fabs(c1.getZoom() - _center.getZoom());
            return d0 < d1;
        }

        return false;
    }

protected:
    TileCoordinate _center;

};


} } // namespace ofx::Maps
