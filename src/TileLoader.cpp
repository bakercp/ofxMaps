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


#include "AbstractTileProvider.h"
#include "TileLoader.h"
#include "Map.h"

void TileLoader::start(const Coordinate& _coord, AbstractTileProvider* _provider, Map *_map) {

	coord = _coord;
	provider = _provider;
	theMap = _map;
	
//	sws.threadedStreamReady += Delegate<TileLoader, StreamEventArgs>(this, &TileLoader::onThreadedStreamReceived);

    ofRegisterURLNotification(this);


    std::vector<std::string> urls = provider->getTileUrls(coord);

	if (urls.size() > 0)
    {
		// TODO: more than one URL?
		// sws.getStreamThreaded(urls[0]);

        requestId = ofLoadURLAsync(urls[0]);
	}
	else
    {
		_map->tileDone(coord, 0);
	}
}


void TileLoader::urlResponse(ofHttpResponse& args)
{
    if (args.request.getID() == requestId)
    {
        ofUnregisterURLNotification(this);
        ofImage* threadedMemImage = new ofImage();
        threadedMemImage->setUseTexture(false); // we can't use a texture with this one
        threadedMemImage->loadImage(args);
        theMap->tileDone(coord, threadedMemImage);
    }
}


//
//void TileLoader::onThreadedStreamReceived(const void* pSender, StreamEventArgs & args_) {
//
//	ofMemoryImage* threadedMemImage = new ofMemoryImage();
//	threadedMemImage->setUseTexture(false); // we can't use a texture with this one	
//	threadedMemImage->loadFromData(args_.buff,args_.bytesToRead);
//	// cleanup:
//	delete args_.buff;
//	
//	theMap->tileDone(coord, threadedMemImage);
//
//}
