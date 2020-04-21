#include <iostream>
#include <deque>
#include <vector>
using namespace std;
enum DishType {//同原来预设enum
	DishEmpty = 0,
	//以下为食材
	Wheat = 1,	         //麦子
	Rice = 2,            //水稻
	Tomato = 3,          //番茄
	Egg = 4,             //鸡蛋
	Milk = 5,            //牛奶
	Apple = 6,           //苹果
	Banana = 7,          //香蕉
	DarkPlum = 8,        //乌梅
	Hawthorn = 9,        //山楂
	Strawberry = 10,     //草莓
	Beef = 11,           //牛肉
	Pork = 12,           //猪肉
	ChickenMeat = 13,    //鸡肉
	Potato = 14,         //土豆
	Lettuce = 15,        //生菜
	Agaric = 16,         //木耳
	NeedleMushroom = 17, //金针菇
	Cabbage = 18,        //包菜
	DishSize1 = 19,
	//以下为中间产物
	Flour = 20,      //面粉
	Noodle = 21,     //面条
	Bread = 22,      //面包片
	CookedRice = 23, //米饭
	Ketchup = 24,    //番茄酱
	Cream = 25,      //奶油
	//以下为菜品
	TomatoFriedEgg = 26,       //番茄炒蛋
	TomatoFriedEggNoodle = 27, //西红柿鸡蛋面
	BeefNoodle = 28,           //清青牛拉
	OverRice = 29,             //盖浇饭
	YellowPheasant = 30,       //黄焖鸡米饭
	Barbecue = 31,             //烤肉
	FrenchFries = 32,          //薯条
	PlumJuice = 33,            //酸梅汤
	Hamburger = 34,            //汉堡
	StrawberryIcecream = 35,   //草莓圣代
	PopcornChicken = 36,       //鸡米花
	AgaricFriedEgg = 37,       //木耳炒蛋
	Cake = 38,                 //蛋糕
	SugarCoatedHaws = 39,      //冰糖葫芦
	FruitSalad = 40,           //水果色拉
	SpicedPot = 41,
	SpicedPot3 = 42,
	SpicedPot4 = 43,
	SpicedPot5 = 44,
	SpicedPot6 = 45,
	SpicedPot7 = 46,
	SpicedPot8 = 47, //香锅
	DishSize2 = 48,
	//以下为垃圾
	OverCookedDish = 49,
	DarkDish = 50, //黑暗料理
	DishSize3 = 51
};

class Obj
{
public:
	XYPosition position;//物品位置
	ObjType objType;//物品所属类型
	BlockType blockType;//如果物品为墙体，所属的墙体类型
	DishType dish;//如果物品为食材，所属的食材类型；如果物品为人，拥有的食材类型
	ToolType tool;//如果物品为道具，所属的道具类型；如果物品为人，拥有的道具类型
	TriggerType trigger;//如果物品为触发器，所属的触发器类型
	Direction facingDiretion;//如果物品为人，面朝的方向
};

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



/*
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
*/

class bag {
public:
	deque< deque<DishType> > gridient; //已知材料
	bag() {
		gridient.resize(4, deque<DishType>{});
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

	void update(DishType t, int x) { //加入新找到食材到背包中第x个灶台
		gridient[x].push_back(t);
	}

	void update_stove(int x, vector<Obj> finding) {//更新灶台x上的食材
		gridient[x].clear();
		for (int i = 0; i < finding.size(); ++i) {
			gridient[x].push_back(finding[i].dish);
		}
	}
	
	vector<DishType> get_what_is_need(DishType goal, int x) {//返回炉灶x还缺什么食材,如果中间食材没有，会加入中间食材及其合成所需食材
		vector<DishType> rc; //返回值
		for (int i = 0; i < table[goal].size(); ++i) {
			if (find_dish_in_bag(table[goal][i], x) == -1) {
				rc.push_back(table[goal][i]);
				vector<DishType> tmp = get_what_is_need(table[goal][i], x);
				for (int j = 0; j < tmp.size(); ++j) {
					rc.push_back(tmp[i]);
				}
			}
		}
		return rc;
	}

	bool remove(DishType t, int x) { //移除背包里第x个灶台的物品，type为t
		for (int i = 0; i < gridient[x].size(); ++i) {
			if (gridient[x][i] == t) {
				gridient[x].erase(gridient[x].begin() + i);
				return true;
			}
		}
		return false; //删除失败
	}

	int find_dish_in_bag(DishType t, int x) { //找到材料在背包中第x个灶台的位置(从0开始）,如果没有，返回-1
		for (int i = 0; i < gridient.size(); ++i) {
			if (t == gridient[i][i]) return i;
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

	DishType get_synchronized_dish(DishType goal, int x) {//检查目标菜品或其中间产品是否可以被灶台x合成，返回可以合成的菜品或中间产品
		if (is_synchronized(goal, x)) return goal; //菜品可以被灶台x合成
		for (int i = 0; i < table[goal].size(); ++i) {
			if (get_synchronized_dish(table[goal][i], x)) return table[goal][i]; //中间材料可以被灶台x合成
		}
		return (DishType)-1;//没有可合成菜品或中间产品
	}


};




int main() {//测试函数功能

}
