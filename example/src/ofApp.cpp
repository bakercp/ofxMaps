//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    coordinates =
    {
        { 42.2610492, -91.4501953 },
        { 43.0046471, -90.4833984 },
        { 43.0367759, -89.3847656 },
        { 41.9676592, -88.4619141 },
        { 41.2117215, -89.0332031 },
        { 40.5805847, -90.1318359 },
        { 40.6806380, -91.1865234 },
        { 41.1124688, -92.4169922 },
        { 42.1959688, -93.2958984 },
        { 43.2932003, -92.1972656 },
        { 44.0560117, -90.7470703 } 
    };

    ofJson json = ofLoadJson("provider.json")["providers"][3];

    tileProvider = std::make_shared<ofxMaps::MapTileProvider>(ofxMaps::MapTileProvider::fromJSON(json));

    Poco::ThreadPool::defaultPool().addCapacity(64);

    bufferCache = std::make_shared<ofxMaps::MBTilesCache>(*tileProvider, "/Volumes/Data/tiles");

    tileSet = std::make_shared<ofxMaps::MapTileSet>(1024,
                                                    tileProvider,
                                                    bufferCache);

    tileLayer = std::make_shared<ofxMaps::MapTileLayer>(tileSet, 1280, 768);

    ofxGeo::Coordinate chicago(41.8827, -87.6233);
    ofxGeo::Coordinate bethel(45.0579, -93.1605);

    tileLayer->setCenter(coordinates[3], 21);

}


void ofApp::update()
{
    tileLayer->update();

    if (!ofIsFloatEqual(animation, 0.f))
        tileLayer->setCenter(tileLayer->getCenter().getNeighbor(animation, 0));
}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor(255), ofColor(0));
    ofFill();
    ofSetColor(255);

//    cam.begin();
    ofPushMatrix();
    //ofTranslate(-tileLayer->getWidth() / 2, -tileLayer->getHeight() / 2);
    tileLayer->draw(0, 0);
    ofPopMatrix();

    ofPushStyle();
    ofNoFill();
    ofSetColor(0, 255, 0);

    for (auto coordinate: coordinates)
    {
        auto tc = tileLayer->geoToPixels(coordinate);
        ofDrawCircle(tc.x, tc.y, 20);
    }
    ofPopStyle();

//    cam.end();

    ofDrawBitmapStringHighlight(tileLayer->getCenter().toString(0), 14, ofGetHeight() - 32);
    ofDrawBitmapStringHighlight("Task Queue:" + ofx::TaskQueue::instance().toString(), 14, ofGetHeight() - 16);
    ofDrawBitmapStringHighlight("Connection Pool: " + bufferCache->toString(), 14, ofGetHeight() - 2);

}


void ofApp::keyPressed(int key)
{
    if (key == 'f' || key == 'F')
    {
		ofToggleFullscreen();
	}
    else if (key == '-')
    {
        tileLayer->setCenter(tileLayer->getCenter().getZoomedBy(-1));
    }
    else if (key == '=')
    {
        tileLayer->setCenter(tileLayer->getCenter().getZoomedBy(1));
    }
    else if (key == 'w')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborUp());
    }
    else if (key == 'a')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborLeft());
    }
    else if (key == 's')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborDown());
    }
    else if (key == 'd')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborRight());
    }
    else if (key == '1')
    {
        animation -= 0.01;;
    }
    else if (key == '2')
    {
        animation += 0.01;;
    }
    else if (key == '3')
    {
        animation = 0;
    }
//    else if (key == ' ')
//    {
//        setsIndex = (setsIndex + 1) % sets.size();
//        tileLayer->setSetId(sets[setsIndex]);
//    }

}

