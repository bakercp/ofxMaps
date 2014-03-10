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

#include "OpenStreetMapProvider.h"

void ofApp::setup()
{
	ofSetVerticalSync(true);

    map.setup(std::shared_ptr<OpenStreetMapProvider>(new OpenStreetMapProvider()),
              ofGetWidth(),
              ofGetHeight());

//	map.setZoom(3);

}


void ofApp::update()
{
}


void ofApp::draw()
{
    ofBackground(0);

//    cam.begin();
    map.draw();


    ofSetColor(255, 127, 255);

    ofDrawBitmapStringHighlight(ofToString(map.getGeoLocationCenter()),
                                ofGetWidth() / 2,
                                ofGetHeight() / 2);

    ofVec2d mousePosition(mouseX, mouseY);

    ofDrawBitmapStringHighlight(ofToString(map.pointToTileCoordinate(mousePosition)),
                                mouseX + 16,
                                mouseY);

    ofDrawBitmapStringHighlight(ofToString(map.pointToGeolocation(mousePosition)),
                                mouseX + 16,
                                mouseY + 14);

//    cam.end();

}


void ofApp::keyPressed(int key)
{
    if (key == 'f' || key == 'F')
    {
		ofToggleFullscreen();
	}
}
