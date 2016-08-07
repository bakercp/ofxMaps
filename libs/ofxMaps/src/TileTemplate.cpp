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
