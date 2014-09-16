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
#include "ofx/Maps/BaseTileProvider.h"
#include "ofx/Maps/TileCoordinate.h"
#include "ofx/Maps/TileLoader.h"


namespace ofx {
namespace Maps {


class BaseTileLayer: public ofBaseDraws
{
public:
	BaseTileLayer(BaseTileProvider& provider, double width, double height);

    virtual ~BaseTileLayer();

    void draw(float x, float y);

	void draw(float x, float y, float w, float h);

    ofVec2d getSize() const;

    float getWidth() const;

    void setWidth(double width);

    float getHeight() const;

    void setHeight(double height);

    const TileCoordinate& getCenter() const;

    void setCenter(const TileCoordinate& center);

protected:
//    bool onTileLoaded(const TileEventArgs& args);
//    bool onTileError(const TileErrorEventArgs& args);


    /// \brief The Map tile Provider.
    BaseTileProvider& _provider;
//    BaseTileLoader& _tileLoader;

    /// \brief Map width.
    double _width;

    /// \brief Map height.
    double _height;

    /// \brief Pan and anchor coordinate.
    TileCoordinate _center;

    std::set<TileCoordinate> getVisibleCoordinates() const;

    /// \brief Allow the MapNavigator class to have direct access.
    friend class MapNavigator;

    TileCoordinate pointToTileCoordinate(const ofVec2d& point) const
    {
        TileCoordinate coord = getCenter();

        coord += (point - getSize() / 2.0) / _provider.getTileSize();

        return coord;
    }

};


} } // namespace ofx::Maps
