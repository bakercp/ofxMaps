// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
// Copyright (c) -2014 Tom Carden <https://github.com/RandomEtc>
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


#include "Poco/File.h"
#include "Poco/Path.h"
#include "ofx/Maps/AbstractMapTypes.h"
#include "ofx/Maps/TileCoordinate.h"
#include "ofx/Maps/MapTile.h"


namespace ofx {
namespace Maps {


/// \brief A simplified BaseMapTileDiskSource.
class DiskTileCache:
    public AbstractTileSink,
    public AbstractTileSource
{
public:
    DiskTileCache(const Poco::Path& cachePath,
                  ofImageFormat imageFormat = DEFAULT_IMAGE_FORMAT,
                  const std::string& imageExtension = DEFAULT_IMAGE_EXTENSION);

    virtual ~DiskTileCache();

    SharedTile getTile(const TileCoordinate& coordinate);

    bool putTile(const TileCoordinate& coordinate, const IO::ByteBuffer& tile);
    bool putTile(const TileCoordinate& coordinate, const SharedTile& tile);

    void clear();

    ofImageFormat getImageFormat() const;
    const std::string& getImageExtension() const;

    static const ofImageFormat DEFAULT_IMAGE_FORMAT;
    static const ofImageQualityType DEFAULT_IMAGE_QUALITY;
    static const std::string DEFAULT_IMAGE_EXTENSION;

protected:
    Poco::Path getTilePath(const TileCoordinate& coordinate) const;

    Poco::Path _cachePath;

    ofImageQualityType _imageQuality;
    ofImageFormat _imageFormat;
    std::string _imageExtension;

};


} } // namespace ofx::Maps
