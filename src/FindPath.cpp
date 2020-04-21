#include <iostream>
#include <stack>
#include <queue>
#include <assert.h>

using namespace std;

/*****************************************************************************/

/* Used for offline debug, disable when use */
#define _ANTONY_LOCAL_DEBUG_
/* Use test map instead of large map */
//#define _USE_TEST_MAP_

/*****************************************************************************/

#ifdef _ANTONY_LOCAL_DEBUG_

struct XYPosition {
	int x, y;
	XYPosition(int x, int y) : x(x), y(y) {}
};

#endif // _ANTONY_LOCAL_DEBUG_

struct Path {
	/* u : up, d : down, l : left, r : right  */
	stack<char> move_list;
	int path_length;
};

/*****************************************************************************/

/* Map-related data are stored below */

#ifdef _USE_TEST_MAP_

/* The height of the map */
const int MAPHEIGHT = 10;

/* The width of the map */
const int MAPWIDTH = 10;

/* Map used in local debug */
/* x-axis: vertical y-axis: horizontal */
bool GameMap[MAPHEIGHT][MAPWIDTH] = {
   /* 0  1  2  3  4  5  6  7  8  9         */
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, /* 0 */
	{ 1, 0, 0, 0, 0, 1, 1, 0, 0, 1 }, /* 1 */
	{ 1, 0, 1, 1, 1, 0, 0, 0, 1, 1 }, /* 2 */
	{ 1, 0, 0, 0, 1, 0, 0, 0, 1, 1 }, /* 3 */
	{ 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 }, /* 4 */
	{ 1, 1, 0, 1, 1, 0, 1, 1, 1, 1 }, /* 5 */
	{ 0, 1, 0, 1, 0, 0, 1, 0, 1, 1 }, /* 6 */
	{ 0, 1, 0, 1, 1, 1, 1, 0, 1, 1 }, /* 7 */
	{ 1, 1, 1, 1, 1, 1, 1, 0, 1, 1 }, /* 8 */
	{ 1, 0, 1, 1, 1, 1, 0, 0, 0, 1 }  /* 9 */
}; 

#endif // _USE_TEST_MAP_

#ifndef _USE_TEST_MAP_

/* The height of the map */
const int MAPHEIGHT = 50;

/* The width of the map */
const int MAPWIDTH = 50;

