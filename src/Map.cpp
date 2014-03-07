#include "Map.h"
#include "QueueSorter.h"


const double Map::LOG_2 = log(2);


void Map::setup(AbstractTileProvider::SharedPtr _provider, int _width, int _height)
{
	provider = _provider;
	width = _width;
	height = _height;
	tx = - _tileSize / 2; // half the world width, at zoom 0
	ty = - _tileSize / 2; // half the world height, at zoom 0
	// fit to screen
    
	sc = std::ceil(std::min(height / _tileSize, width / _tileSize));

    panTargetX = tx;
    panTargetY = ty;
    scaleTarget = sc;

    smoothing = 0.8;

}

void Map::update(ofEventArgs& args) {
    double ep = 0.1;

    // TODO: this is a bit shakey at high zoom.
    tx = fabs(panTargetX - tx) > ep ? ((1 - smoothing) * panTargetX + smoothing * tx) : panTargetX;
    ty = fabs(panTargetY - ty) > ep ? ((1 - smoothing) * panTargetY + smoothing * ty) : panTargetY;
    sc = fabs(scaleTarget - sc) > ep ? ((1 - smoothing) * scaleTarget + smoothing * sc) : scaleTarget;

}

void Map::draw()
{
	// if we're in between zoom levels, we need to choose the nearest:
	int baseZoom = bestZoomForScale((float)sc);
	
	// these are the top left and bottom right tile coordinates
	// we'll be loading everything in between:
	Coordinate startCoord = pointCoordinate(Point2d(0,0)).zoomTo(baseZoom).container();
	Coordinate endCoord = pointCoordinate(Point2d(width,height)).zoomTo(baseZoom).container().right().down();
	
	// find start and end columns
	int minCol = startCoord.column;
	int maxCol = endCoord.column;
	int minRow = startCoord.row;
	int maxRow = endCoord.row;
	
	// pad a bit, for luck (well, because we might be zooming out between zoom levels)
	minCol -= _gridPadding;
	minRow -= _gridPadding;
	maxCol += _gridPadding;
	maxRow += _gridPadding;
	
	visibleKeys.clear();
	
	// grab coords for visible tiles
	for (int col = minCol; col <= maxCol; col++)
    {
		for (int row = minRow; row <= maxRow; row++)
        {
			
			// source coordinate wraps around the world:
			//Coordinate coord = provider->sourceCoordinate(Coordinate(row,col,baseZoom));
			Coordinate coord = Coordinate(row,col,baseZoom);
			
			// keep this for later:
			visibleKeys.insert(coord);
			
			if (images.count(coord) == 0) {
				
				// fetch it if we don't have it
				requestTile(coord);
				
				// see if we have  a parent coord for this tile?
				bool gotParent = false;

				for (int i = (int)coord.zoom; i > 0; i--)
                {
					Coordinate zoomed = coord.zoomTo(i).container();

                    if (images.count(zoomed) > 0)
                    {
						visibleKeys.insert(zoomed);
						gotParent = true;
						break;
					}
				}
				
				// or if we have any of the children
				if (!gotParent)
                {
					Coordinate zoomed = coord.zoomBy(1).container();

                    std::vector<Coordinate> kids;

                    kids.push_back(zoomed);
					kids.push_back(zoomed.right());
					kids.push_back(zoomed.down());
					kids.push_back(zoomed.right().down());

                    for (int i = 0; i < kids.size(); i++)
                    {
						if (images.count(kids[i]) > 0)
                        {
							visibleKeys.insert(kids[i]);
						}
					}
				}
			}
		} // rows
	} // columns
	
	// TODO: sort by zoom so we draw small zoom levels (big tiles) first:
	// can this be done with a different comparison function on the visibleKeys set?

	// Collections.sort(visibleKeys, zoomComparator);
	
	// translate and scale, from the middle
	ofPushMatrix();
	ofTranslate(width / 2.0, height / 2.0);
	ofScale(sc, sc, 1);
	ofTranslate(tx, ty);
	
	int numDrawnImages = 0;
	
	if (visibleKeys.size() > 0)
    {
		double prevZoom = baseZoom;
		ofPushMatrix();
		// correct the scale for this zoom level:
		double correction = 1.0 / pow(2.0, prevZoom);
		ofScale(correction, correction,1);

        std::set<Coordinate>::iterator iter;

        for (iter = visibleKeys.begin(); iter != visibleKeys.end(); ++iter)
        {
			Coordinate coord = *iter;

			if (coord.zoom != prevZoom)
            {
				ofPopMatrix();
				ofPushMatrix();
				// correct the scale for this zoom level:
				correction = 1.0 / pow(2.0, coord.zoom);
				ofScale(correction, correction, 1);

				prevZoom = coord.zoom;
			}

			//ofEnableAlphaBlending();
			//ofSetColor(0,0,0,50);
			//ofRect(coord.column*TILE_SIZE,coord.row*TILE_SIZE,TILE_SIZE,TILE_SIZE);
			
			if (images.count(coord) > 0)
            {
                std::shared_ptr<ofImage> tile = images[coord];
				// TODO: must be a cleaner C++ way to do this?
				// we want this image to be at the end of recentImages, if it's already there we'll remove it and then add it again
                std::vector<std::shared_ptr<ofImage> >::iterator result = find(recentImages.begin(), recentImages.end(), tile);

                if (result != recentImages.end())
                {
					recentImages.erase(result);
				}
				else
                {
					// if it's not in recent images it must be brand new?
					tile->setUseTexture(true);
                    tile->update();
				}

				tile->draw(coord.column * _tileSize, coord.row * _tileSize, _tileSize, _tileSize);

				numDrawnImages++;

				recentImages.push_back(tile);
			}
		}

		ofPopMatrix();
	}    
	
	ofPopMatrix();
		
	// stop fetching things we can't see:
	// (visibleKeys also has the parents and children, if needed, but that shouldn't matter)
	//queue.retainAll(visibleKeys);

    std::vector<Coordinate>::iterator iter = queue.begin();

    while (iter != queue.end())
    {
        if (0 == visibleKeys.count(*iter))
        {
			iter = queue.erase(iter);
		}
		else
        {
			++iter;
		}
	}
	
	// TODO sort what's left by distance from center:
	//queueSorter.setCenter(new Coordinate( (minRow + maxRow) / 2.0f, (minCol + maxCol) / 2.0f, zoom));
	//Collections.sort(queue, queueSorter);
	
	// load up to 4 more things:
	processQueue();
	
	// clear some images away if we have too many...
	int numToKeep = std::max(numDrawnImages,_maxImagesToCache);

	if (recentImages.size() > numToKeep) {
		// first clear the pointers from recentImages
		recentImages.erase(recentImages.begin(), recentImages.end()-numToKeep);
		//images.values().retainAll(recentImages);
		// TODO: re-think the stl collections used so that a simpler retainAll equivalent is available
		// now look in the images map and if the value is no longer in recent images then get rid of it
        std::map<Coordinate,std::shared_ptr<ofImage> >::iterator iter = images.begin();

		while (iter != images.end())
        {
			std::shared_ptr<ofImage> tile = iter->second;

            std::vector<std::shared_ptr<ofImage> >::iterator result = std::find(recentImages.begin(), recentImages.end(), tile);

			if (result == recentImages.end())
            {
				images.erase(iter++);
			}
			else
            {
				++iter;
			}
		}
	}
	
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
		if (getZoom() > 0)
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
    float x = evt.x;
    float y = evt.y;
    int button = evt.button;

	double dx = (x - px) / sc;
	double dy = (y - py) / sc;

	if (button == 0)
    {
		//cout << dx << "," << dy;
		//    float angle = radians(-a);
		//    float rx = cos(angle)*dx - sin(angle)*dy;
		//    float ry = sin(angle)*dx + cos(angle)*dy;
		//    tx += rx;
		//    ty += ry;
		panTargetX += dx;
		panTargetY += dy;
	}
	else if (button == 2)
    {
		double mx = x - width / 2;
		double my = x - height / 2;
		panTargetX -= mx / sc;
		panTargetY -= my / sc;

		if (dy < 0)
        {
			scaleTarget *= 1.0 + (fabs(dy * sc) / 100.0);
		}
		else
        {
			scaleTarget /= 1.0 + ((dy * sc) / 100.0);
		}

        panTargetX += mx / sc;
		panTargetY += my / sc;
	}

	px = x;
	py = y;
}

