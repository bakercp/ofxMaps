#include "ofx/Maps/BaseTileLayer.h"
#include "ofx/Maps/QueueSorter.h"


namespace ofx {
namespace Maps {


BaseTileLayer::BaseTileLayer(BaseTileProvider& provider, double width, double height):
    _provider(provider),
    _width(width),
    _height(height)
{
    // Half the world width, height at zoom 0
    _center = TileCoordinate(0.5, 0.5, 0);

    double z = log2(std::min(_width, _height) / _provider.getTileWidth());

    _center = _center.zoomTo(z);
}


BaseTileLayer::~BaseTileLayer()
{
}


void BaseTileLayer::draw(float x, float y)
{
    draw(x, y, _width, _height);
}


void BaseTileLayer::draw(float x, float y, float w, float h)
{
//    getVisibleCoordinates();
}


ofVec2d BaseTileLayer::getSize() const
{
    return ofVec2d(_width, _height);
}


float BaseTileLayer::getWidth() const
{
    return _width;
}


void BaseTileLayer::setWidth(double width)
{
    _width = width;
}


float BaseTileLayer::getHeight() const
{
    return _height;
}


void BaseTileLayer::setHeight(double height)
{
    _height = height;
}


const TileCoordinate& BaseTileLayer::getCenter() const
{
    return _center;
}


void BaseTileLayer::setCenter(const TileCoordinate& center)
{
    _center = center;
}


std::set<TileCoordinate> BaseTileLayer::getVisibleCoordinates() const
{
    std::set<TileCoordinate> coordinates;

    // If we're in between zoom levels, we need to choose the nearest.
	int baseZoom = CLAMP(static_cast<int>(round(_center.getZoom())),
                         _provider.getMinZoom(),
                         _provider.getMaxZoom());

	// These are the top left and bottom right tile coordinates.
	// We'll be loading everything in between:

    ofVec2d topLeftPoint = ofVec2d(0, 0);
    ofVec2d topRightPoint = ofVec2d(_width, 0);
    ofVec2d bottomLeftPoint = ofVec2d(0, _height);
    ofVec2d bottomRightPoint = ofVec2d(_width, _height);

	TileCoordinate topLeft = pointToTileCoordinate(topLeftPoint).zoomTo(baseZoom);
	TileCoordinate topRight = pointToTileCoordinate(topRightPoint).zoomTo(baseZoom);
	TileCoordinate bottomLeft = pointToTileCoordinate(bottomLeftPoint).zoomTo(baseZoom);
	TileCoordinate bottomRight = pointToTileCoordinate(bottomRightPoint).zoomTo(baseZoom);


	// Find start and end columns
	int minCol = std::floor(std::min(std::min(topLeft.getColumn(),
                                              topRight.getColumn()),
                                     std::min(bottomLeft.getColumn(),
                                              bottomRight.getColumn())));

	int maxCol = std::floor(std::max(std::max(topLeft.getColumn(),
                                              topRight.getColumn()),
                                     std::max(bottomLeft.getColumn(),
                                              bottomRight.getColumn())));

	int minRow = std::floor(std::min(std::min(topLeft.getRow(),
                                              topRight.getRow()),
                                     std::min(bottomLeft.getRow(),
                                              bottomRight.getRow())));

	int maxRow = std::floor(std::max(std::max(topLeft.getRow(),
                                              topRight.getRow()),
                                     std::max(bottomLeft.getRow(),
                                              bottomRight.getRow())));
//	minCol -= _gridPadding;
//	minRow -= _gridPadding;
//	maxCol += _gridPadding;
//	maxRow += _gridPadding;

	coordinates.clear();

//    std::cout << minCol << " " << maxCol << " " << minRow << " " << maxRow << std::endl;

    // grab coords for visible tiles
	for (int col = minCol; col <= maxCol; ++col)
    {
		for (int row = minRow; row <= maxRow; ++row)
        {
			TileCoordinate coord(row, col, baseZoom);
			// Keep this for later.
			coordinates.insert(coord);
		}
	}

    std::cout << "NUM=" << coordinates.size() << ">" << (maxCol - minCol) << ">" << (maxRow - minRow) << std::endl;

    return coordinates;
}



//void Map::update(ofEventArgs& args)
//{
//
//    // grab coords for visible tiles
//	for (int col = minCol; col <= maxCol; ++col)
//    {
//		for (int row = minRow; row <= maxRow; ++row)
//        {
//			TileCoordinate coord(row, col, baseZoom);
//
//			// Keep this for later.
//			_visibleCoordinates.insert(coord);
//
//            // Do we have an image for this coordinate?
//			if (0 == _images.count(coord))
//            {
//				// Fetch it if we don't have it
//				requestTile(coord);
//
//				// See if we have  a parent coord for this tile?
//				bool gotParent = false;
//
//                // Work backward from the current zoom level to check for
//                // parents to display while we wait for children to be
//                // downloaded.
//				for (int i = static_cast<int>(coord.getZoom()); i > 0; --i)
//                {
//					TileCoordinate zoomed = coord.zoomTo(i).getFloored();
//
//                    // mark all parent tiles valid
//                    _visibleCoordinates.insert(zoomed);
//
//                    gotParent = true;
//
//					if (0 == _images.count(zoomed))
//                    {
//						// force load of parent tiles we don't already have
//                        requestTile(coord);
//					}
//				}
//
//				// or if we have any of the children
//                //				if (!gotParent) {
//                //					Coordinate zoomed = coord.zoomBy(1).container();
//                //					std::vector<Coordinate> kids;
//                //					kids.push_back(zoomed);
//                //					kids.push_back(zoomed.right());
//                //					kids.push_back(zoomed.down());
//                //					kids.push_back(zoomed.right().down());
//                //					for (int i = 0; i < kids.size(); i++) {
//                //						if (images.count(kids[i]) > 0) {
//                //							visibleKeys.insert(kids[i]);
//                //						}
//                //					}            
//                //				}
//				
//			}
//			
//		} // rows
//	} // columns
//
//    // stop fetching things we can't see:
//	// (visibleKeys also has the parents and children, if needed, but that shouldn't matter)
//	//queue.retainAll(visibleKeys);
//
//    std::vector<TileCoordinate>::iterator iter = _queue.begin();
//
//    while (iter != _queue.end())
//    {
//        if (0 == _visibleCoordinates.count(*iter))
//        {
//			iter = _queue.erase(iter);
//		}
//		else
//        {
//			++iter;
//		}
//	}
//
//	// TODO sort what's left by distance from center:
//	//queueSorter.setCenter(new Coordinate( (minRow + maxRow) / 2.0f, (minCol + maxCol) / 2.0f, zoom));
//	//Collections.sort(queue, queueSorter);
//
//	processQueue();
//
//	// clear some images away if we have too many...
//	int numToKeep = std::max(_numDrawnImages, _maxImagesToCache);
//
//	if (_recentImages.size() > numToKeep) {
//		// first clear the pointers from recentImages
//		_recentImages.erase(_recentImages.begin(), _recentImages.end()-numToKeep);
//		//images.values().retainAll(recentImages);
//		// TODO: re-think the stl collections used so that a simpler retainAll equivalent is available
//		// now look in the images map and if the value is no longer in recent images then get rid of it
//        std::map<TileCoordinate, std::shared_ptr<ofImage> >::iterator iter = _images.begin();
//
//		while (iter != _images.end())
//        {
//			std::shared_ptr<ofImage> tile = iter->second;
//
//            std::vector<std::shared_ptr<ofImage> >::iterator result = std::find(_recentImages.begin(),
//                                                                                _recentImages.end(),
//                                                                                tile);
//
//			if (result == _recentImages.end())
//            {
//				_images.erase(iter++);
//			}
//			else
//            {
//				++iter;
//			}
//		}
//	}
//}

//void Map::draw()
//{
//    // If we're in between zoom levels, we need to choose the nearest.
//	int baseZoom = CLAMP(static_cast<int>(round(_centerTileCoordinate.getZoom())),
//                         _provider->getMinZoom(),
//                         _provider->getMaxZoom());
//
//	// TODO: sort by zoom so we draw small zoom levels (big tiles) first:
//	// can this be done with a different comparison function on the visibleKeys set?
//
//	// Collections.sort(visibleKeys, zoomComparator);
//
//    ofPushMatrix();
//    ofRotateZ(_rotation * RAD_TO_DEG);
//
//	_numDrawnImages = 0;
//
//	std::set<TileCoordinate>::iterator coordinateIter = _visibleCoordinates.begin();
//
//	while (coordinateIter != _visibleCoordinates.end())
//    {
//		TileCoordinate coord = *coordinateIter;
//
//		double scale = pow(2.0, _centerTileCoordinate.getZoom() - coord.getZoom());
//
//        ofVec2d tileSize(_provider->getTileWidth(), _provider->getTileHeight());
//
//        tileSize *= scale;
//
//		ofVec2d center = _size * 0.5;
//
//		TileCoordinate theCoord = _centerTileCoordinate.zoomTo(coord.getZoom());
//
//		double tx = center.x + (coord.getColumn() - theCoord.getColumn()) * tileSize.x;
//		double ty = center.y + (coord.getRow() - theCoord.getRow()) * tileSize.y;
//
//        if (_images.count(coord) > 0)
//        {
//            std::shared_ptr<ofImage> tile = _images[coord];
//
//            // TODO: must be a cleaner C++ way to do this?
//            // we want this image to be at the end of recentImages, if it's already there we'll remove it and then add it again
//            std::vector<std::shared_ptr<ofImage> >::iterator result = find(_recentImages.begin(),
//                                                                           _recentImages.end(),
//                                                                           tile);
//
//            if (result != _recentImages.end())
//            {
//                _recentImages.erase(result);
//            }
//            else
//            {
//                // if it's not in recent images it must be brand new?
//                tile->setUseTexture(true);
//                tile->update();
//            }
//
//
//            ofPushStyle();
//            ofSetColor(255);
//            
//            tile->draw(tx,
//                       ty,
//                       tileSize.x,
//                       tileSize.y);
//
//            ofEnableAlphaBlending();
//            ofNoFill();
//            ofSetColor(255,0,0,50);
//            ofRect(tx,
//                   ty,
//                   tileSize.x,
//                   tileSize.y);
//
//            ofPopStyle();
//
//            _numDrawnImages++;
//
//            _recentImages.push_back(tile);
//        }
//
//        ++coordinateIter;
//    }
//
//    ofPopMatrix();
//	
//}




//void Map::requestTile(const TileCoordinate& coord)
//{
//	bool isPending = _pending.count(coord) > 0;
//	bool isQueued = std::find(_queue.begin(), _queue.end(), coord) != _queue.end();
//	bool isAlreadyLoaded = _images.count(coord) > 0;
//
//	if (!isPending && !isQueued && !isAlreadyLoaded)
//    {
//		_queue.push_back(coord);
//	}
//}


//void Map::processQueue()
//{
//	if (_queue.size() > (_maxPending - _pending.size()))
//    {
//        std::sort(_queue.begin(),
//                  _queue.end(),
//                  QueueSorter(getTileCoordinateCenter().zoomTo(getZoom())));
//	}
//
////    // Transfer load queued coordinates.
////    while (_pending.size() < _maxPending && !_queue.empty())
////    {
////		const TileCoordinate& coord = *(_queue.begin());
////
////        std::vector<std::string> urls = _provider->getTileUrls(coord);
////
////        if (!urls.empty())
////        {
////            _pending[coord] = ofLoadURLAsync(urls[0]);
////        }
////        else
////        {
////
////        }
////
////		_queue.erase(_queue.begin());
////	}  
//}
//
//
//void Map::urlResponse(ofHttpResponse& args)
//{
//    std::map<TileCoordinate, int>::iterator iter = _pending.begin();
//
//    // Find our async id.
//    while (iter != _pending.end())
//    {
//        if ((*iter).second == args.request.getID())
//        {
//            if (200 == args.status)
//            {
//                const TileCoordinate& coord = (*iter).first;
//                _images[coord] = std::shared_ptr<ofImage>(new ofImage());
//                _images[coord]->setUseTexture(false);
//                _images[coord]->loadImage(args);
//            }
//            else
//            {
//                ofLogError("Map::urlResponse") << " : " << args.status << " : " << args.error << " : " << args.request.url;
//            }
//
//            _pending.erase(iter);
//            break;
//        }
//
//        ++iter;
//    }
//}


} } // namespace ofx::Maps
