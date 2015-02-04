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
