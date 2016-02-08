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
//
//#include "ofx/Maps/MapNavigator.h"
//
//
//namespace ofx {
//namespace Maps {
//
//
//
//void MapNavigator::keyPressed(ofKeyEventArgs& evt)
//{
//    int key = evt.key;
//
//    if (key == '+' || key == '=')
//    {
//        if (getZoom() < 19)
//        {
//            zoomIn();
//        }
//    }
//    else if (key == '-' || key == '_')
//    {
//        if (getZoom() > 1)
//        {
//            zoomOut();
//        }
//    }
//    // TODO: keyboard movement
//}
//
//void MapNavigator::keyReleased(ofKeyEventArgs& evt)
//{
//}
//
//
//void MapNavigator::mouseDragged(ofMouseEventArgs& evt)
//{
//    //    ofVec2d mouse(evt.x, evt.y);
//    //
//    //    int button = evt.button;
//    //
//    //    ofVec2d dMouse = (mouse - _prevMouse) / _position.z;
//    //
//    //	if (button == 0)
//    //    {
//    //        _position += dMouse;
//    //	}
//    //
//    //    _prevMouse = mouse;
//}
//
//
//void MapNavigator::mouseMoved(ofMouseEventArgs& evt)
//{
//}
//
//
//void MapNavigator::mousePressed(ofMouseEventArgs& evt)
//{
//    ofVec2d mouse(evt.x, evt.y);
//
//    int button = evt.button;
//
//    unsigned long long now = ofGetElapsedTimeMillis();
//    unsigned long long dt = now - _lastClickTime;
//
//    if (dt < DEFAULT_DOUBLE_CLICK_TIME)
//    {
//        // Should center.
//        setPointCenter(mouse);
//        zoomIn();
//    }
//
//    _lastClickTime = now;
//    _prevMouse = mouse;
//
//}
//void MapNavigator::mouseReleased(ofMouseEventArgs& evt)
//{
//}
//
//
//double MapNavigator::getZoom() const
//{
//    return _centerTileCoordinate.getZoom();
//}
//
//
//// sets scale according to given zoom level, should leave you with pixel perfect tiles
//void MapNavigator::setZoom(double zoom)
//{
//    _centerTileCoordinate = _centerTileCoordinate.zoomTo(zoom);
//}
//
//
//void MapNavigator::zoom(double zoomStep)
//{
//    setZoom(getZoom() + zoomStep);
//}
//
//
//void MapNavigator::zoomIn()
//{
//    setZoom(getZoom() + 1);
//}
//
//void MapNavigator::zoomOut()
//{
//    setZoom(getZoom() - 1);
//}
//
//
//void MapNavigator::panBy(const ofVec2d& delta)
//{
//    panBy(delta.x, delta.y);
//}
//
//
//void MapNavigator::panBy(double dx, double dy)
//{
//    double sinr = sin(_rotation);
//    double cosr = cos(_rotation);
//
//    double dxr = dx * cosr + dy * sinr;
//    double dyr = dy * cosr - dx * sinr;
//
//    ofVec2d tileSize(_provider->getTileWidth(), _provider->getTileHeight());
//
//    _centerTileCoordinate.setColumn(_centerTileCoordinate.getColumn() - dxr / tileSize.x);
//    _centerTileCoordinate.setRow(_centerTileCoordinate.getRow() - dyr / tileSize.y);
//}
//
//
//void MapNavigator::scaleBy(double s)
//{
//    scaleBy(s, _size * 0.5);
//}
//
//
//void MapNavigator::scaleBy(double s, const ofVec2d& c)
//{
//    scaleBy(s, c.x, c.y);
//}
//
//
//void MapNavigator::scaleBy(double s, double cx, double cy)
//{
//    const double prevRotation = _rotation;
//    rotateBy(-prevRotation, cx, cy);
//    ofVec2d center = _size * 0.5;
//    panBy(-cx + center.x, -cy + center.y);
//    _centerTileCoordinate = _centerTileCoordinate.zoomBy(log2(s));
//    panBy(cx - center.x, cy - center.y);
//    rotateBy(prevRotation, cx, cy);
//}
//
//
//void MapNavigator::rotateBy(double r, double cx, double cy)
//{
//    panBy(-cx, -cy);
//    _rotation += r;
//    panBy(cx, cy);
//}
//
//
//    Geo::Coordinate MapNavigator::getGeoLocationCenter() const
//    {
//        return _provider->tileCoordinateToGeoCoordinate(_centerTileCoordinate);
//    }
//
//
//    TileCoordinate MapNavigator::getTileCoordinateCenter() const
//    {
//        return _centerTileCoordinate;
//    }
//
//
//    void MapNavigator::setPointCenter(const ofVec2d& center)
//    {
//        setTileCoordinateCenter(pointToTileCoordinate(center));
//    }
//
//
//    void MapNavigator::setGeoLocationCenter(const Geo::Coordinate& location)
//    {
//        setTileCoordinateCenter(_provider->geoCoordinateToTileCoordinate(location).zoomTo(getZoom()));
//    }
//
//
//    void MapNavigator::setTileCoordinateCenter(const TileCoordinate& center)
//    {
//        _centerTileCoordinate = center;
//    }
//
//
//    TileCoordinate MapNavigator::pointToTileCoordinate(const ofVec2d& point) const
//    {
//        /* Return a coordinate on the map image for a given x, y point. */
//        // new point coordinate reflecting distance from map center, in tile widths
//        ofVec2d rotated(point);
//
//        const ofVec2d tileSize(_provider->getTileWidth(), _provider->getTileHeight());
//
//        rotated.rotate(-_rotation * RAD_TO_DEG);
//
//        TileCoordinate coord(_centerTileCoordinate);
//
//        coord += (rotated - (_size / 2.0)) / tileSize;
//
//        return coord;
//    }
//
//
//    ofVec2d MapNavigator::tileCoordinateToPoint(const TileCoordinate& target) const
//    {
//        TileCoordinate coord = target;
//
//        if(coord.getZoom() != _centerTileCoordinate.getZoom())
//        {
//            coord = coord.zoomTo(_centerTileCoordinate.getZoom());
//        }
//
//        // distance from the center of the map
//        const ofVec2d tileSize(_provider->getTileWidth(), _provider->getTileHeight());
//
//        ofVec2d point = _size * 0.5;
//
//        point.x += tileSize.x * (coord.getColumn() - _centerTileCoordinate.getColumn());
//        point.y += tileSize.y * (coord.getRow() - _centerTileCoordinate.getRow());
//
//        ofVec2d rotated(point);
//
//        rotated.rotate(_rotation * RAD_TO_DEG);
//        
//        return rotated;
//    }
//    
//    
//    ofVec2d MapNavigator::geoLocationToPoint(const Geo::Coordinate& location) const
//    {
//        return tileCoordinateToPoint(_provider->geoCoordinateToTileCoordinate(location));
//    }
//    
//    
//    Geo::Coordinate MapNavigator::pointToGeolocation(const ofVec2d& point) const
//    {
//        return _provider->tileCoordinateToGeoCoordinate(pointToTileCoordinate(point));
//    }
//
//
//} } // namespace ofx::Maps
