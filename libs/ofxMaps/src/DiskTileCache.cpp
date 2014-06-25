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


#include "ofx/Maps/DiskTileCache.h"
#include "ofx/IO/ByteBufferUtils.h"
#include "Poco/NumberFormatter.h"


namespace ofx {
namespace Maps {


const ofImageFormat DiskTileCache::DEFAULT_IMAGE_FORMAT = OF_IMAGE_FORMAT_PNG;
const ofImageQualityType DiskTileCache::DEFAULT_IMAGE_QUALITY = OF_IMAGE_QUALITY_BEST;
const std::string DiskTileCache::DEFAULT_IMAGE_EXTENSION = "png";


DiskTileCache::DiskTileCache(const Poco::Path& cachePath,
                             ofImageFormat imageFormat,
                             const std::string& imageExtension):
    _cachePath(cachePath),
    _imageFormat(imageFormat),
    _imageExtension(imageExtension)
{
}


DiskTileCache::~DiskTileCache()
{
}


SharedTile DiskTileCache::getTile(const TileCoordinate& coordinate)
{
    SharedTile tile;

    Poco::Path path = getTilePath(coordinate);

    Poco::File file(getTilePath(coordinate));

    if (file.exists())
    {
        tile = SharedTile(new ofImage());
        tile->setUseTexture(false);
        tile->loadImage(path.toString());
        return tile;
    }
    else
    {
        return tile;
    }
}


bool DiskTileCache::putTile(const TileCoordinate& coordinate,
                            const IO::ByteBuffer& tile)
{
    return IO::ByteBufferUtils::saveToFile(tile,
                                           getTilePath(coordinate).toString());
}


bool DiskTileCache::putTile(const TileCoordinate& coordinate,
                            const SharedTile& tile)
{
    Poco::Path path = getTilePath(coordinate);
    ofSaveImage(tile->getPixelsRef(), path.toString(), _imageQuality);
    return true;
}


void DiskTileCache::clear()
{
    Poco::File file(_cachePath);

    if (file.exists())
    {
        file.remove(true);
    }
    else
    {
        ofLogError("MapTileDiskCache::clear") << "Cache " << _cachePath.toString() << " did not exist.";
    }
}


ofImageFormat DiskTileCache::getImageFormat() const
{
    return _imageFormat;
}


const std::string& DiskTileCache::getImageExtension() const
{
    return _imageExtension;
}


Poco::Path DiskTileCache::getTilePath(const TileCoordinate& coordinate) const
{
    Poco::Path path(_cachePath);

    path = path.pushDirectory(coordinate.getId());
    path = path.pushDirectory(Poco::NumberFormatter::format(coordinate.getZoom()));
    path = path.pushDirectory(Poco::NumberFormatter::format(coordinate.getColumn()));
    path.setFileName(Poco::NumberFormatter::format(coordinate.getRow()) + "." + _imageExtension);

    return path;
}


} } // namespace ofx::Maps
