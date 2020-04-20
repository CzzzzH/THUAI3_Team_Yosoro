#include <iostream>
#include <stack>
#include <queue>
#include <assert.h>

using namespace std;

/*****************************************************************************/

/* Used for offline debug, disable when used */
#define _ANTONY_LOCAL_DEBUG_

/*****************************************************************************/

/* The height of the map */
const int MAPHEIGHT = 10;

/* The width of the map */
const int MAPWIDTH = 10;

/*****************************************************************************/

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
		if (cur_query.x - 1 > 0 && !has_visited[cur_query.x - 1][cur_query.y]
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
		if (cur_query.y - 1 > 0 && !has_visited[cur_query.x][cur_query.y - 1]
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
		}
		if (pointer_pos.x == start.x && pointer_pos.y == start.y) break;
	}
	return bfs_path;
}

/*****************************************************************************/

#ifdef _ANTONY_LOCAL_DEBUG_

int main() {
	Path move = BFSFindPath(XYPosition(9, 9), XYPosition(2, 2));
}

#endif // _ANTONY_LOCAL_DEBUG_

/*************************************END*************************************/