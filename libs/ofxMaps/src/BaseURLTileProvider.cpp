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


#include "ofx/Maps/BaseURLTileProvider.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"
#include "ofx/HTTP/GetRequest.h"


namespace ofx {
namespace Maps {


const std::string BaseURLTileProvider::TEMPLATE_PARAM_SUBDOMAIN = "{s}";
const std::string BaseURLTileProvider::TEMPLATE_PARAM_ZOOM      = "{z}";
const std::string BaseURLTileProvider::TEMPLATE_PARAM_X         = "{x}";
const std::string BaseURLTileProvider::TEMPLATE_PARAM_Y         = "{y}";
const std::string BaseURLTileProvider::TEMPLATE_PARAM_ID        = "{id}";


BaseURLTileProvider::BaseURLTileProvider(const std::vector<std::string>& subdomains,
                                         const std::string& URITemplate,
                                         int minZoom,
                                         int maxZoom,
                                         int tileWidth,
                                         int tileHeight,
                                         const BaseProjection& projection,
                                         const std::string& attribution):
    BaseURITileProvider(URITemplate,
                        minZoom,
                        maxZoom,
                        tileWidth,
                        tileHeight,
                        projection,
                        attribution),
    _subdomains(subdomains),
    _currentSubdomainIndex(0)
{
}


BaseURLTileProvider::~BaseURLTileProvider()
{
}


Poco::Task* BaseURLTileProvider::requestTile(const TileCoordinate& coordinate) const
{
    return new URLTileLoaderTask(new HTTP::GetRequest(getTileURI(coordinate).toString()),
                                 createDefaultResponse(),
                                 createDefaultContext());
}


bool BaseURLTileProvider::getTileURITemplateValue(const TileCoordinate& coordinate,
                                                  const std::string& templateParameter,
                                                  std::string& templateValue) const
{
    if (0 == templateParameter.compare(TEMPLATE_PARAM_SUBDOMAIN))
    {
        templateValue = _subdomains[_currentSubdomainIndex];
        _currentSubdomainIndex = (_currentSubdomainIndex + 1) % _subdomains.size();
        return true;
    }
    else if (0 == templateParameter.compare(TEMPLATE_PARAM_ZOOM))
    {
        templateValue = Poco::NumberFormatter::format(coordinate.getZoom());
        return true;
    }
    else if (0 == templateParameter.compare(TEMPLATE_PARAM_X))
    {
        templateValue = Poco::NumberFormatter::format(coordinate.getColumn());
        return true;
    }
    else if (0 == templateParameter.compare(TEMPLATE_PARAM_Y))
    {
        templateValue = Poco::NumberFormatter::format(coordinate.getRow());
        return true;
    }
    else if (0 == templateParameter.compare(TEMPLATE_PARAM_ID))
    {
        templateValue = coordinate.getId();
        return true;
    }
    else
    {
        return false;
    }
}


HTTP::Context* BaseURLTileProvider::createDefaultContext() const
{
    // TODO, attach context info here.
    return new HTTP::Context();
}


HTTP::BaseResponse* BaseURLTileProvider::createDefaultResponse() const
{
    return new HTTP::BaseResponse();
}


} } // namespace ofx::Maps
