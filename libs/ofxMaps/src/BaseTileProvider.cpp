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


#include "ofx/Maps/BaseTileProvider.h"


namespace ofx {
namespace Maps {


const double BaseTileProvider::LOG_2 = log(2);
const SperhicalMercatorProjection BaseTileProvider::DEFAULT_PROJECTION = SperhicalMercatorProjection();


BaseTileProvider::BaseTileProvider(const std::string& attribution,
                                   int minZoom,
                                   int maxZoom,
                                   int tileWidth,
                                   int tileHeight,
                                   const BaseProjection& projection):
    _attribution(attribution),
    _minZoom(minZoom),
    _maxZoom(maxZoom),
    _tileWidth(tileWidth),
    _tileHeight(tileHeight),
    _projection(projection)
{
}


BaseTileProvider::~BaseTileProvider()
{
}


const std::string& BaseTileProvider::getAttribution() const
{
    return _attribution;
}
    

int BaseTileProvider::getMinZoom() const
{
    return _minZoom;
}


int BaseTileProvider::getMaxZoom() const
{
    return _maxZoom;
}


int BaseTileProvider::getTileWidth() const
{
    return _tileWidth;
}


int BaseTileProvider::getTileHeight() const
{
    return _tileHeight;
}


glm::dvec2 BaseTileProvider::getTileSize() const
{
    return glm::dvec2(_tileWidth, _tileHeight);
}

    
double BaseTileProvider::zoomForScale(double scale) const
{
    return log(scale) / LOG_2;
}


TileCoordinate BaseTileProvider::geoToTile(const Geo::Coordinate& location) const
{
    
    return _projection.geoToTile(location);
}


Geo::Coordinate BaseTileProvider::tileToGeo(const TileCoordinate& coordinate) const
{
    return _projection.tileToGeo(coordinate);
}


} } // namespace ofx::Maps
