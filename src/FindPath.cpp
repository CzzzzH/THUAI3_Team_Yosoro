#include <iostream>
#include <vector>
#include <assert.h>

#define _ANTONY_LOCAL_DEBUG_
//#define _USE_BINARY_

using namespace std;

/* Size of the map */
const int MAPSIZE = 50;

#ifdef _USE_BINARY_

/*
BLOCKTYPE has at least 32bits, each bit indicates a property.
The bits are numbered in small endian. Only the first 32 bits are considered valid.
*/
typedef unsigned int BLOCKTYPE;

/* Possible values for BLOCKTYPE */

#define PATH 0
#define OBSTACLE (1 << 0)
#define WALL (1 << 1)
#define TABLE (1 << 2)
#define FOODPOINT (1 << 3)
#define COOKER (1 << 4)
#define RUBBISHBIN (1 << 5)
#define TASKPOINT (1 << 6)

/* Functions of BLOCKTYPE */

#define ISWALKABLE(BLOCK) (bool)!((BLOCK) & 1)
#define CANFLYTHROUGH(BLOCK) (bool)((BLOCK) & TABLE | (BLOCK) & COOKER | (BLOCK) & RUBBISHBIN | !(BLOCK) & OBSTACLE)

/* 
OBJECTTYPE has at least 64bits, each bit represents a certain object.
The bits are numbered in small endian. Only the first 64 bits are considered valid.
*/
typedef unsigned long long OBJECTTYPE;

/* Possible values for OBJECTTYPE */
/* TODO */

#endif // _USE_BINARY_

#ifndef _USE_BINARY_

/* Type of blocks */
enum class BlockType {
    kPath = 0,
    kWall = (1 << 1),
    kTable = (1 << 2),
    kFoodPoint = (1 << 3),
    kCooker = (1 << 4),
    kRubbishBin = (1 << 5),
    kTaskPoint = (1 << 6)
};

bool IsBlockWalkable(BlockType block) {
    return block == BlockType::kPath;
}

bool IsBlockFlyable(BlockType block) {
    return (block == BlockType::kTable) || (block == BlockType::kCooker)
        || (block == BlockType::kRubbishBin) || (block == BlockType::kPath);
}

enum class ObjectType {
    /* TODO */
};

#endif // !_USE_BINARY_


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

#ifndef _USE_BINARY_

    /* The type of block */
    BlockType block_type;
    /* The list of objects on this block */
    vector<ObjectType> object_list;

#else

    /* The type of block */
    BLOCKTYPE block_type;
    /* The list of objects on this block */
    vector<OBJECTTYPE> object_list;

#endif // !_USE_BINARY_

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
Returns the path to the center of the nearest walkable block 
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

#ifdef _ANTONY_LOCAL_DEBUG_

int main() {

}

#endif // _ANTONY_LOCAL_DEBUG_
