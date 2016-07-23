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


#include "ofx/Maps/BaseProjection.h"


namespace ofx {
namespace Maps {


BaseProjection::BaseProjection(double zoom,
                               const Transformation& transformation):
    _zoom(zoom),
    _transformation(transformation)
{
}


BaseProjection::~BaseProjection()
{
}


glm::dvec2 BaseProjection::project(const glm::dvec2& point) const
{
    return _transformation.transform(rawProject(point));
}


glm::dvec2 BaseProjection::unproject(const glm::dvec2& point) const
{
    return rawUnproject(_transformation.untransform(point));
}


TileCoordinate BaseProjection::geoToTile(const Geo::Coordinate& location) const
{
    glm::dvec2 point = project(glm::dvec2(DEG_TO_RAD * location.getLongitude(),
                                          DEG_TO_RAD * location.getLatitude()));

    return TileCoordinate(point.y, point.x, _zoom);
}


Geo::Coordinate BaseProjection::tileToGeo(const TileCoordinate& coordinate) const
{
    TileCoordinate newCoordinate = coordinate.zoomTo(_zoom);

    glm::dvec2 point = unproject(glm::dvec2(newCoordinate.getColumn(),
                                            newCoordinate.getRow()));

    return Geo::Coordinate(RAD_TO_DEG * point.x, RAD_TO_DEG * point.y);
}


} } // namespace ofx::Maps
