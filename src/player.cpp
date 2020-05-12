/* ------------------------------------- 下一阶段的目标 ** 待完善 ** ------------------------------------- */

// 1. 动态连续空间寻路（需要根据当前地图的状态进行分析，期望得到的接口是 [ 在任一状态对任一目标点的输入，得到一个移动方向，且不会被卡 ]

// 2. 一套道具评估系统（需要对不同策略的角色结合当前状态、局势进行分析，期望得到的接口是 [ 给出视野里道具，判断是否要捡 ]

// 3. 无线电通信（主要是通知队友炉子的使用情况、自己放的陷阱的位置等基础信息）

// 4. 测试与调试

// 五一期间完成

/* ------------------------------------- END ------------------------------------- */

/* ------------------------------------- player.cpp ------------------------------------- */

#include "player.h"
#include "API.h"
#include "Constant.h"
#include "OS_related.h"
#include <assert.h>
#include <cmath>
#include <deque>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

#define PI 3.141592653589793238462643383279

using namespace THUAI3;
using namespace Protobuf;
using namespace std;

/* ------------------------------------- 类型定义 ------------------------------------- */

// 状态类型的枚举
enum class Action
{
    findFood,
    setFood,
    cookFood,
    pendMission
};

// 路径结构 **待完善**
struct Path
{
    /* u : up, d : down, l : left, r : right  */
    stack<char> move_list;
    int path_length;
};

// 物品价值
struct ObjValue
{
    Obj object;
    int value;
};

// 整数坐标结构 **待完善**
struct XYIPosition
{
    int x;
    int y;
    XYIPosition(int xx, int yy) : x(xx), y(yy) {}
    XYIPosition(double xx, double yy) : x(int(xx)), y(int(yy)) {}
    XYIPosition(const XYIPosition &pos) : x(pos.x), y(pos.y) {}
    XYIPosition(const XYPosition &pos) : x(int(pos.x)), y(int(pos.y)) {}
    bool operator==(const XYIPosition &t) { return (x == t.x && y == t.y); }
    bool operator!=(const XYIPosition &t) { return (x != t.x || y != t.y); }
    bool isAdjacent(const XYIPosition &t) { return (t.x - x) * (t.x - x) + (t.y - y) * (t.y - y) == 1; }
};

/* ------------------------------------- 重要的全局变量 ------------------------------------- */

int player = 0;                                       // 玩家编号
Talent initTalent = Protobuf::Talent::Cook; // 玩家天赋

Action now_action = Action::findFood, next_action = Action::findFood; // 当前 / 下一个状态
DishType now_dish = DishEmpty;                                        // 当前准备做的菜品（包含中间菜品）

XYIPosition now_pos(0, 0), target_pos(0, 0);                             // 当前位置、目标位置的整数坐标
ObjValue target_obj = {Obj({-1, -1}, Block), -1};                             // 目标食物、道具的集合
vector<int> target_cooker, target_food_point, target_mission_point; // 各种重要位置的*序号*集合（主要用于排序）
vector<XYIPosition> map_block;
vector<DishType> target_dish; // 任务目标含有的菜品集合（直接从task_list搬过来得到）
vector<DishType> redundant;   // 炉子上冗余的食材集合

unsigned long long now_time = 0; // 当前时间
int block_time = 0;              // 走路被阻塞时间（用于防被卡）
vector<int> cook_time, generate_time; // 菜品烹饪剩余时间和食物生成时间
vector<bool> is_fertilized, is_pot;

bool is_act;                     // 是否已经行动（避免进行不必要的冗余操作）

bool init_flag = false; // 初始化

/* 重要地点的位置信息 */
vector<XYIPosition> CookerPosition, FoodPointPosition, MissionPointPosition;
vector<XYIPosition> CookerAdjacentPosition, FoodPointAdjacentPosition, MissionPointAdjacentPosition;

/* ------------------------------------- FindPath.cpp ------------------------------------- */

/* The height of the map */
const int MAPHEIGHT = 50;

/* The width of the map */
const int MAPWIDTH = 50;

/* Map used in real game */
/* x-axis: vertical y-axis: horizontal */
int GameMap[MAPHEIGHT][MAPWIDTH] = {
    /* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 */
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}, /* 00 */
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 01 */
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 02 */
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 03 */
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 04 */
    {5, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 05 */
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
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 5}, /* 31 */
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
    {5, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 43 */
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 44 */
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 45 */
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 46 */
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 47 */
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}, /* 48 */
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}  /* 49 */
    /*   00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49  */
};

