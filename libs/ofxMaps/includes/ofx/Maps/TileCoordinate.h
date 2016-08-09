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


#pragma once


#include <math.h>
#include <string>
#include "ofConstants.h"


namespace ofx {
namespace Maps {


class TileData
{
public:
    TileData();
    TileData(const std::string& providerId,
             const std::string& setId,
             const std::string& tileId);

    virtual ~TileData();

    void setProviderId(const std::string& providerId);
    std::string getProviderId() const;

    void setSetId(const std::string& setId);
    std::string getSetId() const;

    void setTileId(const std::string& tileId);
    std::string getTileId() const;

    bool operator < (const TileData& data) const;

    static const std::string DEFAULT_SET_ID;
    static const std::string DEFAULT_TILE_ID;

private:

    /// \brief This is the id of the provider that generated this tile coordinate.
    ///
    /// Default value is an empty string.
    std::string _providerId;

    /// \brief This is the set id.
    ///
    /// Usually this is empty which indicates that it is part of a the default
    /// set for the given providerId.
    ///
    /// This is often set for a given set of tiled images used in street-view
    /// images, etc.
    ///
    /// Default value is an empty string.
    std::string _setId;

    /// \brief The unique tile id.
    ///
    /// This is usually used as the unique identifier for a given tile. For
    /// instance, this might be the unique ETag delivered in a cacheable web
    /// request.  This might also be used to connect loaded pixels back to
    /// the raw buffered bytes.
    ///
    /// Default value is an empty string.
    std::string _tileId;

};


/// \brief A tile coordinate in a tiled image system.
class TileCoordinate
{
public:
	TileCoordinate();

    TileCoordinate(const glm::dvec2& columnRow, double zoom);

    TileCoordinate(double column,
                   double row,
                   double zoom);

    void setColumn(double column);
    double getColumn() const;

    void setRow(double row);
    double getRow() const;

    void setZoom(double zoom);
    double getZoom() const;

    void zoomTo(double zoom);
    TileCoordinate getZoomedTo(double zoom) const;

    void zoomBy(double zoom);
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

    /// \brief This sorts tiles by zoom and row / column and zoom.
	bool operator < (const TileCoordinate& coordinate) const;

    /// \brief This sorts tiles by zoom and row / column and zoom.
    bool operator == (const TileCoordinate& coordiante) const;

    TileCoordinate& operator = (const TileCoordinate& coordinate);

    std::string toString(int precision = 8) const;

private:
    /// \brief The column position.
    ///
    /// This usually corresponds to the "x" position.
    double _column = 0.0;

    /// \brief The row position.
    ///
    /// This usually corresponds to the "y" position.
    double _row = 0.0;

    /// \brief The tile zoom.
    ///
    /// This usually corresponds to the "z" position.
	double _zoom = 0.0;

};


class TileCoordinateKey
{
public:
    TileCoordinateKey();

    TileCoordinateKey(const TileData& data,
                      const TileCoordinate& coordinate);

    virtual ~TileCoordinateKey();


    TileData data() const;
    TileCoordinate coordinate() const;

    std::string tileId() const;
    std::string setId() const;
    std::string providerId() const;

    /// \returns the floored integer representation of the row.
    int row() const;

    /// \returns the floored integer representation of the column.
    int column() const;

    /// \returns the floored integer representation of the zoom.
    int zoom() const;

    /// \brief This sorts tiles by the coordinate.
    bool operator < (const TileCoordinateKey& coordiante) const;

private:
    TileData _data;
    TileCoordinate _coordinate;

};


/// \brief A set of utilities for doing tile math.
class TileCoordinateUtils
{
public:
    static TileCoordinate normalizeTileCoordinate(const TileCoordinate& coordinate);

    static TileCoordinate floorRowAndColumn(const TileCoordinate& coordinate);

    static TileCoordinate clampRowAndColumn(const TileCoordinate& coordinate);

    static TileCoordinate floor(const TileCoordinate& coordinate);

    static double getScaleForZoom(int zoom);


    static std::string hash(const TileCoordinateKey& key);
};



} } // namespace ofx::Maps
