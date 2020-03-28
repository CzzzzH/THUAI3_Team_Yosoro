#include <iostream>
#include <vector>
#include <assert.h>

#define _ANTONY_LOCAL_DEBUG_

using namespace std;

/* Size of the map */
const int MAPSIZE = 50;

/*
BLOCKTYPE has at least 32bits, each bit indicates a property. (Small Endian)

Usage: 
If the third bit needs to be set as 1, use: 1 << 2;
If multiple bits need to be set as 1, use : 1 << 2 + 1 << 4;

Function of each bit:
1: If the block is walkable, set as 0, otherwise it's 1.
2 - 15: reserved.

*/
typedef unsigned int BLOCKTYPE;

/* 
OBJECTTYPE has at least 64bits, each bit represents a certain object. (Small Endian)

Usage is the same as BlockTpye.

Function of each byte:
1 - 64: reserved.
*/
typedef unsigned long long OBJECTTYPE;

#ifdef _ANTONY_LOCAL_DEBUG_

struct XYPosition {
    double x;
    double y;
    XYPosition(double x, double y) : x(x), y(y) {}
    XYPosition(const XYPosition& xy) : x(xy.x), y(xy.y) {}
};

#endif // _ANTONY_LOCAL_DEBUG_

struct Block {
    /* Cost need to move to adjacent blocks */
    double ul_cost = 1.4, uu_cost = 1, ur_cost = 1.4;
    double ll_cost = 1, rr_cost = 1;
    double dl_cost = 1.4, dd_cost = 1, dr_cost = 1.4;
    /* The type of block */
    BLOCKTYPE block_type;
    /* The list of objects on this block */
    vector<OBJECTTYPE> object_list;
};

/*
  Column (Second dimension)
  0 1 2 
0 # # # <- Row (First dimension)
1 #
2 #
The data structure for the map. All relative 
Grid should be used as a singleton
*/
struct Grid {
    /* Map of the game */
    Block map[MAPSIZE][MAPSIZE];
};

/* 
Returns the path to the center of the nearest block 
*/
vector<double> AlignWithGrid(XYPosition player_pos) {
    return vector<double>();
    /* TODO */
}

/* 
Implemented with A* 
start and end must have integar coordinates.
*/
vector<double> FindShortestPath(XYPosition start, XYPosition end) {
    return vector<double>();
    /* TODO */
}

/* 
Implemented with B* 
start and end must have integar coordinates.
*/
vector<double> FindPathQuick(XYPosition start, XYPosition end) {
    return vector<double>();
    /* TODO */
}

/* 
Implemented with D*, currently not available 
start and end must have integar coordinates.
*/
vector<double> FindDynamicPath(XYPosition start, XYPosition end) {
    assert(false);
    return vector<double>();
}