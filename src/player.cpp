#include "API.h"
#include "Constant.h"
#include "player.h"
#include <iostream>
#include "OS_related.h"
#include <deque>

using namespace THUAI3;
Protobuf::Talent initTalent = Protobuf::Talent::None;//指定人物天赋。选手代码必须定义此变量，否则报错

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


class element { //背包内材料性质
public:
	DishType type; //材料类型
	int x, y; //所在坐标位置
	element(int x, int y, DishType t) {
		this->x = x;
		this->y = y;
		this->type = t;
	}
};

class bag {
public:
	deque<element> gridient; //已知材料
	int not_found(DishType t, int x, int y) {//确认食材是否是之前已经找到的食材
		for (int i = 0; i < gridient.size(); ++i) {
			if (gridient[i].type == t && gridient[i].x == x && gridient[i].y == y) {
				return 0; //找到已知食材, 返回0
			}
		}
		return 1; //找到未知食材，返回1
	}

	/*
	检查该材料是否为当前菜品所需材料,且非已知食材
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

	void update(DishType t, int x, int y) { //加入新找到食材到背包中
		gridient.push_back(element{ x, y, t });
	}

	bool remove(DishType t, int x, int y) { //移除背包里的物品，根据物品类型、坐标（因为可能遇到重复材料）
		for (int i = 0; i < gridient.size(); ++i) {
			if (gridient[i].type == t && gridient[i].x == x && gridient[i].y == y) {
				gridient.erase(gridient.begin() + i);
				return true; //删除成功
			}
		}
		return false; //删除失败
	}

	int find_dish_in_bag(DishType t) { //找到材料在背包中的位置(从0开始）,如果没有，返回-1
		for (int i = 0; i < gridient.size(); ++i) {
			if (t == gridient[i].type) return i;
		}
		return -1;
	}

	bool is_synchronized(DishType t) { //检查t是否可以被合成
		if (table[t].empty()) return false; //原料，不用合成
		for (int i = 0; i < table[t].size(); ++i) { //是否所有需要材料都在背包里
			if (find_dish_in_bag(table[t][i]) == -1) {
				return false;
			}
		}
		return true;
	}

	DishType get_synchronized_dish(DishType goal) {//检查目标菜品或其中间产品是否可以被合成，返回可以合成的菜品或中间产品
		if (is_synchronized(goal)) return goal; //菜品可以被合成
		for (int i = 0; i < table[goal].size(); ++i) {
			if (get_synchronized_dish(table[goal][i])) return table[goal][i]; //中间材料可以被合成
		}
		return (DishType)-1;//没有可合成菜品或中间产品
	}


};

void play()
{
	char c;
	cin >> c;
	switch (c)
	{
		case 'd':move(Protobuf::Direction::Right, 1000); break;
		case 'e':move(Protobuf::Direction::RightUp, 1000); break;
		case 'w':move(Protobuf::Direction::Up, 1000); break;
		case 'q':move(Protobuf::Direction::LeftUp, 1000); break;
		case 'a':move(Protobuf::Direction::Left, 1000); break;
		case 'z':move(Protobuf::Direction::LeftDown, 1000); break;
		case 'x':move(Protobuf::Direction::Down, 1000); break;
		case 'c':move(Protobuf::Direction::RightDown, 1000); break;

	default:
		break;
	}
	
	/*  玩家在这里写代码  */
}