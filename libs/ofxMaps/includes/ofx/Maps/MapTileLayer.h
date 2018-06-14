//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <set>
#include <unordered_map>
#include "ofBaseTypes.h"
#include "ofFbo.h"
#include "ofMath.h"
#include "ofTypes.h"
#include "ofx/Maps/TileCoordinate.h"
#include "ofx/Maps/TileKey.h"
#include "ofx/Maps/MapTileSet.h"
#include "ofMath.h"
#include <unordered_map>
namespace ofx {
namespace Maps {



class QueueSorter
{
public:
    QueueSorter(const TileCoordinate& center): _center(center)
    {
    }

    // TODO: simplify these calculations by using internal vec calculations
    bool operator () (const TileCoordinate& c0, const TileCoordinate& c1) const
    {
        if (ofIsFloatEqual(c0.getZoom(), _center.getZoom()))
        {
            if (ofIsFloatEqual(c1.getZoom(), _center.getZoom()))
            {
                glm::dvec2 offset(0.5, 0.5);
                glm::dvec2 center2d(_center.getColumn(), _center.getRow());
                glm::dvec2 c02d(c0.getColumn(), c0.getRow());
                glm::dvec2 c12d(c1.getColumn(), c1.getRow());
                double d0 = glm::distance2(center2d, c02d + offset);
                double d1 = glm::distance2(center2d, c12d + offset);
                return d0 < d1;
            }
        }
        else if (ofIsFloatEqual(c1.getZoom(), _center.getZoom()))
        {
            return false;
        }
        else
        {
            double d0 = std::fabs(c0.getZoom() - _center.getZoom());
            double d1 = std::fabs(c1.getZoom() - _center.getZoom());
            return d0 < d1;
        }

        return false;
    }

protected:
    TileCoordinate _center;

};



/// \brief A tile layer is a surface on which tiles are rendered.
///
/// A tile layer has a tile provider, a width and a height.
class MapTileLayer: public ofBaseDraws
{
public:
    MapTileLayer(std::shared_ptr<MapTileSet> store, float width, float height);

    virtual ~MapTileLayer();

    void update();

    void draw(float x, float y) const override;

    void draw(float x, float y, float w, float h) const override;

    glm::vec2 getSize() const;

    void setSize(const glm::vec2& size);

    float getWidth() const override;

    void setWidth(float width);

    float getHeight() const override;

    void setHeight(float height);

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
//    virtual void drawMissing(const TileCoordinate& coordinate) const;

    TileKey keyForCoordinate(const TileCoordinate& coordinate) const;

    void cancelQueuedRequests() const;

    bool hasTile(const TileCoordinate& coordinate) const;

    std::shared_ptr<Tile> getTile(const TileCoordinate& coordinate) const;

    void requestTiles(const std::set<TileCoordinate>& coordinates) const;

    virtual std::set<TileCoordinate> calculateVisibleCoordinates() const;

    /// \brief The tile store to render.
    std::shared_ptr<MapTileSet> _tiles;

    /// \brief The current set id being viewed.
    std::string _setId;

    /// \brief The size of the tile layer.
    glm::vec2 _size;

    /// \brief The padding of the layer.
    glm::ivec2 _padding;

    /// \brief Pan and anchor coordinate.
    TileCoordinate _center;

    void onTileCached(const std::pair<TileKey, std::shared_ptr<Tile>>& args);
    void onTileUncached(const TileKey& args);
    void onTileRequestCancelled(const TileKey& key);
    void onTileRequestFailed(const Cache::RequestFailedArgs<TileKey>& args);

    /// \brief The current visible coordinates.
    mutable std::set<TileCoordinate> _visisbleCoords;
    mutable std::set<TileKey> _outstandingRequests;

    std::unordered_map<TileCoordinate, std::shared_ptr<Tile>> _tilesToDraw;

    mutable bool _coordsDirty = true;
    mutable bool _fboNeedsResize = true;

    mutable ofFbo _fbo;

    ofEventListener _onTileCachedListener;
    ofEventListener _onTileUncachedListener;
    ofEventListener _onTileRequestCancelledListener;
    ofEventListener _onTileRequestFailedListener;

};


} } // namespace ofx::Maps
