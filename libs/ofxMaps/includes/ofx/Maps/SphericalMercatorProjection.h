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


#pragma once


#include "ofx/Maps/BaseProjection.h"


namespace ofx {
namespace Maps {


/// \brief An implementation of the EPSG:3857 project used by most tiled maps.
///
/// \sa http://gis.stackexchange.com/questions/48949/epsg-3857-or-4326-for-googlemaps-openstreetmap-and-leaflet
/// \sa http://spatialreference.org/ref/sr-org/epsg3857/
class SperhicalMercatorProjection: public BaseProjection
{
public:
    /// \brief Create a default SperhicalMercatorProjection.
    /// \param zoom The default zoom used when calculating projections.
    /// \param t The transformation used by this projection.
    SperhicalMercatorProjection(double zoom = DEFAULT_ZOOM);

    /// \brief Destroy the SperhicalMercatorProjection.
    virtual ~SperhicalMercatorProjection();

    /// \brief The minimum latitude represented by this projection.
    static const double MINIMUM_LATITUDE;

    /// \brief The maximum latitude represented by this projection.
	static const double MAXIMUM_LATITUDE;

    /// \brief The minimum longitude represented by this projection.
	static const double MINIMUM_LONGITUDE;

    /// \brief The maximum longitude represented by this projection.
	static const double MAXIMUM_LONGITUDE;

    enum
    {
        /// \brief The default zoom level used by this projection.
        DEFAULT_ZOOM = 0
    };

protected:
	glm::dvec2 rawProject(const glm::dvec2& point) const;
	glm::dvec2 rawUnproject(const glm::dvec2& point) const;

};

    
} } // namespace ofx::Maps
