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


#include <set>
#include "ofBaseTypes.h"
#include "ofFbo.h"
#include "ofTypes.h"
#include "ofx/Maps/TileProvider.h"
#include "ofx/Maps/TileCoordinate.h"
#include "ofx/Maps/TileLoader.h"


namespace ofx {
namespace Maps {


/// \brief A tile layer is a surface on which tiles are rendered.
///
/// A tile layer has a tile provider, a width and a height.
class TileLayer: public ofBaseDraws
{
public:
    TileLayer(std::shared_ptr<TileProvider> provider,
              int width = 256,
              int height = 256,
              std::shared_ptr<TileLoader> loader = std::make_shared<TileLoader>());

    virtual ~TileLayer();

    void update();

    void draw(float x, float y) const override;

	void draw(float x, float y, float w, float h) const override;

    glm::vec2 getSize() const;

    void setSize(const glm::vec2& size);

    float getWidth() const override;

    void setWidth(double width);

    float getHeight() const override;

    void setHeight(double height);

    const TileCoordinate& getCenter() const;

    void setCenter(const TileCoordinate& center);

    void setCenter(const Geo::Coordinate& center, double zoom);

    void setSetId(const std::string& setId);

    std::string getSetId() const;

    TileCoordinate pixelsToTile(const glm::vec2& pixelCoordinate) const;
    glm::vec2 tileToPixels(const TileCoordinate& tileCoordinate) const;

    Geo::Coordinate pixelsToGeo(const glm::vec2& pixelCoordinate) const;
    glm::vec2 geoToPixels(const Geo::Coordinate& geoCoordinate) const;

//    Geo::CoordinateBounds visibleBounds() const;

protected:
    TileCoordinateKey keyForCoordinate(const TileCoordinate& coordinate) const;

    void cancelQueuedRequests() const;

    bool hasTile(const TileCoordinate& coordinate) const;

    std::shared_ptr<Tile> getTile(const TileCoordinate& coordinate) const;

    void requestTiles(const std::set<TileCoordinate>& coordinates) const;

    virtual std::set<TileCoordinate> calculateVisibleCoordinates() const;

    /// \brief The Map tile Provider.
    std::shared_ptr<TileProvider> _provider = nullptr;

    /// \brief The current set id being viewed.
    std::string _setId;

    /// \brief The size of the tile layer.
    glm::vec2 _size;

    /// \brief The padding of the layer.
    glm::ivec2 _padding;

    /// \brief Pan and anchor coordinate.
    TileCoordinate _center;

    /// \brief The tile loader.
    mutable std::shared_ptr<TileLoader> _loader;

    void onTileCached(const TileCoordinateKey& args);
    void onTileUncached(const TileCoordinateKey& args);
    void onTileRequestCancelled(const std::string& args);

    /// \brief The current visible coordinates.
    mutable std::set<TileCoordinate> _visisbleCoords;
    mutable std::set<std::string> _outstandingRequests;

    mutable bool _coordsDirty = true;
    mutable bool _fboNeedsResize = true;

    mutable ofFbo _fbo;

    ofEventListener _onTileCachedListener;
    ofEventListener _onTileUncachedListener;
    ofEventListener _onTileRequestCancelledListener;

};


} } // namespace ofx::Maps
