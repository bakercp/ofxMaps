//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>


namespace ofx {
namespace Maps {


/// \brief A unique tile identifier.
class TileKey
{
public:
    /// \brief Create an empty TileKey.
    TileKey();

    /// \brief Create a TileKey by copying another.
    TileKey(const TileKey& key);

    /// \brief Create a TileKey.
    /// \param column The column to use.
    /// \param row The row to use.
    /// \param zoom The zoom level to use.
    /// \param providerId The provider id to use.
    /// \param setId The set id to use.
    /// \param tileId The tile id to use.
    TileKey(int64_t column,
            int64_t row,
            int64_t zoom,
//            const std::string& providerId,
            const std::string& setId = DEFAULT_SET_ID,
            const std::string& tileId = DEFAULT_TILE_ID);

    virtual ~TileKey();

    /// \returns the column.
    int64_t column() const;

    /// \returns the row.
    int64_t row() const;

    /// \returns the zoom level.
    int64_t zoom() const;
    
//    /// \returns the provider id.
//    std::string providerId() const;

    /// \returns the set id.
    std::string setId() const;

    /// \returns the tile id.
    std::string tileId() const;

    /// \returns a debug string representation.
    std::string toString() const;

    /// \returns a non-cryptographic hash.
    std::size_t hash() const;

    /// \brief Stream output.
    /// \param os the std::ostream.
    /// \param coordinate The TileKey to output.
    /// \returns the updated std::ostream reference.
    friend std::ostream& operator << (std::ostream& os,
                                      const TileKey& key);


    /// \brief This sorts tile keys.
    bool operator < (const TileKey& coordinate) const;

    /// \brief The default set id (default is empty).
    static const std::string DEFAULT_SET_ID;

    /// \brief The default tile id (default is empty).
    static const std::string DEFAULT_TILE_ID;

private:
    /// \brief The tile column.
    int64_t _column = 0;

    /// \brief The tile row.
    int64_t _row = 0;

    /// \brief The tile zoom level.
    int64_t _zoom = 0;

//    /// \brief This is the id of the provider that generated this tile coordinate.
//    ///
//    /// Default value is an empty string.
//    std::string _providerId;

    /// \brief This is the set id.
    ///
    /// Usually this is empty which indicates that it is part of a the default
    /// set for the given providerId.
    ///
    /// This is often set for a given set of tiled images used in street-view
    /// images, etc.
    ///
    /// Default value is an empty string.
    std::string _setId;

    /// \brief The unique tile id.
    ///
    /// This is usually used as the unique identifier for a given tile. For
    /// instance, this might be the unique ETag delivered in a cacheable web
    /// request.  This might also be used to connect loaded pixels back to
    /// the raw buffered bytes.
    ///
    /// Default value is an empty string.
    std::string _tileId;

};

    
inline std::ostream& operator<<(std::ostream& os, const TileKey& key)
{
    os << key.toString();
    return os;
}


} } // namespace ofx::Maps


namespace std {


template <> struct hash<ofx::Maps::TileKey>
{
    size_t operator()(const ofx::Maps::TileKey& key) const
    {
        return key.hash();
    }
};
    
    
} // namespace std
