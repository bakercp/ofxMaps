//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/Maps/TileKey.h"
#include <sstream>
#include "ofx/IO/Hash.h"


namespace ofx {
namespace Maps {


const std::string TileKey::DEFAULT_TILE_ID = "";
const std::string TileKey::DEFAULT_SET_ID = "";


TileKey::TileKey()
{
}


TileKey::TileKey(const TileKey& key):
    TileKey(key.column(),
            key.row(),
            key.zoom(),
//            key.providerId(),
            key.setId(),
            key.tileId())
{
}


TileKey::TileKey(int64_t column,
                 int64_t row,
                 int64_t zoom,
//                 const std::string& providerId,
                 const std::string& setId,
                 const std::string& tileId):
    _column(column),
    _row(row),
    _zoom(zoom),
//    _providerId(providerId),
    _setId(setId),
    _tileId(tileId)
{
}


TileKey::~TileKey()
{
}


int64_t TileKey::column() const
{
    return _column;
}


int64_t TileKey::row() const
{
    return _row;
}


int64_t TileKey::zoom() const
{
    return _zoom;
}


//std::string TileKey::providerId() const
//{
//    return _providerId;
//}


std::string TileKey::tileId() const
{
    return _tileId;
}


std::string TileKey::setId() const
{
    return _setId;
}


std::string TileKey::toString() const
{
    std::stringstream ss;
    ss << _column << ",";
    ss << _row << ",";
    ss << _zoom << ",";
    ss << _tileId << ",";
    ss << _setId;
    return ss.str();
}


std::size_t TileKey::hash() const
{
    std::size_t seed = 0;
    IO::Hash::combine(seed, _column);
    IO::Hash::combine(seed, _row);
    IO::Hash::combine(seed, _zoom);
//    IO::Hash::combine(hash, _providerId);
    IO::Hash::combine(seed, _tileId);
    IO::Hash::combine(seed, _setId);
    return seed;
}




bool TileKey::operator < (const TileKey& key) const
{
    /*
    if (_providerId != key.providerId())
    {
        return _providerId < key.providerId();
    }
    else */if (_setId != key.setId())
    {
        return _setId < key.setId();
    }
    else if (_tileId != key.tileId())
    {
        return _tileId < key.tileId();
    }
    else if (_row != key.row())
    {
        return _row < key.row();
    }
    else if (_column != key.column())
    {
        return _column < key.column();
    }
    else
    {
        return _zoom < key.zoom();
    }
}


} } // namespace ofx::Maps
