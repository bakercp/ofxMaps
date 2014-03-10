#include "Map.h"
#include "QueueSorter.h"


Map::Map():
    _tileSize(DEFAULT_TILE_SIZE),
    _maxPending(DEFAULT_MAX_PENDING),
    _maxImagesToCache(DEFAULT_MAX_IMAGES_TO_CACHE),
    _gridPadding(DEFAULT_GRID_PADDING),
    _lastClickTime(0)
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


void Map::setup(AbstractMapProvider::SharedPtr provider, int width, int height)
{
	_provider = provider;
    _size = ofVec2f(width, height);

	_position.x = - _tileSize / 2; // half the world width, at zoom 0
	_position.y = - _tileSize / 2; // half the world height, at zoom 0

	_position.z = std::ceil(std::min(_size.y / _tileSize,
                                     _size.x / _tileSize));

}


void Map::update(ofEventArgs& args)
{
    // if we're in between zoom levels, we need to choose the nearest:
	int baseZoom = _provider->bestZoomForScale(_position.z);


	// these are the top left and bottom right tile coordinates
	// we'll be loading everything in between:
	TileCoordinate startCoord = pointToTileCoordinate(ofVec2d(0, 0)).zoomTo(baseZoom).container();
	TileCoordinate endCoord = pointToTileCoordinate(_size).zoomTo(baseZoom).container().right().down();

	// find start and end columns
    // We pad our min / max coulms
	int minCol = startCoord.getColumn() - _gridPadding;
	int maxCol = endCoord.getColumn() + _gridPadding;
	int minRow = startCoord.getRow() - _gridPadding;
	int maxRow = endCoord.getRow() + _gridPadding;

	_visibleCoordinates.clear();

    // grab coords for visible tiles
	for (int col = minCol; col <= maxCol; col++)
    {
		for (int row = minRow; row <= maxRow; row++)
        {
			TileCoordinate coord(row, col, baseZoom);

			// keep this for later:
			_visibleCoordinates.insert(coord);

			if (_images.count(coord) == 0)
            {
				// fetch it if we don't have it
				requestTile(coord);

				// see if we have  a parent coord for this tile?
				bool gotParent = false;

				for (int i = static_cast<int>(coord.getZoom()); i > 0; i--)
                {
					TileCoordinate zoomed = coord.zoomTo(i).container();

                    if (_images.count(zoomed) > 0)
                    {
						_visibleCoordinates.insert(zoomed);
						gotParent = true;
						break;
					}
				}

				// or if we have any of the children
				if (!gotParent)
                {
					TileCoordinate zoomed = coord.zoomBy(1).container();

                    std::vector<TileCoordinate> kids;

                    kids.push_back(zoomed);
					kids.push_back(zoomed.right());
					kids.push_back(zoomed.down());
					kids.push_back(zoomed.right().down());

                    for (std::size_t i = 0; i < kids.size(); ++i)
                    {
						if (_images.count(kids[i]) > 0)
                        {
							_visibleCoordinates.insert(kids[i]);
						}
					}
				}
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
	int numToKeep = _maxImagesToCache;//std::max(numDrawnImages, _maxImagesToCache);

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
    // if we're in between zoom levels, we need to choose the nearest:
	int baseZoom = _provider->bestZoomForScale(_position.z);

	// TODO: sort by zoom so we draw small zoom levels (big tiles) first:
	// can this be done with a different comparison function on the visibleKeys set?

	// Collections.sort(visibleKeys, zoomComparator);
	
	// translate and scale, from the middle
	ofPushMatrix();
	ofTranslate(_size / 2);
	ofScale(_position.z, _position.z, 1);
	ofTranslate(_position.x, _position.y);
	
	int numDrawnImages = 0;
	
	if (_visibleCoordinates.size() > 0)
    {
		double prevZoom = baseZoom;

		ofPushMatrix();
		// correct the scale for this zoom level:

        double correction = 1.0 / pow(2.0, prevZoom);

        ofScale(correction, correction, 1);

        std::set<TileCoordinate>::iterator iter;

        for (iter = _visibleCoordinates.begin(); iter != _visibleCoordinates.end(); ++iter)
        {
			TileCoordinate coord = *iter;

			if (coord.getZoom() != prevZoom)
            {
				ofPopMatrix();
				ofPushMatrix();
				// correct the scale for this zoom level:
				correction = 1.0 / pow(2.0, coord.getZoom());

                ofScale(correction, correction, 1);

				prevZoom = coord.getZoom();
			}

            ofPushStyle();
            ofEnableAlphaBlending();
            ofNoFill();
            ofSetColor(255,0,0,50);
            ofRect(coord.getColumn() * _tileSize,
                   coord.getRow() * _tileSize,
                   _tileSize,
                   _tileSize);

            std::stringstream ss;

            ofSetColor(0,255,0);
            ofDrawBitmapString(ss.str(),
                               coord.getColumn() * _tileSize,
                               coord.getRow() * _tileSize);
            ofPushStyle();

            ofSetColor(255);

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

                tile->draw(coord.getColumn() * _tileSize,
                           coord.getRow() * _tileSize,
                           _tileSize,
                           _tileSize);

				numDrawnImages++;

				_recentImages.push_back(tile);
			}
		}

		ofPopMatrix();
	}    
	
	ofPopMatrix();

    ofSetColor(255, 0, 0);
    ofNoFill();
    ofRect(0, 0, _size.x, _size.y);
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
    ofVec2f mouse(evt.x, evt.y);

    int button = evt.button;

    ofVec2f dMouse = (mouse - _prevMouse) / _position.z;

	if (button == 0)
    {
        _position += dMouse;
	}

    _prevMouse = mouse;
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
        cout << "in here" << endl;
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


int Map::getZoom() const
{
	return _provider->bestZoomForScale(_position.z);
}


// sets scale according to given zoom level, should leave you with pixel perfect tiles
void Map::setZoom(int zoom)
{
    _position.z = pow(2.0, zoom);
}


void Map::zoom(int zoomStep)
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


GeoLocation Map::getGeoLocationCenter() const
{
	return _provider->tileCoordinateToGeoLocation(getTileCoordinateCenter());
}


TileCoordinate Map::getTileCoordinateCenter() const
{
	double row =    _position.y * _position.z / -_tileSize;
	double column = _position.x * _position.z / -_tileSize;
	double zoom =   _provider->zoomForScale(_position.z);

    return TileCoordinate(row, column, zoom);
}


void Map::setPointCenter(const ofVec2d& center)
{
    setTileCoordinateCenter(pointToTileCoordinate(center));
}


void Map::setGeoLocationCenter(const GeoLocation& location)
{
    setTileCoordinateCenter(pointToTileCoordinate(location).zoomTo(getZoom()));
}


void Map::setTileCoordinateCenter(const TileCoordinate& center)
{
    _position.z = pow(2.0, center.getZoom()); // First, calculate the zoom level.

	_position.x = -_tileSize * center.getColumn() / _position.z;
	_position.y = -_tileSize * center.getRow() / _position.z;
}


TileCoordinate Map::pointToTileCoordinate(const ofVec2d& point) const
{
	TileCoordinate center = getTileCoordinateCenter();
    center += (point - (_size / 2.0)) / _tileSize;
	return center;
}


ofVec2d Map::tileCoordinateToPoint(const TileCoordinate& coord) const
{
	TileCoordinate center = getTileCoordinateCenter();

    TileCoordinate newCoord;

	if(coord.getZoom() != center.getZoom())
    {
		newCoord = newCoord.zoomTo(center.getZoom());
	}
    else
    {
        newCoord = coord;
    }
	
	// distance from the center of the map
	ofVec2d point = _size / 2;

	point.x += _tileSize * (newCoord.getColumn() - center.getColumn());
	point.y += _tileSize * (newCoord.getRow() - center.getRow());
	
	return point;
}


ofVec2d Map::geoLocationToPoint(const GeoLocation& location) const
{
	return tileCoordinateToPoint(_provider->geoLocationToTileCoordinate(location));
}


GeoLocation Map::pointToGeolocation(const ofVec2d& point) const
{
	return _provider->tileCoordinateToGeoLocation(pointToTileCoordinate(point));
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
