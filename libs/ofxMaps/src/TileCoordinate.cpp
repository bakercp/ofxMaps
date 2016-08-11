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


#include "ofx/Maps/TileCoordinate.h"
#include "Poco/DigestStream.h"
#include "Poco/Exception.h"
#include "Poco/MD5Engine.h"
#include "Poco/SHA1Engine.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/Crypto/RSADigestEngine.h"
#include "ofMath.h"


namespace ofx {
namespace Maps {


const std::string TileData::DEFAULT_TILE_ID = "";
const std::string TileData::DEFAULT_SET_ID = "";


TileData::TileData()
{
}


TileData::TileData(const std::string& providerId,
                   const std::string& setId,
                   const std::string& tileId):
    _providerId(providerId),
    _setId(setId),
    _tileId(tileId)
{
}


TileData::~TileData()
{
}


void TileData::setProviderId(const std::string& providerId)
{
    _providerId = providerId;
}


std::string TileData::getProviderId() const
{
    return _providerId;
}


void TileData::setTileId(const std::string& tileId)
{
    _tileId = tileId;
}


std::string TileData::getTileId() const
{
    return _tileId;
}


void TileData::setSetId(const std::string& setId)
{
    _setId = setId;
}


std::string TileData::getSetId() const
{
    return _setId;
}


bool TileData::operator < (const TileData& data) const
{
    if (_providerId != data.getProviderId())
    {
        return _providerId < data.getProviderId();
    }
    else if (_setId != data.getSetId())
    {
        return _setId < data.getSetId();
    }
    else if (_tileId != data.getTileId())
    {
        return _tileId < data.getTileId();
    }
    else
    {
        return false;
    }
}


TileCoordinate::TileCoordinate(): TileCoordinate(0, 0, 0)
{
}


TileCoordinate::TileCoordinate(const glm::dvec2& columnRow, double zoom):
    TileCoordinate(columnRow.x, columnRow.y, zoom)
{
}


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


void TileCoordinate::setRow(double row)
{
    _row = row;
}
    

double TileCoordinate::getRow() const
{
    return _row;
}


void TileCoordinate::setZoom(double zoom)
{
    _zoom = zoom;
}
    

double TileCoordinate::getZoom() const
{
    return _zoom;
}

void TileCoordinate::zoomTo(double zoom)
{
    auto scale = std::pow(2.0, zoom - _zoom);
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
    auto scale = std::pow(2.0, zoom);
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
}


TileCoordinate TileCoordinate::moveLeftBy(double distance)
{
    _column -= distance;
}


TileCoordinate TileCoordinate::moveUpBy(double distance)
{
    _row -= distance;
}

TileCoordinate TileCoordinate::moveDownBy(double distance)
{
    _row += distance;
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
    ss << "column: " << std::fixed << std::setprecision(precision) << _column << std::endl;
    ss << "   row: " << std::fixed << std::setprecision(precision) << _row << std::endl;
    ss << "  zoom: " << std::fixed << std::setprecision(precision) << _zoom;
    return ss.str();
}


TileCoordinateKey::TileCoordinateKey()
{
}


TileCoordinateKey::TileCoordinateKey(const TileData& data,
                                     const TileCoordinate& coordinate):
    _data(data),
    _coordinate(coordinate)
{
}


TileCoordinateKey::~TileCoordinateKey()
{
}


TileData TileCoordinateKey::data() const
{
    return _data;
}


TileCoordinate TileCoordinateKey::coordinate() const
{
    return _coordinate;
}


std::string TileCoordinateKey::tileId() const
{
    return _data.getTileId();
}


std::string TileCoordinateKey::setId() const
{
    return _data.getSetId();
}


std::string TileCoordinateKey::providerId() const
{
    return _data.getProviderId();
}


int TileCoordinateKey::row() const
{
    return static_cast<int>(std::floor(_coordinate.getRow()));
}


int TileCoordinateKey::column() const
{
    return static_cast<int>(std::floor(_coordinate.getColumn()));
}


int TileCoordinateKey::zoom() const
{
    return static_cast<int>(std::floor(_coordinate.getZoom()));
}


bool TileCoordinateKey::operator < (const TileCoordinateKey& coordinate) const
{
    if (_coordinate == coordinate.coordinate())
    {
        return _data < coordinate.data();
    }
    else
    {
        return _coordinate < coordinate.coordinate();
    }
}


TileCoordinate TileCoordinateUtils::normalizeTileCoordinate(const TileCoordinate& coordinate)
{
    // TODO if this zoom is negative, these while loops could get stuck.

    if (coordinate.getZoom() < 0)
    {
        throw Poco::InvalidArgumentException("Zoom must be >= 0");
    }

    int gridSize = TileCoordinateUtils::getScaleForZoom(coordinate.getZoom());

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

    
TileCoordinate TileCoordinateUtils::floorRowAndColumn(const TileCoordinate& coordinate)
{
    return TileCoordinate(std::floor(coordinate.getColumn()),
                          std::floor(coordinate.getRow()),
                          coordinate.getZoom());
}


TileCoordinate TileCoordinateUtils::clampRowAndColumn(const TileCoordinate& coordinate)
{
    double gridSize = getScaleForZoom(coordinate.getZoom()) - 1;

    return TileCoordinate(glm::clamp(coordinate.getColumn(), 0.0, gridSize),
                          glm::clamp(coordinate.getRow(), 0.0, gridSize),
                          coordinate.getZoom());
}


TileCoordinate TileCoordinateUtils::floor(const TileCoordinate& coordinate)
{
    return TileCoordinate(std::floor(coordinate.getColumn()),
                          std::floor(coordinate.getRow()),
                          std::floor(coordinate.getZoom()));
}




double TileCoordinateUtils::getScaleForZoom(int zoom)
{
    return std::pow(2.0, zoom);
}


std::string TileCoordinateUtils::hash(const TileCoordinateKey& key)
{
    // TODO: use hash combine
    // https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
    // http://en.cppreference.com/w/cpp/utility/hash

    // Repeatable tile id based on its set id and column, row and zoom.
    Poco::MD5Engine md5;
    Poco::DigestOutputStream ostr(md5);
    ostr << key.setId();
    ostr << key.column();
    ostr << key.row();
    ostr << key.zoom();
    ostr.flush(); // Ensure everything gets passed to the digest engine
    const auto& digest = md5.digest(); // obtain result
    return Poco::DigestEngine::digestToHex(digest);
}


std::string TileCoordinateUtils::sha256(const ofBuffer& image)
{
    Poco::Crypto::RSAKey key(Poco::Crypto::RSAKey::KL_2048,
                             Poco::Crypto::RSAKey::EXP_LARGE);
    Poco::Crypto::RSADigestEngine eng(key, "SHA256");
    eng.update(image.getData(), image.size());
    const auto& sig = eng.digest(); // We just want the digest, unsigned.
    return Poco::DigestEngine::digestToHex(sig);
}

std::string TileCoordinateUtils::sha1(const ofBuffer& image)
{
    Poco::SHA1Engine sha1;
    sha1.update(image.getData(), image.size());
    const auto& digest = sha1.digest(); // obtain result
    return Poco::DigestEngine::digestToHex(digest);
}


std::string TileCoordinateUtils::md5(const ofBuffer& image)
{
    Poco::MD5Engine md5;
    md5.update(image.getData(), image.size());
    const auto& digest = md5.digest(); // obtain result
    return Poco::DigestEngine::digestToHex(digest);
}


} } // namespace ofx::Maps
