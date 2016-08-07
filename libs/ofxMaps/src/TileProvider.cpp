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


#include "ofx/Maps/TileProvider.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"
#include "Poco/DigestStream.h"
#include "Poco/MD5Engine.h"
#include "ofLog.h"
#include "ofx/Maps/TileTemplate.h"


namespace ofx {
namespace Maps {


const Geo::CoordinateBounds TileProvider::DEFAULT_BOUNDS = { Geo::Coordinate(-180, -90), Geo::Coordinate(180, 90) };
const TileCoordinate TileProvider::DEFAULT_CENTER = { 0, 0, 0 };
const SperhicalMercatorProjection TileProvider::DEFAULT_PROJECTION = SperhicalMercatorProjection();


TileProvider::TileProvider():
    TileProvider(std::vector<std::string>(),
                 DEFAULT_MIN_ZOOM,
                 DEFAULT_MAX_ZOOM,
                 DEFAULT_TILE_WIDTH,
                 DEFAULT_TILE_HEIGHT,
                 DEFAULT_BOUNDS,
                 DEFAULT_CENTER,
                 DEFAULT_PROJECTION)
{
}



TileProvider::TileProvider(const std::vector<std::string>& URITemplates,
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


TileProvider::~TileProvider()
{
}


std::string TileProvider::id() const
{
    return _id;
}


std::string TileProvider::name() const
{
    return _name;
}


std::string TileProvider::description() const
{
    return _description;
}


std::string TileProvider::attribution() const
{
    return _attribution;
}


std::string TileProvider::version() const
{
    return _version;
}


int TileProvider::minZoom() const
{
    return _minZoom;
}


int TileProvider::maxZoom() const
{
    return _maxZoom;
}


int TileProvider::tileWidth() const
{
    return _tileSize.x;
}


int TileProvider::tileHeight() const
{
    return _tileSize.y;
}


glm::ivec2 TileProvider::tileSize() const
{
    return _tileSize;
}


Geo::CoordinateBounds TileProvider::bounds() const
{
    return _bounds;
}


TileCoordinate TileProvider::center() const
{
    return _center;
}


double TileProvider::zoomForScale(double scale) const
{
    return std::log(scale) / glm::ln_two<double>();
}


TileCoordinate TileProvider::geoToTile(const Geo::Coordinate& location) const
{
    return _projection.geoToTile(location);
}


Geo::Coordinate TileProvider::tileToGeo(const TileCoordinate& coordinate) const
{
    return _projection.tileToGeo(coordinate);
}


//std::shared_ptr<Tile> TileProvider::getTile(const TileCoordinate& coordinate) const
//{
//    
//
//    return nullptr;
//}


std::string TileProvider::getTileURI(const TileCoordinateKey& key) const
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
            ofLogWarning("TileProvider::getTileURI") << "Ignoring unknown template parameter: " << templateParameter;
        }
    }

    return tileURI;
}


const std::vector<std::string> TileProvider::URITemplates() const
{
    return _URITemplates;
}


bool TileProvider::getTileURITemplateValue(const TileCoordinateKey& key,
                                           const std::string& templateParameter,
                                           std::string& templateValue) const
{
    if (templateParameter == TileTemplate::TEMPLATE_PARAM_ZOOM)
    {
        templateValue = Poco::NumberFormatter::format(key.zoom());
        return true;
    }
    else if (templateParameter == TileTemplate::TEMPLATE_PARAM_X)
    {
        templateValue = Poco::NumberFormatter::format(key.column());
        return true;
    }
    else if (templateParameter == TileTemplate::TEMPLATE_PARAM_Y)
    {
        templateValue = Poco::NumberFormatter::format(key.row());
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


void TileProvider::_setURITemplates(const std::vector<std::string>& templates)
{
    // Update parameters and generate ID.
    _URITemplates = templates;
    _URITemplateParameters.clear();

    Poco::MD5Engine md5;
    Poco::DigestOutputStream ostr(md5);

    for (auto _URITemplate: _URITemplates)
    {
        ostr << _URITemplate;
        _URITemplateParameters.push_back(TileTemplate::extractTemplateParameters(_URITemplate));
    }

    ostr.flush(); // Ensure everything gets passed to the digest engine
    const Poco::DigestEngine::Digest& digest = md5.digest(); // obtain result
    _id = Poco::DigestEngine::digestToHex(digest);
}


TileProvider TileProvider::fromJSON(const ofJson& json)
{
    TileProvider provider;

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
        else if (key == "template") { ofLogWarning("TileProvider::fromJSON") << "Unsupported TileJSON field: " << key; }
        else if (key == "legend") { ofLogWarning("TileProvider::fromJSON") << "Unsupported TileJSON field: " << key;  }
        else if (key == "scheme") { ofLogWarning("TileProvider::fromJSON") << "Unsupported TileJSON field: " << key;  }
        else if (key == "tiles")
        {
            std::vector<std::string> uriTemplates;
            for (const auto& uriTemplate: value)
            {
                uriTemplates.push_back(uriTemplate);
            }
            provider._setURITemplates(uriTemplates);
        }
        else if (key == "grids") { ofLogWarning("TileProvider::fromJSON") << "Unsupported TileJSON field: " << key; }
        else if (key == "data") { ofLogWarning("TileProvider::fromJSON") << "Unsupported TileJSON field: " << key; }
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
            auto center = provider.geoToTile(Geo::Coordinate(value[0], value[1]));
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


ofJson TileProvider::toJSON(const TileProvider& provider)
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

    json["bounds"] = {  provider.bounds().getNorthwest().getLatitude(),
                        provider.bounds().getNorthwest().getLongitude(),
                        provider.bounds().getSoutheast().getLatitude(),
                        provider.bounds().getSoutheast().getLongitude()
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