/* Get distance square */
int GetSquareDistance(XYIPosition from, XYIPosition to)
{
    from.x = int(from.x);
    from.y = int(from.y);
    to.x = int(to.x);
    to.y = int(to.y);
    return (from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y);
}

/* Find path using BFS */
Path BFSFindPath(XYIPosition start, XYIPosition end)
{

    /* Start and end must be walkable */
    auto tmp = GameMap[end.x][end.y];
    stack<char> null_stack;
    null_stack.push('u');
    GameMap[end.x][end.y] = 0;
    auto it = find(map_block.begin(), map_block.end(), XYIPosition(end.x, end.y));
    if (it != map_block.end())
        map_block.erase(it);
    if (start.x == end.x && start.y == end.y)
        return {null_stack, 0};
    /* Initialize */
    bool has_visited[MAPHEIGHT][MAPWIDTH];
    memset(has_visited, 0, sizeof(has_visited));
    char last_move_map[MAPHEIGHT][MAPWIDTH];
    memset(last_move_map, 0, sizeof(last_move_map));
    queue<XYIPosition> query_list;
    /* BFS */
    query_list.push(start);
    has_visited[start.x][start.y] = true;
    while (query_list.size() != 0)
    {
        /* Get query */
        XYIPosition cur_query = query_list.front();
        query_list.pop();
        /* Check if it's our destination */
        if (cur_query.x == end.x && cur_query.y == end.y)
            break;
        /* Expand */
        if (cur_query.x + 1 < MAPHEIGHT && !has_visited[cur_query.x + 1][cur_query.y] && !GameMap[cur_query.x + 1][cur_query.y] && find(map_block.begin(), map_block.end(), XYIPosition(cur_query.x + 1, cur_query.y)) == map_block.end())
        {
            last_move_map[cur_query.x + 1][cur_query.y] = 'd';
            has_visited[cur_query.x + 1][cur_query.y] = true;
            query_list.push(XYIPosition(cur_query.x + 1, cur_query.y));
        }
        if (cur_query.x - 1 >= 0 && !has_visited[cur_query.x - 1][cur_query.y] && !GameMap[cur_query.x - 1][cur_query.y] && find(map_block.begin(), map_block.end(), XYIPosition(cur_query.x - 1, cur_query.y)) == map_block.end())
        {
            last_move_map[cur_query.x - 1][cur_query.y] = 'u';
            has_visited[cur_query.x - 1][cur_query.y] = true;
            query_list.push(XYIPosition(cur_query.x - 1, cur_query.y));
        }
        if (cur_query.y + 1 < MAPWIDTH && !has_visited[cur_query.x][cur_query.y + 1] && !GameMap[cur_query.x][cur_query.y + 1] && find(map_block.begin(), map_block.end(), XYIPosition(cur_query.x, cur_query.y + 1)) == map_block.end())
        {
            last_move_map[cur_query.x][cur_query.y + 1] = 'r';
            has_visited[cur_query.x][cur_query.y + 1] = true;
            query_list.push(XYIPosition(cur_query.x, cur_query.y + 1));
        }
        if (cur_query.y - 1 >= 0 && !has_visited[cur_query.x][cur_query.y - 1] && !GameMap[cur_query.x][cur_query.y - 1] && find(map_block.begin(), map_block.end(), XYIPosition(cur_query.x, cur_query.y - 1)) == map_block.end())
        {
            last_move_map[cur_query.x][cur_query.y - 1] = 'l';
            has_visited[cur_query.x][cur_query.y - 1] = true;
            query_list.push(XYIPosition(cur_query.x, cur_query.y - 1));
        }
    }
    /* Build path */
    Path bfs_path;
    bfs_path.path_length = 0;
    XYIPosition pointer_pos(end);
    while (true)
    {
        bfs_path.move_list.push(last_move_map[pointer_pos.x][pointer_pos.y]);
        bfs_path.path_length++;
        switch (last_move_map[pointer_pos.x][pointer_pos.y])
        {
        case 'u':
            pointer_pos.x++;
            break;
        case 'd':
            pointer_pos.x--;
            break;
        case 'r':
            pointer_pos.y--;
            break;
        case 'l':
            pointer_pos.y++;
            break;
        default:
            assert(false);
        }
        if (pointer_pos.x == start.x && pointer_pos.y == start.y)
            break;
    }
    GameMap[end.x][end.y] = tmp;
    return bfs_path;
}

