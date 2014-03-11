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


#include "TileStore.h"



void TileStore::queueTile(const TileCoordinate& coord)
{
//    bool isPending = _pending.count(coord) > 0;
//    bool isQueued = std::find(_queue.begin(), _queue.end(), coord) != _queue.end();
//    bool isAlreadyLoaded = _images.count(coord) > 0;
//
//    if (!isPending && !isQueued && !isAlreadyLoaded)
//    {
//        _queue.push_back(coord);
//    }
}


void TileStore::urlResponse(ofHttpResponse& args)
{
    std::map<TileCoordinate, int>::iterator iter = _pending.begin();

    // Find our async id.
    while (iter != _pending.end())
    {
        if ((*iter).second == args.request.getID())
        {
            if (200 == args.status)
            {
                const TileCoordinate& coord = (*iter).first;
                _images[coord] = std::shared_ptr<ofImage>(new ofImage());
                _images[coord]->setUseTexture(false);
                _images[coord]->loadImage(args);
            }
            else
            {
                ofLogError("Map::urlResponse") << " : " << args.status << " : " << args.error << " : " << args.request.url;
            }

            _pending.erase(iter);
            break;
        }

        ++iter;
    }
}
