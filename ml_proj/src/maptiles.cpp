/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    int row = theSource.getRows();
    if (row == 0) return NULL;
    int col = theSource.getColumns();
    if (col == 0) return NULL;
    auto* mosaic = new MosaicCanvas(row, col);
    std::map<Point<3>, int> tile_map;
    std::vector<Point<3>> newPoints;
    for (int i = 0; i < (int) theTiles.size(); ++i) {
        LUVAPixel average_color_pixel = theTiles[i].getAverageColor();
        Point<3> position = convertToXYZ(average_color_pixel);
        tile_map[position] = i;
        newPoints.push_back(position);
    }
    auto* kd_tree = new KDTree<3>(newPoints);
    kd_tree->printTree(cout);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            LUVAPixel region_color = theSource.getRegionColor(i, j);
            Point<3> query = convertToXYZ(region_color);
            Point<3> closest = kd_tree->findNearestNeighbor(query);
            mosaic->setTile(i, j, &theTiles[tile_map[closest]]);
        }
    }
    return mosaic;
}