/* Map used in real game */
/* x-axis: vertical y-axis: horizontal */
int GameMap[MAPHEIGHT][MAPWIDTH] = {
/*   00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49        */
	{5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}, /* 00 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 01 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 02 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 03 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 04 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 05 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 06 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 5}, /* 07 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 08 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 09 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 10 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 11 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 12 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 13 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 14 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 5, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 15 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 16 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 17 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5}, /* 18 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5}, /* 19 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5}, /* 20 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5}, /* 21 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5}, /* 22 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5}, /* 23 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0, 0, 0, 6, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5}, /* 24 */
	{5, 0, 0, 0, 0, 2, 0, 0, 0, 5, 5, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5}, /* 25 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 26 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 27 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 28 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 29 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 30 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 31 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 32 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 33 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 34 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 35 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 36 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 37 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 38 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 39 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 40 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 41 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 42 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 43 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 44 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 45 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 46 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 47 */
	{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 48 */
	{5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}  /* 49 */
	/*    00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49   */
};

#endif // _USE_TEST_MAP_

/* Position around each cooker */
const XYPosition CookerAdjacentPosition[9]{ XYPosition(7, 24), XYPosition(8, 25),
XYPosition(24, 38), XYPosition(25, 37), XYPosition(25, 39), XYPosition(33, 17),
XYPosition(33, 19), XYPosition(41, 27), XYPosition(40, 28), };

/* Position of each cooker */
const XYPosition CookerPosition[4]{ XYPosition(8, 24), XYPosition(25, 38),
XYPosition(18, 33), XYPosition(28, 41) };

/* Position around each food point */
const XYPosition FoodPointAdjacentPosition[12]{ XYPosition(7, 40), XYPosition(7, 42),
XYPosition(6, 41), XYPosition(8, 41), XYPosition(24, 5), XYPosition(26, 5),
XYPosition(25, 4), XYPosition(25, 6), XYPosition(41, 40), XYPosition(43, 40),
XYPosition(42, 39), XYPosition(42, 41) };

/* Position of each foodpoint */
const XYPosition FoodPointPosition[3]{ XYPosition(7, 41), XYPosition(25, 5), XYPosition(42, 40) };

/*****************************************************************************/

/* Algorithms provided in FindPath.cpp */

/* Get distance square */
int GetSquareDistance(XYPosition from, XYPosition to) {
	from.x = int(from.x); from.y = int(from.y);
	to.x = int(to.x); to.y = int(to.y);
	return (from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y);
}

/* Find path using BFS */
Path BFSFindPath(XYPosition start, XYPosition end) {
	/* Truncate data */
	start.x = int(start.x); start.y = int(start.y);
	end.x = int(end.x); end.y = int(end.y);
	/* Start and end must be walkable */
	assert(GameMap[start.x][start.y] == 0 && GameMap[end.x][end.y] == 0);
	assert(start.x != end.x || start.y != end.y);
	/* Initialize */
	bool has_visited[MAPHEIGHT][MAPWIDTH];
	memset(has_visited, 0, sizeof(has_visited));
	char last_move_map[MAPHEIGHT][MAPWIDTH];
	memset(last_move_map, 0, sizeof(last_move_map));
	queue<XYPosition> query_list;
	/* BFS */
	query_list.push(start); has_visited[start.x][start.y] = true;
	while (query_list.size() != 0) {
		/* Get query */
		XYPosition cur_query = query_list.front();
		query_list.pop();
		/* Check if it's our destination */
		if (cur_query.x == end.x && cur_query.y == end.y) break;
		/* Expand */
		if (cur_query.x + 1 < MAPHEIGHT && !has_visited[cur_query.x + 1][cur_query.y]
			&& !GameMap[cur_query.x + 1][cur_query.y]) {
			last_move_map[cur_query.x + 1][cur_query.y] = 'd';
			has_visited[cur_query.x + 1][cur_query.y] = true;
			query_list.push(XYPosition(cur_query.x + 1, cur_query.y));
		}
		if (cur_query.x - 1 >= 0 && !has_visited[cur_query.x - 1][cur_query.y]
			&& !GameMap[cur_query.x - 1][cur_query.y]) {
			last_move_map[cur_query.x - 1][cur_query.y] = 'u';
			has_visited[cur_query.x - 1][cur_query.y] = true;
			query_list.push(XYPosition(cur_query.x - 1, cur_query.y));
		}
		if (cur_query.y + 1 < MAPWIDTH && !has_visited[cur_query.x][cur_query.y + 1]
			&& !GameMap[cur_query.x][cur_query.y + 1]) {
			last_move_map[cur_query.x][cur_query.y + 1] = 'r';
			has_visited[cur_query.x][cur_query.y + 1] = true;
			query_list.push(XYPosition(cur_query.x, cur_query.y + 1));
		}
		if (cur_query.y - 1 >= 0 && !has_visited[cur_query.x][cur_query.y - 1]
			&& !GameMap[cur_query.x][cur_query.y - 1]) {
			last_move_map[cur_query.x][cur_query.y - 1] = 'l';
			has_visited[cur_query.x][cur_query.y - 1] = true;
			query_list.push(XYPosition(cur_query.x, cur_query.y - 1));
		}
	}
	/* Build path */
	Path bfs_path; bfs_path.path_length = 0;
	XYPosition pointer_pos(end);
	while (true) {
		bfs_path.move_list.push(last_move_map[pointer_pos.x][pointer_pos.y]);
		bfs_path.path_length++;
		switch (last_move_map[pointer_pos.x][pointer_pos.y]) {
		case 'u': pointer_pos.x++; break;
		case 'd': pointer_pos.x--; break;
		case 'r': pointer_pos.y--; break;
		case 'l': pointer_pos.y++; break;
		default: assert(false);
		}
		if (pointer_pos.x == start.x && pointer_pos.y == start.y) break;
	}
	return bfs_path;
}

/* Find the nearest cooker */
Path FindPath2NearestCooker(XYPosition player_position,
							char& relative_position) {
	/* Truncate data */
	player_position.x = int(player_position.x);
	player_position.y = int(player_position.y);
	/* Initialize */
	Path optimal_path; XYPosition destination(-1, -1);
	int min_path_length = 1000;
	/* Find path to a point adjacent to a cooker i.e. with distance 1 */
	for (XYPosition cur_destination_pos : CookerAdjacentPosition) {
		Path cur_path = BFSFindPath(player_position, cur_destination_pos);
		if (cur_path.path_length < min_path_length) {
			optimal_path = cur_path; destination = cur_destination_pos;
			min_path_length = cur_path.path_length;
		}
	}
	/* Check cooker position relative to the point */
	assert(destination.x != -1 && destination.y != -1);
	for (XYPosition cooker_pos : CookerPosition) {
		if (GetSquareDistance(cooker_pos, destination) == 1) {
			int relative_x = cooker_pos.x - destination.x;
			int relative_y = cooker_pos.y - destination.y;
			if (relative_x == -1 && relative_y == 0) {
				relative_position = 'u';
			} else if (relative_x == 1 && relative_y == 0) {
				relative_position = 'd';
			} else if (relative_x == 0 && relative_y == -1) {
				relative_position = 'l';
			} else if (relative_x == 0 && relative_y == 1) {
				relative_position = 'r';
			} else { assert(false); }
			break;
		}
	}
	return optimal_path;
}

/* Find the nearest food point */
Path FindPath2NearestFoodPoint(XYPosition player_position,
							   char& relative_position) {
	/* Truncate data */
	player_position.x = int(player_position.x);
	player_position.y = int(player_position.y);
	/* Initialize */
	Path optimal_path; XYPosition destination(-1, -1);
	int min_path_length = 1000;
	/* Find path to a point adjacent to a food point i.e. with distance 1 */
	for (XYPosition cur_destination_pos : FoodPointAdjacentPosition) {
		Path cur_path = BFSFindPath(player_position, cur_destination_pos);
		if (cur_path.path_length < min_path_length) {
			optimal_path = cur_path; destination = cur_destination_pos;
			min_path_length = cur_path.path_length;
		}
	}
	/* Check food point position relative to the point */
	assert(destination.x != -1 && destination.y != -1);
	for (XYPosition foodpoint_pos : FoodPointPosition) {
		if (GetSquareDistance(foodpoint_pos, destination) == 1) {
			int relative_x = foodpoint_pos.x - destination.x;
			int relative_y = foodpoint_pos.y - destination.y;
			if (relative_x == -1 && relative_y == 0) {
				relative_position = 'u';
			} else if (relative_x == 1 && relative_y == 0) {
				relative_position = 'd';
			} else if (relative_x == 0 && relative_y == -1) {
				relative_position = 'l';
			} else if (relative_x == 0 && relative_y == 1) {
				relative_position = 'r';
			} else { assert(false); }
			break;
		}
	}
	return optimal_path;
}


/*****************************************************************************/

#ifdef _ANTONY_LOCAL_DEBUG_

int main() {
	Path move1 = BFSFindPath(XYPosition(1, 1), XYPosition(46, 37));
	char relative_pos;
	Path move2 = FindPath2NearestCooker(XYPosition(1, 1), relative_pos);
}

#endif // _ANTONY_LOCAL_DEBUG_

/*************************************END*************************************/