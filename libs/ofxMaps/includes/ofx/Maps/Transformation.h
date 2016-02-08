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


#include "ofx/Geo/GeoUtils.h"
#include "ofMain.h"


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

    ofVec2d transform(const ofVec2d& point) const;
    
    ofVec2d untransform(const ofVec2d& point) const;

    /// \brief Solves a system of linear equations.
    ///
    ///     t1 = (a * r1) + (b + s1) + c
    ///     t2 = (a * r2) + (b + s2) + c
    ///     t3 = (a * r3) + (b + s3) + c
    ///
    /// r1 - t3 are the known values.
    /// a, b, c are the unknowns to be solved.
    ///
    /// \returns true iff a, b and c are valid.
    static bool linearSolution(double r1, double s1, double t1,
                               double r2, double s2, double t2,
                               double r3, double s3, double t3,
                               double& a, double& b, double& c);

    double ax;
    double bx;
    double cx;
    double ay;
    double by;
    double cy;

};


} } // namespace ofx::Maps