/*************************************END*************************************/

/* ------------------------------------- FoodAnalysis.cpp ------------------------------------- */

deque<deque<DishType>> table = {
    //菜品合成列表，第i项为第i项菜品(或食材)所需材料
    {},                       //空
    {},                       //Wheat
    {},                       //Rice
    {},                       //Tomato
    {},                       //Egg
    {},                       //Beef
    {},                       //Pork
    {},                       //Potato
    {},                       //Lettuce
    {Wheat},                  //Flour
    {Flour},                  //Noodle
    {Egg, Flour},             //Bread
    {Rice},                   //CoockedRice
    {Tomato},                 //Ketchup
    {Tomato, Egg},            //TomatoFriedEgg
    {Noodle, TomatoFriedEgg}, // TomatoFriedEggNoodle
    {Beef, Noodle},           //BeefNoodle
    {Pork, Potato, Rice},     // OverRice
    {Lettuce, Pork},          //Barbecue
    {Potato, Ketchup},        //FrenchFries
    {Beef, Lettuce, Bread},   //Hamburger
};

deque<int> tool_score = //10000代表马上、一定要捡的东西，包括立即使用物品、陷阱，百利而无害
    {
        0,     //没有物品
        1000,  //虎头鞋
        10000, //加速
        10000, //加力量
        1000,  //望远镜
        1000,  //调料
        1000,  //肥料
        1000,  //护心镜
        10000, //传送门
        10000, //胶水
        10000, //地雷
        10000, //陷阱
        10000, //闪光炸弹
        10000, //锤子
        10000, //弓箭
};

deque<int> one_time_use = {2, 3, 8, 9, 10, 11, 12, 13, 14}; //一次性道具

deque<deque<int>> occupation_tool = {
    {}, //空
    {
        //跑者道具分数加成
        0, //没有物品
        1, //虎头鞋
        1, //加速
        1, //加力量
        1, //望远镜
        1, //调料
        1, //肥料
        1, //护心镜
        1, //传送门
        1, //胶水
        1, //地雷
        1, //陷阱
        1, //闪光炸弹
        1, //锤子
        1, //弓箭
    },
    {
        //力量男道具分数加成
        0,  //没有物品
        1,  //虎头鞋
        1,  //加速
        1,  //加力量
        1,  //望远镜
        1,  //调料
        1,  //肥料
        1,  //护心镜
        1,  //传送门
        1,  //胶水
        1,  //地雷
        1,  //陷阱
        1,  //闪光炸弹
        10, //锤子
        10, //弓箭
    },
    {
        //厨师道具分数加成
        0,  //没有物品
        1,  //虎头鞋
        1,  //加速
        1,  //加力量
        1,  //望远镜
        10, //调料
        1,  //肥料
        1,  //护心镜
        1,  //传送门
        1,  //胶水
        1,  //地雷
        1,  //陷阱
        1,  //闪光炸弹
        1,  //锤子
        1,  //弓箭
    },
    {
        //技工道具分数加成
        0,  //没有物品
        1,  //虎头鞋
        1,  //加速
        1,  //加力量
        1,  //望远镜
        1,  //调料
        1,  //肥料
        1,  //护心镜
        10, //传送门
        1,  //胶水
        1,  //地雷
        1,  //陷阱
        1,  //闪光炸弹
        1,  //锤子
        1,  //弓箭
    },
    {
        //幸运男孩道具分数加成
        0, //没有物品
        1, //虎头鞋
        1, //加速
        1, //加力量
        1, //望远镜
        1, //调料
        1, //肥料
        1, //护心镜
        1, //传送门
        1, //胶水
        1, //地雷
        1, //陷阱
        1, //闪光炸弹
        1, //锤子
        1, //弓箭
    }};

/*
物品比较
思路：
道具优先级高于食材，一次性道具优于常驻型道具，如果返回发现是一次性道具（调用函数is_one_time_use）
周围还有其它道具，就捡起来把它用掉再捡其它道具，如果返回 < 0，代表不要捡
*/

class Bag
{
public:
    deque<deque<DishType>> gridient; //已知材料
    Bag()
    {
        gridient.resize(4, deque<DishType>{});
        cook_time.resize(4);
    }

    /*
	检查该材料是否为当前菜品所需材料
	如果找到新的需要食材，返回1
	如果找到新的不需要食材，返回0
	*/
    int is_need(DishType goal, DishType t)
    {
        if (goal == t)
            return 1;
        for (int i = 0; i < table[goal].size(); ++i)
        {
            if (is_need(table[goal][i], t) == 1)
            {
                return 1;
            }
        }
        return 0; //找到新的不需要食材
    }

