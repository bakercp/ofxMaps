// =============================================================================
//
// Copyright (c) 2014-2016 Christopher Baker <http://christopherbaker.net>
// Copyright (c) -2014 Tom Carden <https://github.com/RandomEtc>
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
