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


#include "ofx/Maps/BaseURITileProvider.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"


namespace ofx {
namespace Maps {


//const Poco::RegularExpression BaseURITileProvider::TEMPLATE_PARAM_REGEX("{[a-zA-Z0-9]+}");
//
//
//std::vector<std::string> BaseURITileProvider::extractTemplateParameters(const std::string& URITemplate)
//{
//    std::vector<std::string> URITemplateParameters;
//
//    Poco::RegularExpression::Match match;
//
//    std::string::size_type offset = 0;
//
//    while (TEMPLATE_PARAM_REGEX.match(URITemplate, offset, match))
//    {
//        offset = match.offset + match.length;
//        URITemplateParameters.push_back(std::string(URITemplate, match.offset, match.length));
//    }
//
//    return URITemplateParameters;
//}
//
//
//BaseURITileProvider::BaseURITileProvider(const std::string& URITemplate,
//                                         int minZoom,
//                                         int maxZoom,
//                                         int tileWidth,
//                                         int tileHeight,
//                                         const BaseProjection& projection,
//                                         const std::string& attribution):
//    BaseTileProvider(minZoom,
//                     maxZoom,
//                     tileWidth,
//                     tileHeight,
//                     projection,
//                     attribution),
//    _URITemplate(URITemplate),
//    _URITemplateParameters(extractTemplateParameters(_URITemplate))
//{
//}
//
//
//BaseURITileProvider::~BaseURITileProvider()
//{
//}
//
//
//Poco::URI BaseURITileProvider::getTileURI(const TileCoordinate& coordinate) const
//{
//    std::string URITemplate(_URITemplate);
//
//    std::vector<std::string>::const_iterator iter = _URITemplateParameters.begin();
//
//    while (iter != _URITemplateParameters.end())
//    {
//        const std::string& templateParam = *iter;
//
//        std::string templateValue;
//
//        if (getTileURITemplateValue(coordinate, templateParam, templateValue))
//        {
//            Poco::replaceInPlace(URITemplate,
//                                 templateParam,
//                                 templateValue);
//        }
//        else
//        {
//            ofLogWarning("BaseURITileProvider::getTileURI") << "Ignoring unknown template parameter: " << templateParam;
//        }
//
//        ++iter;
//    }
//
//    try
//    {
//        return Poco::URI(URITemplate);
//    }
//    catch (const Poco::SyntaxException& exc)
//    {
//        ofLogError("BaseURITileProvider::getTileURI") << exc.displayText();
//        return Poco::URI();
//    }
//}


} } // namespace ofx::Maps
