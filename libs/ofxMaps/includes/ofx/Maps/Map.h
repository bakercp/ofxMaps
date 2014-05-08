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
//#include "ofxTween.h"
#include "ofx/Geo/Coordinate.h"
#include "ofx/Geo/Utils.h"
#include "ofx/Maps/TileStore.h"
#include "ofx/Maps/BaseMapProvider.h"
#include "ofx/Maps/TileCoordinate.h"


namespace ofx {
namespace Maps {


class Map
{
public:
	Map();
    virtual ~Map();

	void setup(BaseMapProvider::SharedPtr provider, int width, int height);
	void update(ofEventArgs& args);
	void draw();
		
	void keyPressed(ofKeyEventArgs& evt);
	void keyReleased(ofKeyEventArgs& evt);

	void mouseMoved(ofMouseEventArgs& evt);
	void mouseDragged(ofMouseEventArgs& evt);
	void mousePressed(ofMouseEventArgs& evt);
	void mouseReleased(ofMouseEventArgs& evt);

    void windowEntered(ofEntryEventArgs& evt)
    {
        cout << "entered!" << endl;
    }

	double getZoom() const;
	void setZoom(double zoom);
    void zoom(double zoomStep);
	void zoomIn();
	void zoomOut();

    void panBy(const ofVec2d& delta);
    void panBy(double dx, double dy);

    void scaleBy(double s);

    void scaleBy(double s, const ofVec2d& c);
    void scaleBy(double s, double cx, double cy);
    void rotateBy(double r, double cx, double cy);

	Geo::Coordinate getGeoLocationCenter() const;
	TileCoordinate getTileCoordinateCenter() const;
	
	void setPointCenter(const ofVec2d& center);
	void setGeoLocationCenter(const Geo::Coordinate& location);
	void setTileCoordinateCenter(const TileCoordinate& center);

	Geo::Coordinate pointToGeolocation(const ofVec2d& point) const;
	TileCoordinate pointToTileCoordinate(const ofVec2d& point) const;

	ofVec2d geoLocationToPoint(const Geo::Coordinate& location) const;
    ofVec2d tileCoordinateToPoint(const TileCoordinate& coord) const;

    static std::set<TileCoordinate> getVisibleTileCoordinates(const ofVec3d& position);

	void requestTile(const TileCoordinate& coord);
	void processQueue();


    enum
    {
        DEFAULT_TILE_SIZE = 256,
        /// \brief Limit simultaneous calls to loadImage.
        DEFAULT_MAX_PENDING = 32,
        /// \brief Limit tiles in memory
        /// 256 would be 64 MB, you may want to lower this quite a bit for your app
        /// (we'll always keep as many images as needed to fill the screen though)
        DEFAULT_MAX_IMAGES_TO_CACHE = 256,
        /// \brief Upping this can help appearances when zooming out, but also
        /// loads many more tiles
        DEFAULT_GRID_PADDING = 3,
        DEFAULT_DOUBLE_CLICK_TIME = 250
    };

    void urlResponse(ofHttpResponse& args);

protected:
//    int _tileSize;
    int _maxPending;
    int _maxImagesToCache;
    int _gridPadding;

    int _numDrawnImages;

    unsigned long long _lastClickTime;

    BaseTileStore _tileStore;

    /// \brief The Map tile Provider.
    BaseMapProvider::SharedPtr _provider;

    /// \brief Map Size.
    ofVec2d _size;

    /// \brief Pan and anchor coordinate.
    TileCoordinate _centerTileCoordinate;

    double _rotation;

    /// \brief Tiles waiting to load.
    std::map<TileCoordinate, int> _pending;

    /// \brief Image store.
    std::map<TileCoordinate, std::shared_ptr<ofImage> > _images;

    /// \brief Map of the most recent images MAX_IMAGES_TO_KEEP
    std::vector<std::shared_ptr<ofImage> > _recentImages;

    /// \brief Coordinates waiting to load
	std::vector<TileCoordinate> _queue;

    /// \brief Coordinates that we can see already.
    std::set<TileCoordinate> _visibleCoordinates;

	// for sorting coordinates by zoom
	//ZoomComparator zoomComparator;

	// for loading tiles from the inside first
	//QueueSorter queueSorter;

    ofVec2d _prevMouse;

//    void updateMapPostion(const ofVec2f& vec, unsigned duration = 1000, unsigned delay = 100, ofxEasingType easing = easeIn);
//

};


} } // namespace ofx::Maps
