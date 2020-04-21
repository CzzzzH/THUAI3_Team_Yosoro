#include <iostream>
#include <stack>
#include <queue>
#include <assert.h>

using namespace std;

/*****************************************************************************/

/* Used for offline debug, disable when use */
#define _ANTONY_LOCAL_DEBUG_

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

/* The height of the map */
const int MAPHEIGHT = 10;

/* The width of the map */
const int MAPWIDTH = 10;

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

/* Position around each cooker */
const XYPosition CookerAdjacentPosition[4]{ XYPosition(8, 3), XYPosition(8, 5), XYPosition(7, 4), XYPosition(9, 4) };

/* Position of each cooker */
const XYPosition CookerPosition[1]{ XYPosition(8, 4) };

/* Position around each food point */
const XYPosition FoodPointAdjacentPosition[4]{ XYPosition(8, 3), XYPosition(8, 5), XYPosition(7, 4), XYPosition(9, 4) };

/* Position of each foodpoint */
const XYPosition FoodPointPosition[1]{ XYPosition(8, 4) };

/*****************************************************************************/

/* Algorithms provided in FindPath.cpp */

/* Get distance square */
int GetSquareDistance(XYPosition from, XYPosition to) {
	return (from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y);
}

/* Find path using BFS */
Path BFSFindPath(const XYPosition& start, const XYPosition& end) {
	/* Start and end must be walkable */
	assert(GameMap[start.x][start.y] == 1 && GameMap[end.x][end.y] == 1);
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
			&& GameMap[cur_query.x + 1][cur_query.y]) {
			last_move_map[cur_query.x + 1][cur_query.y] = 'd';
			has_visited[cur_query.x + 1][cur_query.y] = true;
			query_list.push(XYPosition(cur_query.x + 1, cur_query.y));
		}
		if (cur_query.x - 1 >= 0 && !has_visited[cur_query.x - 1][cur_query.y]
			&& GameMap[cur_query.x - 1][cur_query.y]) {
			last_move_map[cur_query.x - 1][cur_query.y] = 'u';
			has_visited[cur_query.x - 1][cur_query.y] = true;
			query_list.push(XYPosition(cur_query.x - 1, cur_query.y));
		}
		if (cur_query.y + 1 < MAPWIDTH && !has_visited[cur_query.x][cur_query.y + 1]
			&& GameMap[cur_query.x][cur_query.y + 1]) {
			last_move_map[cur_query.x][cur_query.y + 1] = 'r';
			has_visited[cur_query.x][cur_query.y + 1] = true;
			query_list.push(XYPosition(cur_query.x, cur_query.y + 1));
		}
		if (cur_query.y - 1 >= 0 && !has_visited[cur_query.x][cur_query.y - 1]
			&& GameMap[cur_query.x][cur_query.y - 1]) {
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
Path FindPath2NearestCooker(const XYPosition& player_position, char& relative_position) {
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
Path FindPath2NearestFoodPoint(const XYPosition& player_position, char& relative_position) {
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
	/* Check cooker position relative to the point */
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
	//Path move1 = BFSFindPath(XYPosition(3, 0), XYPosition(0, 0));
	char relative_pos;
	Path move = FindPath2NearestCooker(XYPosition(3, 4), relative_pos);
}

#endif // _ANTONY_LOCAL_DEBUG_

/*************************************END*************************************/