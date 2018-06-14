//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
// Copyright (c) -2014 Tom Carden <https://github.com/RandomEtc>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/Maps/BaseProjection.h"
#include "ofx/Maps/TileCoordinate.h"


namespace ofx {
namespace Maps {


Transformation::Transformation()
{
}


Transformation::Transformation(double _ax,
                               double _bx,
                               double _cx,
                               double _ay,
                               double _by,
                               double _cy):
    ax(_ax),
    bx(_bx),
    cx(_cx),
    ay(_ay),
    by(_by),
    cy(_cy)
{
}


Transformation::Transformation(double a1x, double a1y,
                               double a2x, double a2y,
                               double b1x, double b1y,
                               double b2x, double b2y,
                               double c1x, double c1y,
                               double c2x, double c2y)
{
    glm::dvec3 x = linearSolution(a1x, a1y, a2x,
                                  b1x, b1y, b2x,
                                  c1x, c1y, c2x);

    ax = x.x;
    bx = x.y;
    cx = x.z;

    glm::dvec3 y = linearSolution(a1x, a1y, a2y,
                                  b1x, b1y, b2y,
                                  c1x, c1y, c2y);

    ay = y.x;
    by = y.y;
    cy = y.z;
}


glm::dvec2 Transformation::transform(const glm::dvec2& point) const
{
    //    y = ay * point.x + by*point.y + cy);
    //    x = ax * point.x + bx*point.y + cx


    double x = ay * point.x + by * point.y + cy;
    double y = ax * point.x + bx * point.y + cx;

    return glm::dvec2(x, y);
}


glm::dvec2 Transformation::untransform(const glm::dvec2& point) const
{
    double x = (point.x * by - point.y * bx - cx * by + cy * bx) / (ax * by - ay * bx);
    double y = (point.x * ay - point.y * ax - cx * ay + cy * ax) / (bx * ay - by * ax);

    //    x = point.x*by - point.y*bx - cx*by + cy*bx) / (ax*by - ay*bx)
    //    y = point.x*ay - point.y*ax - cx*ay + cy*ax) / (bx*ay - by*ax)

    return glm::dvec2(x, y);
}


glm::dvec3 Transformation::linearSolution(double r1, double s1, double t1,
                                          double r2, double s2, double t2,
                                          double r3, double s3, double t3)
{
    glm::dvec3 result;

    result.x = (((t2 - t3) * (s1 - s2)) - ((t1 - t2) * (s2 - s3)))
    / (((r2 - r3) * (s1 - s2)) - ((r1 - r2) * (s2 - s3)));

    result.y = (((t2 - t3) * (r1 - r2)) - ((t1 - t2) * (r2 - r3)))
    / (((s2 - s3) * (r1 - r2)) - ((s1 - s2) * (r2 - r3)));

    result.z = t1 - (r1 * result.x) - (s1 * result.y);

    return result;
};


BaseProjection::BaseProjection(const std::string& name,
                               double zoom,
                               const Transformation& transformation):
    _name(name),
    _zoom(zoom),
    _transformation(transformation)
{
}


BaseProjection::~BaseProjection()
{
}


std::string BaseProjection::name() const
{
    return _name;
}


double BaseProjection::zoom() const
{
    return _zoom;
}


glm::dvec2 BaseProjection::project(const glm::dvec2& point) const
{
    return _transformation.transform(rawProject(point));
}


glm::dvec2 BaseProjection::unproject(const glm::dvec2& point) const
{
    return rawUnproject(_transformation.untransform(point));
}


TileCoordinate BaseProjection::geoToWorld(const Geo::Coordinate& location) const
{
    glm::dvec2 point = project(glm::dvec2(location.getLongitudeRad(),
                                          location.getLatitudeRad()));

    return TileCoordinate(point.y, point.x, _zoom);
}


Geo::Coordinate BaseProjection::tileToGeo(const TileCoordinate& coordinate) const
{
    auto newCoordinate = coordinate.getZoomedTo(_zoom);

    glm::dvec2 point = glm::degrees(unproject(glm::dvec2(newCoordinate.getColumn(),
                                                         newCoordinate.getRow())));

    return Geo::Coordinate(point.y, point.x);
}


} } // namespace ofx::Maps
