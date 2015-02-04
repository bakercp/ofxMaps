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
#include "ofx/Maps/QueueSorter.h"


namespace ofx {
namespace Maps {


/// \brief A TileLoader with default settings.
class TileLoader: public TaskQueue_<TileCoordinate>
{
public:
    TileLoader(long LRUCacheSize = DEFAULT_LRU_CACHE_SIZE,
               int maxTasks = TaskQueue_<TileCoordinate>::UNLIMITED_TASKS,
               Poco::ThreadPool& threadPool = Poco::ThreadPool::defaultPool());

    /// \brief Destroy the TileLoader.
    virtual ~TileLoader();

    SharedTile getTile(const TileCoordinate& coordinate);

    ofEvent<const TileCoordinate> onTileCached;
    ofEvent<const TileCoordinate> onTileUncached;

    enum
    {
        DEFAULT_LRU_CACHE_SIZE = 2048
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

    ofx::LRUCache<TileCoordinate, Tile> _LRUTileCache;

};


} } // namespace ofx::Maps