    void update(DishType t, int x)
    { //加入新找到食材到背包中第x个灶台
        gridient[x].push_back(t);
    }

    void update_stove(int x, list<Obj> finding)
    { //更新灶台x上的食材
        bool flag = false;
        gridient[x].clear();
        for (auto it = finding.begin(); it != finding.end(); ++it)
        {
            if (it->objType == Tool && it->tool == Condiment)
                is_pot[x] = true;
            if (it->objType == Dish)
                gridient[x].push_back(it->dish);
            if (it->objType == CookingDish && cook_time[x] == 0)
            {
                flag = true; // 有食物正在被做
                cook_time[x] += 10000; // 由于不知道食物完成时间，预设一个10秒
            }
            else cook_time[x] = 0; // 没有食物正在被做
        }
    }

    vector<DishType> get_what_is_need(DishType goal, int x)
    {                        //返回炉灶x还缺什么食材,如果中间食材没有，会加入中间食材及其合成所需食材
        vector<DishType> rc; //返回值
        for (int i = 0; i < table[goal].size(); ++i)
        {
            if (find_dish_in_bag(table[goal][i], x) == -1)
            {
                rc.push_back(table[goal][i]);
                vector<DishType> tmp = get_what_is_need(table[goal][i], x);
                for (int j = 0; j < tmp.size(); ++j)
                {
                    rc.push_back(tmp[j]);
                }
            }
        }
        return rc;
    }

    bool remove(DishType t, int x)
    { //移除背包里第x个灶台的物品，type为t，注意：只会删除第一个t食材
        for (int i = 0; i < gridient[x].size(); ++i)
        {
            if (gridient[x][i] == t)
            {
                gridient[x].erase(gridient[x].begin() + i);
                return true;
            }
        }
        return false; //删除失败
    }

    int find_dish_in_bag(DishType t, int x)
    { //找到材料在背包中第x个灶台的位置(从0开始）,如果没有，返回-1
        for (int i = 0; i < gridient[x].size(); ++i)
        {
            if (t == gridient[x][i])
                return i;
        }
        return -1;
    }

    bool is_synchronized(DishType t, int x)
    { //检查t是否可以被灶台x合成
        if (table[t].empty())
            return false; //原料，不用合成
        for (int i = 0; i < table[t].size(); ++i)
        { //是否所有需要材料都在灶台x上
            if (find_dish_in_bag(table[t][i], x) == -1)
            {
                return false;
            }
        }
        return true;
    }

    vector<DishType> get_what_is_redundant(DishType goal, int x)
    {                        //检查灶台x上是否有对于goal来说多余的食材
        vector<DishType> rc; //返回值
        for (int i = 0; i < gridient[x].size(); ++i)
        {
            bool redundant = true;
            for (int j = 0; j < table[goal].size(); ++j)
            {
                if (table[goal][j] == gridient[x][i])
                {
                    redundant = false;
                }
            }
            if (redundant)
            {
                rc.push_back(gridient[x][i]);
            }
        }
        return rc;
    }

    DishType get_synchronized_dish(DishType goal, int x)
    { //检查目标菜品或其中间产品是否可以被灶台x合成，返回第一个找到的可以合成的菜品或中间产品
        if (goal < 20)
            return DishEmpty;
        if (is_synchronized(goal, x))
            return goal; //菜品可以被灶台x合成
        for (int i = 0; i < table[goal].size(); ++i)
        {
            if (get_synchronized_dish(table[goal][i], x) != DishEmpty)
                return table[goal][i]; //中间材料可以被灶台x合成
        }
        return DishEmpty; //没有可合成菜品或中间产品
    }

