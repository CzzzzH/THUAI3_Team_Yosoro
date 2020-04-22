#include "API.h"
#include "Constant.h"
#include "player.h"
#include <iostream>
#include "OS_related.h"
#include <deque>
#include <vector>
#include <stack>
#include <assert.h>
#include <queue>

#define PLYAER 0 // 0为正常策略，1为强盗策略
#define PI 3.141592653589793238462643383279

using namespace THUAI3;
Protobuf::Talent initTalent = Protobuf::Talent::Cook;//指定人物天赋。选手代码必须定义此变量，否则报错

/* ------------------------------------- FindPath.cpp ------------------------------------- */

/*****************************************************************************/

/* Used for offline debug, disable when use */
//#define _ANTONY_LOCAL_DEBUG_
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
	/*		 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 */
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
		/*   00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49  */ 
};

#endif // _USE_TEST_MAP_

struct XYIPosition
{
	int x;
	int y;
	XYIPosition(double x, double y) : x(int(x)), y(int(y))
	{ }
	XYIPosition(const XYIPosition& xy) : x(xy.x), y(xy.y)
	{ }
	XYIPosition(const XYPosition& xy) : x(int(xy.x)), y(int(xy.y))
	{ }
	bool operator== (const XYIPosition& t)
	{
		return (this->x == t.x && this->y == t.y);
	}
};

/* Position around each cooker */
const XYIPosition CookerAdjacentPosition[9]{ XYIPosition(7, 24), XYIPosition(8, 25),
XYIPosition(24, 38), XYIPosition(25, 37), XYIPosition(25, 39), XYIPosition(33, 17),
XYIPosition(33, 19), XYIPosition(41, 27), XYIPosition(40, 28) };

/* Position of each cooker */
const XYIPosition CookerPosition[4]{ XYIPosition(8, 24), XYIPosition(25, 38),
XYIPosition(33, 18), XYIPosition(41, 28) };

/* Position around each food point */
const XYIPosition FoodPointAdjacentPosition[12]{ XYIPosition(7, 40), XYIPosition(7, 42),
XYIPosition(6, 41), XYIPosition(8, 41), XYIPosition(24, 5), XYIPosition(26, 5),
XYIPosition(25, 4), XYIPosition(25, 6), XYIPosition(41, 40), XYIPosition(43, 40),
XYIPosition(42, 39), XYIPosition(42, 41) };

/* Position of each foodpoint */
const XYIPosition FoodPointPosition[3]{ XYIPosition(7, 41), XYIPosition(25, 5), XYIPosition(42, 40) };

/* Position of each mission point */
const XYIPosition MissionPointPosition[4]{ XYIPosition(24, 24), XYIPosition(24, 25), XYIPosition(25, 24), XYIPosition(25, 25) };

/* Position around each mission point */
const XYIPosition MissionPointAdjacentPosition[8]{ XYIPosition(23, 24), XYIPosition(23, 25), 
XYIPosition(24, 23), XYIPosition(25, 23), XYIPosition(26, 24), XYIPosition(26, 25),
XYIPosition(24, 26), XYIPosition(25, 26)};

/*****************************************************************************/

/* Algorithms provided in FindPath.cpp */

/* Get distance square */
int GetSquareDistance(XYIPosition from, XYIPosition to) {
	from.x = int(from.x); from.y = int(from.y);
	to.x = int(to.x); to.y = int(to.y);
	return (from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y);
}

