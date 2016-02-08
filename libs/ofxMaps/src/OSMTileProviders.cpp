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


#include "ofx/Maps/OSMTileProviders.h"


namespace ofx {
namespace Maps {


const std::string OSMTileProvider::DEFAULT_OSM_URI_TEMPLATE = "http://{s}.tile.osm.org/{z}/{x}/{y}.png";
const std::string OSMTileProvider::DEFAULT_OSM_ATTRIBUTION = "&copy; <a href=\"http://osm.org/copyright\">OpenStreetMap</a> contributors";


OSMTileProvider::OSMTileProvider(const std::string& URITemplate,
                                 const std::string& attribution):
    BaseURLTileProvider(URITemplate,
                        attribution,
                        DEFAULT_ALPHABETICAL_SUBDOMAINS,
                        DEFAULT_MIN_ZOOM,
                        DEFAULT_MAX_ZOOM,
                        DEFAULT_TILE_WIDTH,
                        DEFAULT_TILE_HEIGHT,
                        DEFAULT_PROJECTION)
{
}


OSMTileProvider::OSMTileProvider(const std::string& URITemplate,
                                 const std::string& attribution,
                                 const std::vector<std::string>& subdomains,
                                 int minZoom,
                                 int maxZoom,
                                 int tileWidth,
                                 int tileHeight,
                                 const BaseProjection& projection):
    BaseURLTileProvider(URITemplate,
                        attribution,
                        subdomains,
                        minZoom,
                        maxZoom,
                        tileWidth,
                        tileHeight,
                        projection)
{
}


OSMTileProvider::~OSMTileProvider()
{
}


} } // namespace ofx::Maps
