//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxMaps.h"


using namespace ofx;


class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);

    std::string Gd(int zoomLevel, double latitude, double longitude) {

        double e = 1 << zoomLevel;

        cout << "e=" << e << endl;

        double b = 32 - zoomLevel;

        cout << "b=" << b << endl;

        double Pd = pow(2.0, 32.0) / 360.0;

        unsigned int cx = (Pd * (latitude + 90));
        unsigned int cy = (Pd * (longitude + 180));

        unsigned int xx = e * ((cx >> zoomLevel) + e);
        unsigned int yy = cy >> zoomLevel;

        std::stringstream ss;

        ss << xx << yy;

        cout << "x=" << cx << endl;
        cout << "y=" << cy << endl;
        cout << "xx=" << xx << endl;
        cout << "yy=" << yy << endl;

        return ss.str();

//        ofVec2d v(Pd * (c + 90) >>> 0, Pd * (d + 180) >>> 0);

//        return (e * ((c[0] >>> zoomLevel) + e) + (c[1] >>> zoomLevel)).toString(10)
    }
    


    std::vector<Maps::TileLayer::SharedPtr> layers;

};
