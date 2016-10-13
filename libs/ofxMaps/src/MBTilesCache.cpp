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


#include "ofx/Maps/MBTilesCache.h"
#include "Poco/SHA1Engine.h"
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


const std::string MBTilesConnection::QUERY_SELECT_METADATA = "SELECT * FROM `metadata`";
const std::string MBTilesConnection::QUERY_INSERT_METADATA = "INSERT INTO `metadata` (`name`, `value`) VALUES (:name, :value)";
const std::string MBTilesConnection::CREATE_TABLE_METADATA = "CREATE TABLE IF NOT EXISTS metadata (name TEXT, value TEXT)";
const std::string MBTilesConnection::DROP_TABLE_METADATA = "DROP TABLE IF EXISTS metadata";

const std::string MBTilesConnection::QUERY_TILES = "SELECT tile_data FROM `tiles` WHERE zoom_level = :zoom_level AND tile_column = :tile_column AND tile_row = :tile_row";
const std::string MBTilesConnection::COUNT_TILES = "SELECT COUNT(tile_data) FROM `tiles` WHERE zoom_level = :zoom_level AND tile_column = :tile_column AND tile_row = :tile_row";

//const std::string MBTilesConnection::INSERT_IMAGE = "INSERT OR IGNORE INTO `images` (`tile_id`, `tile_data`) VALUES (:tile_id, :tile_data)";
const std::string MBTilesConnection::INSERT_IMAGE = "INSERT INTO `images` (`tile_id`, `tile_data`) VALUES (:tile_id, :tile_data)";

const std::string MBTilesConnection::COUNT_IMAGE = "SELECT COUNT(tile_id) FROM `images` WHERE tile_id = :tile_id";

const std::string MBTilesConnection::INSERT_MAP = "INSERT INTO `map` (`zoom_level`, `tile_column`, `tile_row`, `tile_id`) VALUES (:zoom_level,  :tile_column,  :tile_row,  :tile_id)";
const std::string MBTilesConnection::COUNT_MAP = "SELECT COUNT(tile_id) FROM `map` WHERE zoom_level = :zoom_level AND tile_column = :tile_column AND tile_row = :tile_row";

const std::string MBTilesConnection::COUNT_ALL = "SELECT COUNT(*) FROM `tiles`";



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


//MBTilesConnection::MBTilesConnection(const std::string& filename,
//                                     Mode mode,
//                                     uint64_t databaseTimeoutMilliseconds,
//                                     Poco::RWLock* mutex):
//    SQLite::SQLiteConnection(filename,
//                             mode,
//                             databaseTimeoutMilliseconds,
//                             mutex)
//{
//}


MBTilesConnection::~MBTilesConnection()
{
}


bool MBTilesConnection::setMetaData(const MBTilesMetadata& metadata) noexcept
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

            auto& query = getStatement(QUERY_INSERT_METADATA);

            for (const auto& entry: metadata)
            {
                query.bind(":name", entry.first);
                query.bind(":value", entry.second);
                query.exec();
                query.reset();
            }

            // Commit all values.
            transaction.commit();

            return true;
        }
        catch (const std::exception& e)
        {
            ofLogError("MBTilesConnection::setMetaData()") << "SQLite exception: " << e.what();
            return false;
        }
    }
    else
    {
        ofLogError("MBTilesConnection::setMetaData()") << "No setting data on a read-only database.";
        return false;
    }
}


