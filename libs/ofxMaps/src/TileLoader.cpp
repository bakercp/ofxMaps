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


#include "ofx/Maps/TileLoader.h"


namespace ofx {
namespace Maps {


TileLoader::TileLoader(long LRUCacheSize,
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


TileLoader::~TileLoader()
{
    unregisterTaskProgressEvents(this);

    _LRUTileCache.Add    -= Poco::delegate(this, &TileLoader::onAdd);
    _LRUTileCache.Update -= Poco::delegate(this, &TileLoader::onUpdate);
    _LRUTileCache.Remove -= Poco::delegate(this, &TileLoader::onRemove);
    _LRUTileCache.Get    -= Poco::delegate(this, &TileLoader::onGet);
    _LRUTileCache.Clear  -= Poco::delegate(this, &TileLoader::onClear);
}


SharedTile TileLoader::getTile(const TileCoordinate& coordinate)
{
    return _LRUTileCache.get(coordinate);
}


void TileLoader::handleTaskCustomNotification(const TileCoordinate& taskID,
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


void TileLoader::onAdd(const Poco::KeyValueArgs<TileCoordinate, Tile>& args)
{
    ofNotifyEvent(onTileCached, args.key(), this);
}


void TileLoader::onUpdate(const Poco::KeyValueArgs<TileCoordinate, Tile>& args)
{
}


void TileLoader::onRemove(const TileCoordinate& args)
{
    ofNotifyEvent(onTileUncached, args, this);
}


void TileLoader::onGet(const TileCoordinate& args)
{
}


void TileLoader::onClear(const Poco::EventArgs& args)
{
}


void TileLoader::onTaskQueued(const ofx::TaskQueueEventArgs_<TileCoordinate>& args)
{
}


void TileLoader::onTaskStarted(const ofx::TaskQueueEventArgs_<TileCoordinate>& args)
{
}


void TileLoader::onTaskCancelled(const ofx::TaskQueueEventArgs_<TileCoordinate>& args)
{
}


void TileLoader::onTaskFinished(const ofx::TaskQueueEventArgs_<TileCoordinate>& args)
{
}


void TileLoader::onTaskFailed(const ofx::TaskFailedEventArgs_<TileCoordinate>& args)
{
    ofLogError("TileLoader::onTaskFailed") << args.getTaskName();
    ofLogError("TileLoader::onTaskFailed") << args.getException().displayText();
}


void TileLoader::onTaskProgress(const ofx::TaskProgressEventArgs_<TileCoordinate>& args)
{
}


} } // namespace ofx::Maps
