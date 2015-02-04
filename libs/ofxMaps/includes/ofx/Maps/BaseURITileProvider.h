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

            if (img->load(buffer))
            {
                postNotification(new Poco::TaskCustomNotification<std::shared_ptr<ofImage> >(this, img));
            }
            else
            {
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
    /// \brief Create a BaseURITileProvider.
    /// \param URITemplate The template URL for requesting tiles by coordinate.
    /// \param attribution The attribution string for the tile source.
    /// \param minZoom The minimum zoom level supported by the provider.
    /// \param maxZoom The maximum zoom level supported by the provider.
    /// \param tileWidth The width of the provider's tiles in pixels.
    /// \param tileHeight The height of the provider's tiles in pixels.
    /// \param projection The projection used by the provider.
    BaseURITileProvider(const std::string& URITemplate,
                        const std::string& attribution,
                        int minZoom,
                        int maxZoom,
                        int tileWidth,
                        int tileHeight,
                        const BaseProjection& projection);

    /// \brief Destroy the BaseMapProvider.
    virtual ~BaseURITileProvider();

    virtual std::string getID() const;

    Poco::Task* requestTile(const TileCoordinate& coordinate) const;

    Poco::URI getTileURI(const TileCoordinate& coordinate) const;

protected:
    /// \brief Extracts a template parameter value if it is available.
    ///
    /// This class should be overriden if the provider uses additional URI
    /// template variables.
    ///
    /// \param coordinate The tile coordinate requested.
    /// \param templateParameter The template parameter requested.
    /// \param templateValue The extracted value to be filled.
    /// \returns true iff the extraction was successful.
    virtual bool getTileURITemplateValue(const TileCoordinate& coordinate,
                                         const std::string& templateParameter,
                                         std::string& templateValue) const = 0;

    /// \brief The URI template used for extraction.
    const std::string _URITemplate;

    /// \brief A collection of URI template parameters for the template.
    const std::vector<std::string> _URITemplateParameters;

    /// \brief A regular expression used for extracting the template parameters.
    static const Poco::RegularExpression TEMPLATE_PARAM_REGEX;

    /// \brief A utility method for extracting parameters from a template.
    static std::vector<std::string> extractTemplateParameters(const std::string& URITemplate);

private:
    /// \brief A unique ID for the provider, based on the URI template.
    std::string _ID;
    
};


} } // namespace ofx::Maps
