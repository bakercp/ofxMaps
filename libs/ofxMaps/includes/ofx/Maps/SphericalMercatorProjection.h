//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
// Copyright (c) -2014 Tom Carden <https://github.com/RandomEtc>
//
// SPDX-License-Identifier:	MIT
//


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
    /// \param name The name of the projection.
    /// \param zoom The default zoom used when calculating projections.
    SperhicalMercatorProjection();

    /// \brief Destroy the SperhicalMercatorProjection.
    virtual ~SperhicalMercatorProjection();

    enum
    {
        /// \brief The default zoom level used by this projection.
        DEFAULT_ZOOM = 0
    };

    static const std::string EPSG_3857;

protected:
    glm::dvec2 rawProject(const glm::dvec2& point) const;
    glm::dvec2 rawUnproject(const glm::dvec2& point) const;

};


} } // namespace ofx::Maps
