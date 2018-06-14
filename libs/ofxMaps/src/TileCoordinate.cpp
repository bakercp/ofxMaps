//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
// Copyright (c) -2014 Tom Carden <https://github.com/RandomEtc>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/Maps/TileCoordinate.h"
#include "Poco/Exception.h"
#include "ofMath.h"
#include "ofVectorMath.h"
#include "ofx/IO/Hash.h"
#include "glm/common.hpp"
#include <iomanip>

namespace ofx {
namespace Maps {


TileCoordinate::TileCoordinate(): TileCoordinate(0, 0, 0)
{
}


//TileCoordinate::TileCoordinate(const glm::dvec2& columnRow, double zoom):
//    TileCoordinate(columnRow.x, columnRow.y, zoom)
//{
//}


TileCoordinate::TileCoordinate(double column,
                               double row,
                               double zoom):
    _column(column),
    _row(row),
    _zoom(zoom)
{
}


void TileCoordinate::setColumn(double column)
{
    _column = column;
}


double TileCoordinate::getColumn() const
{
    return _column;
}


double TileCoordinate::getNormalizedColumn() const
{
    double scale = getScaleForZoom(_zoom);

    if (_column < 0 || _column > scale)
    {
        return _column - scale * std::floor(_column / scale);
    }

    return _column;
}



int64_t TileCoordinate::getFlooredColumn() const
{
    return static_cast<int64_t>(std::floor(getNormalizedColumn()));
}


void TileCoordinate::setRow(double row)
{
    _row = row;
}


double TileCoordinate::getRow() const
{
    return _row;
}


double TileCoordinate::getNormalizedRow() const
{
    double scale = getScaleForZoom(_zoom);

    if (_row < 0 || _row > scale)
    {
        return _row - scale * std::floor(_row / scale);
    }

    return _row;
}


int64_t TileCoordinate::getFlooredRow() const
{
    return static_cast<uint64_t>(std::floor(getNormalizedRow()));
}


void TileCoordinate::setZoom(double zoom)
{
    _zoom = zoom;
}


double TileCoordinate::getZoom() const
{
    return _zoom;
}


int64_t TileCoordinate::getFlooredZoom() const
{
    return static_cast<uint64_t>(std::floor(_zoom));
}


double TileCoordinate::scaleForZoom() const
{
    return getScaleForZoom(_zoom);
}


void TileCoordinate::zoomTo(double zoom)
{
    auto scale = getScaleForZoom(zoom - _zoom);
    _column *= scale;
    _row *= scale;
    _zoom = zoom;
}


TileCoordinate TileCoordinate::getZoomedTo(double zoom) const
{
    TileCoordinate result = *this;
    result.zoomTo(zoom);
    return result;
}


void TileCoordinate::zoomBy(double zoom)
{
    auto scale = getScaleForZoom(zoom);
    _column *= scale;
    _row *= scale;
    _zoom += zoom;
}


TileCoordinate TileCoordinate::getZoomedBy(double zoom) const
{
    TileCoordinate result = *this;
    result.zoomBy(zoom);
    return result;
}


TileCoordinate TileCoordinate::moveRightBy(double distance)
{
    _column += distance;
    return *this;
}


TileCoordinate TileCoordinate::moveLeftBy(double distance)
{
    _column -= distance;
    return *this;
}


TileCoordinate TileCoordinate::moveUpBy(double distance)
{
    _row -= distance;
    return *this;
}

TileCoordinate TileCoordinate::moveDownBy(double distance)
{
    _row += distance;
    return *this;
}


TileCoordinate TileCoordinate::getNeighbor(double columnDistance,
                                           double rowDistance) const
{
    return TileCoordinate(_column + columnDistance,
                          _row + rowDistance,
                          _zoom);
}


TileCoordinate TileCoordinate::getNeighborRight() const
{
    return getNeighbor(1, 0);
}


TileCoordinate TileCoordinate::getNeighborLeft() const
{
    return getNeighbor(-1, 0);
}


TileCoordinate TileCoordinate::getNeighborUp() const
{
    return getNeighbor(0, -1);
}


TileCoordinate TileCoordinate::getNeighborDown() const
{
    return getNeighbor(0, 1);
}


TileCoordinate TileCoordinate::getNormalizedTileCoordinate() const
{
    TileCoordinate coordinate = *this;
    // TODO if this zoom is negative, these while loops could get stuck.

    if (coordinate.getZoom() < 0)
    {
        throw Poco::InvalidArgumentException("Zoom must be >= 0");
    }

    int gridSize = getScaleForZoom(coordinate.getZoom());

    double wrappedColumn = std::fmod(coordinate.getColumn(), gridSize);

    while (wrappedColumn < 0)
    {
        wrappedColumn += gridSize;
    }

    double wrappedRow = std::fmod(coordinate.getRow(), gridSize);

    while (wrappedRow < 0)
    {
        wrappedRow += gridSize;
    }

    return TileCoordinate(wrappedColumn,
                          wrappedRow,
                          coordinate.getZoom());
}


//TileCoordinate TileCoordinate::getFlooredRowAndColumn() const
//{
//    return TileCoordinate(getFlooredColumn(),
//                          getFlooredRow(),
//                          getZoom());
//}


TileCoordinate TileCoordinate::getClampedRowAndColumn() const
{
    double gridSize = getScaleForZoom(getZoom()) - 1;

    return TileCoordinate(glm::clamp(getColumn(), 0.0, gridSize),
                          glm::clamp(getRow(), 0.0, gridSize),
                          getZoom());
}


double TileCoordinate::getScaleForZoom(int zoom)
{
    return std::pow(2.0, zoom);
}


bool TileCoordinate::operator < (const TileCoordinate& coordinate) const
{
    bool areZoomsEqual = ofIsFloatEqual(_zoom, coordinate._zoom);

    return (_zoom < coordinate._zoom)
        || (areZoomsEqual && _row < coordinate._row)
        || (areZoomsEqual && ofIsFloatEqual(_row, coordinate._row) && _column < coordinate._column);
}


bool TileCoordinate::operator == (const TileCoordinate& coordinate) const
{
    return ofIsFloatEqual(_column, coordinate.getColumn())
        && ofIsFloatEqual(_row, coordinate.getRow())
        && ofIsFloatEqual(_zoom, coordinate.getZoom());
}


TileCoordinate& TileCoordinate::operator = (const TileCoordinate& coordinate)
{
    _column = coordinate._column;
    _row = coordinate._row;
    _zoom = coordinate._zoom;
    return *this;
}


std::string TileCoordinate::toString(int precision) const
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << _column << ",";
    ss << std::fixed << std::setprecision(precision) << _row << ",";
    ss << std::fixed << std::setprecision(precision) << _zoom;
    return ss.str();
}


std::size_t TileCoordinate::hash() const
{
    std::size_t seed = 0;
    IO::Hash::combine(seed, _column);
    IO::Hash::combine(seed, _row);
    IO::Hash::combine(seed, _zoom);
    return seed;
}


} } // namespace ofx::Maps
