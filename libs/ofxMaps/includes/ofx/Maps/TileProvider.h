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


#pragma once


#include "Poco/RegularExpression.h"
#include "ofJson.h"
#include "ofx/Maps/AbstractMapTypes.h"
#include "ofx/Maps/BaseProjection.h"
#include "ofx/Maps/SphericalMercatorProjection.h"


namespace ofx {
namespace Maps {


/// \brief A TileProvider with default settings.
class TileProvider: public AbstractURITileProvider
{
public:
    /// \brief Create a default TileProvider with no endpoint.
    TileProvider();

    /// \brief Create a TileProvider.
    /// \param URITemplates The collection of template URLs.
    /// \param minZoom The minimum zoom level supported by the provider.
    /// \param maxZoom The maximum zoom level supported by the provider.
    /// \param tileWidth The width of the provider's tiles in pixels.
    /// \param tileHeight The height of the provider's tiles in pixels.
    /// \param bounds The bounds of the provider.
    /// \param center The initial center for this provider.
    /// \param projection The projection used by the provider.
    TileProvider(const std::vector<std::string>& URITemplates,
                 int minZoom,
                 int maxZoom,
                 int tileWidth,
                 int tileHeight,
                 const Geo::CoordinateBounds& bounds,
                 const TileCoordinate& center,
                 const BaseProjection& projection);

    /// \brief Destroy the TileProvider.
    virtual ~TileProvider();

    virtual std::string id() const override;
    std::string name() const override;
    std::string description() const override;
    std::string attribution() const override;
    std::string version() const override;
    int minZoom() const override;
    int maxZoom() const override;
    int tileWidth() const override;
    int tileHeight() const override;
    glm::ivec2 tileSize() const override;
    Geo::CoordinateBounds bounds() const override;
    TileCoordinate center() const override;
    double zoomForScale(double scale) const override;
    TileCoordinate geoToTile(const Geo::Coordinate& location) const override;
    Geo::Coordinate tileToGeo(const TileCoordinate& coordinate) const override;
    std::string getTileURI(const TileCoordinateKey& coordinate) const override;

    /// \returns the URI templates.
    const std::vector<std::string> URITemplates() const;

    enum
    {
        /// \brief The minimum zoom level supported by most map tile providers.
        DEFAULT_MIN_ZOOM = 0,
        /// \brief The maximum zoom level supported by most map tile providers.
        DEFAULT_MAX_ZOOM = 22,
        /// \brief The default tile width supported by most map tile providers.
        DEFAULT_TILE_WIDTH = 256,
        /// \brief The default tile height supported by most map tile providers.
        DEFAULT_TILE_HEIGHT = 256
    };

    /// \brief The default map bounds.
    static const Geo::CoordinateBounds DEFAULT_BOUNDS;

    /// \brief The default map center.
    static const TileCoordinate DEFAULT_CENTER;

    /// \brief The default projection used by most map tile providers.
    static const SperhicalMercatorProjection DEFAULT_PROJECTION;

    /// \brief Create a TileProvider from JSON.
    ///
    /// This parses the JSON in the TileJSON 2.1.0 format. Not all features
    /// are supported.
    ///
    /// \param json The json to parse.
    /// \returns a configured TileProvider.
    /// \sa https://github.com/mapbox/tilejson-spec/tree/master/2.1.0
    static TileProvider fromJSON(const ofJson& json);

    /// \brief Export the TileProvider as JSON.
    ///
    /// This exports the provider in a TileJSON 2.1.0 format. Not all features
    /// are supported.
    ///
    /// \param provider The provider to save.
    /// \returns the json..
    /// \sa https://github.com/mapbox/tilejson-spec/tree/master/2.1.0
    static ofJson toJSON(const TileProvider& provider);

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
    virtual bool getTileURITemplateValue(const TileCoordinateKey& key,
                                         const std::string& templateParameter,
                                         std::string& templateValue) const;

    /// \brief The URI template used for extraction.
    std::vector<std::string> _URITemplates;

    /// \brief A collection of URI template parameters for each of the templates.
    std::vector<std::vector<std::string>> _URITemplateParameters;

private:
    void _setURITemplates(const std::vector<std::string>& templates);

    /// \brief A unique ID for the provider, based on the URI template.
    std::string _id;

    /// \brief This provider's name.
    std::string _name;

    /// \brief This provider's description.
    std::string _description;

    /// \brief A string containing this provider's attribution.
    std::string _attribution;

    /// \brief A string containing this provider's data version.
    std::string _version;

    /// \brief The minimum zoom level for this provider.
    int _minZoom;

    /// \brief The maximum zoom level for this provider.
    int _maxZoom;

    /// \brief The tile width and height used by this provider.
    glm::ivec2 _tileSize;

    /// \brief The bounds for this provider.
    Geo::CoordinateBounds _bounds;

    /// \brief The initial center for this provider.
    TileCoordinate _center;

    /// \brief A reference to this provider's projection.
    const BaseProjection& _projection;

    /// \brief A dictionary of unknown parameters that can be used for template matching.
    std::map<std::string, std::string> _dictionary;

};


} } // namespace ofx::Maps
