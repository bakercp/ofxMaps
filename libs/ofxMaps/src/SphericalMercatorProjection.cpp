//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
// Copyright (c) -2014 Tom Carden <https://github.com/RandomEtc>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/Maps/SphericalMercatorProjection.h"


namespace ofx {
namespace Maps {


const std::string SperhicalMercatorProjection::EPSG_3857 = "EPSG:3857";


SperhicalMercatorProjection::SperhicalMercatorProjection():
    BaseProjection(EPSG_3857,
                   DEFAULT_ZOOM,
                   Transformation(-glm::pi<double>(),  glm::pi<double>(), 0, 0,
                                   glm::pi<double>(),  glm::pi<double>(), 1, 0,
                                  -glm::pi<double>(), -glm::pi<double>(), 0, 1))
{
}


SperhicalMercatorProjection::~SperhicalMercatorProjection()
{
}


glm::dvec2 SperhicalMercatorProjection::rawProject(const glm::dvec2& point) const
{
    return glm::dvec2(point.x,
                      std::log(std::tan(glm::quarter_pi<double>() + 0.5 * point.y)));
}


glm::dvec2 SperhicalMercatorProjection::rawUnproject(const glm::dvec2& point) const
{
    return glm::dvec2(point.x,
                      2.0 * std::atan(std::pow(glm::e<double>(), 1.0 * point.y)) - glm::half_pi<double>());
}


} } // namespace ofx::Maps
