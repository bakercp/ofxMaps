// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
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


#include <set>
#include "ofBaseTypes.h"
#include "ofFbo.h"
#include "ofx/Maps/BaseTileProvider.h"
#include "ofx/Maps/BaseURITileProvider.h"
#include "ofx/Maps/TileCoordinate.h"
#include "ofx/Maps/TileLoader.h"


namespace ofx {
namespace Maps {


class TileLayer: public ofBaseDraws
{
public:
    typedef std::shared_ptr<TileLayer> SharedPtr;
    typedef std::shared_ptr<BaseURITileProvider> Provider;

	TileLayer();

    virtual ~TileLayer();

    void setup(Provider provider,
               int width,
               int height);

    void draw(float x, float y) const;

	void draw(float x, float y, float w, float h) const;

    ofVec2d getSize() const;

    void setSize(const ofVec2d& size);

    float getWidth() const;

    void setWidth(double width);

    float getHeight() const;

    void setHeight(double height);

    const TileCoordinate& getCenter() const;

    void setCenter(const TileCoordinate& center);

    void setCenter(const Geo::Coordinate& center, double zoom);

protected:
    std::set<TileCoordinate> getVisibleCoordinates() const;

    TileCoordinate layerPointToTileCoordinate(const ofVec2d& layerPoint) const;

    /// \brief The Map tile Provider.
    Provider _provider;

    /// \brief Layer width.
    double _width;

    /// \brief Layer height.
    double _height;

    int _padColumn;

    int _padRow;

    /// \brief Pan and anchor coordinate.
    TileCoordinate _center;

    mutable TileLoader _loader;

    void onTileCached(const TileCoordinate& args);
    void onTileUncached(const TileCoordinate& args);

    mutable std::set<TileCoordinate> _visisbleCoords;

    mutable bool _coordsDirty;

    mutable ofFbo _fbo;
};


} } // namespace ofx::Maps
