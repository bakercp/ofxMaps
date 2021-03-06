//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofPixels.h"
#include "ofTexture.h"


namespace ofx {
namespace Maps {


/// \brief A simple class representing an image tile.
class Tile: public ofBaseDraws
{
public:
    enum class Type
    {
        /// \brief Indicates an empty tile, for instance returned if a zoom level is not available.
        EMPTY,
        /// \brief A standard ofPixels-based raster image.
        RASTER
    };

    /// \brief Create an empty un-allocated tile.
    Tile();

    /// \brief Create a tile with the given pixels.
    /// \param pixels The pixels to set.
    Tile(const ofPixels& pixels);

    /// \brief Destroy the Tile.
    virtual ~Tile();

    using ofBaseDraws::draw;
    void draw(float x, float y, float width, float height) const override;
    float getWidth() const override;
    float getHeight() const override;

    /// \returns true if this is an empty un-allocated tile.
    bool empty() const;

    /// \returns a const reference to the pixels.
    const ofPixels& pixels() const;

    /// \returns a const reference to the texture.
    const ofTexture& texture() const;

    /// \returns the Tile type.
    Type type() const;

    /// \returns true if the texture is uploaded.
    bool hasTexture() const;

    /// \brief Upload the pixels to a texture if needed.
    void loadTexture();

    /// \brief Clear the texture memory, but retain the pixels.
    void clearTexture();

private:
    /// \brief The Tile Type.
    Type _type = Type::EMPTY;

    /// \brief The pixels.
    ofPixels _pixels;

    /// \brief The texture.
    ofTexture _texture;

};


} } // namespace ofx::Maps
