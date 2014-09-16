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


#include <string>
#include "Poco/RegularExpression.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/TaskNotification.h"
#include "Poco/URI.h"
#include "ofx/Maps/BaseTileProvider.h"
#include "ofFileUtils.h"
#include "ofImage.h"


namespace ofx {
namespace Maps {


class URITileLoaderTask: public Poco::Task
{
public:
	URITileLoaderTask(const Poco::URI& uri): Poco::Task(uri.toString())
    {
    }

    virtual void runTask()
    {
        if (_uri.getScheme() == "file")
        {
            Poco::FileInputStream fis(_uri.getPath());

            ofBuffer buffer;

            fis >> buffer;

            std::shared_ptr<ofImage> img = std::shared_ptr<ofImage>(new ofImage());

            img->setUseTexture(false);

            if (img->loadImage(buffer))
            {
                postNotification(new Poco::TaskCustomNotification<std::shared_ptr<ofImage> >(this, img));
            }
            else
            {
                std::cout << "><>>><><>" << _uri.toString() << endl;
                throw Poco::Exception("Unable to load image: " + _uri.toString());
            }
        }
        else
        {
            throw Poco::Exception("Invalid URI scheme: " + _uri.getScheme());
        }
    }

private:
    Poco::URI _uri;

};


/// \brief A BaseMapTileProvider with default settings.
class BaseURITileProvider: public BaseTileProvider
{
public:
    BaseURITileProvider(const std::string& URITemplate,
                        int minZoom,
                        int maxZoom,
                        int tileWidth,
                        int tileHeight,
                        const BaseProjection& projection,
                        const std::string& attribution);

    /// \brief Destroy the BaseMapProvider.
    virtual ~BaseURITileProvider();

    virtual std::string getID() const;

    Poco::Task* requestTile(const TileCoordinate& coordinate) const;

    Poco::URI getTileURI(const TileCoordinate& coordinate) const;

protected:
    virtual bool getTileURITemplateValue(const TileCoordinate& coordinate,
                                         const std::string& templateParameter,
                                         std::string& templateValue) const = 0;

    const std::string _URITemplate;
    const std::vector<std::string> _URITemplateParameters;

    static const Poco::RegularExpression TEMPLATE_PARAM_REGEX;
    static std::vector<std::string> extractTemplateParameters(const std::string& URITemplate);

private:
    std::string _id;
    
};


} } // namespace ofx::Maps
