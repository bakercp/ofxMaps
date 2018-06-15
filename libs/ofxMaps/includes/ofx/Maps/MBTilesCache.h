//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "Poco/Net/NameValueCollection.h"
#include "SQLiteCpp.h"
#include "SQLiteConnection.h"
#include "SQLiteConnectionPool.h"
#include "ofThreadChannel.h"
#include "ofx/IO/ThreadChannel.h"
#include "ofx/Cache/BaseCache.h"
#include "ofx/Geo/CoordinateBounds.h"
#include "ofx/Geo/Coordinate.h"
#include "ofx/Maps/Tile.h"
#include "ofx/Maps/TileKey.h"
#include "ofx/Maps/TileCoordinate.h"
#include "ofx/Maps/MapTileProvider.h"


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


class MBTilesConnection: public SQLite::SQLiteConnection
{
public:
    using SQLite::SQLiteConnection::SQLiteConnection;

//    MBTilesConnection(const std::string& filename,
//                      Mode mode = Mode::READ_ONLY,
//                      uint64_t databaseTimeoutMilliseconds = 0,
//                      Poco::RWLock* mutex = nullptr);

    /// \brief Destroy the MBTiles.
    virtual ~MBTilesConnection();

    MBTilesMetadata getMetaData() const noexcept;

    bool setMetaData(const MBTilesMetadata& metadata) noexcept;

    bool has(const TileKey& key) const noexcept;

    std::shared_ptr<ofBuffer> getBuffer(const TileKey& key) const noexcept;

    std::shared_ptr<Tile> getTile(const TileKey& key) const noexcept;

    bool setTile(const TileKey& key,
                 const ofBuffer& image) noexcept;

    std::size_t size() const noexcept;

    static const std::string QUERY_SELECT_METADATA;
    static const std::string QUERY_INSERT_METADATA;
    static const std::string CREATE_TABLE_METADATA;
    static const std::string DROP_TABLE_METADATA;

    static const std::string QUERY_TILES;
    static const std::string QUERY_TILES_WITH_SET_ID;
    static const std::string COUNT_TILES;
    static const std::string COUNT_TILES_WITH_SET_ID;

    static const std::string INSERT_IMAGE;
    static const std::string COUNT_IMAGE;

    static const std::string INSERT_MAP;
    static const std::string INSERT_MAP_WITH_SET_ID;
    static const std::string COUNT_MAP;
    static const std::string COUNT_MAP_WITH_SET_ID;

    static const std::string COUNT_ALL;

    static const std::string MBTILES_SCHEMA;

};


class MBTilesCache: public Cache::BaseCache<TileKey, ofBuffer>
{
public:
    typedef SQLite::SQLiteConnectionPool_<MBTilesConnection> MBTilesConnectionPool;

    MBTilesCache(const MapTileProvider& tileProvider,
                 const std::string& cachePath,
                 uint64_t databaseTimeoutMilliseconds = 5000,
                 std::size_t capacity = MBTilesConnectionPool::DEFAULT_CAPACITY,
                 std::size_t peakCapacity = MBTilesConnectionPool::DEFAULT_PEAK_CAPACITY);

    virtual ~MBTilesCache();
    
    const MBTilesConnectionPool& readConnectionPool() const;

    std::string path() const
    {
        return _writeConnection->database().getFilename();
    }

    std::string to_string() const
    {
        return _readConnectionPool->toString() + " Writer: ";// std::to_string(_writeChannel.size());
    }

    std::string toString() const
    {
        return to_string();
    }

protected:
    bool doHas(const TileKey& key) const override;

    std::shared_ptr<ofBuffer> doGet(const TileKey& key) override;

    void doAdd(const TileKey& key, std::shared_ptr<ofBuffer> entry) override;

    void doRemove(const TileKey& key) override;

    std::size_t doSize() override;

    void doClear() override;

private:
    std::thread _writeThread;

    ofThreadChannel<std::pair<TileKey, std::shared_ptr<ofBuffer>>> _writeChannel;

    std::unique_ptr<MBTilesConnection> _writeConnection = nullptr;

    mutable std::unique_ptr<MBTilesConnectionPool> _readConnectionPool = nullptr;

};


//class MBTilesProvider


} } // namespace ofx::Maps
