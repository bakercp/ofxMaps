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


#include "ofx/LRUCache.h"
#include "ofx/TaskQueue.h"
#include "ofx/Maps/AbstractMapTypes.h"
#include "ofx/Maps/Tile.h"
#include "ofx/Maps/TileCoordinate.h"


namespace ofx {
namespace Maps {


/// \brief A TileLoader with default settings.
template<typename ProviderType>
class TileLoader: public TaskQueue_<TileCoordinate>
{
public:
    TileLoader(long LRUCacheSize = DEFAULT_LRU_CACHE_SIZE,
               int maxTasks = TaskQueue_<TileCoordinate>::UNLIMITED_TASKS,
               Poco::ThreadPool& threadPool = Poco::ThreadPool::defaultPool());

    /// \brief Destroy the TileLoader.
    virtual ~TileLoader();

    SharedTile getTile(const TileCoordinate& coordinate);

    enum
    {
        DEFAULT_LRU_CACHE_SIZE = 128
    };

    void onTaskQueued(const ofx::TaskQueueEventArgs_<TileCoordinate>& args);
    void onTaskStarted(const ofx::TaskQueueEventArgs_<TileCoordinate>& args);
    void onTaskCancelled(const ofx::TaskQueueEventArgs_<TileCoordinate>& args);
    void onTaskFinished(const ofx::TaskQueueEventArgs_<TileCoordinate>& args);
    void onTaskFailed(const ofx::TaskFailedEventArgs_<TileCoordinate>& args);
    void onTaskProgress(const ofx::TaskProgressEventArgs_<TileCoordinate>& args);


protected:
    void handleTaskCustomNotification(const TileCoordinate& taskID,
                                      TaskNotificationPtr pNotification);

    void onAdd(const Poco::KeyValueArgs<TileCoordinate, Tile>& args);
    void onUpdate(const Poco::KeyValueArgs<TileCoordinate, Tile>& args);
    void onRemove(const TileCoordinate& args);
    void onGet(const TileCoordinate& args);
    void onClear(const Poco::EventArgs& args);

    virtual SharedTile getDefaultTile(const TileCoordinate& coordinate);

    ofx::LRUCache<TileCoordinate, Tile> _LRUTileCache;

    ProviderType _provider;