    /*
    分析食物的分数
    每个炉子中任务列表中的食物分数 * dish由多少食材合成 / 所需食材数量
    */
    int parse_food(DishType dish)
    {
        bool needed = false;
        for (int i = 0; i < target_dish.size(); ++i)
        {
            if (is_need(target_dish[i], dish))
            {
                needed = true;
            }
        }
        if (!needed)
            return -1;
        int rc = 0;
        int importance = table[dish].size() + 1;
        for (int i = 0; i < target_dish.size(); ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (is_need(target_dish[i], dish))
                {
                    int source_num = get_what_is_need(target_dish[i], j).size();
                    rc += Constant::DishInfo.at(target_dish[i]).Score * importance / source_num;
                }
            }
        }
        return rc;
    }

    /*
    分析道具重要性
    如果是一次性道具，优先捡起来把它用掉（免得被别人拿来用）
    注意身上如果是一次性道具要换就把它使用掉
    */
    int parse_tool(ToolType tool)
    {
        return tool_score[tool] + occupation_tool[PlayerInfo.talent][tool];
    }
    /*
    分析物品
    如果小于0则不捡
    */
    ObjValue parseObject(std::vector<Obj> obj_list)
    {
        deque<int> score;
        score.resize(obj_list.size(), 0);
        ObjValue best = {obj_list[0], -1};
        best.value = -1;
        int cur_food = parse_food(PlayerInfo.dish);
        int cur_tool = parse_tool(PlayerInfo.tool);
        for (int i = 0; i < obj_list.size(); ++i)
        {
            if (obj_list[i].objType == 2)
            {
                score[i] += parse_food(obj_list[i].dish);
                if (cur_food > score[i])
                    score[i] = -1;
            }
            else if (obj_list[i].objType == 3)
            {
                score[i] += parse_tool(obj_list[i].tool);
                if (cur_tool < 10000)
                { //是常备道具
                    if (score[i] < cur_tool)
                        score[i] = -1;
                }
            }
            if (score[i] > best.value)
            {
                best.object = obj_list[i];
                best.value = score[i];
            }
        }
        return best;
    }

    bool is_one_time_use(ToolType tool)
    { //是否为一次性道具
        for (int i = 0; i < one_time_use.size(); ++i)
        {
            if (tool == one_time_use[i])
                return true;
        }
        return false;
    }
};

Bag now_bag;

/* ------------------------------------- 主行动逻辑 ------------------------------------- */

// 检查是否已经可以提交任务了
bool check_Mission(DishType target_dish)
{
    if (find(::target_dish.begin(), ::target_dish.end(), target_dish) != ::target_dish.end())
        return true;
    return false;
}

// 食物产生点的比较函数 **待完善**
bool food_point_compare(int a, int b)
{
    if (generate_time[a] != generate_time[b])
        return generate_time[a] < generate_time[b];
    auto length_a = BFSFindPath(now_pos, FoodPointPosition[a]).path_length;
    auto length_b = BFSFindPath(now_pos, FoodPointPosition[b]).path_length;
    return length_a < length_b;
}

// 炉子的比较函数 **待完善**
bool cooker_compare(int a, int b)
{
    if (now_bag.gridient[a].size() != now_bag.gridient[b].size())
        return now_bag.gridient[a].size() > now_bag.gridient[b].size();
    auto length_a = BFSFindPath(now_pos, CookerPosition[a]).path_length;
    auto length_b = BFSFindPath(now_pos, CookerPosition[b]).path_length;
    return length_a < length_b;
}

// 任务提交点的比较函数
bool mission_point_compare(int a, int b)
{
    auto length_a = BFSFindPath(now_pos, MissionPointPosition[a]).path_length;
    auto length_b = BFSFindPath(now_pos, MissionPointPosition[b]).path_length;
    return length_a < length_b;
}

// 检查该原料是否被当前任务菜品需要（调用了FoodAnalysis.cpp中的函数）
bool check_need(DishType target_dish)
{
    for (auto it = ::target_dish.begin(); it != ::target_dish.end(); ++it)
    {
        auto need_list = now_bag.get_what_is_need(*it, target_cooker[0]);
        if (std::find(need_list.begin(), need_list.end(), target_dish) != need_list.end())
            return true;
    }
    return false;
}

// 检查是否满足使用use函数的条件
bool ready_to_use()
{
    int x = target_pos.x - now_pos.x;
    int y = target_pos.y - now_pos.y;
    if (x == 1 && y == 0 && PlayerInfo.facingDirection == Right)
        return true;
    if (x == -1 && y == 0 && PlayerInfo.facingDirection == Left)
        return true;
    if (y == 1 && x == 0 && PlayerInfo.facingDirection == Up)
        return true;
    if (y == -1 && x == 0 && PlayerInfo.facingDirection == Down)
        return true;
    if (x == 1 && y == 1 && PlayerInfo.facingDirection == RightUp)
        return true;
    if (x == 1 && y == -1 && PlayerInfo.facingDirection == RightDown)
        return true;
    if (x == -1 && y == 1 && PlayerInfo.facingDirection == LeftUp)
        return true;
    if (x == -1 && y == -1 && PlayerInfo.facingDirection == LeftDown)
        return true;
    return false;
}

