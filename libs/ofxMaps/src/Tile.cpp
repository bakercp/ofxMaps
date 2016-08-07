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


#include "ofx/Maps/Tile.h"


namespace ofx {
namespace Maps {


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
