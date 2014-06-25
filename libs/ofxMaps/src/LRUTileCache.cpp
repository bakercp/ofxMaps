// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
// Copyright (c) -2014 Tom Carden <https://github.com/RandomEtc>
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


#include "ofx/Maps/LRUTileCache.h"


namespace ofx {
namespace Maps {


LRUTileCache::LRUTileCache(long LRUCacheSize):
    _LRUCache(LRUCacheSize)
{
    _LRUCache.Add    += Poco::delegate(this, &LRUTileCache::onAdd);
    _LRUCache.Update += Poco::delegate(this, &LRUTileCache::onUpdate);
    _LRUCache.Remove += Poco::delegate(this, &LRUTileCache::onRemove);
    _LRUCache.Get    += Poco::delegate(this, &LRUTileCache::onGet);
    _LRUCache.Clear  += Poco::delegate(this, &LRUTileCache::onClear);
}


LRUTileCache::~LRUTileCache()
{
    _LRUCache.Add    -= Poco::delegate(this, &LRUTileCache::onAdd);
    _LRUCache.Update -= Poco::delegate(this, &LRUTileCache::onUpdate);
    _LRUCache.Remove -= Poco::delegate(this, &LRUTileCache::onRemove);
    _LRUCache.Get    -= Poco::delegate(this, &LRUTileCache::onGet);
    _LRUCache.Clear  -= Poco::delegate(this, &LRUTileCache::onClear);
}


bool LRUTileCache::putTile(const TileCoordinate& coordinate,
                           const SharedTile& tile)
{

}

SharedTile LRUTileCache::getTile(const TileCoordinate& coordinate)
{
    return _LRUCache.get(coordinate);
}


void LRUTileCache::clear()
{
    _LRUCache.clear();
}


void LRUTileCache::onAdd(const Poco::KeyValueArgs<TileCoordinate, Tile>& args)
{
    ofLogVerbose("BaseMapTileCache::onAdd") << "onAdd";
}


void LRUTileCache::onUpdate(const Poco::KeyValueArgs<TileCoordinate, Tile>& args)
{
    ofLogVerbose("BaseMapTileCache::onUpdate") << "onUpdate";
}


void LRUTileCache::onRemove(const TileCoordinate& args)
{
    ofLogVerbose("BaseMapTileCache::onRemove") << "onRemove";
}


void LRUTileCache::onGet(const TileCoordinate& args)
{
    ofLogVerbose("BaseMapTileCache::onGet") << "onGet";
}


void LRUTileCache::onClear(const Poco::EventArgs& args)
{
    ofLogVerbose("BaseMapTileCache::onClear") << "onClear";
}


} } // namespace ofx::Maps
