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


#include "ofx/Maps/BaseURLTileProvider.h"


namespace ofx {
namespace Maps {


class OSMTileProvider: public BaseURLTileProvider
{
public:
    OSMTileProvider(const std::vector<std::string>& subdomains = DEFAULT_OSM_SUBDOMAINS,
                    const std::string& URITemplate = DEFAULT_OSM_URI_TEMPLATE,
                    int minZoom = DEFAULT_OSM_MIN_ZOOM,
                    int maxZoom = DEFAULT_OSM_MAX_ZOOM,
                    int tileWidth = DEFAULT_OSM_TILE_WIDTH,
                    int tileHeight = DEFAULT_OSM_TILE_HEIGHT,
                    const BaseProjection& projection = DEFAULT_OSM_PROJECTION,
                    const std::string& attribution = DEFAULT_OSM_ATTRIBUTION);

    virtual ~OSMTileProvider();

    enum
    {
        DEFAULT_OSM_MIN_ZOOM = 0,
        DEFAULT_OSM_MAX_ZOOM = 19,
        DEFAULT_OSM_TILE_WIDTH = 256,
        DEFAULT_OSM_TILE_HEIGHT = 256
    };

    static const std::string DEFAULT_OSM_URI_TEMPLATE;

    /// \brief An unfortunate compromise until C++11.
    /// \note C++ is not able to initialize static collections until
    ///        after C++11.  This is a compromise until then.
    static const std::string DEFAULT_OSM_SUBDOMAINS_ARRAY[];

    /// \brief The default HTTP methods for this route.
    static const std::vector<std::string> DEFAULT_OSM_SUBDOMAINS;

    static const SperhicalMercatorProjection DEFAULT_OSM_PROJECTION;

    static const std::string DEFAULT_OSM_ATTRIBUTION;

};



} } // namespace ofx::Maps