MBTilesMetadata MBTilesConnection::getMetaData() const noexcept
{
    MBTilesMetadata result;

    try
    {
        auto& query = getStatement(QUERY_SELECT_METADATA);

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


bool MBTilesConnection::setTile(const TileKey& key,
                                const ofBuffer& image) noexcept
{
    if (_mode != Mode::READ_ONLY)
    {
        std::string tileId = key.tileId();

        if (tileId.empty())
        {
            Poco::SHA1Engine sha1;
            sha1.update(image.getData(), image.size());
            const auto& digest = sha1.digest(); // obtain result
            tileId = Poco::DigestEngine::digestToHex(digest);
        }

        try
        {
            SQLite::Statement& countImage = getStatement(COUNT_IMAGE);
            countImage.bind(":tile_id", tileId);
            int result = countImage.executeStep();
            auto column = countImage.getColumn(0);

            if (column.getInt64() == 0)
            {
                SQLite::Statement& insertImage = getStatement(INSERT_IMAGE);
                insertImage.bind(":tile_id", tileId);
                insertImage.bind(":tile_data", image.getData(), image.size());

                if (insertImage.exec() != 1)
                {
                    ofLogError("MBTilesConnection::setTile()") << "Unable to insert image.";
                    return false;
                }
            }
        }
        catch (const std::exception& e)
        {
            ofLogError("MBTilesConnection::setTile()") << "INSERTING TILE SQLite exception: " << e.what() << " " << index() << " " << useCount();
            return false;
        }

        try
        {
            SQLite::Statement& selectMap = getStatement(COUNT_MAP);
            selectMap.bind(":tile_column", key.column());
            selectMap.bind(":tile_row", key.row());
            selectMap.bind(":zoom_level", key.zoom());
            int result = selectMap.executeStep();
            auto column = selectMap.getColumn(0);

            if (column.getInt64() == 0)
            {
                SQLite::Statement& insertMap = getStatement(INSERT_MAP);
                insertMap.bind(":tile_column", key.column());
                insertMap.bind(":tile_row", key.row());
                insertMap.bind(":zoom_level", key.zoom());
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
                return true;
            }

        }
        catch (const std::exception& e)
        {
            ofLogError("MBTilesConnection::setTile()") << "INSERTING MAP SQLite exception: " << e.what() << " " << index() << " " << useCount();
            return false;
        }

    }
    else
    {
        ofLogError("MBTilesConnection::setTile()") << "No setting data on a read-only database.";
        mutex().unlock();
        return false;
    }
}


bool MBTilesConnection::has(const TileKey& key) const noexcept
{
    try
    {
        SQLite::Statement& query = getStatement(COUNT_TILES);
        query.bind(":tile_row", key.row());
        query.bind(":zoom_level", key.zoom());
        query.bind(":tile_column", key.column());
        int result = query.executeStep();
        auto column = query.getColumn(0);
        return column.getInt64() > 0;
    }
    catch (const std::exception& e)
    {
        ofLogError("MBTilesConnection::has()") << "SQLite exception: " << e.what();
        return nullptr;
    }
}


std::shared_ptr<ofBuffer> MBTilesConnection::getBuffer(const TileKey& key) const noexcept
{
    try
    {
        SQLite::Statement& query = getStatement(QUERY_TILES);

        query.bind(":tile_row", key.row());
        query.bind(":zoom_level", key.zoom());
        query.bind(":tile_column", key.column());

        int result = query.executeStep();

        if (result)
        {
            auto column = query.getColumn("tile_data");

            if (column.isBlob())
            {
                return std::make_shared<ofBuffer>(reinterpret_cast<const char*>(column.getBlob()), column.getBytes());
            }
            else
            {
                ofLogError("MBTilesConnection::getBuffer") << "Tile data existed, but wasn't a blob: " << this->database().getFilename() << " ";// << key.coordinate().toString();
                return nullptr;
            }
        }
        else
        {
            // We simply don't have it.
            return nullptr;
        }
    }
    catch (const std::exception& e)
    {
        ofLogError("MBTilesConnection::getBuffer") << "SQLite exception: " << e.what();
        return nullptr;
    }
}


size_t MBTilesConnection::size() const noexcept
{
    try
    {
        SQLite::Statement& query = getStatement(COUNT_ALL);
        int result = query.executeStep();
        auto column = query.getColumn(0);
        return static_cast<std::size_t>(column.getInt64());
    }
    catch (const std::exception& e)
    {
        ofLogError("MBTilesConnection::size") << "SQLite exception: " << e.what();
        return 0;
    }
}



std::shared_ptr<Tile> MBTilesConnection::getTile(const TileKey& key) const noexcept
{
    auto buffer = getBuffer(key);

    if (buffer != nullptr)
    {
        ofPixels pixels;

        if (ofLoadImage(pixels, *buffer))
        {
            return std::make_shared<Tile>(pixels);
        }
        else
        {
            ofLogError("MBTilesConnection::getTile") << "Error loading pixels.";
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}


MBTilesCache::MBTilesCache(const MapTileProvider& tileProvider,
                           const std::string& cachePath,
                           uint64_t databaseTimeoutMilliseconds,
                           std::size_t capacity,
                           std::size_t peakCapacity):
    _writeConnection(cachePath + "/" + tileProvider.id() + ".mbtiles",
                     SQLite::SQLiteConnection::Mode::READ_WRITE_CREATE,
                     databaseTimeoutMilliseconds),
    _readConnectionPool(cachePath + "/" + tileProvider.id() + ".mbtiles",
                        SQLite::SQLiteConnection::Mode::READ_ONLY,
                        databaseTimeoutMilliseconds,
                        capacity,
                        peakCapacity)
{
    try
    {
        SQLite::Transaction transaction(_writeConnection.database());
        _writeConnection.database().exec(MBTilesConnection::MBTILES_SCHEMA);
        transaction.commit();

        _writeConnection.database().exec("PRAGMA journal_mode=WAL");

    }
    catch (const std::exception& e)
    {
        ofLogError("MBTilesConnection::MBTilesConnection()") << "Error initializing database - SQLite exception: " << e.what();
    }

    try
    {
        MBTilesMetadata metadata;
        metadata.set(MBTilesMetadata::KEY_MIN_ZOOM, std::to_string(tileProvider.minZoom()));
        metadata.set(MBTilesMetadata::KEY_MAX_ZOOM, std::to_string(tileProvider.maxZoom()));
        metadata.set(MBTilesMetadata::KEY_BOUNDS, tileProvider.bounds().toString());
        metadata.set(MBTilesMetadata::KEY_CENTER, tileProvider.center().toString());
        metadata.set(MBTilesMetadata::KEY_NAME, tileProvider.name());
        metadata.set(MBTilesMetadata::KEY_ATTRIBUTION, tileProvider.attribution());

        auto dictionary = tileProvider.dictionary();

        auto dictAdd = [&](const std::string& key)
        {
            if (dictionary.find(key) != dictionary.end())
                metadata.set(key, dictionary.find(key)->second);
        };

        dictAdd(MBTilesMetadata::KEY_TYPE);
        dictAdd(MBTilesMetadata::KEY_VERSION);
        dictAdd(MBTilesMetadata::KEY_FORMAT);

        _writeConnection.setMetaData(metadata);
    }
    catch (const std::exception& e)
    {
        ofLogError("MBTilesConnection::MBTilesConnection()") << "Error setting metadata - SQLite exception: " << e.what();
    }

    _writeThread = std::thread([&]() {

        std::pair<TileKey, std::shared_ptr<ofBuffer>> value;
        while (_writeChannel.receive(value))
        {
            try
            {
                if (!_writeConnection.has(value.first))
                {
                    _writeConnection.setTile(value.first, *value.second);
                }
            }
            catch (const std::exception& e)
            {
                ofLogError("MBTilesConnection::MBTilesConnection()") << "Error setting tile - SQLite exception: " << e.what();
            }
        }
    });
}


MBTilesCache::~MBTilesCache()
{
    _writeChannel.close();
    _writeThread.join();
}


const MBTilesCache::MBTilesConnectionPool& MBTilesCache::readConnectionPool() const
{
    return _readConnectionPool;
}


bool MBTilesCache::doHas(const TileKey& key) const
{
    auto connection = _readConnectionPool.borrowObject();
    auto result = connection->has(key);
    _readConnectionPool.returnObject(connection);
    return result;
}


std::shared_ptr<ofBuffer> MBTilesCache::doGet(const TileKey& key)
{
    auto connection = _readConnectionPool.borrowObject();
    auto result = connection->getBuffer(key);
    _readConnectionPool.returnObject(connection);
    return result;
}


void MBTilesCache::doAdd(const TileKey& key, std::shared_ptr<ofBuffer> entry)
{
    _writeChannel.send(std::make_pair(key, entry));
}


void MBTilesCache::doRemove(const TileKey& key)
{
    ofLogError("MBTilesCache::doRemove") << "Not yet implemented.";
}


std::size_t MBTilesCache::doSize()
{
    auto connection = _readConnectionPool.borrowObject();
    auto result = connection->size();
    _readConnectionPool.returnObject(connection);
    return result;
}


void MBTilesCache::doClear()
{
    ofLogError("MBTilesCache::doClear") << "Not yet implemented.";
}


} } // namespace ofx::Maps
