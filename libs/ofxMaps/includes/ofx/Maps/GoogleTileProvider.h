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


class BaseGoogleTileProvider: public BaseURLTileProvider
{
public:
    BaseGoogleTileProvider(const std::string& URITemplate):
        BaseURLTileProvider(URITemplate,
                            DEFAULT_GOOGLE_ATTRIBUTION,
                            DEFAULT_NUMERICAL_SUBDOMAINS,
                            DEFAULT_MIN_ZOOM,
                            DEFAULT_MAX_ZOOM,
                            DEFAULT_TILE_WIDTH,
                            DEFAULT_TILE_HEIGHT,
                            DEFAULT_PROJECTION)
    {
    }


    virtual ~BaseGoogleTileProvider()
    {
    }


    static const std::string DEFAULT_GOOGLE_ATTRIBUTION;


};


class GoogleNormalTileProvider: public BaseGoogleTileProvider
{
public:
    GoogleNormalTileProvider():
        BaseGoogleTileProvider("http://mt{s}.google.com/vt/lyrs=m&x={x}&y={y}&z={z}")
    {
    }

    virtual ~GoogleNormalTileProvider()
    {
    }

};


class GoogleTerrainTileProvider: public BaseGoogleTileProvider
{
public:
    GoogleTerrainTileProvider():
        BaseGoogleTileProvider("http://mt{s}.google.com/vt/lyrs=t&x={x}&y={y}&z={z}")
    {
    }

    virtual ~GoogleTerrainTileProvider()
    {
    }
    
};


class GoogleTerrainPlusTileProvider: public BaseGoogleTileProvider
{
public:
    GoogleTerrainPlusTileProvider():
        BaseGoogleTileProvider("http://mt{s}.google.com/vt/lyrs=p&x={x}&y={y}&z={z}")
    {
    }

    virtual ~GoogleTerrainPlusTileProvider()
    {
    }
    
};


class GoogleAerialTileProvider: public BaseGoogleTileProvider
{
public:
    GoogleAerialTileProvider():
        BaseGoogleTileProvider("http://mt{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}")
    {
    }

    virtual ~GoogleAerialTileProvider()
    {
    }
    
};


class GoogleAerialPlusTileProvider: public BaseGoogleTileProvider
{
public:
    GoogleAerialPlusTileProvider():
        BaseGoogleTileProvider("http://mt{s}.google.com/vt/lyrs=y&x={x}&y={y}&z={z}")
    {
    }

    virtual ~GoogleAerialPlusTileProvider()
    {
    }
    
};


} } // namespace ofx::Maps
