//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/Maps/MapTileLayer.h"
#include "ofGraphics.h"


namespace ofx {
namespace Maps {


MapTileLayer::MapTileLayer(std::shared_ptr<MapTileSet> tiles, float width, float height):
    _tiles(tiles),
    _size(width, height),
    _coordsDirty(true),
    _center(0.5, 0.5, 0),
    _padding(0, 0),
    _fboNeedsResize(true),
    _onTileCachedListener(_tiles->onAdd.newListener(this, &MapTileLayer::onTileCached)),
//    _onTileUncachedListener(_tiles->onRemoved.newListener(this, &MapTileLayer::onTileUncached)),
    _onTileRequestCancelledListener(_tiles->onRequestCancelled.newListener(this, &MapTileLayer::onTileRequestCancelled)),
    _onTileRequestFailedListener(_tiles->onRequestFailed.newListener(this, &MapTileLayer::onTileRequestFailed))
{



//    double z = log2(std::min(_width, _height) /
//                    std::min(_provider->tileWidth(),
//                             _provider->tileHeight()));

//    _center = TileCoordinateUtils::zoomTo(_center, z);

}


MapTileLayer::~MapTileLayer()
{
    try
    {
        cancelQueuedRequests();
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("MapTileLayer::~MapTileLayer") << ">> " << exc.displayText();
    }
}


void MapTileLayer::update()
{
    if (_fboNeedsResize)
    {
        _fbo.allocate(_size.x, _size.y);
        _fboNeedsResize = false;
    }

    if (_coordsDirty)
    {
        _visisbleCoords = calculateVisibleCoordinates();
        _coordsDirty = false;
    }
}


void MapTileLayer::draw(float x, float y) const
{
//    _fbo.begin();

//    ofClear(0, 0, 0);

    std::set<TileCoordinate>::const_reverse_iterator iter = _visisbleCoords.rbegin();

    while (iter != _visisbleCoords.rend())
    {
        const TileCoordinate& coord = *iter;

        auto tile = getTile(coord);

        glm::vec2 position = tileToPixels(coord);
        auto dZoom = _center.getZoom() - coord.getZoom();
        double scale = dZoom < 1 ? 1 : std::pow(2.0, dZoom);
        glm::dvec2 tileSize = _tiles->provider()->tileSize() * scale;
        ofPushMatrix();
        ofTranslate(position.x, position.y);

        if (tile)
        {
//            ofScale(1, -1, 0);
            tile->draw(0, 0, tileSize.x, tileSize.y);

//            ofPushStyle();
//            ofNoFill();
//            ofSetColor(255, 0, 0, 127);
//            ofDrawRectangle(position.x, position.y, tileSize.x, tileSize.y);
//            ofPopStyle();

            //ofDrawBitmapStringHighlight(coord.toString(0), position.x + 20, position.y + 20);
        }
        else
        {
            ofPushStyle();
            ofNoFill();
            ofSetColor(255, 127);
            ofDrawRectangle(0, 0, tileSize.x, tileSize.y);
            ofDrawLine(0, 0, tileSize.x, tileSize.y);
            ofDrawLine(tileSize.x, 0, 0, tileSize.y);
            ofPopStyle();
        }
        ofPopMatrix();
        ++iter;
    }

//    if (center.)

//    _fbo.end();
//
//    _fbo.draw(x, y);



    ofSetColor(255);
    ofNoFill();
    ofDrawRectangle(0, 0, _size.x, _size.y);
}


void MapTileLayer::draw(float x, float y, float w, float h) const
{
    ofPushMatrix();
    ofScale(_size.x / w, _size.y / h);
    draw(x, y);
    ofPopMatrix();
}


glm::vec2 MapTileLayer::getSize() const
{
    return _size;
}


void MapTileLayer::setSize(const glm::vec2& size)
{
    setWidth(size.x);
    setHeight(size.y);
}


float MapTileLayer::getWidth() const
{
    return _size.x;
}


void MapTileLayer::setWidth(float width)
{
    _size.x = width;
    _coordsDirty = true;
    _fboNeedsResize = true;
}


float MapTileLayer::getHeight() const
{
    return _size.y;
}


void MapTileLayer::setHeight(float height)
{
    _size.y = height;
    _coordsDirty = true;
    _fboNeedsResize = true;
}


const TileCoordinate& MapTileLayer::getCenter() const
{
    return _center;
}


void MapTileLayer::setCenter(const TileCoordinate& center)
{
    _center = center;
    _coordsDirty = true;
}


void MapTileLayer::setCenter(const Geo::Coordinate& center, double zoom)
{
    setCenter(_tiles->provider()->geoToWorld(center).getZoomedTo(zoom));
}


std::set<TileCoordinate> MapTileLayer::calculateVisibleCoordinates() const
{
    // Round the current zoom in case we are in between levels.
    int baseZoom = glm::clamp(static_cast<int>(std::round(_center.getZoom())),
                              _tiles->provider()->minZoom(),
                              _tiles->provider()->maxZoom());

    // Get the layers points of the current screen.
    glm::dvec2 topLeftPoint(0, 0);
    glm::dvec2 topRightPoint(_size.x, 0);
    glm::dvec2 bottomLeftPoint(0, _size.y);
    glm::dvec2 bottomRightPoint = _size;

    // Translate the layer points to tile coordinates, then zoom them to baseZoom.
    auto topLeft = pixelsToTile(topLeftPoint).getZoomedTo(baseZoom);
    auto topRight = pixelsToTile(topRightPoint).getZoomedTo(baseZoom);
    auto bottomLeft = pixelsToTile(bottomLeftPoint).getZoomedTo(baseZoom);
    auto bottomRight = pixelsToTile(bottomRightPoint).getZoomedTo(baseZoom);

    int minCol = std::floor(std::min(std::min(topLeft.getColumn(),
                                              topRight.getColumn()),
                                     std::min(bottomLeft.getColumn(),
                                              bottomRight.getColumn())));

    int maxCol = std::ceil(std::max(std::max(topLeft.getColumn(),
                                             topRight.getColumn()),
                                    std::max(bottomLeft.getColumn(),
                                             bottomRight.getColumn())));

    int minRow = std::floor(std::min(std::min(topLeft.getRow(),
                                              topRight.getRow()),
                                     std::min(bottomLeft.getRow(),
                                              bottomRight.getRow())));

    int maxRow = std::ceil(std::max(std::max(topLeft.getRow(),
                                             topRight.getRow()),
                                    std::max(bottomLeft.getRow(),
                                             bottomRight.getRow())));

    minCol -= _padding.x;
    minRow -= _padding.y;
    maxCol += _padding.x;
    maxRow += _padding.y;

    int gridSize = TileCoordinate::getScaleForZoom(baseZoom);

    minCol = glm::clamp(minCol, 0, gridSize);
    maxCol = glm::clamp(maxCol, 0, gridSize);
    minRow = glm::clamp(minRow, 0, gridSize);
    maxRow = glm::clamp(maxRow, 0, gridSize);

    std::set<TileCoordinate> coordinatesToDraw;
    std::set<TileCoordinate> requestedCoordinates;

    // Collect visible tile coordinates.

    for (int col = minCol; col < maxCol; ++col)
    {
        for (int row = minRow; row < maxRow; ++row)
        {
            TileCoordinate coord(col, row, baseZoom);

            // Do we have this tile?
            if (!hasTile(coord))
            {
                requestedCoordinates.insert(coord);

//                // Since we don't have this coordinate, let's try to find an
//                // existing coordinate at a different scale that we can draw in
//                // its place.
//
//                // First we look backwards from the current zoom level.
//
//                bool foundParent = false;
//
//				for (int i = coord.getZoom(); i >= _tiles->provider()->minZoom(); --i)
//                {
//                    auto parentCoord = coord.getZoomedTo(i);
//
//                    parentCoord = TileCoordinate(parentCoord.getFlooredColumn(),
//                                                 parentCoord.getFlooredRow(),
//                                                 parentCoord.getZoom());
//
//                    parentCoord = parentCoord.getClampedRowAndColumn();
//
//                    if (!hasTile(coord))
//                    {
//                        // We don't have the parent coordinate, so let's request
//                        // it.
//                        requestedCoordinates.insert(parentCoord);
//                    }
//                    else
//                    {
//                        // If we do have the parent coordinate, add it to the
//                        // drawing queue.
//                        coordinatesToDraw.insert(parentCoord);
//                        foundParent = true;
//                        break;
//                    }
//				}


//                if (!foundParent)
//                {
//					TileCoordinate zoomed = coord.getZoomedBy(1);
//					std::vector<TileCoordinate> kids;
//					kids.push_back(zoomed);
//					kids.push_back(zoomed.getCoordinateRight());
//					kids.push_back(zoomed.getCoordinateDown());
//					kids.push_back(zoomed.getCoordinateRight().getCoordinateDown());
//					for (int i = 0; i < kids.size(); ++i)
//                    {
//                        coordinatesToDraw.insert(kids[i]);
//					}
//				}
            }
            else
            {
                // We have it so, add it to the drawing queue.
                coordinatesToDraw.insert(coord);
            }
        }
    }

    requestTiles(requestedCoordinates);

    return coordinatesToDraw;
}


TileKey MapTileLayer::keyForCoordinate(const TileCoordinate& coordinate) const
{
    return TileKey(coordinate.getFlooredColumn(),
                   coordinate.getFlooredRow(),
                   coordinate.getFlooredZoom(),
                   _setId);
}


bool MapTileLayer::hasTile(const TileCoordinate& coordinate) const
{
    return _tiles->has(keyForCoordinate(coordinate));
}


std::shared_ptr<Tile> MapTileLayer::getTile(const TileCoordinate& coordinate) const
{
    return _tiles->get(keyForCoordinate(coordinate));
}


void MapTileLayer::cancelQueuedRequests() const
{
    for (const auto& requestId: _outstandingRequests)
    {
        try
        {
            _tiles->cancelQueuedRequest(requestId);
        }
        catch (const std::exception& exc)
        {
            //std::cout << "<<>>" << exc.what() << std::endl;
        }
    }
}


void MapTileLayer::requestTiles(const std::set<TileCoordinate>& coordinates) const
{
    //cancelQueuedRequests();

    std::vector<TileCoordinate> coordinatesToRequest(coordinates.begin(),
                                                     coordinates.end());

    QueueSorter sorter(_center);

    std::sort(coordinatesToRequest.begin(),
              coordinatesToRequest.end(),
              sorter);

    for (const auto& coordinate: coordinatesToRequest)
    {
        try
        {
            auto key = keyForCoordinate(coordinate);
            _tiles->request(key);
            _outstandingRequests.insert(key);
        }
        catch (const Poco::ExistsException& exc)
        {
            //std::cout << exc.displayText() << endl;
        }
    }
}


void MapTileLayer::onTileCached(const std::pair<TileKey, std::shared_ptr<Tile>>& args)
{
//    std::cout << "tile cached!" << std::endl;
    _coordsDirty = true;
}


void MapTileLayer::onTileUncached(const TileKey& args)
{
//    std::cout << "tile uncached!" << std::endl;
//    _coordsDirty = true;
}


void MapTileLayer::onTileRequestCancelled(const TileKey& key)
{
    _outstandingRequests.erase(key);
}


void MapTileLayer::onTileRequestFailed(const Cache::RequestFailedArgs<TileKey>& args)
{
    ofLogError("MapTileLayer::onTileRequestFailed") << "Failed to load " << args.key().toString() << ": " << args.error();
    _outstandingRequests.erase(args.key());
}


void MapTileLayer::setSetId(const std::string& setId)
{
    _setId = setId;
    _coordsDirty = true;
}


std::string MapTileLayer::getSetId() const
{
    return _setId;
}


TileCoordinate MapTileLayer::pixelsToTile(const glm::vec2& pixelCoordinate) const
{
    glm::dvec2 factor = (pixelCoordinate - (_size * 0.5)) / _tiles->provider()->tileSize();

    return _center.getNeighbor(factor.x, factor.y);

//    return TileCoordinate(_center.getColumn() + factor.x,
//                          _center.getRow() + factor.y,
//                          _center.getZoom());
}


glm::vec2 MapTileLayer::tileToPixels(const TileCoordinate& tileCoordinate) const
{
    glm::dvec2 pixelCenter = _size * 0.5;

    double scale = std::pow(2.0, _center.getZoom() - tileCoordinate.getZoom());

    glm::dvec2 scaledTileSize = _tiles->provider()->tileSize() * scale;

    TileCoordinate zoomedCenterCoordinate = _center.getZoomedTo(tileCoordinate.getZoom());

    glm::dvec2 _coordinate(tileCoordinate.getColumn() - zoomedCenterCoordinate.getColumn(),
                           tileCoordinate.getRow() - zoomedCenterCoordinate.getRow());

    glm::dvec2 result = pixelCenter + (scaledTileSize * _coordinate);

    return result;
}


Geo::Coordinate MapTileLayer::pixelsToGeo(const glm::vec2& pixelCoordinate) const
{
    return _tiles->provider()->tileToGeo(pixelsToTile(pixelCoordinate).getZoomedTo(_center.getZoom()));
}


glm::vec2 MapTileLayer::geoToPixels(const Geo::Coordinate& geoCoordinate) const
{
    return tileToPixels(_tiles->provider()->geoToWorld(geoCoordinate).getZoomedTo(_center.getZoom()));
}


} } // namespace ofx::Maps
