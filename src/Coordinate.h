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


#include "Point2d.h"


class Coordinate {
	
public:

	Coordinate():
        row(0),
        column(0),
        zoom(0)
    {
    }

	Coordinate(double _row,
               double _column,
               double _zoom):
        row(_row),
        column(_column),
        zoom(_zoom)
    {
    }
	
	Coordinate(const Coordinate &coord):
        row(coord.row),
        column(coord.column),
        zoom(coord.zoom)
    {
    }
	
	friend std::ostream &operator << (std::ostream &stream, const Coordinate& c) {
		// TODO: number format
		stream << "(" << c.row << ", " << c.column << " @" << c.zoom << ")";
		return stream;
	}
	
    bool operator == (const Coordinate &c) const
    {
        return c.row == row
            && c.column == column
            && c.zoom == zoom;
    }

	bool operator < (const Coordinate &c) const
    {
        return zoom < c.zoom
            || (zoom == c.zoom && row < c.row)
            || (zoom == c.zoom && row == c.row && column < c.column);
    }
	
	Coordinate container() const
    {
		return Coordinate(floor(row), floor(column), zoom);
	}
	
	Coordinate zoomTo(double destination) const
    {
		return Coordinate(row * pow(2, destination - zoom),
						  column * pow(2, destination - zoom),
						  destination);
	}
	
	Coordinate zoomBy(double distance) const {
		return Coordinate(row * pow(2, distance),
						  column * pow(2, distance),
						  zoom + distance);
	}
	
	Coordinate up(double distance = 1) const
    {
		return Coordinate(row - distance, column, zoom);
	}
	
	Coordinate right(double distance = 1) const
    {
		return Coordinate(row, column + distance, zoom);
	}
	
	Coordinate down(double distance = 1) const
    {
		return Coordinate(row + distance, column, zoom);
	}
	
	Coordinate left(double distance = 1) const
    {
		return Coordinate(row, column - distance, zoom);
	}


	double row;
	double column;
	double zoom;

};
