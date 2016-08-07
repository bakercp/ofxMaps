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


#include "Poco/Net/NameValueCollection.h"
#include "SQLiteCpp.h"
#include "ofx/Geo/CoordinateBounds.h"
#include "ofx/Geo/Coordinate.h"
#include "ofx/Maps/Tile.h"
#include "ofx/Maps/TileCoordinate.h"


namespace ofx {
namespace Maps {


class MBTilesMetadata: public Poco::Net::NameValueCollection
{
public:
    MBTilesMetadata();

    virtual ~MBTilesMetadata();

    /// \returns the minimum zoom level.
    int minZoom() const;

    /// \returns the maximum zoom level.
    int maxZoom() const;

    /// \returns the bounding box of the tiles.
    Geo::CoordinateBounds bounds() const;

    /// \returns the center of the tiles.
    TileCoordinate center() const;

    /// \returns the plain-english name of the tileset.
    std::string attribution() const;

    /// \returns the plain-english name of the tileset.
    std::string name() const;

    /// \returns the data type, overlay or baselayer.
    std::string type() const;

    /// \returns the version of the tileset, as a plain number.
    std::string version() const;

    /// \returns a description of the layer as plain text.
    std::string description();

    /// \returns the image file format of the data.
    std::string format() const;

    static const std::string KEY_MIN_ZOOM;
    static const std::string KEY_MAX_ZOOM;
    static const std::string KEY_BOUNDS;
    static const std::string KEY_CENTER;
    static const std::string KEY_NAME;
    static const std::string KEY_TYPE;
    static const std::string KEY_VERSION;
    static const std::string KEY_DESCRIPTION;
    static const std::string KEY_ATTRIBUTION;
    static const std::string KEY_FORMAT;

};



class MBTilesConnection
{
public:
    enum class Mode
    {
        READ_ONLY,
        READ_WRITE,
        READ_WRITE_CREATE
    };

    /// \param filename The filename of the sqlite database.
    /// \param mode The access mode used during this connection.
    MBTilesConnection(const std::string& filename, Mode mode = Mode::READ_ONLY);

    /// \brief Destroy the MBTiles.
    virtual ~MBTilesConnection();

    MBTilesMetadata getMetaData() const;
    void setMetaData(const MBTilesMetadata& metadata);

    std::shared_ptr<Tile> getTile(const TileCoordinateKey& key) const;
    
    bool setTile(const TileCoordinateKey& key,
                 const ofBuffer& image,
                 const std::string& cachedDate,
                 const std::string& expiresDate);

    static const std::string QUERY_SELECT_METADATA;
    static const std::string QUERY_INSERT_METADATA;
    static const std::string CREATE_TABLE_METADATA;
    static const std::string DROP_TABLE_METADATA;

    static const std::string QUERY_TILES;
    static const std::string QUERY_TILES_WITH_SET_ID;

    static const std::string INSERT_IMAGE;
    static const std::string INSERT_MAP;

    static const std::string MBTILES_SCHEMA;

protected:
    static int _toAccessFlag(Mode mode);

    SQLite::Statement& _getStatement(const std::string& query) const;

    std::string _filename;
    Mode _mode = Mode::READ_ONLY;
    mutable SQLite::Database _database;

    /// \brief A collection of prepared statements for this connection.
    ///
    /// These will be destroyed when the connection is destroyed.
    mutable std::map<std::string, std::unique_ptr<SQLite::Statement>> _statements;
    
};


//class MBTilesProvider


} } // namespace ofx::Maps
