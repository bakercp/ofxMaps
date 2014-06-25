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


//#include "Poco/URI.h"
//#include "ofVec2d.h"
//#include "ofx/Geo/Coordinate.h"
//#include "ofx/Maps/AbstractMapTypes.h"
//#include "ofx/Maps/BaseProjection.h"
#include "ofx/Maps/SphericalMercatorProjection.h"
//#include "ofx/Maps/TileCoordinate.h"
#include "ofx/Maps/BaseMapTileProvider.h"


namespace ofx {
namespace Maps {


/// \brief A BaseMapTileProvider with default settings.
class DefaultMapTileProvider: public BaseMapTileProvider
{
public:
    /// \brief Create a DefaultMapTileProvider.
    DefaultMapTileProvider();

    /// \brief Create a DefaultMapTileProvider.
    /// \param tileWidth The width of the provider's tiles in pixels.
    /// \param tileHeight The height of the provider's tiles in pixels.
    /// \param minZoom The minimum zoom level supported by the provider.
    /// \param maxZoom The maximum zoom level supported by the provider.
    /// \param projection The projection used by the provider.
    DefaultMapTileProvider(int minZoom,
                           int maxZoom,
                           int tileWidth,
                           int tileHeight,
                           const BaseProjection& projection);

    /// \brief Destroy the BaseMapProvider.
    virtual ~DefaultMapTileProvider();

    enum
    {
        DEFAULT_MIN_ZOOM = 0,
        DEFAULT_MAX_ZOOM = 19,
        DEFAULT_TILE_WIDTH = 256,
        DEFAULT_TILE_HEIGHT = 256
    };

    static const SperhicalMercatorProjection DEFAULT_PROJECTION;
};


} } // namespace ofx::Maps