void Map::mouseMoved(ofMouseEventArgs& evt)
{
}

void Map::mousePressed(ofMouseEventArgs& evt)
{
    float x = evt.x;
    float y = evt.y;
    int button = evt.button;

	// TODO: doubleclick?
	px = x;
	py = y;

    unsigned long now = ofGetElapsedTimeMillis();
    unsigned long dt = now - _lastClickTime;

    if (dt < DEFAULT_DOUBLE_CLICK_TIME)
    {
        // Should center.
        zoomIn();
    }

    _lastClickTime = now;
}
void Map::mouseReleased(ofMouseEventArgs& evt)
{
}


/** @return zoom level of currently visible tile layer */
int Map::getZoom() const
{
	return bestZoomForScale(sc);
}

Location Map::getCenter() const
{
	return provider->coordinateLocation(getCenterCoordinate());
}

Coordinate Map::getCenterCoordinate() const
{
	float row = ty * sc / -_tileSize;
	float column = tx * sc / -_tileSize;
	float zoom = zoomForScale(sc);

    return Coordinate(row, column, zoom);
}

void Map::setCenter(const Coordinate& center)
{
	sc = pow(2.0, center.zoom);
	tx = -_tileSize*center.column / sc;
	ty = -_tileSize*center.row / sc;
}

