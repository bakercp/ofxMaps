//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/Maps/MapTileProvider.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"
#include "ofx/IO/Hash.h"
#include "ofLog.h"
#include "ofMath.h"
#include "ofx/Maps/TileKey.h"
#include "ofx/Maps/TileTemplate.h"
#include "ofMath.h"


namespace ofx {
namespace Maps {


const Geo::CoordinateBounds MapTileProvider::DEFAULT_BOUNDS = { Geo::Coordinate(-180, -90), Geo::Coordinate(180, 90) };
const TileCoordinate MapTileProvider::DEFAULT_CENTER = { 0, 0, 0 };
const SperhicalMercatorProjection MapTileProvider::DEFAULT_PROJECTION = SperhicalMercatorProjection();


MapTileProvider::MapTileProvider():
    MapTileProvider(std::vector<std::string>(),
                 DEFAULT_MIN_ZOOM,
                 DEFAULT_MAX_ZOOM,
                 DEFAULT_TILE_WIDTH,
                 DEFAULT_TILE_HEIGHT,
                 DEFAULT_BOUNDS,
                 DEFAULT_CENTER,
                 DEFAULT_PROJECTION)
{
}



MapTileProvider::MapTileProvider(const std::vector<std::string>& URITemplates,
                                 int minZoom,
                                 int maxZoom,
                                 int tileWidth,
                                 int tileHeight,
                                 const Geo::CoordinateBounds& bounds,
                                 const TileCoordinate& center,
                                 const BaseProjection& projection):
    _minZoom(minZoom),
    _maxZoom(maxZoom),
    _tileSize(tileWidth, tileHeight),
    _bounds(bounds),
    _center(center),
    _projection(projection)
{
    _setURITemplates(URITemplates);
}


MapTileProvider::~MapTileProvider()
{
}


std::string MapTileProvider::id() const
{
    return _id;
}


std::string MapTileProvider::name() const
{
    return _name;
}


std::string MapTileProvider::description() const
{
    return _description;
}


std::string MapTileProvider::attribution() const
{
    return _attribution;
}


std::string MapTileProvider::version() const
{
    return _version;
}


int MapTileProvider::minZoom() const
{
    return _minZoom;
}


int MapTileProvider::maxZoom() const
{
    return _maxZoom;
}


float MapTileProvider::tileWidth() const
{
    return _tileSize.x;
}


float MapTileProvider::tileHeight() const
{
    return _tileSize.y;
}


glm::vec2 MapTileProvider::tileSize() const
{
    return _tileSize;
}


Geo::CoordinateBounds MapTileProvider::bounds() const
{
    return _bounds;
}


TileCoordinate MapTileProvider::center() const
{
    return _center;
}


double MapTileProvider::zoomForScale(double scale) const
{
    return std::log(scale) / glm::ln_two<double>();
}


TileCoordinate MapTileProvider::geoToWorld(const Geo::Coordinate& location) const
{
    return _projection.geoToWorld(location);
}


Geo::Coordinate MapTileProvider::tileToGeo(const TileCoordinate& coordinate) const
{
    return _projection.tileToGeo(coordinate);
}


std::string MapTileProvider::getTileURI(const TileKey& key) const
{
    std::size_t index = static_cast<std::size_t>(ofRandom(_URITemplates.size()));

    auto tileURI = _URITemplates[index];
    auto templateParameters = _URITemplateParameters[index];

    for (const auto& templateParameter: templateParameters)
    {
        std::string templateValue;

        if (getTileURITemplateValue(key, templateParameter, templateValue))
        {
            Poco::replaceInPlace(tileURI, templateParameter, templateValue);
        }
        else
        {
            ofLogWarning("MapTileProvider::getTileURI") << "Ignoring unknown template parameter: " << templateParameter;
        }
    }

    return tileURI;
}


bool MapTileProvider::isCacheable() const
{
    return true;
}


const std::vector<std::string> MapTileProvider::URITemplates() const
{
    return _URITemplates;
}


std::map<std::string, std::string> MapTileProvider::dictionary() const
{
    return _dictionary;
}


bool MapTileProvider::getTileURITemplateValue(const TileKey& key,
                                           const std::string& templateParameter,
                                           std::string& templateValue) const
{
    if (templateParameter == TileTemplate::TEMPLATE_PARAM_X)
    {
        templateValue = Poco::NumberFormatter::format(static_cast<Poco::Int64>(key.column()));
        return true;
    }
    else if (templateParameter == TileTemplate::TEMPLATE_PARAM_Y)
    {
        templateValue = Poco::NumberFormatter::format(static_cast<Poco::Int64>(key.row()));
        return true;
    }
    if (templateParameter == TileTemplate::TEMPLATE_PARAM_ZOOM)
    {
        templateValue = Poco::NumberFormatter::format(static_cast<Poco::Int64>(key.zoom()));
        return true;
    }
    else if (templateParameter == TileTemplate::TEMPLATE_PARAM_TILE_ID)
    {
        templateValue = key.tileId();
        return true;
    }
    else if (templateParameter == TileTemplate::TEMPLATE_PARAM_SET_ID)
    {
        templateValue = key.setId();
        return true;
    }
    else
    {
        if (templateParameter.length() > 1)
        {
            auto search = templateParameter.substr(1, templateParameter.length() - 2);

            auto iter = _dictionary.find(search);

            if (iter != _dictionary.end())
            {
                templateValue = iter->second;
                return true;
            }
        }

        return false;
    }
}


void MapTileProvider::_setURITemplates(const std::vector<std::string>& templates)
{
    // Update parameters and generate ID.
    _URITemplates = templates;
    _URITemplateParameters.clear();

    std::size_t hash = 0;

    for (auto _URITemplate: _URITemplates)
    {
        IO::Hash::combine(hash, _URITemplate);
        _URITemplateParameters.push_back(TileTemplate::extractTemplateParameters(_URITemplate));
    }

    _id = std::to_string(hash);
}


MapTileProvider MapTileProvider::fromJSON(const ofJson& json)
{
    MapTileProvider provider;

    auto iter = json.cbegin();
    while (iter != json.cend())
    {
        const auto& key = iter.key();
        const auto& value = iter.value();

        if (key == "tilejson") { }
        else if (key == "name") provider._name = value;
        else if (key == "description") provider._description = value;
        else if (key == "version") provider._version = value;
        else if (key == "attribution") provider._attribution = value;
        else if (key == "template") { ofLogWarning("MapTileProvider::fromJSON") << "Unsupported TileJSON field: " << key; }
        else if (key == "legend") { ofLogWarning("MapTileProvider::fromJSON") << "Unsupported TileJSON field: " << key;  }
        else if (key == "scheme") { ofLogWarning("MapTileProvider::fromJSON") << "Unsupported TileJSON field: " << key;  }
        else if (key == "tiles")
        {
            std::vector<std::string> uriTemplates;
            for (const auto& uriTemplate: value)
            {
                uriTemplates.push_back(uriTemplate);
            }
            provider._setURITemplates(uriTemplates);
        }
        else if (key == "grids") { ofLogWarning("MapTileProvider::fromJSON") << "Unsupported TileJSON field: " << key; }
        else if (key == "data") { ofLogWarning("MapTileProvider::fromJSON") << "Unsupported TileJSON field: " << key; }
        else if (key == "minzoom") provider._minZoom = value;
        else if (key == "maxzoom") provider._maxZoom = value;
        else if (key == "tilewidth") provider._tileSize.x = value.get<int>();
        else if (key == "tileheight") provider._tileSize.y = value.get<int>();
        else if (key == "bounds")
        {
            provider._bounds = Geo::CoordinateBounds(Geo::Coordinate(value[0], value[1]),
                                                     Geo::Coordinate(value[2], value[3]));
        }
        else if (key == "center")
        {
            auto center = provider.geoToWorld(Geo::Coordinate(value[0], value[1]));
            center.setZoom(value[2]);
            provider._center = center;
        }
        else
        {
            provider._dictionary[key] = value;
        }

        ++iter;
    }

    return provider;
}


ofJson MapTileProvider::toJSON(const MapTileProvider& provider)
{
    ofJson json;

    json["tilejson"] = "2.1.0";

    if (!provider.name().empty()) json["name"] = provider.name();
    if (!provider.description().empty()) json["description"] = provider.description();
    if (!provider.version().empty()) json["version"] = provider.version();
    if (!provider.attribution().empty()) json["attribution"] = provider.attribution();
    // if (!provider.template().empty()) json["template"] = provider.template();
    // if (!provider.legend().empty()) json["legend"] = provider.legend();
    // if (!provider.scheme().empty()) json["scheme"] = provider.scheme();
    if (!provider.attribution().empty()) json["tiles"] = provider.URITemplates();
    // if (!provider.grids().empty()) json["grids"] = provider.grids();
    // if (!provider.data().empty()) json["data"] = provider.data();

    json["minzoom"] = provider.minZoom();
    json["maxzoom"] = provider.maxZoom();

    json["tilewidth"] = provider.tileWidth();
    json["tileheight"] = provider.tileHeight();

    if (!provider.maxZoom()) json["maxzoom"] = provider.maxZoom();

    json["bounds"] = {  provider.bounds().northwest().getLatitude(),
                        provider.bounds().northwest().getLongitude(),
                        provider.bounds().southeast().getLatitude(),
                        provider.bounds().southeast().getLongitude()
    };

    auto centerGeo = provider.tileToGeo(provider.center());


    json["center"] = {  centerGeo.getLatitude(),
                        centerGeo.getLongitude(),
                        provider.center().getZoom()
    };

    for (const auto& entry: provider._dictionary)
    {
        json[entry.first] = entry.second;
    }

    return json;
}



} } // namespace ofx::Maps
