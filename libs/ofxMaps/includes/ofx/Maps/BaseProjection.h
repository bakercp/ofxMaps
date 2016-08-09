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


#include "ofConstants.h"
#include "ofx/Geo/Coordinate.h"
#include "ofx/Maps/TileCoordinate.h"


namespace ofx {
namespace Maps {


/// \brief A 2D linear transformation.
class Transformation
{
public:
    /// \brief Create an identity Transformation.
    Transformation();

    /// \brief Create a Transform with a, b and c coefficients.
    Transformation(double ax, double bx, double cx,
                   double ay, double by, double cy);

    /// \brief Generates a transform based on three pairs of points.
    ///
    /// a1 -> a2, b1 -> b2, c1 -> c2.
    ///
    /// \returns the derived Transformation.
    Transformation(double a1x, double a1y,
                   double a2x, double a2y,
                   double b1x, double b1y,
                   double b2x, double b2y,
                   double c1x, double c1y,
                   double c2x, double c2y);

    glm::dvec2 transform(const glm::dvec2& point) const;

    glm::dvec2 untransform(const glm::dvec2& point) const;

    /// \brief Solves a system of linear equations.
    ///
    ///     t1 = (x * r1) + (y + s1) + z
    ///     t2 = (x * r2) + (y + s2) + z
    ///     t3 = (x * r3) + (y + s3) + z
    ///
    /// r1 - t3 are the known values.
    /// x, y, z are the unknowns to be solved.
    ///
    /// \returns a vector of a, b, c values.
    static glm::dvec3 linearSolution(double r1, double s1, double t1,
                                     double r2, double s2, double t2,
                                     double r3, double s3, double t3);
    
    double ax = 1.0;
    double bx = 0.0;
    double cx = 0.0;
    double ay = 0.0;
    double by = 1.0;
    double cy = 0.0;
    
};


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
    ///
    /// World corodinates are tile coordinates at the default zoom (typically 0).
    ///
    /// \param location The the Geo::Coordinate at the default zoom level.
    /// \returns the TileCoordinate at the default zoom level.
    TileCoordinate geoToWorld(const Geo::Coordinate& location) const;
    
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