// 获得弧度制的方位角（目前写死的是目标移动方向的角度） **待完善**
double getAngle(bool reverse = false)
{
    double angle;
    angle = atan2(target_pos.y - now_pos.y, target_pos.x - now_pos.x);
    if (reverse)
        angle += PI;
    return angle;
}

// 移动函数 **待完善**
void start_move(char dir)
{
    // 如果被卡的时间长了，直接随机动一下尝试解卡（未必有效）**待完善**
    /*
	if (block_time > 1000)
	{
		int dir = (getGameTime() % 4) * 2;
		move(Direction(dir), 50);
		return;
	}
	*/

    // 朝四个方向的移动，目前是会强制使人走在格点中心，以减少被卡的概率 **待完善**
    if ((dir == 'u' || dir == 'd'))
    {
        if (double(PlayerInfo.position.y - now_pos.y) - 0.5 > 0.2)
            move(Down, 50);
        else if (double(PlayerInfo.position.y - now_pos.y) - 0.5 < -0.2)
            move(Up, 50);
        else if (dir == 'u')
            move(Left, 50);
        else
            move(Right, 50);
        return;
    }
    if ((dir == 'l' || dir == 'r'))
    {
        if (double(PlayerInfo.position.x - now_pos.x) - 0.5 > 0.2)
            move(Left, 50);
        else if (double(PlayerInfo.position.x - now_pos.x) - 0.5 < -0.2)
            move(Right, 50);
        else if (dir == 'r')
            move(Up, 50);
        else
            move(Down, 50);
        return;
    }
}

// 更新地图信息以及目标列表
void update_info()
{
    target_obj.value = -1;
    target_dish.clear();
    map_block.clear();
    for (auto it = task_list.begin(); it != task_list.end(); ++it)
        target_dish.push_back(*it);
    vector<Obj> all_obj;
    for (int x = 0; x < 50; ++x)
        for (int y = 0; y < 50; ++y)
        {
            // 获得地图上的元素（MapInfo::get_mapcell这个函数非常重要，是一切物体信息的来源（**注意**：只能得到视野内的物体信息）
            auto obj_list = MapInfo::get_mapcell(x, y);
            bool is_cook = false;

            // 判断当前位置是炉子的情况，不把炉子上的食材添加到目标列表，仅更新背包
            for (int i = 0; i < 4; ++i)
            {
                if (XYIPosition(x, y) == CookerPosition[i] && !obj_list.empty())
                {
                    is_cook = true;
                    now_bag.update_stove(i, obj_list);
                }
                if (XYIPosition(x, y) == FoodPointPosition[i])
                {
                    if (obj_list.empty()) generate_time[i] = 15000;
                    else generate_time[i] = 0;
                }
            }
            if (is_cook) continue;

            // 更新地图block
            for (auto it = obj_list.begin(); it != obj_list.end(); ++it)
            {
                if (it->objType == People)
                    map_block.push_back(XYIPosition(x, y));
                if (it->objType == Dish || it->objType == Tool)
                    all_obj.push_back(*it);
            }
        }
    std::sort(target_cooker.begin(), target_cooker.end(), cooker_compare);
    std::sort(target_food_point.begin(), target_food_point.end(), food_point_compare);
    std::sort(target_mission_point.begin(), target_mission_point.end(), mission_point_compare);
    std::sort(target_dish.begin(), target_dish.end());
    target_obj = now_bag.parseObject(all_obj);
    // 假设东西快煮完了，就赶紧跑回去拿以免被偷或烧焦 **待完善**
    if (cook_time[target_cooker[0]] > 0 && cook_time[target_cooker[0]] < 2000 && next_action == Action::findFood)
        next_action = Action::setFood;
}

// 更新重要状态
void update_state()
{
    is_act = false;
    now_pos = PlayerInfo.position;
    if (now_pos == XYIPosition(PlayerInfo.position))
        block_time += getGameTime() - now_time;
  
    // 更新炉子计时和食物生产点计时
    for (int i = 0; i < cook_time.size(); ++i)
    {
        if (cook_time[i] > 0)
            cook_time[i] -= getGameTime() - now_time;
    }
    for (int i = 0; i < generate_time.size(); ++i)
    {
        if (generate_time[i] > 0)
            generate_time[i] -= getGameTime() - now_time;
    }
    now_time = getGameTime();

    // 更新状态
    now_action = next_action;
}

