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


#include "ofx/Maps/BaseMapProvider.h"
#include "ofx/Maps/MercatorProjection.h"


namespace ofx {
namespace Maps {


class OpenStreetMapProvider: public BaseMapProvider
{
public:

    typedef std::shared_ptr<OpenStreetMapProvider> SharedPtr;

	OpenStreetMapProvider():
		BaseMapProvider(BaseProjection::SharedPtr(new MercatorProjection()),
                        OSM_TILE_WIDTH,
                        OSM_TILE_HEIGHT,
                        OSM_MIN_ZOOM,
                        OSM_MAX_ZOOM)
    
	{
		_subdomains.push_back("");
		_subdomains.push_back("a.");
		_subdomains.push_back("b.");
		_subdomains.push_back("c.");
	}

    std::vector<std::string> getTileUrls(const TileCoordinate& rawCoordinate) const
    {
		std::vector<std::string> urls;

        if (rawCoordinate.getRow() >= 0 && rawCoordinate.getRow() < pow(2, rawCoordinate.getZoom()))
        {
			TileCoordinate coordinate = TileCoordinate::normalizeTileCoordinate(rawCoordinate);

            std::stringstream url;

			std::string subdomain = _subdomains[(int)ofRandom(0, _subdomains.size())];

			url << "http://"<< subdomain << "tile.openstreetmap.org/";
            url << (int)coordinate.getZoom() << "/" << (int)coordinate.getColumn();
            url << "/" << (int)coordinate.getRow() << ".png";

			urls.push_back(url.str());
		}
        
		return urls;
	}

    static SharedPtr makeShared()
    {
        return SharedPtr(new OpenStreetMapProvider());
    }

    enum
    {
        OSM_TILE_WIDTH = 256,
        OSM_TILE_HEIGHT = 256,
        OSM_MIN_ZOOM = 0,
        OSM_MAX_ZOOM = 19
    };


protected:
    std::vector<std::string> _subdomains;

	
};


} } // namespace ofx::Maps