    SharedTile _defaultTile;

};


template<typename ProviderType>
TileLoader<ProviderType>::TileLoader(long LRUCacheSize,
                                     int maxTasks,
                                     Poco::ThreadPool& threadPool):
    TaskQueue_<TileCoordinate>(maxTasks, threadPool),
    _LRUTileCache(LRUCacheSize)
{
    _LRUTileCache.Add    += Poco::delegate(this, &TileLoader::onAdd);
    _LRUTileCache.Update += Poco::delegate(this, &TileLoader::onUpdate);
    _LRUTileCache.Remove += Poco::delegate(this, &TileLoader::onRemove);
    _LRUTileCache.Get    += Poco::delegate(this, &TileLoader::onGet);
    _LRUTileCache.Clear  += Poco::delegate(this, &TileLoader::onClear);

    registerTaskProgressEvents(this);
}


template<typename ProviderType>
TileLoader<ProviderType>::~TileLoader()
{
    unregisterTaskProgressEvents(this);

    _LRUTileCache.Add    -= Poco::delegate(this, &TileLoader::onAdd);
    _LRUTileCache.Update -= Poco::delegate(this, &TileLoader::onUpdate);
    _LRUTileCache.Remove -= Poco::delegate(this, &TileLoader::onRemove);
    _LRUTileCache.Get    -= Poco::delegate(this, &TileLoader::onGet);
    _LRUTileCache.Clear  -= Poco::delegate(this, &TileLoader::onClear);
}


template<typename ProviderType>
SharedTile TileLoader<ProviderType>::getTile(const TileCoordinate& coordinate)
{
    SharedTile tile = _LRUTileCache.get(coordinate);

    if (tile)
    {
        return tile;
    }
    else
    {
        try
        {
            start(coordinate, _provider.requestTile(coordinate));
        }
        catch (const Poco::ExistsException& exc)
        {
        }

        return getDefaultTile(coordinate);
    }
}


template<typename ProviderType>
void TileLoader<ProviderType>::handleTaskCustomNotification(const TileCoordinate& taskID,
                                                            TaskNotificationPtr pNotification)
{
    Poco::AutoPtr<Poco::TaskCustomNotification<SharedTile> > taskCustomNotification = 0;

    if (!(taskCustomNotification = pNotification.cast<Poco::TaskCustomNotification<SharedTile> >()).isNull())
    {
        SharedTile tile = taskCustomNotification->custom();

        if (tile && !_LRUTileCache.has(taskID))
        {
            tile->setUseTexture(true);
            tile->update();

            _LRUTileCache.add(taskID, tile);
        }
    }
    else
    {
        TaskQueue_<TileCoordinate>::handleTaskCustomNotification(taskID, pNotification);
    }
}


template<typename ProviderType>
void TileLoader<ProviderType>::onAdd(const Poco::KeyValueArgs<TileCoordinate, Tile>& args)
{
    ofLogVerbose("TileLoader::onAdd") << "onAdd";
}


template<typename ProviderType>
void TileLoader<ProviderType>::onUpdate(const Poco::KeyValueArgs<TileCoordinate, Tile>& args)
{
    ofLogVerbose("TileLoader::onUpdate") << "onUpdate";
}


template<typename ProviderType>
void TileLoader<ProviderType>::onRemove(const TileCoordinate& args)
{
    ofLogVerbose("TileLoader::onRemove") << "onRemove";
}


template<typename ProviderType>
void TileLoader<ProviderType>::onGet(const TileCoordinate& args)
{
    ofLogVerbose("TileLoader::onGet") << "onGet";
}


template<typename ProviderType>
void TileLoader<ProviderType>::onClear(const Poco::EventArgs& args)
{
    ofLogVerbose("TileLoader::onClear") << "onClear";
}


template<typename ProviderType>
void TileLoader<ProviderType>::onTaskQueued(const ofx::TaskQueueEventArgs_<TileCoordinate>& args)
{
    ofLogError("TileLoader::onTaskQueued") << args.getTaskName();
}


template<typename ProviderType>
void TileLoader<ProviderType>::onTaskStarted(const ofx::TaskQueueEventArgs_<TileCoordinate>& args)
{
}

    
template<typename ProviderType>
void TileLoader<ProviderType>::onTaskCancelled(const ofx::TaskQueueEventArgs_<TileCoordinate>& args)
{
}

    
template<typename ProviderType>
void TileLoader<ProviderType>::onTaskFinished(const ofx::TaskQueueEventArgs_<TileCoordinate>& args)
{
}


template<typename ProviderType>
void TileLoader<ProviderType>::onTaskFailed(const ofx::TaskFailedEventArgs_<TileCoordinate>& args)
{
    ofLogError("TileLoader::onTaskFailed") << args.getException().displayText();
}

    
template<typename ProviderType>
void TileLoader<ProviderType>::onTaskProgress(const ofx::TaskProgressEventArgs_<TileCoordinate>& args)
{
}


template<typename ProviderType>
SharedTile TileLoader<ProviderType>::getDefaultTile(const TileCoordinate& coordinate)
{
    // Create a default empty tile.
    if (!_defaultTile)
    {
        ofPixels pixels;

        pixels.allocate(_provider.getTileWidth(),
                        _provider.getTileHeight(),
                        OF_PIXELS_RGBA);

        pixels.setColor(ofColor(255, 0, 0, 127));

        _defaultTile = SharedTile(new ofImage(pixels));
    }

    return _defaultTile;
}



} } // namespace ofx::Maps
