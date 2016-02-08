// =============================================================================
//
// Copyright (c) 2014-2016 Christopher Baker <http://christopherbaker.net>
// Portions Copyright (c) 2006-2009 Microsoft Corporation.
// Source: http://msdn.microsoft.com/en-us/library/bb259689.aspx
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


#include "ofx/Maps/BaseURLTileProvider.h"


namespace ofx {
namespace Maps {


class MicrosoftTileProvider: public BaseURLTileProvider
{
public:
    MicrosoftTileProvider(const std::string& URITemplate,
                          const std::string& attribution,
                          const std::vector<std::string>& subdomains,
                          int minZoom,
                          int maxZoom,
                          int tileWidth,
                          int tileHeight,
                          const BaseProjection& projection);

    virtual ~MicrosoftTileProvider();


    /// \brief Converts tile coordinates into a QuadKey at a specified level of detail.
    /// <param name="tileX">Tile X coordinate.</param>
    /// <param name="tileY">Tile Y coordinate.</param>
    /// <param name="levelOfDetail">Level of detail, from 1 (lowest detail)
    /// to 23 (highest detail).</param>
    /// <returns>A string containing the QuadKey.</returns>
    static std::string tileCoordinateToQuadKey(int tileX,
                                               int tileY,
                                               int levelOfDetail);



    /// <summary>
    /// Converts a QuadKey into tile XY coordinates.
    /// </summary>
    /// <param name="quadKey">QuadKey of the tile.</param>
    /// <param name="tileX">Output parameter receiving the tile X coordinate.</param>
    /// <param name="tileY">Output parameter receiving the tile Y coordinate.</param>
    /// <param name="levelOfDetail">Output parameter receiving the level of detail.</param>
    static bool quadKeyToTileCoordinate(const std::string& quadKey,
                                        int& tileX,
                                        int& tileY,
                                        int& levelOfDetail);

    virtual bool getTileURITemplateValue(const TileCoordinate& coordinate,
                                         const std::string& templateParameter,
                                         std::string& templateValue) const;

    enum
    {
        DEFAULT_MICROSOFT_MIN_ZOOM = 1,
        DEFAULT_MICROSOFT_MAX_ZOOM = DEFAULT_MAX_ZOOM,
        DEFAULT_MICROSOFT_TILE_WIDTH = DEFAULT_TILE_WIDTH,
        DEFAULT_MICROSOFT_TILE_HEIGHT = DEFAULT_TILE_HEIGHT
    };

    static const std::string DEFAULT_MICROSOFT_ATTRIBUTION;

    static const std::string TEMPLATE_PARAM_QUADKEY;

};



class BaseMicrosoftTileProvider: public MicrosoftTileProvider
{
public:
    BaseMicrosoftTileProvider(const std::string& URITemplate):
        MicrosoftTileProvider(URITemplate,
                              DEFAULT_MICROSOFT_ATTRIBUTION,
                              DEFAULT_NUMERICAL_SUBDOMAINS,
                              DEFAULT_MICROSOFT_MIN_ZOOM,
                              DEFAULT_MICROSOFT_MAX_ZOOM,
                              DEFAULT_MICROSOFT_TILE_WIDTH,
                              DEFAULT_MICROSOFT_TILE_HEIGHT,
                              DEFAULT_PROJECTION)
    {
    }


    virtual ~BaseMicrosoftTileProvider()
    {
    }
    
};


class MicrosoftHybridStyleProvider: public BaseMicrosoftTileProvider
{
public:
    MicrosoftHybridStyleProvider():
        BaseMicrosoftTileProvider("http://h{s}.ortho.tiles.virtualearth.net/tiles/h{quadkey}.jpg?g=72")
    {
    }

    virtual ~MicrosoftHybridStyleProvider()
    {
    }
    
};


class MicrosoftAerialStyleProvider: public BaseMicrosoftTileProvider
{
public:
    MicrosoftAerialStyleProvider():
        BaseMicrosoftTileProvider("http://a{s}.ortho.tiles.virtualearth.net/tiles/a{quadkey}.jpg?g=72")
    {
    }

    virtual ~MicrosoftAerialStyleProvider()
    {
    }
    
};


class MicrosoftRoadStyleProvider: public BaseMicrosoftTileProvider
{
public:
    MicrosoftRoadStyleProvider():
        BaseMicrosoftTileProvider("http://r{s}.ortho.tiles.virtualearth.net/tiles/r{quadkey}.jpg?g=72&shading=hill")
    {
    }

    virtual ~MicrosoftRoadStyleProvider()
    {
    }
    
};



} } // namespace ofx::Maps
