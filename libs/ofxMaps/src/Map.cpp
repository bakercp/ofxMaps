#include "ofx/Maps/Map.h"
#include "ofx/Maps/QueueSorter.h"


namespace ofx {
namespace Maps {


Map::Map():
    _maxPending(DEFAULT_MAX_PENDING),
    _maxImagesToCache(DEFAULT_MAX_IMAGES_TO_CACHE),
    _gridPadding(DEFAULT_GRID_PADDING),
    _lastClickTime(0),
    _numDrawnImages(0)
{
    ofRegisterMouseEvents(this);
    ofRegisterKeyEvents(this);
    ofAddListener(ofEvents().update, this, &Map::update);

    ofRegisterURLNotification(this);
}

Map::~Map()
{
    ofUnregisterMouseEvents(this);
    ofUnregisterKeyEvents(this);
    ofRemoveListener(ofEvents().update, this, &Map::update);

    ofUnregisterURLNotification(this);
}


void Map::setup(BaseMapProvider::SharedPtr provider, int width, int height)
{
	_provider = provider;

    // The width / height of the map.
    _size = ofVec2d(width, height);

    // Half the world width, height at zoom 0
    _centerTileCoordinate = TileCoordinate(0.5, 0.5, 0);

    double z = log2(std::min(_size.x, _size.y) / _provider->getTileWidth());

    _centerTileCoordinate = _centerTileCoordinate.zoomTo(z);

    // Start with north up.
    _rotation = 0;

}


void Map::update(ofEventArgs& args)
{
    // If we're in between zoom levels, we need to choose the nearest.
	int baseZoom = CLAMP(static_cast<int>(round(_centerTileCoordinate.getZoom())),
                         _provider->getMinZoom(),
                         _provider->getMaxZoom());

	// These are the top left and bottom right tile coordinates.
	// We'll be loading everything in between:
	TileCoordinate tl = pointToTileCoordinate(ofVec2d::zero()).zoomTo(baseZoom);
	TileCoordinate tr = pointToTileCoordinate(ofVec2d(_size.x, 0)).zoomTo(baseZoom);
	TileCoordinate bl = pointToTileCoordinate(ofVec2d(0, _size.y)).zoomTo(baseZoom);
	TileCoordinate br = pointToTileCoordinate(_size).zoomTo(baseZoom);

	// Find start and end columns
	int minCol = floor(std::min(std::min(tl.getColumn(),
                                         tr.getColumn()),
                                std::min(bl.getColumn(),
                                         br.getColumn())));

	int maxCol = floor(std::max(std::max(tl.getColumn(),
                                         tr.getColumn()),
                                std::max(bl.getColumn(),
                                         br.getColumn())));

	int minRow = floor(std::min(std::min(tl.getRow(),
                                         tr.getRow()),
                                std::min(bl.getRow(),
                                         br.getRow())));

	int maxRow = floor(std::max(std::max(tl.getRow(),
                                         tr.getRow()),
                                std::max(bl.getRow(),
                                         br.getRow())));

	minCol -= _gridPadding;
	minRow -= _gridPadding;
	maxCol += _gridPadding;
	maxRow += _gridPadding;

	_visibleCoordinates.clear();

    // grab coords for visible tiles
	for (int col = minCol; col <= maxCol; ++col)
    {
		for (int row = minRow; row <= maxRow; ++row)
        {
			TileCoordinate coord(row, col, baseZoom);

			// Keep this for later.
			_visibleCoordinates.insert(coord);

            // Do we have an image for this coordinate?
			if (0 == _images.count(coord))
            {
				// Fetch it if we don't have it
				requestTile(coord);

				// See if we have  a parent coord for this tile?
				bool gotParent = false;

                // Work backward from the current zoom level to check for
                // parents to display while we wait for children to be
                // downloaded.
				for (int i = static_cast<int>(coord.getZoom()); i > 0; --i)
                {
					TileCoordinate zoomed = coord.zoomTo(i).getFloored();

                    // mark all parent tiles valid
                    _visibleCoordinates.insert(zoomed);

                    gotParent = true;

					if (0 == _images.count(zoomed))
                    {
						// force load of parent tiles we don't already have
                        requestTile(coord);
					}
				}

				// or if we have any of the children
                //				if (!gotParent) {
                //					Coordinate zoomed = coord.zoomBy(1).container();
                //					std::vector<Coordinate> kids;
                //					kids.push_back(zoomed);
                //					kids.push_back(zoomed.right());
                //					kids.push_back(zoomed.down());
                //					kids.push_back(zoomed.right().down());
                //					for (int i = 0; i < kids.size(); i++) {
                //						if (images.count(kids[i]) > 0) {
                //							visibleKeys.insert(kids[i]);
                //						}
                //					}            
                //				}
				
			}
			
		} // rows
	} // columns

    // stop fetching things we can't see:
	// (visibleKeys also has the parents and children, if needed, but that shouldn't matter)
	//queue.retainAll(visibleKeys);

    std::vector<TileCoordinate>::iterator iter = _queue.begin();

    while (iter != _queue.end())
    {
        if (0 == _visibleCoordinates.count(*iter))
        {
			iter = _queue.erase(iter);
		}
		else
        {
			++iter;
		}
	}

	// TODO sort what's left by distance from center:
	//queueSorter.setCenter(new Coordinate( (minRow + maxRow) / 2.0f, (minCol + maxCol) / 2.0f, zoom));
	//Collections.sort(queue, queueSorter);

	processQueue();

	// clear some images away if we have too many...
	int numToKeep = std::max(_numDrawnImages, _maxImagesToCache);

	if (_recentImages.size() > numToKeep) {
		// first clear the pointers from recentImages
		_recentImages.erase(_recentImages.begin(), _recentImages.end()-numToKeep);
		//images.values().retainAll(recentImages);
		// TODO: re-think the stl collections used so that a simpler retainAll equivalent is available
		// now look in the images map and if the value is no longer in recent images then get rid of it
        std::map<TileCoordinate, std::shared_ptr<ofImage> >::iterator iter = _images.begin();

		while (iter != _images.end())
        {
			std::shared_ptr<ofImage> tile = iter->second;

            std::vector<std::shared_ptr<ofImage> >::iterator result = std::find(_recentImages.begin(),
                                                                                _recentImages.end(),
                                                                                tile);

			if (result == _recentImages.end())
            {
				_images.erase(iter++);
			}
			else
            {
				++iter;
			}
		}
	}
}

void Map::draw()
{
    // If we're in between zoom levels, we need to choose the nearest.
	int baseZoom = CLAMP(static_cast<int>(round(_centerTileCoordinate.getZoom())),
                         _provider->getMinZoom(),
                         _provider->getMaxZoom());

	// TODO: sort by zoom so we draw small zoom levels (big tiles) first:
	// can this be done with a different comparison function on the visibleKeys set?

	// Collections.sort(visibleKeys, zoomComparator);

    ofPushMatrix();
    ofRotateZ(_rotation * RAD_TO_DEG);

	_numDrawnImages = 0;

	std::set<TileCoordinate>::iterator coordinateIter = _visibleCoordinates.begin();

	while (coordinateIter != _visibleCoordinates.end())
    {
		TileCoordinate coord = *coordinateIter;

		double scale = pow(2.0, _centerTileCoordinate.getZoom() - coord.getZoom());

        ofVec2d tileSize = _provider->getTileSize() * scale;
		ofVec2d center = _size * 0.5;

		TileCoordinate theCoord = _centerTileCoordinate.zoomTo(coord.getZoom());

		double tx = center.x + (coord.getColumn() - theCoord.getColumn()) * tileSize.x;
		double ty = center.y + (coord.getRow() - theCoord.getRow()) * tileSize.y;

        if (_images.count(coord) > 0)
        {
            std::shared_ptr<ofImage> tile = _images[coord];

            // TODO: must be a cleaner C++ way to do this?
            // we want this image to be at the end of recentImages, if it's already there we'll remove it and then add it again
            std::vector<std::shared_ptr<ofImage> >::iterator result = find(_recentImages.begin(),
                                                                           _recentImages.end(),
                                                                           tile);

            if (result != _recentImages.end())
            {
                _recentImages.erase(result);
            }
            else
            {
                // if it's not in recent images it must be brand new?
                tile->setUseTexture(true);
                tile->update();
            }


            ofPushStyle();
            ofSetColor(255);
            
            tile->draw(tx,
                       ty,
                       tileSize.x,
                       tileSize.y);

            ofEnableAlphaBlending();
            ofNoFill();
            ofSetColor(255,0,0,50);
            ofRect(tx,
                   ty,
                   tileSize.x,
                   tileSize.y);

            ofPopStyle();

            _numDrawnImages++;

            _recentImages.push_back(tile);
        }

        ++coordinateIter;
    }

    ofPopMatrix();
	
}

void Map::keyPressed(ofKeyEventArgs& evt)
{
    int key = evt.key;

	if (key == '+' || key == '=')
    {
		if (getZoom() < 19)
        {
			zoomIn();
		}
	}
	else if (key == '-' || key == '_')
    {
		if (getZoom() > 1)
        {
			zoomOut();
		}
	}
	// TODO: keyboard movement
}

void Map::keyReleased(ofKeyEventArgs& evt)
{
}


void Map::mouseDragged(ofMouseEventArgs& evt)
{
//    ofVec2d mouse(evt.x, evt.y);
//
//    int button = evt.button;
//
//    ofVec2d dMouse = (mouse - _prevMouse) / _position.z;
//
//	if (button == 0)
//    {
//        _position += dMouse;
//	}
//
//    _prevMouse = mouse;
}


void Map::mouseMoved(ofMouseEventArgs& evt)
{
}


void Map::mousePressed(ofMouseEventArgs& evt)
{
    ofVec2d mouse(evt.x, evt.y);

    int button = evt.button;

    unsigned long long now = ofGetElapsedTimeMillis();
    unsigned long long dt = now - _lastClickTime;

    if (dt < DEFAULT_DOUBLE_CLICK_TIME)
    {
        // Should center.
        setPointCenter(mouse);
        zoomIn();
    }

    _lastClickTime = now;
    _prevMouse = mouse;

}
void Map::mouseReleased(ofMouseEventArgs& evt)
{
}


double Map::getZoom() const
{
    return _centerTileCoordinate.getZoom();
}


// sets scale according to given zoom level, should leave you with pixel perfect tiles
void Map::setZoom(double zoom)
{
    _centerTileCoordinate = _centerTileCoordinate.zoomTo(zoom);
}


void Map::zoom(double zoomStep)
{
    setZoom(getZoom() + zoomStep);
}


void Map::zoomIn()
{
    setZoom(getZoom() + 1);
}

void Map::zoomOut()
{
    setZoom(getZoom() - 1);
}


void Map::panBy(const ofVec2d& delta)
{
    panBy(delta.x, delta.y);
}


void Map::panBy(double dx, double dy)
{
    double sinr = sin(_rotation);
    double cosr = cos(_rotation);
	double dxr = dx * cosr + dy * sinr;
	double dyr = dy * cosr - dx * sinr;
    ofVec2d tileSize = _provider->getTileSize();

	_centerTileCoordinate.column -= dxr / tileSize.x;
	_centerTileCoordinate.row    -= dyr / tileSize.y;
}


void Map::scaleBy(double s)
{
	scaleBy(s, _size * 0.5);
}


void Map::scaleBy(double s, const ofVec2d& c)
{
	scaleBy(s, c.x, c.y);
}


void Map::scaleBy(double s, double cx, double cy)
{
    const double prevRotation = _rotation;
	rotateBy(-prevRotation, cx, cy);
    ofVec2d center = _size * 0.5;
	panBy(-cx + center.x, -cy + center.y);
	_centerTileCoordinate = _centerTileCoordinate.zoomBy(log2(s));
	panBy(cx - center.x, cy - center.y);
	rotateBy(prevRotation, cx, cy);
}


void Map::rotateBy(double r, double cx, double cy)
{
	panBy(-cx, -cy);
	_rotation += r;
	panBy(cx, cy);
}


Geo::Coordinate Map::getGeoLocationCenter() const
{
	return _provider->tileCoordinateToGeoCoordinate(_centerTileCoordinate);
}


TileCoordinate Map::getTileCoordinateCenter() const
{
    return _centerTileCoordinate;
}


void Map::setPointCenter(const ofVec2d& center)
{
    setTileCoordinateCenter(pointToTileCoordinate(center));
}


void Map::setGeoLocationCenter(const Geo::Coordinate& location)
{
    setTileCoordinateCenter(_provider->geoCoordinateToTileCoordinate(location).zoomTo(getZoom()));
}


void Map::setTileCoordinateCenter(const TileCoordinate& center)
{
	_centerTileCoordinate = center;
}


TileCoordinate Map::pointToTileCoordinate(const ofVec2d& point) const
{
    /* Return a coordinate on the map image for a given x, y point. */
	// new point coordinate reflecting distance from map center, in tile widths
	ofVec2d rotated(point);

    const ofVec2d tileSize = _provider->getTileSize();

    rotated.rotate(-_rotation * RAD_TO_DEG);

	TileCoordinate coord(_centerTileCoordinate);

    coord += (rotated - (_size / 2.0)) / tileSize;

	return coord;
}


ofVec2d Map::tileCoordinateToPoint(const TileCoordinate& target) const
{
	TileCoordinate coord = target;

	if(coord.getZoom() != _centerTileCoordinate.getZoom())
    {
		coord = coord.zoomTo(_centerTileCoordinate.getZoom());
	}

	// distance from the center of the map
    const ofVec2d tileSize = _provider->getTileSize();

    ofVec2d point = _size * 0.5;

	point.x += tileSize.x * (coord.getColumn() - _centerTileCoordinate.getColumn());
	point.y += tileSize.y * (coord.getRow() - _centerTileCoordinate.getRow());

	ofVec2d rotated(point);

    rotated.rotate(_rotation * RAD_TO_DEG);

	return rotated;
}


ofVec2d Map::geoLocationToPoint(const Geo::Coordinate& location) const
{
	return tileCoordinateToPoint(_provider->geoCoordinateToTileCoordinate(location));
}


Geo::Coordinate Map::pointToGeolocation(const ofVec2d& point) const
{
	return _provider->tileCoordinateToGeoCoordinate(pointToTileCoordinate(point));
}


void Map::requestTile(const TileCoordinate& coord)
{
	bool isPending = _pending.count(coord) > 0;
	bool isQueued = std::find(_queue.begin(), _queue.end(), coord) != _queue.end();
	bool isAlreadyLoaded = _images.count(coord) > 0;

	if (!isPending && !isQueued && !isAlreadyLoaded)
    {
		_queue.push_back(coord);
	}
}


void Map::processQueue()
{
	if (_queue.size() > (_maxPending - _pending.size()))
    {
        std::sort(_queue.begin(),
                  _queue.end(),
                  QueueSorter(getTileCoordinateCenter().zoomTo(getZoom())));
	}

    // Transfer load queued coordinates.
    while (_pending.size() < _maxPending && !_queue.empty())
    {
		const TileCoordinate& coord = *(_queue.begin());

        std::vector<std::string> urls = _provider->getTileUrls(coord);

        if (!urls.empty())
        {
            _pending[coord] = ofLoadURLAsync(urls[0]);
        }
        else
        {

        }

		_queue.erase(_queue.begin());
	}  
}


void Map::urlResponse(ofHttpResponse& args)
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


} } // namespace ofx::Maps