/* Find path using BFS */
Path BFSFindPath(XYIPosition start, XYIPosition end) {
	
	/* Start and end must be walkable */
	auto tmp = GameMap[end.x][end.y];
	stack<char> null_stack;
	null_stack.push('u');
	GameMap[end.x][end.y] = 0;
	if (start.x == end.x && start.y == end.y) return { null_stack, 0};
	/* Initialize */
	bool has_visited[MAPHEIGHT][MAPWIDTH];
	memset(has_visited, 0, sizeof(has_visited));
	char last_move_map[MAPHEIGHT][MAPWIDTH];
	memset(last_move_map, 0, sizeof(last_move_map));
	queue<XYIPosition> query_list;
	/* BFS */
	query_list.push(start); has_visited[start.x][start.y] = true;
	while (query_list.size() != 0) {
		/* Get query */
		XYIPosition cur_query = query_list.front();
		query_list.pop();
		/* Check if it's our destination */
		if (cur_query.x == end.x && cur_query.y == end.y) break;
		/* Expand */
		if (cur_query.x + 1 < MAPHEIGHT && !has_visited[cur_query.x + 1][cur_query.y]
			&& !GameMap[cur_query.x + 1][cur_query.y]) {
			last_move_map[cur_query.x + 1][cur_query.y] = 'd';
			has_visited[cur_query.x + 1][cur_query.y] = true;
			query_list.push(XYIPosition(cur_query.x + 1, cur_query.y));
		}
		if (cur_query.x - 1 >= 0 && !has_visited[cur_query.x - 1][cur_query.y]
			&& !GameMap[cur_query.x - 1][cur_query.y]) {
			last_move_map[cur_query.x - 1][cur_query.y] = 'u';
			has_visited[cur_query.x - 1][cur_query.y] = true;
			query_list.push(XYIPosition(cur_query.x - 1, cur_query.y));
		}
		if (cur_query.y + 1 < MAPWIDTH && !has_visited[cur_query.x][cur_query.y + 1]
			&& !GameMap[cur_query.x][cur_query.y + 1]) {
			last_move_map[cur_query.x][cur_query.y + 1] = 'r';
			has_visited[cur_query.x][cur_query.y + 1] = true;
			query_list.push(XYIPosition(cur_query.x, cur_query.y + 1));
		}
		if (cur_query.y - 1 >= 0 && !has_visited[cur_query.x][cur_query.y - 1]
			&& !GameMap[cur_query.x][cur_query.y - 1]) {
			last_move_map[cur_query.x][cur_query.y - 1] = 'l';
			has_visited[cur_query.x][cur_query.y - 1] = true;
			query_list.push(XYIPosition(cur_query.x, cur_query.y - 1));
		}
	}
	/* Build path */
	Path bfs_path; bfs_path.path_length = 0;
	XYIPosition pointer_pos(end);
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
	GameMap[end.x][end.y] = tmp;
	return bfs_path;
}

