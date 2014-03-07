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


#pragma once


#include <set>
#include <map>
#include "ofMain.h"
#include "AbstractTileProvider.h"
#include "Location.h"
#include "Coordinate.h"
#include "Point2d.h"


class Map
{
public:
	Map():
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

    virtual ~Map()
    {
        ofUnregisterMouseEvents(this);
        ofUnregisterKeyEvents(this);
        ofRemoveListener(ofEvents().update, this, &Map::update);

        ofUnregisterURLNotification(this);
    }

	void setup(AbstractTileProvider::SharedPtr _provider, int _width, int _height);
	void update(ofEventArgs& args);
	void draw();
		
	void keyPressed(ofKeyEventArgs& evt);
	void keyReleased(ofKeyEventArgs& evt);

	void mouseMoved(ofMouseEventArgs& evt);
	void mouseDragged(ofMouseEventArgs& evt);
	void mousePressed(ofMouseEventArgs& evt);
	void mouseReleased(ofMouseEventArgs& evt);
	
	int getZoom() const;
	Location getCenter() const ;
	Coordinate getCenterCoordinate() const ;
	
	void setCenter(const Coordinate& center);
	void setCenter(const Location& location);
	void setCenterZoom(const Location& location, int zoom);
	
	void setZoom(int zoom);
	void zoom(int dir);
	void zoomIn();
	void zoomOut();
	
	// TODO: extent functions
	//	    public function setExtent(extent:MapExtent):void
	//	    public function getExtent():MapExtent
	
	Point2d coordinatePoint(const Coordinate& coord) const ;
	Coordinate pointCoordinate(const Point2d& point) const ;
	Point2d locationPoint(const Location& location) const ;
	Location pointLocation(const Point2d& point) const ;
	
	// TODO: pan by proportion of screen size, not by coordinate grid
	void panUp();
	void panDown();
	void panLeft();
	void panRight();
	
	void panAndZoomIn(const Location& location);
	void panTo(const Location& location);
	float scaleForZoom(int zoom) const;
	float zoomForScale(float scale) const;
	int bestZoomForScale(float scale) const;

	void requestTile(const Coordinate& coord);
	void tileDone(const Coordinate& coord, ofImage* img);
	void processQueue();

    static const double LOG_2;

    enum
    {
        DEFAULT_TILE_SIZE = 256,
        DEFAULT_MAX_PENDING = 16, ///< \brief Limit simultaneous calls to loadImage.
        DEFAULT_MAX_IMAGES_TO_CACHE = 256,
                                ///< \brief Limit tiles in memory
                                ///< 256 would be 64 MB, you may want to lower this quite a bit for your app
                                ///< (we'll always keep as many images as needed to fill the screen though)
        DEFAULT_GRID_PADDING = 1, ///< \brief Upping this can help appearances when zooming out, but also loads many more tiles
        DEFAULT_DOUBLE_CLICK_TIME = 250
    };

    void urlResponse(ofHttpResponse& args);

protected:
    int _tileSize;
    int _maxPending;
    int _maxImagesToCache;
    int _gridPadding;

    unsigned long long _lastClickTime;

    double tx; ///< \brief Pan coordinates x.
    double ty; ///< \brief Pan coordinates y.
    double sc; ///< \brief Zoom scale factor.

    double panTargetX;
    double panTargetY;
    double scaleTarget;

    double smoothing;

    ///< \brief The Map tile Provider.
    AbstractTileProvider::SharedPtr provider;

    ///< \brief Map Size.
	int width;
    int height;

    std::map<Coordinate, int> pending; ///< \brief Tiles waiting to load.

    std::map<Coordinate, std::shared_ptr<ofImage> > images; ///< \brief Image store.
	std::vector<std::shared_ptr<ofImage> > recentImages; /// <\brief Map of the most recent images MAX_IMAGES_TO_KEEP

	std::vector<Coordinate> queue; /// \brief Coordinates waiting to load
	std::set<Coordinate> visibleKeys; /// \brief Coordinates that we can see already.

	// for sorting coordinates by zoom
	//ZoomComparator zoomComparator;

	// for loading tiles from the inside first
	//QueueSorter queueSorter;

	double px;
	double py;


};
