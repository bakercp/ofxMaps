//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/Maps/TileTemplate.h"
#include <sstream>
#include <iostream>


namespace ofx {
namespace Maps {


const Poco::RegularExpression TileTemplate::TEMPLATE_PARAM_REGEX("{[a-zA-Z0-9_-]+}");

const std::string TileTemplate::TEMPLATE_PARAM_QUADKEY   = "{quadkey}";
const std::string TileTemplate::TEMPLATE_PARAM_ZOOM      = "{z}";
const std::string TileTemplate::TEMPLATE_PARAM_X         = "{x}";
const std::string TileTemplate::TEMPLATE_PARAM_Y         = "{y}";
const std::string TileTemplate::TEMPLATE_PARAM_TILE_ID   = "{tile_id}";
const std::string TileTemplate::TEMPLATE_PARAM_SET_ID    = "{set_id}";


std::string TileTemplate::tileCoordinateToQuadKey(int tileX,
                                                  int tileY,
                                                  int levelOfDetail)
{
    std::stringstream quadKey;

    for (int i = levelOfDetail; i > 0; --i)
    {
        char digit = '0';

        int mask = 1 << (i - 1);

        if ((tileX & mask) != 0)
        {
            digit++;
        }

        if ((tileY & mask) != 0)
        {
            digit++;
            digit++;
        }

        quadKey << digit;
    }

    return quadKey.str();
}


bool TileTemplate::quadKeyToTileCoordinate(const std::string& quadKey,
                                           int& tileX,
                                           int& tileY,
                                           int& levelOfDetail)
{
    tileX = 0;
    tileY = 0;

    levelOfDetail = quadKey.length();

    for (int i = levelOfDetail; i > 0; --i)
    {
        int mask = 1 << (i - 1);

        switch (quadKey[levelOfDetail - i])
        {
            case '0':
                break;

            case '1':
                tileX |= mask;
                break;

            case '2':
                tileY |= mask;
                break;

            case '3':
                tileX |= mask;
                tileY |= mask;
                break;

            default:
                return false;
        }
    }

    return true;
}


std::vector<std::string> TileTemplate::extractTemplateParameters(const std::string& URITemplate)
{
    std::vector<std::string> URITemplateParameters;
    
    Poco::RegularExpression::Match match;
    
    std::string::size_type offset = 0;
    
    while (TEMPLATE_PARAM_REGEX.match(URITemplate, offset, match))
    {
        offset = match.offset + match.length;

        std::string parameter = std::string(URITemplate, match.offset, match.length);

        URITemplateParameters.push_back(parameter);
    }

    return URITemplateParameters;
}


} } // namespace ofx::Maps
