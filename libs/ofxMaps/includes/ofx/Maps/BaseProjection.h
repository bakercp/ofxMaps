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


#pragma once


#include "ofConstants.h"
#include "ofx/Geo/Coordinate.h"
#include "ofx/Maps/Transformation.h"
#include "ofx/Maps/TileCoordinate.h"


namespace ofx {
namespace Maps {


/// \brief A class responsible for projecting coordinates.
class BaseProjection
{
public:
    /// \brief Construct a BaseProjection with a default zoom and Transformation.
    /// \param zoom The default zoom level used by this Projection.
    /// \param transformation The transformation used by this projection.
    BaseProjection(const std::string& name,
                   double zoom,
                   const Transformation& transformation);

    /// \brief Destroy the BaseProjection.
    virtual ~BaseProjection();

    /// \returns the name of this projection.
    std::string name() const;

    /// \returns the default zoom level used with this projection.
    double zoom() const;

    /// \brief Get the TileCoordinate from the given Geo::Coordinate at the default zoom.
    /// \param location The the Geo::Coordinate at the default zoom level.
    /// \returns the TileCoordinate at the default zoom level.
    TileCoordinate geoToTile(const Geo::Coordinate& location) const;
    
    /// \brief Get the GeoCoordinate from the given TileCoordinate.
    /// \param coordinate The TileCoordinate to transform to a Geo::Coordinate.
    /// \returns the GeoCoordinate corresponding to the TileCoordinate.
    Geo::Coordinate tileToGeo(const TileCoordinate& coordinate) const;

protected:
    /// \brief Calculate the raw projection between two points.
    /// \param point The point to be projected.
    /// \returns The projected point.
	virtual glm::dvec2 rawProject(const glm::dvec2& unProjectedPoint) const = 0;

    /// \brief Calculate the raw reverse projection between two points.
    /// \param point The point to be unprojected.
    /// \returns The unprojected point.
	virtual glm::dvec2 rawUnproject(const glm::dvec2& projectedPoint) const = 0;

	glm::dvec2 project(const glm::dvec2& point) const;
	glm::dvec2 unproject(const glm::dvec2& point) const;

    /// \brief The default zoom level used by this Projection.
	double _zoom;

    /// \brief The transformation used by this projection.
	Transformation _transformation;

    /// \brief The name of the proejction.
    std::string _name;
};


} } // namespace ofx::Maps
