// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/Maps/TileCoordinate.h"


namespace ofx {
namespace Maps {


const std::string TileCoordinate::DEFAULT_SET_ID = "default";


TileCoordinate::TileCoordinate():
    ofVec3d(0, 0, 0),
    _column(x),
    _row(y),
    _zoom(z),
    _id(DEFAULT_SET_ID)
{
}


TileCoordinate::TileCoordinate(const TileCoordinate& coordinate):
    ofVec3d(coordinate),
    _column(x),
    _row(y),
    _zoom(z),
    _id(coordinate._id)
{
}


TileCoordinate::TileCoordinate(double row,
                               double column,
                               double zoom,
                               const std::string& id):
    ofVec3d(column, row, zoom),
    _column(x),
    _row(y),
    _zoom(z),
    _id(id)
{
}


double TileCoordinate::getColumn() const
{
    return _column;
}


double TileCoordinate::getRow() const
{
    return _row;
}


double TileCoordinate::getZoom() const
{
    return _zoom;
}


const std::string& TileCoordinate::getId() const
{
    return _id;
}


void TileCoordinate::setColumn(double column)
{
    _column = column;
}


void TileCoordinate::setRow(double row)
{
    _row = row;
}


void TileCoordinate::setZoom(double zoom)
{
    _zoom = zoom;
}


void TileCoordinate::setId(const std::string& id)
{
    _id = id;
}


TileCoordinate TileCoordinate::getFloored() const
{
    return TileCoordinate(std::floor(_row), std::floor(_column), _zoom);
}


TileCoordinate TileCoordinate::zoomTo(double destination) const
{
    double p = std::pow(2.0, destination - _zoom);

    return TileCoordinate(_row * p, _column * p, destination);
}

TileCoordinate TileCoordinate::zoomBy(double distance) const
{
    double p = std::pow(2.0, distance);
    
    return TileCoordinate(_row * p, _column * p, _zoom + distance);
}


TileCoordinate TileCoordinate::up(double distance) const
{
    return TileCoordinate(_row - distance, _column, _zoom);
}


TileCoordinate TileCoordinate::right(double distance) const
{
    return TileCoordinate(_row, _column + distance, _zoom);
}


TileCoordinate TileCoordinate::down(double distance) const
{
    return TileCoordinate(_row + distance, _column, _zoom);
}


TileCoordinate TileCoordinate::left(double distance) const
{
    return TileCoordinate(_row, _column - distance, _zoom);
}


bool TileCoordinate::operator < (const TileCoordinate& coordiante) const
{
    return (_zoom < coordiante._zoom)
        || (_zoom == coordiante._zoom && _row < coordiante._row)
        || (_zoom == coordiante._zoom && _row == coordiante._row && _column < coordiante._column);
}


TileCoordinate& TileCoordinate::operator = (const TileCoordinate& coordiante)
{
    set(coordiante);
    return *this;
}


TileCoordinate TileCoordinate::normalizeTileCoordinate(const TileCoordinate& coordinate)
{
    double gridSize = pow(2.0, coordinate.getZoom());

    double wrappedColumn = fmod(coordinate.getColumn(), gridSize);

    while (wrappedColumn < 0)
    {
        wrappedColumn += gridSize;
    }

    double wrappedRow = fmod(coordinate.getRow(), gridSize);

    while (wrappedRow < 0)
    {
        wrappedRow += gridSize;
    }

    return TileCoordinate(wrappedRow, wrappedColumn, coordinate.getZoom());
}


double TileCoordinate::scaleForZoom(int zoom)
{
    return pow(2.0, zoom);
}


} } // namespace ofx::Maps
