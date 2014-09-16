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


#include "ofApp.h"


void ofApp::setup()
{
    ofSetFrameRate(60);
	ofSetVerticalSync(true);


//    std::cout << osmTileProvider.getTileURI(c0).toString() << std::endl;
//    std::cout << osmTileProvider.getTileURI(c1).toString() << std::endl;
//    std::cout << osmTileProvider.getTileURI(c2).toString() << std::endl;
//
//    tile0 = tileLoader.getTile(c0);
//    tile1 = tileLoader.getTile(c1);
//    tile2 = tileLoader.getTile(c2);


//    map.setup(std::shared_ptr<ofx::Maps::OpenStreetMapProvider>(new ofx::Maps::OpenStreetMapProvider()),
//              ofGetWidth(),
//              ofGetHeight());

//    map.setGeoLocationCenter(ofx::Geo::Coordinate(41.878247, -87.629767));
//	map.setZoom(12);
}


void ofApp::update()
{
}


void ofApp::draw()
{
    ofBackground(0);

//    map.draw(0, 0);

    ofSetColor(255);

    tileLoader.getTile(ofx::Maps::TileCoordinate(1, 0, 3))->draw(0, 0);
    tileLoader.getTile(ofx::Maps::TileCoordinate(1, 1, 3))->draw(256, 0);
    tileLoader.getTile(ofx::Maps::TileCoordinate(1, 2, 3))->draw(512, 0);
    tileLoader.getTile(ofx::Maps::TileCoordinate(1, 3, 3))->draw(512+256, 0);
    tileLoader.getTile(ofx::Maps::TileCoordinate(2, 0, 3))->draw(0, 256);
    tileLoader.getTile(ofx::Maps::TileCoordinate(2, 1, 3))->draw(256, 256);
    tileLoader.getTile(ofx::Maps::TileCoordinate(2, 2, 3))->draw(512, 256);
    tileLoader.getTile(ofx::Maps::TileCoordinate(2, 3, 3))->draw(512+256, 256);
    tileLoader.getTile(ofx::Maps::TileCoordinate(3, 0, 3))->draw(0, 512);
    tileLoader.getTile(ofx::Maps::TileCoordinate(3, 1, 3))->draw(256, 512);
    tileLoader.getTile(ofx::Maps::TileCoordinate(3, 2, 3))->draw(512, 512);
    tileLoader.getTile(ofx::Maps::TileCoordinate(3, 3, 3))->draw(512+256, 512);

//    std::cout << "Hi!" << endl;

//    cout << map.getGeoLocationCenter() << endl;

//    ofDrawBitmapStringHighlight(ofToString(map.getGeoLocationCenter()),
//                                ofGetWidth() / 2,
//                                ofGetHeight() / 2);
//
//    ofVec2d mousePosition(mouseX, mouseY);
//
//    ofDrawBitmapStringHighlight(">>" + ofToString(map.pointToTileCoordinate(mousePosition)),
//                                mouseX + 16,
//                                mouseY);
//
//    ofDrawBitmapStringHighlight("<<" + ofToString(map.pointToGeolocation(mousePosition)),
//                                mouseX + 16,
//                                mouseY + 14);


}


void ofApp::keyPressed(int key)
{
    if (key == 'f' || key == 'F')
    {
		ofToggleFullscreen();
	}
    else if (key == 'r')
    {
//        map.rotateBy(TWO_PI / 180, 40, 40);
    }
    else if (key == 'R')
    {
//        map.rotateBy(-TWO_PI / 180, 40, 40);
    }
}
