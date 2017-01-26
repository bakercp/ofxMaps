//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/Maps/Tile.h"


namespace ofx {
namespace Maps {


Tile::Tile():
    _type(Type::EMPTY)
{
}


Tile::Tile(const ofPixels& pixels):
    _type(Type::RASTER),
    _pixels(pixels)
{
}


Tile::~Tile()
{
}


void Tile::draw(float x, float y, float width, float height) const
{
    _texture.draw(x, y, width, height);
}

    
float Tile::getWidth() const
{
    return _pixels.getWidth();
}


float Tile::getHeight() const
{
    return _pixels.getHeight();
}


bool Tile::empty() const
{
    return _type == Type::EMPTY;
}


const ofPixels& Tile::pixels() const
{
    return _pixels;
}


const ofTexture& Tile::texture() const
{
    return _texture;
}


Tile::Type Tile::type() const
{
    return _type;
}


bool Tile::hasTexture() const
{
    return _texture.isAllocated();
}


void Tile::loadTexture()
{
    _texture.loadData(_pixels);
}


void Tile::clearTexture()
{
    _texture.clear();
}


} } // namespace ofx::Maps
