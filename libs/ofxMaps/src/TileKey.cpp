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
