// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
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


// More here:
// http://mc.bbbike.org/mc/js/mc.js

class OSMTileProvider: public BaseURLTileProvider
{
public:
    OSMTileProvider(const std::string& URITemplate,
                    const std::string& attribution);

    OSMTileProvider(const std::string& URITemplate,
                    const std::string& attribution,
                    const std::vector<std::string>& subdomains,
                    int minZoom,
                    int maxZoom,
                    int tileWidth,
                    int tileHeight,
                    const BaseProjection& projection);

    virtual ~OSMTileProvider();

    static const std::string DEFAULT_OSM_URI_TEMPLATE;
    static const std::string DEFAULT_OSM_ATTRIBUTION;

};


class BaseStamenTileProvider: public OSMTileProvider
{
public:
    BaseStamenTileProvider(const std::string& URITemplate,
                           const std::string& attribution = "Map tiles by <a href=\"http://stamen.com\">Stamen Design</a>, under <a href=\"http://creativecommons.org/licenses/by/3.0\">CC BY 3.0</a>. Data by <a href=\"http://openstreetmap.org\">OpenStreetMap</a>, under <a href=\"http://www.openstreetmap.org/copyright\">ODbL</a>."):
        OSMTileProvider(URITemplate, attribution)
    {
    }


    virtual ~BaseStamenTileProvider()
    {
    }

};


class StamenTerrainStyleProvider: public BaseStamenTileProvider
{
public:
    StamenTerrainStyleProvider(): BaseStamenTileProvider("http://{s}.tile.stamen.com/terrain/{z}/{x}/{y}.jpg")
    {
    }


    virtual ~StamenTerrainStyleProvider()
    {
    }

};


class StamenWatercolorTileProvider: public BaseStamenTileProvider
{
public:
    StamenWatercolorTileProvider(): BaseStamenTileProvider("http://{s}.tile.stamen.com/watercolor/{z}/{x}/{y}.jpg")
    {
    }


    virtual ~StamenWatercolorTileProvider()
    {
    }

};


class StamenTonerStyleProvider: public BaseStamenTileProvider
{
public:
    StamenTonerStyleProvider():
    BaseStamenTileProvider("http://{s}.tile.stamen.com/toner/{z}/{x}/{y}.png",
                           "Map tiles by <a href=\"http://stamen.com\">Stamen Design</a>, under <a href=\"http://creativecommons.org/licenses/by/3.0\">CC BY 3.0</a>. Data by <a href=\"http://openstreetmap.org\">OpenStreetMap</a>, under <a href=\"http://www.openstreetmap.org/copyright\">ODbL</a>.")
    {
    }
    
    
    virtual ~StamenTonerStyleProvider()
    {
    }
    
};


class BaseMapQuestTileProvider: public OSMTileProvider
{
public:
    BaseMapQuestTileProvider(const std::string& URITemplate,
                             const std::string& attribution = "Tiles Courtesy of <a href=\"http://www.mapquest.com/\" target=\"_blank\">MapQuest</a> <img src=\"http://developer.mapquest.com/content/osm/mq_logo.png\">"):
        OSMTileProvider(URITemplate, attribution)
    {
    }
    
    
    virtual ~BaseMapQuestTileProvider()
    {
    }
    
};


class MapQuestOSMTileProvider: public BaseMapQuestTileProvider
{
public:
    MapQuestOSMTileProvider():
        BaseMapQuestTileProvider("http://otile1.mqcdn.com/tiles/1.0.0/map/{z}/{x}/{y}.jpg")
    {
    }


    virtual ~MapQuestOSMTileProvider()
    {
    }
    
};


class MapQuestSatTileProvider: public BaseMapQuestTileProvider
{
public:
    MapQuestSatTileProvider():
        BaseMapQuestTileProvider("http://otile1.mqcdn.com/tiles/1.0.0/sat/{z}/{x}/{y}.jpg")
    {
    }


    virtual ~MapQuestSatTileProvider()
    {
    }
    
};



