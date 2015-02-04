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


#include <vector>
#include "Poco/AtomicCounter.h"
#include "ofx/HTTP/DefaultClientTask.h"
#include "ofx/IO/ByteBuffer.h"
#include "ofx/Maps/BaseURITileProvider.h"


namespace ofx {
namespace Maps {


class URLTileLoaderTask: public HTTP::DefaultClientTask
{
public:
    URLTileLoaderTask(HTTP::BaseRequest* request,
                      HTTP::BaseResponse* response,
                      HTTP::Context* context):
        HTTP::DefaultClientTask(request, response, context)
    {
    }


    virtual ~URLTileLoaderTask()
    {
    }


    virtual void handleBufferEvent(const HTTP::ClientResponseBufferEventArgs& bufferEvent)
    {
        IO::ByteBuffer byteBuffer = bufferEvent.getByteBuffer();
        ofBuffer buffer(byteBuffer.getCharPtr(), byteBuffer.size());
        std::shared_ptr<ofImage> img = std::shared_ptr<ofImage>(new ofImage());
        img->setUseTexture(false);

        if (img->loadImage(buffer))
        {
            postNotification(new Poco::TaskCustomNotification<std::shared_ptr<ofImage> >(this, img));
        }
        else
        {
            throw Poco::Exception("Unable to load image: " + bufferEvent.getRequest().getURI());
        }
    }

};


/// \brief A BaseMapTileProvider with default settings.
class BaseURLTileProvider: public BaseURITileProvider
{
public:
    BaseURLTileProvider(const std::string& URITemplate,
                        const std::string& attribution,
                        const std::vector<std::string>& subdomains,
                        int minZoom,
                        int maxZoom,
                        int tileWidth,
                        int tileHeight,
                        const BaseProjection& projection);

    /// \brief Destroy the BaseMapProvider.
    virtual ~BaseURLTileProvider();

    Poco::Task* requestTile(const TileCoordinate& coordinate) const;

    static const std::string TEMPLATE_PARAM_SUBDOMAIN;
    static const std::string TEMPLATE_PARAM_ZOOM;
    static const std::string TEMPLATE_PARAM_X;
    static const std::string TEMPLATE_PARAM_Y;
    static const std::string TEMPLATE_PARAM_ID;

    static const std::vector<std::string> DEFAULT_NUMERICAL_SUBDOMAINS;
    static const std::vector<std::string> DEFAULT_ALPHABETICAL_SUBDOMAINS;

    static std::vector<std::string> parseSubdomains(const std::string& subdomains);

protected:
    virtual bool getTileURITemplateValue(const TileCoordinate& coordinate,
                                         const std::string& templateParameter,
                                         std::string& templateValue) const;

    const std::vector<std::string> _subdomains;
    
    mutable Poco::AtomicCounter _currentSubdomainIndex;

    virtual HTTP::Context* createDefaultContext() const;

    virtual HTTP::BaseResponse* createDefaultResponse() const;

};


} } // namespace ofx::Maps
