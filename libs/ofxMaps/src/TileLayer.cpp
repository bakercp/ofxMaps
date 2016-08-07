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


#include "ofx/Maps/TileLayer.h"
#include "ofGraphics.h"


namespace ofx {
namespace Maps {


TileLayer::TileLayer(std::shared_ptr<TileProvider> provider,
                     int width,
                     int height,
                     std::shared_ptr<TileLoader> loader):
    _provider(provider),
    _size(width, height),
    _loader(loader),
    _coordsDirty(true),
    _center(0.5, 0.5, 0),
    _padding(0, 0),
    _fboNeedsResize(true),
    _onTileCachedListener(_loader->onTileCached.newListener(this, &TileLayer::onTileCached)),
    _onTileUncachedListener(_loader->onTileUncached.newListener(this, &TileLayer::onTileUncached)),
    _onTileRequestCancelledListener(_loader->onTaskCancelled.newListener(this, &TileLayer::onTileRequestCancelled))
{



//    double z = log2(std::min(_width, _height) /
//                    std::min(_provider->tileWidth(),
//                             _provider->tileHeight()));

//    _center = TileCoordinateUtils::zoomTo(_center, z);

}


TileLayer::~TileLayer()
{
    try
    {
        cancelQueuedRequests();
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("TileLayer::~TileLayer") << exc.displayText();
    }
}


void TileLayer::update()
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


glm::vec2 TileLayer::getSize() const
{
    return _size;
}


void TileLayer::setSize(const glm::vec2& size)
{
    setWidth(size.x);
    setHeight(size.y);
}


float TileLayer::getWidth() const
{
    return _size.x;
}


void TileLayer::setWidth(double width)
{
    _size.x = width;
    _coordsDirty = true;
    _fboNeedsResize = true;
}


float TileLayer::getHeight() const
{
    return _size.y;
}


void TileLayer::setHeight(double height)
{
    _size.y = height;
    _coordsDirty = true;
    _fboNeedsResize = true;
}


const TileCoordinate& TileLayer::getCenter() const
{
    return _center;
}


void TileLayer::setCenter(const TileCoordinate& center)
{
    _center = center;
    _coordsDirty = true;
}


void TileLayer::setCenter(const Geo::Coordinate& center, double zoom)
{
    if (_provider != nullptr)
    {
        setCenter(_provider->geoToTile(center).getZoomedTo(zoom));
    }
    else
    {
        ofLogError("TileLayer::setCenter") << "Provider is not defined.";
    }
}


std::set<TileCoordinate> TileLayer::calculateVisibleCoordinates() const
{
    if (_provider == nullptr)
    {
        ofLogError("TileLayer::layerPointToTileCoordinate") << "Provider is not defined.";
        std::set<TileCoordinate> coordinates;
        return coordinates;
    }

    // Round the current zoom in case we are in between levels.
    int baseZoom = glm::clamp(static_cast<int>(std::round(_center.getZoom())),
                              _provider->minZoom(),
                              _provider->maxZoom());

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

   int gridSize = TileCoordinateUtils::getScaleForZoom(baseZoom);

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

                // Since we don't have this coordinate, let's try to find an
                // existing coordinate at a different scale that we can draw in
                // its place.

                // First we look backwards from the current zoom level.

                bool foundParent = false;

				for (int i = coord.getZoom(); i >= _provider->minZoom(); --i)
                {
                    auto parentCoord = coord.getZoomedTo(i);
                    parentCoord = TileCoordinateUtils::floorRowAndColumn(parentCoord);
                    parentCoord = TileCoordinateUtils::clampRowAndColumn(parentCoord);

                    if (!hasTile(coord))
                    {
                        // We don't have the parent coordinate, so let's request
                        // it.
                        requestedCoordinates.insert(parentCoord);
                    }
                    else
                    {
                        // If we do have the parent coordinate, add it to the
                        // drawing queue.
                        coordinatesToDraw.insert(parentCoord);
                        foundParent = true;
                        break;
                    }
				}


//                if (!foundParent)
//                {
//					TileCoordinate zoomed = coord.getZoomedBy(1);
//					std::vector<Coordinate> kids;
//					kids.push_back(zoomed);
//					kids.push_back(zoomed.right());
//					kids.push_back(zoomed.down());
//					kids.push_back(zoomed.right().down());
//					for (int i = 0; i < kids.size(); i++)
//                    {
//						if (images.count(kids[i]) > 0)
//                        {
//							visibleKeys.insert(kids[i]);
//						}
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


TileCoordinateKey TileLayer::keyForCoordinate(const TileCoordinate& coordinate) const
{
    return TileCoordinateKey(TileData(_provider->id(),
                                      _setId,
                                      TileData::DEFAULT_TILE_ID),
                             coordinate);
}


bool TileLayer::hasTile(const TileCoordinate& coordinate) const
{
    return _loader->has(keyForCoordinate(coordinate));
}


std::shared_ptr<Tile> TileLayer::getTile(const TileCoordinate& coordinate) const
{
    return _loader->get(keyForCoordinate(coordinate));
}


void TileLayer::cancelQueuedRequests() const
{
    for (const auto& requestId: _outstandingRequests)
    {
        _loader->cancelQueuedRequest(requestId);
    }
}


void TileLayer::requestTiles(const std::set<TileCoordinate>& coordinates) const
{
//    cancelQueuedRequests();

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


            TileLoaderRequest request(key, _provider->getTileURI(key));
            std::string requestId = _loader->request(request);
            _outstandingRequests.insert(requestId);
        }
        catch (const Poco::ExistsException& exc)
        {
            std::cout << exc.displayText() << endl;
        }
    }
}


//TileCoordinate TileLayer::layerPointToTileCoordinate(const glm::dvec2& layerPoint) const
//{
//    TileCoordinate coord = getCenter();
//
//    if (_provider != nullptr)
//    {
//
//        glm::dvec2 layerSize(_width, _height);
//
//        glm::dvec2 tileSize(_provider->tileWidth(),
//                            _provider->tileHeight());
//
//        glm::dvec2 factor = (layerPoint - layerSize / 2.0) / tileSize;
//
//        coord.setColumn(coord.getColumn() + factor.x);
//        coord.setRow(coord.getRow() + factor.y);
//    }
//    else
//    {
//        ofLogError("TileLayer::layerPointToTileCoordinate") << "Provider is not defined.";
//    }
//
//    return coord;
//}


void TileLayer::onTileRequestCancelled(const std::string& args)
{
    _outstandingRequests.erase(args);
}


void TileLayer::onTileCached(const TileCoordinateKey& args)
{
    _coordsDirty = true;
}


void TileLayer::onTileUncached(const TileCoordinateKey& args)
{
//    _coordsDirty = true;
}


void TileLayer::setSetId(const std::string& setId)
{
    _setId = setId;
    _coordsDirty = true;
}


std::string TileLayer::getSetId() const
{
    return _setId;
}


TileCoordinate TileLayer::pixelsToTile(const glm::vec2& coordinates) const
{
    TileCoordinate coord = getCenter();

    if (_provider != nullptr)
    {
        glm::vec2 tileSize(_provider->tileWidth(),
                            _provider->tileHeight());

        glm::vec2 factor = (coordinates - _size / 2.0) / tileSize;

        coord.setColumn(coord.getColumn() + factor.x);
        coord.setRow(coord.getRow() + factor.y);
    }
    else
    {
        ofLogError("TileLayer::layerPointToTileCoordinate") << "Provider is not defined.";
    }
    
    return coord;
}


glm::vec2 TileLayer::tileToPixels(const TileCoordinate& coordinates) const
{
    TileCoordinate c = coordinates.getZoomedTo(_center.getZoom());
    
    return glm::vec2();
}


Geo::Coordinate TileLayer::pixelsToGeo(const glm::vec2& coordinates) const
{
    return Geo::Coordinate();
}


glm::vec2 TileLayer::geoToPixels(const Geo::Coordinate& coordinates) const
{
    return glm::vec2();
}


} } // namespace ofx::Maps
