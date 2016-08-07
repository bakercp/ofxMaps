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


#include "ofx/Maps/TileLoader.h"
#include "Poco/URI.h"
#include "ofUtils.h"
#include "ofx/MediaTypeMap.h"
#include "ofx/HTTP/GetRequest.h"


namespace ofx {
namespace Maps {


TileLoaderRequest::TileLoaderRequest()
{
}


TileLoaderRequest::TileLoaderRequest(const TileCoordinateKey& key,
                                     const std::string& uri):
    _key(key),
    _uri(uri)
{
}


TileLoaderRequest::~TileLoaderRequest()
{

}

TileCoordinateKey TileLoaderRequest::key() const
{
    return _key;
}


std::string TileLoaderRequest::uri() const
{
    return _uri;
}


TileLoaderResponse::TileLoaderResponse()
{
}


TileLoaderResponse::TileLoaderResponse(const TileLoaderRequest& request,
                                       std::shared_ptr<Tile> tile,
                                       const ofBuffer& buffer,
                                       const Poco::Net::NameValueCollection& responseHeaders,
                                       CacheStatus cacheStatus):
    TileLoaderRequest(request),
    _tile(tile),
    _buffer(buffer),
    _responseHeaders(responseHeaders),
    _cacheStatus(cacheStatus)
{
}


TileLoaderResponse::~TileLoaderResponse()
{
}


std::shared_ptr<Tile> TileLoaderResponse::tile() const
{
    return _tile;
}


ofBuffer TileLoaderResponse::buffer() const
{
    return _buffer;
}


Poco::Net::NameValueCollection TileLoaderResponse::responseHeaders() const
{
    return _responseHeaders;
}


TileLoaderResponse::CacheStatus TileLoaderResponse::cacheStatus() const
{
    return _cacheStatus;
}


TileLoaderTask::TileLoaderTask(const TileLoaderRequest& request):
    Poco::Task(request.uri()),
    _request(request)
{
}


TileLoaderTask::~TileLoaderTask()
{
}

std::string TileLoaderTask::cachePathForTile(const TileCoordinateKey& key) const
{
//    std::string fileExtension = MediaTypeMap::getDefault()->getBestFileExtensionsForMediaType(Poco::Net::MediaType(fileType));



    std::filesystem::path path = ofToDataPath("cache", true);
    path /= key.providerId();
    path /= key.setId();
    path /= std::to_string(key.zoom());
    path /= std::to_string(key.column());
    path /= std::to_string(key.row());
    path.replace_extension(".jpg");
    return path.string();
}


void TileLoaderTask::runTask()
{
    try
    {
        std::string path = cachePathForTile(_request.key());

        ofFile file(path);

        if (file.exists())
        {
            ofBuffer buffer = ofBufferFromFile(path);

            loadCacheAndNotify(buffer,
                               Poco::Net::NameValueCollection(),
                               TileLoaderResponse::CacheStatus::CACHE_HIT);
        }
        else
        {
            // Launch a thread to go get it!
            Poco::URI uri = Poco::URI(_request.uri());

            if (uri.getScheme() == "http" || uri.getScheme() == "https")
            {
                HTTP::Client client;
                HTTP::Context context;
                HTTP::GetRequest request(_request.uri());
                auto l0 = context.events.onHTTPClientStateChange.newListener(this, &TileLoaderTask::onHTTPClientStateChange);
                auto l1 = context.events.onHTTPClientRequestProgress.newListener(this, &TileLoaderTask::onHTTPClientRequestProgress);
                auto l2 = context.events.onHTTPClientResponseProgress.newListener(this, &TileLoaderTask::onHTTPClientResponseProgress);
                auto l3 = context.events.onHTTPClientResponseStream.newListener(this, &TileLoaderTask::onHTTPClientResponseStream);
                auto l4 = context.events.onHTTPClientError.newListener(this, &TileLoaderTask::onHTTPClientError);
                client.submit(context, request);
            }
            //        else if (uri.getScheme() == "sqlite")
            //        {
            //            std::cout << "need to grab a db connection." << std::endl;
            //        }
            else
            {
                throw Poco::UnknownURISchemeException("Unknown URI scheme: " + uri.getScheme());
            }
        }
    }
    catch (const Poco::SyntaxException& exc)
    {
        ofLogError("TileLoader::get") << exc.displayText();
    }
}


bool TileLoaderTask::onHTTPClientStateChange(HTTP::ClientStateChangeEventArgs& args)
{
    return true;
}


bool TileLoaderTask::onHTTPClientRequestProgress(HTTP::ClientRequestProgressEventArgs& args)
{
    float progress = args.progress().progress();

    if (progress == Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH)
    {
        setProgress(0);
    }
    else
    {
        setProgress(progress);
    }

    return true;
}


bool TileLoaderTask::onHTTPClientResponseProgress(HTTP::ClientResponseProgressEventArgs& args)
{
    float progress = args.progress().progress();

    if (progress == Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH)
    {
        setProgress(0);
    }
    else
    {
        setProgress(progress);
    }

    return true;
}



void TileLoaderTask::loadCacheAndNotify(const ofBuffer& buffer,
                                        const Poco::Net::NameValueCollection& headers,
                                        TileLoaderResponse::CacheStatus cacheStatus)
{
    ofPixels pixels;

    if (!ofLoadImage(pixels, buffer))
    {
        // TODO and this was loaded from the cache, we need to make sure the cache is cleaned.
        throw Poco::IOException("Error loading pixels from buffer.");
    }

    auto tile = std::make_shared<Tile>(pixels);

    auto notification = new Poco::TaskCustomNotification<TileLoaderResponse>(this,
                                                                             TileLoaderResponse(_request,
                                                                                                tile,
                                                                                                buffer,
                                                                                                headers,
                                                                                                cacheStatus));
    if (cacheStatus != TileLoaderResponse::CacheStatus::CACHE_HIT)
    {
        std::string path = cachePathForTile(_request.key());

        if (!ofBufferToFile(path, buffer))
        {
            throw Poco::IOException("Error caching pixels to disk.");
        }
    }

    postNotification(notification);
}



bool TileLoaderTask::onHTTPClientResponseStream(HTTP::ClientResponseStreamEventArgs& args)
{
    ofBuffer buffer(args.stream());
    Poco::Net::MediaType mediaType(args.response().getContentType());
    if (mediaType.matches("image"))
    {
        loadCacheAndNotify(buffer,
                           args.request(),
                           TileLoaderResponse::CacheStatus::CACHE_MISS);
        return true;
    }
    else
    {
        throw Poco::IOException("Unknown content type: " + mediaType.toString());
    }    
}


bool TileLoaderTask::onHTTPClientError(HTTP::ClientErrorEventArgs& args)
{
    // We throw an exception to be caught by the task manager.
    throw args.exception();
    
    // Unreachable.
    return true;
}



//    std::string DiskCache::cachePathForTile(const std::string& providerId,
//                                            const TileCoordinate& coordinate,
//                                            const std::string& fileType)
//    {
//        std::string fileExtension = MediaTypeMap::getDefault()->getBestFileExtensionsForMediaType(Poco::Net::MediaType(fileType));
//        std::filesystem::path path = ofToDataPath(_cachePath, true);
//        path /= providerId;
//        path /= coordinate.getId();
//        path /= std::to_string(static_cast<int>(std::floor(coordinate.getZoom())));
//        path /= std::to_string(static_cast<int>(std::floor(coordinate.getRow())));
//        path /= std::to_string(static_cast<int>(std::floor(coordinate.getColumn())));
//        path.replace_extension(fileExtension);
//        return path.string();
//    }

//DiskCache::DiskCache(const std::string& cacheLocation,
//                     const std::string& pathTemplate):
//    _cacheLocation(cacheLocation),
//    _pathTemplate(pathTemplate)
//{
//}
//



const std::size_t TileLoader::DEFAULT_CACHE_SIZE = 1024;


TileLoader::TileLoader(): TileLoader(std::make_shared<TaskQueue>())
{
}


TileLoader::TileLoader(std::shared_ptr<TaskQueue> taskQueue):
    TileLoader(DEFAULT_CACHE_SIZE,
              //DEFAULT_CACHE_PATH,
              taskQueue)
{
}


TileLoader::TileLoader(std::size_t cacheSize,
//                       const std::string& cachePath,
                       std::shared_ptr<TaskQueue> taskQueue):
    _cache(cacheSize),
//    _cachePath(cachePath),
    _taskQueue(taskQueue),
    _onTaskCustomNotificationListener(_taskQueue->onTaskCustomNotification.newListener(this, &TileLoader::onTaskCustomNotification)),
    _onTaskCancelledListener(_taskQueue->onTaskCancelled.newListener(this, &TileLoader::onTaskCancelledNotification))

{
    _cache.Add    += Poco::delegate(this, &TileLoader::onAdd);
    _cache.Update += Poco::delegate(this, &TileLoader::onUpdate);
    _cache.Remove += Poco::delegate(this, &TileLoader::onRemove);
    _cache.Get    += Poco::delegate(this, &TileLoader::onGet);
    _cache.Clear  += Poco::delegate(this, &TileLoader::onClear);
}


TileLoader::~TileLoader()
{
    _cache.Add    -= Poco::delegate(this, &TileLoader::onAdd);
    _cache.Update -= Poco::delegate(this, &TileLoader::onUpdate);
    _cache.Remove -= Poco::delegate(this, &TileLoader::onRemove);
    _cache.Get    -= Poco::delegate(this, &TileLoader::onGet);
    _cache.Clear  -= Poco::delegate(this, &TileLoader::onClear);
}


bool TileLoader::has(const TileCoordinateKey& key) const
{
    return _cache.has(key);
}


std::shared_ptr<Tile> TileLoader::get(const TileCoordinateKey& key) const
{
    return _cache.get(key);
}


std::string TileLoader::request(const TileLoaderRequest& request)
{
    return _taskQueue->start(TileCoordinateUtils::hash(request.key()), new TileLoaderTask(request));
}


bool TileLoader::cancelQueuedRequest(const std::string& requestId)
{
    try
    {
        _taskQueue->cancelQueued(requestId);
    }
    catch (const Poco::NotFoundException& exc)
    {
    }
}


bool TileLoader::onTaskCustomNotification(const TaskCustomNotificationEventArgs& args)
{
    TileLoaderResponse response;

    if (args.extract<TileLoaderResponse>(response))
    {
        // Add it to the ram cache.
        response.tile()->loadTexture();
        _cache.add(response.key(), response.tile());
        return true;
    }
    else
    {
        return false;
    }
}


bool TileLoader::onTaskCancelledNotification(const TaskQueueEventArgs& args)
{
    ofNotifyEvent(onTaskCancelled, args.taskId(), this);
    return false;
}


void TileLoader::onAdd(const Poco::KeyValueArgs<TileCoordinateKey, Tile>& args)
{
    ofNotifyEvent(onTileCached, args.key(), this);
}


void TileLoader::onUpdate(const Poco::KeyValueArgs<TileCoordinateKey, Tile>& args)
{
}


void TileLoader::onRemove(const TileCoordinateKey& args)
{
    ofNotifyEvent(onTileUncached, args, this);
}


void TileLoader::onGet(const TileCoordinateKey& args)
{
}


void TileLoader::onClear(const Poco::EventArgs& args)
{
}