void Map::setCenter(const Location& location)
{
	setCenter(provider->locationCoordinate(location).zoomTo(getZoom()));
}

void Map::setCenterZoom(const Location& location, int zoom)
{
	setCenter(provider->locationCoordinate(location).zoomTo(zoom));
}

/** sets scale according to given zoom level, should leave you with pixel perfect tiles */
void Map::setZoom(int zoom)
{
	scaleTarget = pow(2.0, zoom);
}

void Map::zoom(int dir)
{
	scaleTarget = pow(2.0, getZoom() + dir);
}

void Map::zoomIn()
{
	scaleTarget = pow(2.0, getZoom() + 1);
}  

void Map::zoomOut()
{
	scaleTarget = pow(2.0f, getZoom() - 1);
}

// TODO: extent functions
//	    public function setExtent(extent:MapExtent):void
//	    public function getExtent():MapExtent

Point2d Map::coordinatePoint(const Coordinate& coord) const
{
	/* Return an x, y point on the map image for a given coordinate. */
	
	Coordinate center = getCenterCoordinate();

    Coordinate newCoord;

	if(coord.zoom != center.zoom)
    {
		newCoord = newCoord.zoomTo(center.zoom);
	}
    else
    {
        newCoord = coord;
    }
	
	// distance from the center of the map
	Point2d point = Point2d(width / 2, height / 2);

	point.x += _tileSize * (newCoord.column - center.column);
	point.y += _tileSize * (newCoord.row - center.row);
	
	return point;
}

Coordinate Map::pointCoordinate(const Point2d& point) const
{
	/* Return a coordinate on the map image for a given x, y point. */		
	// new point coordinate reflecting distance from map center, in tile widths
	Coordinate coord = getCenterCoordinate();
	coord.column += (point.x - width  / 2) / _tileSize;
	coord.row    += (point.y - height / 2) / _tileSize;
	return coord;
}

Point2d Map::locationPoint(const Location& location) const
{
	return coordinatePoint(provider->locationCoordinate(location));
}

Location Map::pointLocation(const Point2d& point) const
{
	return provider->coordinateLocation(pointCoordinate(point));
}

// TODO: pan by proportion of screen size, not by coordinate grid
void Map::panUp()
{
	setCenter(getCenterCoordinate().up());
}

void Map::panDown()
{
	setCenter(getCenterCoordinate().down());
}

void Map::panLeft()
{
	setCenter(getCenterCoordinate().left());
}

void Map::panRight()
{
	setCenter(getCenterCoordinate().right());
}

void Map::panAndZoomIn(const Location& location)
{
	// TODO: animate?
	setCenterZoom(location, getZoom() + 1);
}

void Map::panTo(const Location& location)
{
	// TODO: animate?
	setCenter(location);
}

///////////////////////////////////////////////////////////////////////

float Map::scaleForZoom(int zoom) const
{
	return pow(2.0, zoom);
}

float Map::zoomForScale(float scale) const
{
	return log(scale) / LOG_2;
}

int Map::bestZoomForScale(float scale) const
{
	// TODO get min/max zoom level from provider?
	return std::min(20,
                    std::max(1, (int)round(log(scale) / LOG_2))
                    );
}

//////////////////////////////////////////////////////////////////////////

void Map::requestTile(const Coordinate& coord)
{
	bool isPending = pending.count(coord) > 0;



	bool isQueued = std::find(queue.begin(), queue.end(), coord) != queue.end();
	bool isAlreadyLoaded = images.count(coord) > 0;

	if (!isPending && !isQueued && !isAlreadyLoaded)
    {
		queue.push_back(coord);
	}
}

void Map::processQueue()
{
	if (queue.size() > (_maxPending - pending.size()))
    {
        std::sort(queue.begin(),
                  queue.end(),
                  QueueSorter(getCenterCoordinate().zoomTo(getZoom())));
	}

    // Transfer load queued coordinates.
    while (pending.size() < _maxPending && !queue.empty())
    {
		Coordinate coord = *(queue.begin());

        std::vector<std::string> urls = provider->getTileUrls(coord);

        if (!urls.empty())
        {
            pending[coord] = ofLoadURLAsync(urls[0]);
        }
        else
        {

        }

		queue.erase(queue.begin());
	}  
}

void Map::urlResponse(ofHttpResponse& args)
{
    std::map<Coordinate, int>::iterator iter = pending.begin();

    // Find our async id.
    while (iter != pending.end())
    {
        if ((*iter).second == args.request.getID())
        {
            if (200 == args.status)
            {
                Coordinate coord = (*iter).first;
                images[coord] = std::shared_ptr<ofImage>(new ofImage());
                images[coord]->setUseTexture(false);
                images[coord]->loadImage(args);
            }
            else
            {
                ofLogError("Map::urlResponse") << " : " << args.status << " : " << args.error << " : " << args.request.url;
            }

            pending.erase(iter);
            break;
        }

        ++iter;
    }
}


