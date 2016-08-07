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


#include "ofx/Maps/MBTilesConnection.h"
#include "ofImage.h"
#include "ofUtils.h"


namespace ofx {
namespace Maps {


const std::string MBTilesMetadata::KEY_MIN_ZOOM = "minzoom";
const std::string MBTilesMetadata::KEY_MAX_ZOOM = "maxzoom";
const std::string MBTilesMetadata::KEY_BOUNDS = "bounds";
const std::string MBTilesMetadata::KEY_CENTER = "center";
const std::string MBTilesMetadata::KEY_NAME = "name";
const std::string MBTilesMetadata::KEY_TYPE = "type";
const std::string MBTilesMetadata::KEY_VERSION = "version";
const std::string MBTilesMetadata::KEY_DESCRIPTION = "description";
const std::string MBTilesMetadata::KEY_ATTRIBUTION = "attribution";
const std::string MBTilesMetadata::KEY_FORMAT = "format";


MBTilesMetadata::MBTilesMetadata()
{
}


MBTilesMetadata::~MBTilesMetadata()
{
}


int MBTilesMetadata::minZoom() const
{
    return ofToInt(get(KEY_MIN_ZOOM, "0"));
}



int MBTilesMetadata::maxZoom() const
{
    return ofToInt(get(KEY_MAX_ZOOM, "0"));
}


Geo::CoordinateBounds MBTilesMetadata::bounds() const
{
    try
    {
        auto tokens = ofSplitString(get(KEY_BOUNDS), ",", true, true);
        return Geo::CoordinateBounds(Geo::Coordinate(ofToDouble(tokens[0]),
                                                     ofToDouble(tokens[1])),
                                     Geo::Coordinate(ofToDouble(tokens[2]),
                                                     ofToDouble(tokens[3])));
    }
    catch (const Poco::NotFoundException&)
    {
        return Geo::CoordinateBounds(Geo::Coordinate(-180.0, -85),
                                     Geo::Coordinate(180, 85));
    }
}


TileCoordinate MBTilesMetadata::center() const
{
    try
    {
        auto tokens = ofSplitString(get(KEY_CENTER), ",", true, true);
        return TileCoordinate(ofToDouble(tokens[0]),
                              ofToDouble(tokens[1]),
                              ofToDouble(tokens[2]));
    }
    catch (const Poco::NotFoundException&)
    {
        return TileCoordinate();
    }
}



std::string MBTilesMetadata::attribution() const
{
    return get(KEY_ATTRIBUTION, "");
}



std::string MBTilesMetadata::name() const
{
    return get(KEY_NAME, "");
}



std::string MBTilesMetadata::type() const
{
    return get(KEY_TYPE, "");
}


std::string MBTilesMetadata::version() const
{
    return get(KEY_VERSION, "");
}


std::string MBTilesMetadata::description()
{
    return get(KEY_DESCRIPTION, "");
}



std::string MBTilesMetadata::format() const
{
    return get(KEY_FORMAT, "");
}


const std::string MBTilesConnection::QUERY_SELECT_METADATA = "SELECT * FROM `metadata`;";
const std::string MBTilesConnection::QUERY_INSERT_METADATA = "INSERT INTO `metadata` (`name`, `value`) VALUES (:name, :value);";
const std::string MBTilesConnection::CREATE_TABLE_METADATA = "CREATE TABLE IF NOT EXISTS metadata (name TEXT, value TEXT);";
const std::string MBTilesConnection::DROP_TABLE_METADATA = "DROP TABLE IF EXISTS metadata";

const std::string MBTilesConnection::QUERY_TILES = "SELECT tile_data FROM `tiles` WHERE zoom_level = :zoom_level AND tile_column = :tile_column AND tile_row = :tile_row;";
const std::string MBTilesConnection::INSERT_IMAGE = "INSERT INTO `images` (`tile_id`, `tile_data`) VALUES (:tile_id, :tile_data)";
const std::string MBTilesConnection::INSERT_MAP = "INSERT INTO `map` (`zoom_level`, `tile_column`, `tile_row`, `tile_id`) VALUES (:zoom_level,  :tile_column,  :tile_row,  :tile_id)";

//"-- via https://github.com/mapbox/node-mbtiles/blob/master/lib/schema.sql"
// Several additions to assist with caching.
// images table - added tile_expires_date and tile_cached_date
// tiles -
// images.tile_data AS tile_data,"
// images.tile_expires_date AS tile_expires_date,"
// images.tile_cached_date AS tile_cached_date"

const std::string MBTilesConnection::MBTILES_SCHEMA =
"CREATE TABLE IF NOT EXISTS map ("
"   zoom_level INTEGER,"
"   tile_column INTEGER,"
"   tile_row INTEGER,"
"   tile_id TEXT,"
"   grid_id TEXT"
");"
""
"CREATE TABLE IF NOT EXISTS grid_key ("
"    grid_id TEXT,"
"    key_name TEXT"
");"
""
"CREATE TABLE IF NOT EXISTS keymap ("
"    key_name TEXT,"
"    key_json TEXT"
");"
""
"CREATE TABLE IF NOT EXISTS grid_utfgrid ("
"    grid_id TEXT,"
"    grid_utfgrid BLOB"
");"
""
"CREATE TABLE IF NOT EXISTS images ("
"    tile_data blob,"
"    tile_id text"
");"
""
"CREATE TABLE IF NOT EXISTS metadata ("
"    name text,"
"    value text"
");"
""
"CREATE TABLE IF NOT EXISTS geocoder_data ("
"    type TEXT,"
"    shard INTEGER,"
"    data BLOB"
");"
""
"CREATE UNIQUE INDEX IF NOT EXISTS map_index ON map (zoom_level, tile_column, tile_row);"
"CREATE UNIQUE INDEX IF NOT EXISTS grid_key_lookup ON grid_key (grid_id, key_name);"
"CREATE UNIQUE INDEX IF NOT EXISTS keymap_lookup ON keymap (key_name);"
"CREATE UNIQUE INDEX IF NOT EXISTS grid_utfgrid_lookup ON grid_utfgrid (grid_id);"
"CREATE UNIQUE INDEX IF NOT EXISTS images_id ON images (tile_id);"
"CREATE UNIQUE INDEX IF NOT EXISTS name ON metadata (name);"
"CREATE INDEX IF NOT EXISTS map_grid_id ON map (grid_id);"
"CREATE INDEX IF NOT EXISTS geocoder_type_index ON geocoder_data (type);"
"CREATE UNIQUE INDEX IF NOT EXISTS geocoder_shard_index ON geocoder_data (type, shard);"
""
"CREATE VIEW IF NOT EXISTS tiles AS"
"    SELECT"
"        map.zoom_level AS zoom_level,"
"        map.tile_column AS tile_column,"
"        map.tile_row AS tile_row,"
"        images.tile_data AS tile_data"
"    FROM map"
"    JOIN images ON images.tile_id = map.tile_id;"
""
"CREATE VIEW IF NOT EXISTS grids AS"
"    SELECT"
"        map.zoom_level AS zoom_level,"
"        map.tile_column AS tile_column,"
"        map.tile_row AS tile_row,"
"        grid_utfgrid.grid_utfgrid AS grid"
"    FROM map"
"    JOIN grid_utfgrid ON grid_utfgrid.grid_id = map.grid_id;"
""
"CREATE VIEW IF NOT EXISTS grid_data AS"
"    SELECT"
"        map.zoom_level AS zoom_level,"
"        map.tile_column AS tile_column,"
"        map.tile_row AS tile_row,"
"        keymap.key_name AS key_name,"
"        keymap.key_json AS key_json"
"    FROM map"
"    JOIN grid_key ON map.grid_id = grid_key.grid_id"
"    JOIN keymap ON grid_key.key_name = keymap.key_name;"
"";

//"CREATE TABLE IF NOT EXISTS map ("
//"   zoom_level INTEGER,"
//"   tile_column INTEGER,"
//"   tile_row INTEGER,"
//"   tile_id TEXT,"
//"   grid_id TEXT"
//");"
//""
//"CREATE TABLE IF NOT EXISTS grid_key ("
//"    grid_id TEXT,"
//"    key_name TEXT"
//");"
//""
//"CREATE TABLE IF NOT EXISTS keymap ("
//"    key_name TEXT,"
//"    key_json TEXT"
//");"
//""
//"CREATE TABLE IF NOT EXISTS grid_utfgrid ("
//"    grid_id TEXT,"
//"    grid_utfgrid BLOB"
//");"
//""
//"CREATE TABLE IF NOT EXISTS images ("
//"    tile_data blob,"
//"    tile_id text,"
//"    tile_set_id text,"
//"    tile_expires_date text,"
//"    tile_cached_date text"
//");"
//""
//"CREATE TABLE IF NOT EXISTS metadata ("
//"    name text,"
//"    value text"
//");"
//""
//"CREATE TABLE IF NOT EXISTS geocoder_data ("
//"    type TEXT,"
//"    shard INTEGER,"
//"    data BLOB"
//");"
//""
//"CREATE UNIQUE INDEX IF NOT EXISTS map_index ON map (zoom_level, tile_column, tile_row);"
//"CREATE UNIQUE INDEX IF NOT EXISTS grid_key_lookup ON grid_key (grid_id, key_name);"
//"CREATE UNIQUE INDEX IF NOT EXISTS keymap_lookup ON keymap (key_name);"
//"CREATE UNIQUE INDEX IF NOT EXISTS grid_utfgrid_lookup ON grid_utfgrid (grid_id);"
//"CREATE UNIQUE INDEX IF NOT EXISTS images_id ON images (tile_id);"
//"CREATE UNIQUE INDEX IF NOT EXISTS images_set_id ON images (tile_set_id);"
//"CREATE UNIQUE INDEX IF NOT EXISTS name ON metadata (name);"
//"CREATE INDEX IF NOT EXISTS map_grid_id ON map (grid_id);"
//"CREATE INDEX IF NOT EXISTS geocoder_type_index ON geocoder_data (type);"
//"CREATE UNIQUE INDEX IF NOT EXISTS geocoder_shard_index ON geocoder_data (type, shard);"
//""
//"CREATE VIEW IF NOT EXISTS tiles AS"
//"    SELECT"
//"        map.zoom_level AS zoom_level,"
//"        map.tile_column AS tile_column,"
//"        map.tile_row AS tile_row,"
//"        images.tile_id AS tile_id,"
//"        images.tile_set_id AS tile_set_id,"
//"        images.tile_data AS tile_data,"
//"        images.tile_expires_date AS tile_expires_date,"
//"        images.tile_cached_date AS tile_cached_date"
//"    FROM map"
//"    JOIN images ON images.tile_id = map.tile_id;"
//""
//"CREATE VIEW IF NOT EXISTS grids AS"
//"    SELECT"
//"        map.zoom_level AS zoom_level,"
//"        map.tile_column AS tile_column,"
//"        map.tile_row AS tile_row,"
//"        grid_utfgrid.grid_utfgrid AS grid"
//"    FROM map"
//"    JOIN grid_utfgrid ON grid_utfgrid.grid_id = map.grid_id;"
//""
//"CREATE VIEW IF NOT EXISTS grid_data AS"
//"    SELECT"
//"        map.zoom_level AS zoom_level,"
//"        map.tile_column AS tile_column,"
//"        map.tile_row AS tile_row,"
//"        keymap.key_name AS key_name,"
//"        keymap.key_json AS key_json"
//"    FROM map"
//"    JOIN grid_key ON map.grid_id = grid_key.grid_id"
//"    JOIN keymap ON grid_key.key_name = keymap.key_name;"
//"";


MBTilesConnection::MBTilesConnection(const std::string& filename, Mode mode):
    _filename(ofToDataPath(filename, true)),
    _mode(mode),
    _database(_filename, _toAccessFlag(_mode))
{
    if (_mode != Mode::READ_ONLY)
    {
        try
        {
            SQLite::Transaction transaction(_database);
            _database.exec(MBTILES_SCHEMA);
            transaction.commit();
        }
        catch (const std::exception& e)
        {
            ofLogError("MBTilesConnection::MBTilesConnection()") << "SQLite exception: " << e.what();
        }
    }
}


MBTilesConnection::~MBTilesConnection()
{
}


void MBTilesConnection::setMetaData(const MBTilesMetadata& metadata)
{
    if (_mode != Mode::READ_ONLY)
    {
        try
        {
            SQLite::Transaction transaction(_database);

            // Delete the old table.
            _database.exec(DROP_TABLE_METADATA);

            // Recreate it.
            _database.exec(CREATE_TABLE_METADATA);

            auto& query = _getStatement(QUERY_INSERT_METADATA);

            for (const auto& entry: metadata)
            {
                query.bind(":name", entry.first);
                query.bind(":value", entry.second);
                query.exec();
            }

            transaction.commit();
        }
        catch (const std::exception& e)
        {
            ofLogError("MBTilesConnection::setMetaData()") << "SQLite exception: " << e.what();
        }
    }
    else
    {
        ofLogError("MBTilesConnection::setMetaData()") << "No setting data on a read-only database.";
    }
}


MBTilesMetadata MBTilesConnection::getMetaData() const
{
    MBTilesMetadata result;

    try
    {
        auto& query = _getStatement(QUERY_SELECT_METADATA);

        while (query.executeStep())
        {
            result.set(query.getColumn("name"), query.getColumn("value"));
        }
    }
    catch (const std::exception& e)
    {
        ofLogError("MBTilesConnection::getMetaData()") << "SQLite exception: " << e.what();
    }

    return result;
}


bool MBTilesConnection::setTile(const TileCoordinateKey& key,
                                const ofBuffer& image,
                                const std::string& cachedDate,
                                const std::string& expiresDate)
{
    if (_mode != Mode::READ_ONLY)
    {
        try
        {
            std::string tileId = key.tileId();

            if (tileId.empty())
            {
                tileId = TileCoordinateUtils::hash(key);
            }

            SQLite::Statement& insertImage = _getStatement(INSERT_IMAGE);
            insertImage.bind(":tile_id", tileId);
            insertImage.bind(":tile_data", image.getData(), image.size());

            if (insertImage.exec() == 1)
            {
                SQLite::Statement& insertMap = _getStatement(INSERT_MAP);
                insertMap.bind(":tile_column", static_cast<int>(key.column()));
                insertMap.bind(":tile_row", static_cast<int>(key.row()));
                insertMap.bind(":zoom_level", static_cast<int>(key.zoom()));
                insertMap.bind(":tile_id", tileId);

                if (insertMap.exec() == 1)
                {
                    return true;
                }
                else
                {
                    ofLogError("MBTilesConnection::setTile()") << "Unable to insert map.";
                    return false;
                }
            }
            else
            {
                ofLogError("MBTilesConnection::setTile()") << "Unable to insert image.";
                return false;
            }
        }
        catch (const std::exception& e)
        {
            ofLogError("MBTilesConnection::setTile()") << "SQLite exception: " << e.what();
            return false;
        }
    }
    else
    {
        ofLogError("MBTilesConnection::setTile()") << "No setting data on a read-only database.";
        return false;
    }
}


std::shared_ptr<Tile> MBTilesConnection::getTile(const TileCoordinateKey& key) const
{
    try
    {
        SQLite::Statement& query = _getStatement(QUERY_TILES);

        query.bind(":zoom_level", static_cast<int>(key.zoom()));
        query.bind(":tile_column", static_cast<int>(key.column()));
        query.bind(":tile_row", static_cast<int>(key.row()));

        int result = query.executeStep();
        auto column = query.getColumn("tile_data");

        if (column.isBlob())
        {
            ofBuffer buffer(reinterpret_cast<const char*>(column.getBlob()), column.getBytes());

            ofPixels pixels;

            if (ofLoadImage(pixels, buffer))
            {
                return std::make_shared<Tile>(pixels);
            }
            else
            {
                ofLogError("MBTilesConnection::getTile()") << "Error loading pixels.";
                return nullptr;
            }
        }
        else
        {
            ofLogError("MBTilesConnection::getTile()") << "Tile data existed, but wasn't a blob: " << _filename << " " << key.coordinate().toString();
            return nullptr;
        }
    }
    catch (const std::exception& e)
    {
        ofLogError("MBTilesConnection::getTile()") << "SQLite exception: " << e.what();
        return nullptr;
    }
}


int MBTilesConnection::_toAccessFlag(Mode mode)
{
    switch (mode)
    {
        case Mode::READ_ONLY:
            return SQLITE_OPEN_READONLY;
        case Mode::READ_WRITE:
            return SQLITE_OPEN_READWRITE;
        case Mode::READ_WRITE_CREATE:
            return SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    }
}


SQLite::Statement& MBTilesConnection::_getStatement(const std::string& query) const
{
    auto iter = _statements.find(query);

    if (iter != _statements.end())
    {
        iter->second->reset();
        iter->second->clearBindings();
        return *iter->second.get();
    }
    else
    {
        // Blah ...
        auto result = _statements.insert(std::make_pair(query, std::make_unique<SQLite::Statement>(_database, query)));
        // Get ready for it ...
        return *result.first->second.get();
    }
}


} } // namespace ofx::Maps
