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


#include "ofx/Maps/MicrosoftTileProvider.h"


namespace ofx {
namespace Maps {


const std::string MicrosoftTileProvider::DEFAULT_MICROSOFT_ATTRIBUTION = "&copy; <a href=\"http://msdn.microsoft.com/en-us/library/bb259689.aspx\">Microsoft</a>";
const std::string MicrosoftTileProvider::TEMPLATE_PARAM_QUADKEY = "{quadkey}";


MicrosoftTileProvider::MicrosoftTileProvider(const std::string& URITemplate,
                                             const std::string& attribution,
                                             const std::vector<std::string>& subdomains,
                                             int minZoom,
                                             int maxZoom,
                                             int tileWidth,
                                             int tileHeight,
                                             const BaseProjection& projection):
    BaseURLTileProvider(URITemplate,
                        attribution,
                        subdomains,
                        minZoom,
                        maxZoom,
                        tileWidth,
                        tileHeight,
                        projection)
{
}


MicrosoftTileProvider::~MicrosoftTileProvider()
{
}


std::string MicrosoftTileProvider::tileCoordinateToQuadKey(int tileX,
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


bool MicrosoftTileProvider::quadKeyToTileCoordinate(const std::string& quadKey,
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


bool MicrosoftTileProvider::getTileURITemplateValue(const TileCoordinate& coordinate,
                                                    const std::string& templateParameter,
                                                    std::string& templateValue) const
{
    if (0 == templateParameter.compare(TEMPLATE_PARAM_QUADKEY))
    {
        templateValue = tileCoordinateToQuadKey(coordinate.getColumn(),
                                                coordinate.getRow(),
                                                coordinate.getZoom());
        return true;
    }
    else
    {
        return BaseURLTileProvider::getTileURITemplateValue(coordinate,
                                                            templateParameter,
                                                            templateValue);
    }
}

} } // namespace ofx::Maps