    //
    //
    //SharedTile TileLoader::getTile(const TileCoordinate& coordinate)
    //{
    //    if (_LRUTileCache.has(coordinate))
    //    {
    //        return _LRUTileCache.get(coordinate);
    //    }
    //    else
    //    {
    //        return nullptr;
    //    }
    //}
    //
    //
    //std::size_t TileLoader::size()
    //{
    //    return _LRUTileCache.size();
    //}
    //
    //
    //void TileLoader::clear()
    //{
    //    return _LRUTileCache.clear();
    //}
    //
    //
    //void TileLoader::handleTaskCustomNotification(const TileCoordinate& taskID,
    //                                              TaskNotificationPtr pNotification)
    //{
    //    Poco::AutoPtr<Poco::TaskCustomNotification<SharedTile>> taskCustomNotification = 0;
    //
    //    if (!(taskCustomNotification = pNotification.cast<Poco::TaskCustomNotification<SharedTile>>()).isNull())
    //    {
    //        SharedTile tile = taskCustomNotification->custom();
    //
    //        if (tile != nullptr && !_LRUTileCache.has(taskID))
    //        {
    //            tile->setUseTexture(true);
    //            tile->update();
    //
    //            _LRUTileCache.add(taskID, tile);
    //        }
    //    }
    //    else
    //    {
    //        TaskQueue_<TileCoordinate>::handleTaskCustomNotification(taskID, pNotification);
    //    }
    //}
    //
    //
    //
    //
    //void TileLoader::onTaskQueued(const TaskQueueEventArgs_<TileCoordinate>& args)
    //{
    //}
    //
    //
    //void TileLoader::onTaskStarted(const TaskQueueEventArgs_<TileCoordinate>& args)
    //{
    //}
    //
    //
    //void TileLoader::onTaskCancelled(const TaskQueueEventArgs_<TileCoordinate>& args)
    //{
    //}
    //
    //
    //void TileLoader::onTaskFinished(const TaskQueueEventArgs_<TileCoordinate>& args)
    //{
    //}
    //
    //
    //void TileLoader::onTaskFailed(const ofx::TaskFailedEventArgs_<TileCoordinate>& args)
    //{
    //    ofLogError("TileLoader::onTaskFailed") << args.taskName();
    //    ofLogError("TileLoader::onTaskFailed") << args.getException().displayText();
    //}
    //
    //
    //void TileLoader::onTaskProgress(const ofx::TaskProgressEventArgs_<TileCoordinate>& args)
    //{
    //}


} } // namespace ofx::Maps
