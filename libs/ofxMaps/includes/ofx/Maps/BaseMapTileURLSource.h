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


#pragma once


#include "Poco/URI.h"
#include "ofVec2d.h"
#include "ofx/Geo/Coordinate.h"
#include "ofx/Maps/AbstractMapTypes.h"
#include "ofx/Maps/BaseProjection.h"
#include "ofx/Maps/SphericalMercatorProjection.h"
#include "ofx/Maps/TileCoordinate.h"


namespace ofx {
namespace Maps {


/// \brief A simplified BaseMapTileURLSource.
template<typename MapType>
class BaseMapTileURLSource: public AbstractMapTileURISource<MapType>
{
public:
    /// \brief Destroy the BaseMapProvider.
    virtual ~BaseMapTileURLSource();

    /// \brief Get a map tile for a given TileCoordinate.
    /// \param coordinate The tile coordinate.
    /// \returns A tile of type TileType.
    Poco::UUID requestTile(const TileCoordinate& coordinate) const
    {
    }

};


} } // namespace ofx::Maps