// 初始化信息
void initialize()
{
    init_flag = true;
    for (int i = 0; i < MAPHEIGHT; ++i)
        for (int j = 0; j < MAPWIDTH; ++j)
        {
            if (GameMap[i][j] == 1)
                MissionPointPosition.push_back({i, j});
            if (GameMap[i][j] == 2)
                FoodPointPosition.push_back({i, j});
            if (GameMap[i][j] == 3)
                CookerPosition.push_back({i, j});
        }

    for (int i = 0; i < MAPHEIGHT; ++i)
        for (int j = 0; j < MAPWIDTH; ++j)
        {
            if (GameMap[i][j] != 0)
                continue;
            for (int k = 0; k < FoodPointPosition.size(); ++k)
                if (FoodPointPosition[k].isAdjacent({i, j}))
                    FoodPointAdjacentPosition.push_back({i, j});
            for (int k = 0; k < CookerPosition.size(); ++k)
                if (CookerPosition[k].isAdjacent({i, j}))
                    CookerAdjacentPosition.push_back({i, j});
            for (int k = 0; k < MissionPointPosition.size(); ++k)
                if (MissionPointPosition[k].isAdjacent({i, j}))
                    MissionPointAdjacentPosition.push_back({i, j});
        }

    target_cooker.resize(CookerPosition.size());
    target_food_point.resize(FoodPointPosition.size());
    target_mission_point.resize(MissionPointPosition.size());
    for (int i = 0; i < target_cooker.size(); ++i)
        target_cooker[i] = i;
    for (int i = 0; i < target_food_point.size(); ++i)
        target_food_point[i] = i;
    for (int i = 0; i < target_mission_point.size(); ++i)
        target_mission_point[i] = i;

    cook_time.resize(CookerPosition.size());
    generate_time.resize(FoodPointPosition.size());
    is_fertilized.resize(FoodPointPosition.size());
}