/* Find the nearest cooker */
Path FindPath2NearestCooker(XYPosition player_position,
	char& relative_position) {
	/* Truncate data */
	player_position.x = int(player_position.x);
	player_position.y = int(player_position.y);
	/* Initialize */
	Path optimal_path; XYIPosition destination(-1, -1);
	int min_path_length = 1000;
	/* Find path to a point adjacent to a cooker i.e. with distance 1 */
	for (XYIPosition cur_destination_pos : CookerAdjacentPosition) {
		Path cur_path = BFSFindPath(player_position, cur_destination_pos);
		if (cur_path.path_length < min_path_length) {
			optimal_path = cur_path; destination = cur_destination_pos;
			min_path_length = cur_path.path_length;
		}
	}
	/* Check cooker position relative to the point */
	assert(destination.x != -1 && destination.y != -1);
	for (XYIPosition cooker_pos : CookerPosition) {
		if (GetSquareDistance(cooker_pos, destination) == 1) {
			int relative_x = cooker_pos.x - destination.x;
			int relative_y = cooker_pos.y - destination.y;
			if (relative_x == -1 && relative_y == 0) {
				relative_position = 'u';
			}
			else if (relative_x == 1 && relative_y == 0) {
				relative_position = 'd';
			}
			else if (relative_x == 0 && relative_y == -1) {
				relative_position = 'l';
			}
			else if (relative_x == 0 && relative_y == 1) {
				relative_position = 'r';
			}
			else { assert(false); }
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
	Path optimal_path; XYIPosition destination(-1, -1);
	int min_path_length = 1000;
	/* Find path to a point adjacent to a food point i.e. with distance 1 */
	for (XYIPosition cur_destination_pos : FoodPointAdjacentPosition) {
		Path cur_path = BFSFindPath(player_position, cur_destination_pos);
		if (cur_path.path_length < min_path_length) {
			optimal_path = cur_path; destination = cur_destination_pos;
			min_path_length = cur_path.path_length;
		}
	}
	/* Check food point position relative to the point */
	assert(destination.x != -1 && destination.y != -1);
	for (XYIPosition foodpoint_pos : FoodPointPosition) {
		if (GetSquareDistance(foodpoint_pos, destination) == 1) {
			int relative_x = foodpoint_pos.x - destination.x;
			int relative_y = foodpoint_pos.y - destination.y;
			if (relative_x == -1 && relative_y == 0) {
				relative_position = 'u';
			}
			else if (relative_x == 1 && relative_y == 0) {
				relative_position = 'd';
			}
			else if (relative_x == 0 && relative_y == -1) {
				relative_position = 'l';
			}
			else if (relative_x == 0 && relative_y == 1) {
				relative_position = 'r';
			}
			else { assert(false); }
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


/* ------------------------------------- FoodAnalysis.cpp ------------------------------------- */

deque< deque<DishType> > table = { //菜品合成列表，第i项为第i项菜品(或食材)所需材料
	{}, //空
	{}, //Wheat 
	{}, //Rice
	{}, //Tomato
	{}, //Egg
	{}, //Milk
	{}, //Apple
	{}, //Banana
	{}, //DarkPlum
	{}, //Hawthorn
	{}, //Strawberry //10
	{}, //Beef
	{}, //Pork
	{}, //ChickenMeat
	{}, //Potato
	{}, //Lettuce
	{}, //Agaric
	{}, //NeedleMushroom
	{}, //Cabbage
	{}, //DishSize1 //19
	{Wheat}, //Flour
	{Flour}, //Noodle 
	{Egg,Flour}, //Bread
	{Rice}, //CoockedRice
	{Tomato}, //Ketchup
	{Milk}, //Cream
	{Tomato, Egg}, //TomatoFriedEgg
	{Noodle,TomatoFriedEgg},// TomatoFriedEggNoodle
	{Beef,Noodle}, //BeefNoodle
	{Pork, Cabbage, Rice}, // OverRice
	{ChickenMeat, Potato, Rice}, // YellowPheasant
	{Lettuce, Pork}, //Barbecue
	{Potato, Ketchup}, //FrenchFries
	{DarkPlum, Hawthorn}, //PlumJuice
	{Beef, Lettuce, Bread}, //Hamburger
	{Strawberry, Cream},//StrawberryIcecream
	{ChickenMeat, Ketchup}, //PopcornChicken
	{Egg, Agaric}, //AgaricFriedEgg
	{Egg, Strawberry, Flour, Cream}, //Cake
	{Hawthorn}, //SugarCoatedHaws
	{Tomato, Apple,Banana, Strawberry}//FruitSalad
};

std::map<int, Constant::DishProperty> DishInfo =
{
	{int(Protobuf::Wheat), {0,0,0} },
	{int(Protobuf::Rice), {0,0,0} },
	{int(Protobuf::Tomato), {0,0,0} },
	{int(Protobuf::Egg), {0,0,0} },
	{int(Protobuf::Milk), {0,0,0} },
	{int(Protobuf::Apple), {0,0,0} },
	{int(Protobuf::Banana), {0,0,0} },
	{int(Protobuf::DarkPlum), {0,0,0} },
	{int(Protobuf::Hawthorn), {0,0,0} },
	{int(Protobuf::Strawberry), {0,0,0} },
	{int(Protobuf::Beef), {20,0,0} },
	{int(Protobuf::Pork), {20,0,0} },
	{int(Protobuf::ChickenMeat), {20,0,0} },
	{int(Protobuf::Potato), {20,0,0} },
	{int(Protobuf::Lettuce), {20,0,0} },
	{int(Protobuf::Agaric), {20,0,0} },
	{int(Protobuf::NeedleMushroom), {20,0,0} },
	{int(Protobuf::Cabbage), {20,0,0} },
	{int(Protobuf::Flour), {0,10000,0} },
	{int(Protobuf::Noodle), {0,10000,0} },
	{int(Protobuf::Bread), {0,10000,0} },
	{int(Protobuf::CookedRice), {0,10000,0} },
	{int(Protobuf::Ketchup), {0,10000,0} },
	{int(Protobuf::Cream), {0,10000,0} },
	{int(Protobuf::TomatoFriedEgg), {50,10000,60000} },
	{int(Protobuf::TomatoFriedEggNoodle), {100,15000,90000} },
	{int(Protobuf::BeefNoodle), {80,20000,90000} },
	{int(Protobuf::OverRice), {90,20000,90000} },
	{int(Protobuf::YellowPheasant), {100,20000,90000} },
	{int(Protobuf::Barbecue), {55,20000,90000} },
	{int(Protobuf::FrenchFries), {60,15000,90000} },
	{int(Protobuf::PlumJuice), {50,10000,90000} },
	{int(Protobuf::Hamburger), {110,20000,100000} },
	{int(Protobuf::StrawberryIcecream), {60,10000,90000} },
	{int(Protobuf::PopcornChicken), {60,15000,90000} },
	{int(Protobuf::AgaricFriedEgg), {50,15000,90000} },
	{int(Protobuf::Cake), {160,30000,120000} },
	{int(Protobuf::SugarCoatedHaws), {20,10000,60000} },
	{int(Protobuf::FruitSalad), {100,20000,120000} },
	{int(Protobuf::SpicedPot), {0,60000,300000} },
	{int(Protobuf::DarkDish), {-10,60000,30000} },
	{int(Protobuf::OverCookedDish), {-10,60000,300000} },
};


class Bag {
public:
	deque< deque<DishType> > gridient; //已知材料
	Bag() {
		gridient.resize(4, deque<DishType>{});
	}

	/*
	检查该材料是否为当前菜品所需材料
	如果找到新的需要食材，返回1
	如果找到新的不需要食材，返回0
	*/
	int is_need(DishType goal, DishType t) {
		if (goal == t) return 1;
		for (int i = 0; i < table[goal].size(); ++i) {
			if (is_need(table[goal][i], t) == 1) {
				return 1;
			}
		}
		return 0;//找到新的不需要食材
	}

	void update(DishType t, int x) { //加入新找到食材到背包中第x个灶台
		gridient[x].push_back(t);
	}

	void update_stove(int x, list<Obj> finding) {//更新灶台x上的食材
		gridient[x].clear();
		for (auto it = finding.begin(); it != finding.end(); ++it) {
			if (it->objType == Dish) gridient[x].push_back(it->dish);
		}
	}

	vector<DishType> get_what_is_need(DishType goal, int x) {//返回炉灶x还缺什么食材,如果中间食材没有，会加入中间食材及其合成所需食材
		vector<DishType> rc; //返回值
		for (int i = 0; i < table[goal].size(); ++i) {
			if (find_dish_in_bag(table[goal][i], x) == -1) {
				rc.push_back(table[goal][i]);
				vector<DishType> tmp = get_what_is_need(table[goal][i], x);
				for (int j = 0; j < tmp.size(); ++j) {
					rc.push_back(tmp[j]);
				}
			}
		}
		return rc;
	}

	bool remove(DishType t, int x) { //移除背包里第x个灶台的物品，type为t，注意：只会删除第一个t食材
		for (int i = 0; i < gridient[x].size(); ++i) {
			if (gridient[x][i] == t) {
				gridient[x].erase(gridient[x].begin() + i);
				return true;
			}
		}
		return false; //删除失败
	}

	int find_dish_in_bag(DishType t, int x) { //找到材料在背包中第x个灶台的位置(从0开始）,如果没有，返回-1
		for (int i = 0; i < gridient[x].size(); ++i) {
			if (t == gridient[x][i]) return i;
		}
		return -1;
	}

	bool is_synchronized(DishType t, int x) { //检查t是否可以被灶台x合成
		if (table[t].empty()) return false; //原料，不用合成
		for (int i = 0; i < table[t].size(); ++i) { //是否所有需要材料都在灶台x上
			if (find_dish_in_bag(table[t][i], x) == -1) {
				return false;
			}
		}
		return true;
	}

	vector<DishType> get_what_is_redundant(DishType goal, int x) {//检查灶台x上是否有对于goal来说多余的食材
		vector<DishType> rc;//返回值
		for (int i = 0; i < gridient[x].size(); ++i) {
			bool redundant = true;
			for (int j = 0; j < table[goal].size(); ++j) {
				if (table[goal][j] == gridient[x][i]) {
					redundant = false;
				}
			}
			if (redundant) {
				rc.push_back(gridient[x][i]);
			}
		}
		return rc;
	}

	DishType get_synchronized_dish(DishType goal, int x) {//检查目标菜品或其中间产品是否可以被灶台x合成，返回第一个找到的可以合成的菜品或中间产品
		if (goal < 20) return DishEmpty;
		if (is_synchronized(goal, x)) return goal; //菜品可以被灶台x合成
		for (int i = 0; i < table[goal].size(); ++i) {
			if (get_synchronized_dish(table[goal][i], x) != DishEmpty) return table[goal][i]; //中间材料可以被灶台x合成
		}
		return DishEmpty;//没有可合成菜品或中间产品
	}


};

/* ------------------------------------- mainLogic ------------------------------------- */

enum class Action {findFood, setFood, cookFood, pendMission};
Action now_action = Action::findFood;
DishType now_dish = DishEmpty;
std::vector<DishType> target_dish, redundant;
std::vector<Obj> target_food, target_tool;
std::vector<int> target_cooker, target_food_point, target_mission_point;
XYIPosition now_pos(0, 0), target_pos(0, 0);
Bag now_bag;
unsigned long long now_time = 0;
int cook_time = -1, block_time = 0;

bool obj_compare(Obj a, Obj b)
{
	auto length_a = BFSFindPath(now_pos, a.position).path_length;
	auto length_b = BFSFindPath(now_pos, b.position).path_length;
	return length_a < length_b;
}

bool food_point_compare(int a, int b)
{
	auto length_a = BFSFindPath(now_pos, FoodPointPosition[a]).path_length;
	auto length_b = BFSFindPath(now_pos, FoodPointPosition[b]).path_length;
	return length_a < length_b;
}

bool cooker_compare(int a, int b)
{
	if (now_bag.gridient[a].size() != now_bag.gridient[b].size())
		return now_bag.gridient[a].size() > now_bag.gridient[b].size();
	auto length_a = BFSFindPath(now_pos, CookerPosition[a]).path_length;
	auto length_b = BFSFindPath(now_pos, CookerPosition[b]).path_length;
    return length_a < length_b;
}

bool mission_point_compare(int a, int b)
{
	auto length_a = BFSFindPath(now_pos, MissionPointPosition[a]).path_length;
	auto length_b = BFSFindPath(now_pos, MissionPointPosition[b]).path_length;
	return length_a < length_b;
}

bool check_need(DishType target_dish)
{
    for (auto it = ::target_dish.begin(); it != ::target_dish.end(); ++it)
    {
        auto need_list = now_bag.get_what_is_need(*it, target_cooker[0]);
        if (find(need_list.begin(), need_list.end(), target_dish) != need_list.end())
            return true;
    }
    return false;
}

bool check_Mission(DishType target_dish)
{
	if (find(::target_dish.begin(), ::target_dish.end(), target_dish) != ::target_dish.end())
		return true;
	return false;
}

bool check_face()
{
	if (PlayerInfo.position.x - double(now_pos.x) < 0.45 || PlayerInfo.position.x - double(now_pos.x) > 0.55) return false;
	if (PlayerInfo.position.y - double(now_pos.y) < 0.45 || PlayerInfo.position.y - double(now_pos.y) > 0.55) return false;
	int x = target_pos.x - now_pos.x;
	int y = target_pos.y - now_pos.y;
	if (x == 1 && PlayerInfo.facingDirection == Right) return true;
	if (x == -1 && PlayerInfo.facingDirection == Left) return true;
	if (y == 1 && PlayerInfo.facingDirection == Up) return true;
	if (y == -1 && PlayerInfo.facingDirection == Down) return true;
	return false;
}

// 移动函数
void start_move(char dir)
{
	if (block_time > 50)
	{
		if (dir == 'u') dir = 'd';
		if (dir == 'd') dir = 'u';
		if (dir == 'r') dir = 'l';
		if (dir == 'l') dir = 'r';
		block_time = 0;
	}
	cout << "Move Dir: " << dir << endl;
	if ((dir == 'u' || dir == 'd'))
	{
		if (double(PlayerInfo.position.y - now_pos.y) - 0.5 > 0.1) move(Down, 50);
		else if (double(PlayerInfo.position.y - now_pos.y) - 0.5 < -0.1) move(Up, 50);
		else if (dir == 'u') move(Left, 200);
		else move(Right, 200);
		return;
	}
	if ((dir == 'l' || dir == 'r'))
	{
		if (double(PlayerInfo.position.x - now_pos.x) - 0.5 > 0.1) move(Left, 50);
		else if (double(PlayerInfo.position.x - now_pos.x) - 0.5 < -0.1) move(Right, 50);
		else if (dir == 'r') move(Up, 200);
		else move(Down, 200);
		return;
	}
}

double getAngle(bool reverse = false)
{
	double angle;
	angle = atan2(target_pos.y - now_pos.y, target_pos.x - now_pos.x);
	if (reverse) angle += PI;
	return angle;
}

// 更新地图信息以及目标列表
void update_info()
{
	target_food.clear();
	target_tool.clear();
	target_dish.clear();
	for (auto it = task_list.begin(); it != task_list.end(); ++it)
		target_dish.push_back(*it);
	std::sort(target_cooker.begin(), target_cooker.end(), cooker_compare);
	std::sort(target_food_point.begin(), target_food_point.end(), food_point_compare);
	std::sort(target_mission_point.begin(), target_mission_point.end(), mission_point_compare);
	std::sort(target_dish.begin(), target_dish.end());
    for (int x = 0; x < 50; ++x)
        for (int y = 0; y < 50; ++y)
        {
            auto obj_list = MapInfo::get_mapcell(x, y);
			bool is_cooker = false;
			// 判断当前位置是炉子的情况
			for (int i = 0; i < 4; ++i)
				if (XYIPosition(x, y) == CookerPosition[i] && !obj_list.empty())
				{
					is_cooker = true;
					now_bag.update_stove(i, obj_list);
				}
			if (is_cooker) continue;
			// 把可以拿到的食物添加到需求列表里
            for (auto it = obj_list.begin(); it != obj_list.end(); ++it)
            {   
				if (it->objType == Dish && check_need(it->dish) && it->blockType == 0)
					target_food.push_back(*it);
            }
        }
    std::sort(target_food.begin(), target_food.end(), obj_compare);
    std::sort(target_tool.begin(), target_tool.end(), obj_compare);
}

// 初始化信息
void initialize()
{
	target_cooker.resize(4);
	target_food_point.resize(3);
	target_mission_point.resize(4);
	for (int i = 0; i < target_cooker.size(); ++i)
		target_cooker[i] = i;
	for (int i = 0; i < target_food_point.size(); ++i)
		target_food_point[i] = i;
	for (int i = 0; i < target_mission_point.size(); ++i)
		target_mission_point[i] = i;
}

// 输出调试信息
void debug_info()
{
	if (!getGameTime() % 100) return;
	cout << "Now Time: " << now_time << endl;
	cout << "Now Position: " << "( " << PlayerInfo.position.x << " " << PlayerInfo.position.y << " ) " << endl;
	cout << "Target Position: " << "( " << target_pos.x << " " << target_pos.y << " ) " << endl;
	cout << "Now Action(0: findFood,  1:setFood,  2:cookFood,  3:pendMission):  " << int(now_action) << endl;
	cout << "Now Dish in Hand: " << PlayerInfo.dish << endl;
	cout << "Now Target Dish: " << now_dish << endl;
	cout << "Now Cook Time: " << cook_time << endl;
	cout << "Now Block Time: " << block_time << endl;
	cout << "Face Dir(0:Right, 2:Up  4:Left, 6:Down): " << PlayerInfo.facingDirection << endl;

	cout << "Now Bag Info: " << endl;
	for (int i = 0; i < 4; ++i)
	{
		cout << "Bag " << i << ":  ";
		for (int j = 0; j < now_bag.gridient[i].size(); ++j)
			cout << now_bag.gridient[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}

// 主循环函数
void play()
{
    bool is_act;
	initialize();
    while (true)
    {
        is_act = false;
		if (now_pos == XYIPosition(PlayerInfo.position)) block_time++;
		else block_time = 0;
        now_pos = PlayerInfo.position;
		if (cook_time > 0)
			cook_time -= getGameTime() - now_time;
		now_time = getGameTime();
        update_info();

		// 检查手上的东西是否可以提交或者已经不需要
        if (PlayerInfo.dish != DishEmpty)
        {
			if (check_Mission(PlayerInfo.dish))
				now_action = Action::pendMission;
			else if (now_action == Action::cookFood || !check_need(PlayerInfo.dish))
			{
				put(0, getAngle(), true);
				if (now_action != Action::cookFood) now_action = Action::findFood;
				is_act = true;
			}
			else now_action = Action::setFood;
        }

		// 提交任务
		if (now_action == Action::pendMission && !is_act)
		{
			if (PlayerInfo.dish == DishEmpty) now_action = Action::findFood;
			else
			{
				int arrive_distance = 5;
				for (int i = 0; i < 8; ++i)
					if (now_pos == MissionPointAdjacentPosition[i] && check_face()) arrive_distance = 1;
				if (arrive_distance > 1)
				{
					target_pos = MissionPointPosition[target_mission_point[0]];
					Path now_path = BFSFindPath(now_pos, target_pos);
					start_move(now_path.move_list.top());
				}
				else use(0, 0, 0);
				is_act = true;
			}	
		}

		// 放置食材并检查是否可以开炉
		if (now_action == Action::setFood && !is_act)
		{
			int arrive_distance = 5;
			for (int i = 0; i < 9; ++i)
				if (now_pos == CookerAdjacentPosition[i] && check_face()) arrive_distance = 1;
			if (arrive_distance > 1)
			{
				target_pos = CookerPosition[target_cooker[0]];
				Path now_path = BFSFindPath(now_pos, target_pos);
				start_move(now_path.move_list.top());
			}
			else
			{
				put(1, getAngle(), true);
				if (cook_time <= 0)
				{
					now_action = Action::cookFood;
					now_dish = (DishType)-1;
				}
				else now_action = Action::findFood;
			}
			is_act = true;
		}

		// 烹饪食材
		if (now_action == Action::cookFood && !is_act)
		{
			if (now_dish == -1)
			{
				now_dish = DishEmpty;
				pick(false, Block, 0);
				for (auto it = target_dish.begin(); it != target_dish.end(); ++it)
				{
					now_dish = now_bag.get_synchronized_dish(*it, target_cooker[0]);
					if (now_dish != DishEmpty) break;
				}
			}
			else if (now_dish == DishEmpty) now_action = Action::findFood;
			else
			{
				redundant = now_bag.get_what_is_redundant(now_dish, target_cooker[0]);
				if (redundant.empty())
				{
					int arrive_distance = 5;
					for (int i = 0; i < 9; ++i)
						if (now_pos == CookerAdjacentPosition[i] && check_face()) arrive_distance = 1;
					if (arrive_distance > 1)
					{
						target_pos = CookerPosition[target_cooker[0]];
						Path now_path = BFSFindPath(now_pos, target_pos);
						start_move(now_path.move_list.top());
					}
					else
					{
						cout << "Cook!!!" << endl;
						use(0, 0, 0);
						now_action = Action::findFood;
						cook_time = DishInfo[now_dish].CookTime;
					}
				}
				else pick(false, Dish, redundant[0]);
			}
			is_act = true;
		}

		// 寻找食材
		if (now_action == Action::findFood && !is_act)
		{
			int arrive_distance = 5;
			if (!target_food.empty()) target_pos = target_food[0].position;
			else target_pos = FoodPointPosition[target_food_point[0]];
			for (int i = 0; i < 8; ++i)
				if (now_pos == FoodPointAdjacentPosition[i] && check_face()) arrive_distance = 1;
			if (target_pos == now_pos) arrive_distance = 0;
			if (arrive_distance > 1)
			{
				Path now_path = BFSFindPath(now_pos, target_pos);
				start_move(now_path.move_list.top());
			}
			else
			{
				if (arrive_distance) pick(false, Block, 0);
				else if (!target_food.empty()) pick(true, Dish, target_food[0].dish);
			}
			is_act = true;
		}

		debug_info();
        while (getGameTime() - now_time < 50); // 等待下一帧到来
    }
}