class BaseEsriTileProvider: public OSMTileProvider
{
public:
    BaseEsriTileProvider(const std::string& URITemplate,
                         const std::string& attribution = "Tiles Courtesy of ESRI"):
        OSMTileProvider(URITemplate, attribution)
    {
    }


    virtual ~BaseEsriTileProvider()
    {
    }
    
};


class EsriStreetMapTileProvider: public BaseEsriTileProvider
{
public:
    EsriStreetMapTileProvider():
        BaseEsriTileProvider("http://server.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/MapServer/tile/{z}/{y}/{x}.jpg")
    {
    }

    virtual ~EsriStreetMapTileProvider()
    {
    }
    
};


class EsriBoundariesAndPlacesTileProvider: public BaseEsriTileProvider
{
public:
    EsriBoundariesAndPlacesTileProvider():
        BaseEsriTileProvider("http://server.arcgisonline.com/ArcGIS/rest/services/Reference/World_Boundaries_and_Places/MapServer/tile/{z}/{y}/{x}.jpg")
    {
    }

    virtual ~EsriBoundariesAndPlacesTileProvider()
    {
    }
    
};


class EsriWorldLightGrayTileProvider: public BaseEsriTileProvider
{
public:
    EsriWorldLightGrayTileProvider():
    BaseEsriTileProvider("http://services.arcgisonline.com/ArcGIS/rest/services/Canvas/World_Light_Gray_Base/MapServer/tile/{z}/{y}/{x}.jpg")
    {
    }

    virtual ~EsriWorldLightGrayTileProvider()
    {
    }
    
};


class EsriSatelliteTileProvider: public BaseEsriTileProvider
{
public:
    EsriSatelliteTileProvider():
        BaseEsriTileProvider("http://services.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}.png")
    {
    }

    virtual ~EsriSatelliteTileProvider()
    {
    }
    
};


class BaseOviTileProvider: public OSMTileProvider
{
public:
    BaseOviTileProvider(const std::string& URITemplate,
                        const std::string& attribution = "Tiles Courtesy of OVI"):
        OSMTileProvider(URITemplate, attribution)
    {
    }


    virtual ~BaseOviTileProvider()
    {
    }
    
};


class OviNormalTileProvider: public BaseOviTileProvider
{
public:
    OviNormalTileProvider():
        BaseOviTileProvider("http://maptile.maps.svc.ovi.com/maptiler/maptile/newest/normal.day/{z}/{x}/{y}/256/png8")
    {
    }

    virtual ~OviNormalTileProvider()
    {
    }
    
};


class OviGraylTileProvider: public BaseOviTileProvider
{
public:
    OviGraylTileProvider():
        BaseOviTileProvider("http://maptile.maps.svc.ovi.com/maptiler/maptile/newest/normal.day.grey/{z}/{x}/{y}/256/png8")
    {
    }

    virtual ~OviGraylTileProvider()
    {
    }
    
};


class OviTransitTileProvider: public BaseOviTileProvider
{
public:
    OviTransitTileProvider():
        BaseOviTileProvider("http://maptile.maps.svc.ovi.com/maptiler/maptile/newest/normal.day.transit/{z}/{x}/{y}/256/png8")
    {
    }

    virtual ~OviTransitTileProvider()
    {
    }
    
};


class OviSatelliteTileProvider: public BaseOviTileProvider
{
public:
    OviSatelliteTileProvider():
        BaseOviTileProvider("http://maptile.maps.svc.ovi.com/maptiler/maptile/newest/satellite.day/{z}/{x}/{y}/256/png8")
    {
    }

    virtual ~OviSatelliteTileProvider()
    {
    }
    
};


class OviTerrainTileProvider: public BaseOviTileProvider
{
public:
    OviTerrainTileProvider():
        BaseOviTileProvider("http://maptile.maps.svc.ovi.com/maptiler/maptile/newest/terrain.day/{z}/{x}/{y}/256/png8")
    {
    }

    virtual ~OviTerrainTileProvider()
    {
    }
    
};


} } // namespace ofx::Maps