// 输出调试信息
void debug_info()
{
    // 不要过频繁的输出消息，以方便调试
    if (!(getGameTime() % 100))
        return;
    cout << "Now Time: " << now_time << endl;
    cout << "Now Position: "
         << "( " << PlayerInfo.position.x << " " << PlayerInfo.position.y << " ) " << endl;
    cout << "Target Position: "
         << "( " << target_pos.x << " " << target_pos.y << " ) " << endl;
    cout << "Now Action(0: findFood,  1:setFood,  2:cookFood,  3:pendMission):  " << int(now_action) << endl;
    cout << "Now Dish in Hand: " << PlayerInfo.dish << endl;
    cout << "Now Target Dish: " << now_dish << endl;
    cout << "Now Cook Time: " << cook_time[target_cooker[0]] << endl;
    cout << "Now Block Time: " << block_time << endl;
    cout << "Now Score: " << PlayerInfo.score << endl;
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
    // 初始化
    if (!init_flag)
        initialize();

    // 更新重要状态和地图信息
    update_state();
    update_info();

    // 检查手上的东西是否可以提交或者已经不需要，部分情况下不执行动作，因此直接更新now_action  **待完善**
    if (PlayerInfo.dish != DishEmpty)
    {
        if (check_Mission(PlayerInfo.dish)) // 提交任务的优先级最高
            next_action = now_action = Action::pendMission;
        else if (now_action == Action::cookFood || !check_need(PlayerInfo.dish)) // 放下手中不需要的食材
        {
            if (now_action != Action::cookFood)
            {
                next_action = Action::findFood;
                put(5, -PI, true);
            }
            else
                put(0, getAngle(), true);
            is_act = true;
        }
        else
            next_action = now_action = Action::setFood; // 手上食物是需要的，自然准备回去放到炉子上
    }

    // 使用道具
    if (PlayerInfo.tool != ToolEmpty && now_bag.is_one_time_use(PlayerInfo.tool) && !is_act)
    {
        if (PlayerInfo.tool == Fertilizer && now_action == Action::findFood
                && !is_fertilized[target_food_point[0]] && ready_to_use())
        {
            use(1, 0, 0);
            is_fertilized[target_food_point[0]] = true;
            is_act = true;
        }
        else if (PlayerInfo.tool == Condiment && ready_to_use())
        {
            if (now_action == Action::pendMission)
            {
                use(1, 0, 0);
                next_action = Action::findFood;
                is_act = true;
            }
            if (now_action == Action::setFood)
            {
                put(1, getAngle(), true);
                is_act = true;
            }
        }
        else if (PlayerInfo.tool == SpaceGate)
        {
            int offsetX = target_pos.x - now_pos.x;
            int offsetY = target_pos.y - now_pos.y;
            int minusX = 0;
            bool is_block = false;
            for (int i = 0; i < map_block.size(); ++i)
                if (map_block[i].x == now_pos.x + offsetX && map_block[i].y == now_pos.y + offsetY)
                    is_block = true;
            while (GameMap[now_pos.x + offsetX][now_pos.y + offsetY] || is_block)
            {
                minusX ? offsetX-- : offsetY--;
                minusX ^= 1;
                is_block = false;
                for (int i = 0; i < map_block.size(); ++i)
                    if (map_block[i].x == now_pos.x + offsetX && map_block[i].y == now_pos.y + offsetY)
                        is_block = true;
            }
            use(1, offsetX, offsetY);
            is_act = true;
        }
        else
        {
            use(1, 0, 0);
            is_act = true;
        }
    }

    // 如果需要捡新的道具，就先把手上的道具先放下
    if (PlayerInfo.tool != ToolEmpty && target_obj.object.objType == Tool && ready_to_use())
    {
        put(0, getAngle(), true);
        is_act = true;
    }

    // 提交任务
    if (now_action == Action::pendMission && !is_act)
    {
        if (ready_to_use())
        {
            use(0, 0, 0);
            next_action = Action::findFood;
        }
        else
        {
            target_pos = MissionPointPosition[target_mission_point[0]];
            Path now_path = BFSFindPath(now_pos, target_pos);
            start_move(now_path.move_list.top());
        }
        is_act = true;
    }

    // 放置食材并检查是否可以开炉
    if (now_action == Action::setFood && !is_act)
    {
        target_pos = CookerPosition[target_cooker[0]];
        if (ready_to_use())
        {
            put(1, getAngle(), true);
            if (cook_time[target_cooker[0]] <= 0) // 可以做菜了，开炉  **待完善（这里涉及到两个玩家共享变量的问题）**
            {
                next_action = Action::cookFood;
                now_dish = (DishType)-1; // 将now_dish置-1，给cookFood提供初值条件
            }
            else
                next_action = Action::findFood; // 炉子正在用，继续去找食物
        }
        else
        {
            Path now_path = BFSFindPath(now_pos, target_pos);
            start_move(now_path.move_list.top());
        }
        is_act = true;
    }

    // 烹饪食材
    if (now_action == Action::cookFood && !is_act)
    {
        if (now_dish == -1) // 搜索能做的菜品，顺便拿出锅里的食材
        {
            now_dish = DishEmpty;
            pick(false, Block, 0);
            for (auto it = target_dish.begin(); it != target_dish.end(); ++it)
            {
                now_dish = now_bag.get_synchronized_dish(*it, target_cooker[0]);
                if (now_dish != DishEmpty)
                    break;
            }
        }
        else if (now_dish == DishEmpty)
            next_action = Action::findFood; // 没有能做的菜品
        else                                // 有能做的菜品，搬出多余物品后开做
        {
            redundant = now_bag.get_what_is_redundant(now_dish, target_cooker[0]);
            if (redundant.empty())
            {
                target_pos = CookerPosition[target_cooker[0]];
                if (ready_to_use())
                {
                    //cout << "Cook!!!" << endl;
                    use(0, 0, 0);
                    next_action = Action::findFood;
                    cook_time[target_cooker[0]] = Constant::DishInfo.find(now_dish)->second.CookTime;
                }
                else
                {
                    Path now_path = BFSFindPath(now_pos, target_pos);
                    start_move(now_path.move_list.top());
                }
            }
            else pick(false, Dish, redundant[0]);
        }
        is_act = true;
    }

    // 寻找食材
    if (now_action == Action::findFood && !is_act)
    {
        if (target_obj.value > 0)
            target_pos = target_obj.object.position;
        else
            target_pos = FoodPointPosition[target_food_point[0]];

        if (ready_to_use() || target_pos == now_pos)
        {
            if (target_pos == FoodPointPosition[target_food_point[0]])
                pick(false, Block, 0);
            else if (target_obj.value > 0)
            {
                if (target_obj.object.blockType == Dish)
                    pick(true, Dish, target_obj.object.dish);
                else
                    pick(true, Tool, target_obj.object.tool);
            }
        }
        else
        {
            Path now_path = BFSFindPath(now_pos, target_pos);
            start_move(now_path.move_list.top());
        }
        is_act = true;
    }
    debug_info();
}
