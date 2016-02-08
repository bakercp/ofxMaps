// =============================================================================
//
// Copyright (c) 2014-2016 Christopher Baker <http://christopherbaker.net>
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
    Geo::Coordinate dresden(51.0530, 13.7337);
    Geo::Coordinate saic(41.880645, -87.624415);
    Geo::Coordinate bean(41.882672, -87.62336);
    Geo::Coordinate ts(40.758895, -73.985131);

    cout << Gd(18, 41.89155578613281, -87.63175964355469) << endl;


//    ofExit();

    ofSetFrameRate(60);
	ofSetVerticalSync(true);

    ofEnableAlphaBlending();

    Poco::ThreadPool::defaultPool().addCapacity(32);

    layers.push_back(std::make_shared<Maps::TileLayer>());
    layers.push_back(std::make_shared<Maps::TileLayer>());
    layers.push_back(std::make_shared<Maps::TileLayer>());

    layers[0]->setup(std::make_shared<Maps::MicrosoftAerialStyleProvider>(),
                     ofGetWidth(),
                     ofGetHeight());

    layers[1]->setup(std::make_shared<Maps::EsriSatelliteTileProvider>(),
                     ofGetWidth(),
                     ofGetHeight());

    layers[2]->setup(std::make_shared<Maps::GoogleAerialTileProvider>(),
                     ofGetWidth(),
                     ofGetHeight());


    for (std::size_t i = 0; i < layers.size(); ++i)
    {
        layers[i]->setCenter(saic, 19);
    }


    cout << "here" << endl;

}


void ofApp::update()
{
    
}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor(255), ofColor(0));

    ofDrawBitmapString(ofToString(layers[0]->getCenter()), ofVec2f(15, 15));

    ofFill();
    ofSetColor(255);

    ofPushMatrix();


    if (ofGetKeyPressed('0'))
    {
        layers[0]->draw(0, 0);
    }
    else if (ofGetKeyPressed('1'))
    {
        layers[1]->draw(0, 0);
    }
    else
    {
//        ofFill();
//        ofSetColor(255, 150);

//        for (std::size_t i = 0; i < layers.size(); ++i)
//        {
            layers[ofGetFrameNum() % layers.size()]->draw(0, 0);
//        }
    }






    ofPopMatrix();
}


void ofApp::keyPressed(int key)
{
    if (key == 'f' || key == 'F')
    {
		ofToggleFullscreen();
	}
    else if (key == '-')
    {
        for (std::size_t i = 0; i < layers.size(); ++i)
        {
            layers[i]->setCenter(layers[i]->getCenter().zoomBy(-0.5));
        }
    }
    else if (key == '=')
    {
        for (std::size_t i = 0; i < layers.size(); ++i)
        {
            layers[i]->setCenter(layers[i]->getCenter().zoomBy(0.5));
        }
    }
    else if (key == 'w')
    {
        for (std::size_t i = 0; i < layers.size(); ++i)
        {
            layers[i]->setCenter(layers[i]->getCenter().up(0.5));
        }
    }
    else if (key == 'a')
    {
        for (std::size_t i = 0; i < layers.size(); ++i)
        {
            layers[i]->setCenter(layers[i]->getCenter().left(0.5));
        }
    }
    else if (key == 's')
    {
        for (std::size_t i = 0; i < layers.size(); ++i)
        {
            layers[i]->setCenter(layers[i]->getCenter().down(0.5));
        }
    }
    else if (key == 'd')
    {
        for (std::size_t i = 0; i < layers.size(); ++i)
        {
            layers[i]->setCenter(layers[i]->getCenter().right(0.5));
        }
    }
}

