//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofAppRunner.h"
#include "ofApp.h"


int main()
{
    ofGLWindowSettings settings;
    settings.width = 1280;
    settings.height = 768;
    settings.setGLVersion(3, 2);
    settings.windowMode = OF_WINDOW;
    auto window = ofCreateWindow(settings);
    auto app = std::make_shared<ofApp>();
    return ofRunApp(app);
}