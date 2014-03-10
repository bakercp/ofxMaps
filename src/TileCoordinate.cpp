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


#include "TileCoordinate.h"


TileCoordinate::TileCoordinate():
    ofVec3d(0, 0, 0),
    column(x),
    row(y),
    zoom(z)
{
}


TileCoordinate::TileCoordinate(const TileCoordinate& coordinate):
    ofVec3d(coordinate),
    column(x),
    row(y),
    zoom(z)
{
}


TileCoordinate::TileCoordinate(double _row, double _column, double _zoom):
    ofVec3d(_column, _row, _zoom),
    column(x),
    row(y),
    zoom(z)
{
}


double TileCoordinate::getColumn() const
{
    return column;
}


double TileCoordinate::getRow() const
{
    return row;
}


double TileCoordinate::getZoom() const
{
    return zoom;
}


TileCoordinate TileCoordinate::container() const
{
    return TileCoordinate(floor(row), floor(column), zoom);
}


TileCoordinate TileCoordinate::zoomTo(double destination) const
{
    double p = pow(2.0, destination - zoom);

    return TileCoordinate(row * p, column * p, destination);
}

TileCoordinate TileCoordinate::zoomBy(double distance) const
{
    double p = pow(2.0, distance);
    
    return TileCoordinate(row * p, column * p, zoom + distance);
}


TileCoordinate TileCoordinate::up(double distance) const
{
    return TileCoordinate(row - distance, column, zoom);
}


TileCoordinate TileCoordinate::right(double distance) const
{
    return TileCoordinate(row, column + distance, zoom);
}


TileCoordinate TileCoordinate::down(double distance) const
{
    return TileCoordinate(row + distance, column, zoom);
}


TileCoordinate TileCoordinate::left(double distance) const
{
    return TileCoordinate(row, column - distance, zoom);
}


bool TileCoordinate::operator < (const TileCoordinate &c) const
{
    return (zoom < c.zoom)
        || (zoom == c.zoom && row < c.row)
        || (zoom == c.zoom && row == c.row && column < c.column);
}


TileCoordinate& TileCoordinate::operator = (const TileCoordinate& c)
{
    set(c);
    return *this;
}


