//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
// Copyright (c) -2014 Tom Carden <https://github.com/RandomEtc>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <math.h>
#include <string>
#include "ofConstants.h"
#include "ofFileUtils.h"


namespace ofx {
namespace Maps {




/// \brief A tile coordinate in a tiled image system.
class TileCoordinate
{
public:
    TileCoordinate();

//    TileCoordinate(const glm::dvec2& columnRow, double zoom);

    /// \brief Create a tile coordinate with the given parameters.
    /// \param column The coordinate column.
    /// \param row The coordinate row.
    /// \param zoom The coordinate zoom level.
    TileCoordinate(double column,
                   double row,
                   double zoom);

    /// \brief Set the tile column.
    /// \param column The column value to set.
    void setColumn(double column);

    /// \returns the current column.
    double getColumn() const;

    /// \returns the normalized column at the current zoom level.
    double getNormalizedColumn() const;

    /// \returns the normalized and floored column at the current zoom level.
    int64_t getFlooredColumn() const;

    /// \brief Set the tile row.
    /// \param column The row value to set.
    void setRow(double row);

    /// \returns the current row.
    double getRow() const;

    /// \returns the normalized row at the current zoom level.
    double getNormalizedRow() const;

    /// \returns the normalized and floored row at the current zoom level.
    int64_t getFlooredRow() const;

    /// \brief Set the zoom level.
    /// \param zoom The zoom value to set.
    void setZoom(double zoom);

    /// \returns the current zoom level.
    double getZoom() const;

    /// \returns the scale for the current zoom.
    double scaleForZoom() const;

    /// \brief Get the normalized and floored zoom level.
    int64_t getFlooredZoom() const;

    /// \brief Zoom thiscoordinate to the given zoom.
    /// \param zoom The zoom level to zoom to.
    void zoomTo(double zoom);

    /// \brief Get this coordinate at the given zoom level.
    /// \param zoom The zoom level to zoom to.
    /// \returns a copy of this coordinate at the given zoom level.
    TileCoordinate getZoomedTo(double zoom) const;

    /// \brief Zoom this coordinate by a given zoom amount.
    /// \param zoom The zoom level to zoom by.
    void zoomBy(double zoom);

    /// \brief Get this coordinate zoomed by the given zoom level.
    /// \param zoom The zoom level to zoom by.
    /// \returns a copy of this coordinate zoomed by the given zoom level.
    TileCoordinate getZoomedBy(double zoom) const;

    TileCoordinate moveRightBy(double distance);
    TileCoordinate moveLeftBy(double distance);
    TileCoordinate moveUpBy(double distance);
    TileCoordinate moveDownBy(double distance);

    TileCoordinate getNeighbor(double columnDistance, double rowDistance) const;

    TileCoordinate getNeighborRight() const;
    TileCoordinate getNeighborLeft() const;
    TileCoordinate getNeighborUp() const;
    TileCoordinate getNeighborDown() const;

    /// \brief Get the normalized version of this tile's coordinates.
    /// \throws InvalidArgumentException if the zoom is < 0.
    /// \returns the normalized tile coordinate.
    TileCoordinate getNormalizedTileCoordinate() const;

//    TileCoordinate getFlooredRowAndColumn() const;

    TileCoordinate getClampedRowAndColumn() const;


    /// \brief Get the scaling value for the given zoom.
    ///
    /// This is equivalent to double scale = std::pow(2.0, zoom);
    ///
    /// \param zoom The integer zoom level to get the scale for.
    /// \returns the scale for the given zoom.
    static double getScaleForZoom(int zoom);

    /// \brief This sorts tiles by zoom and row / column and zoom.
    /// \param coordinate The coordinate to compare.
    /// \returns true if the this coordinate is less than the other.
    bool operator < (const TileCoordinate& coordinate) const;

    /// \brief This compares two tiles for equality.
    /// \param coordinate The coordinate to compare.
    /// \returns true if the this coordinate is equal to the other.
    bool operator == (const TileCoordinate& coordinate) const;

    /// \brief This sets one tile equal to another.
    /// \param coordinate The to set equal to.
    /// \returns a reference to this update coordinate.
    TileCoordinate& operator = (const TileCoordinate& coordinate);

    /// \brief Get a debug string.
    /// \param precision The number decimal places to use.
    /// \returns a debug string with the given precision.
    std::string toString(int precision = 8) const;

    /// \returns a non-cryptographic hash.
    std::size_t hash() const;

    /// \brief Stream output.
    /// \param os the std::ostream.
    /// \param coordinate The TileCoordinate to output.
    /// \returns the updated std::ostream reference.
    friend std::ostream& operator << (std::ostream& os,
                                      const TileCoordinate& coordinate);

private:
    /// \brief The column value.
    ///
    /// This usually corresponds to the "x" position.
    double _column = 0.0;

    /// \brief The row value.
    ///
    /// This usually corresponds to the "y" position.
    double _row = 0.0;

    /// \brief The tile value.
    ///
    /// This usually corresponds to the "z" position.
    double _zoom = 0.0;

};


inline std::ostream& operator<<(std::ostream& os, const TileCoordinate& coordinate)
{
    os << coordinate.toString();
    return os;
}


} } // namespace ofx::Maps


namespace std {


template <> struct hash<ofx::Maps::TileCoordinate>
{
    size_t operator()(const ofx::Maps::TileCoordinate& coordinate) const
    {
        return coordinate.hash();
    }
};


} // namespace std
