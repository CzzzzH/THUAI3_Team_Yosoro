#include <iostream>
#include <deque>
using namespace std;

/*
使用方法：
因为一次只能捡起一样食材，所以用一个背包的结构存储已经找到的食材的类型与位置(x,y)
每次找到新的食材，先用not_found函数确认不是之前已经找到的食材，然后使用is_need函数确认当前要做的菜品是否需要该食材
对于新的食材，用update函数将新找到的食材加入到背包中，并且可用get_synchronized_dish函数检查背包中是否已经可以合成当前目标菜品或其中间产品(如果有，返回该菜品类型，否则返回-1)
可根据菜品编号去搜索table中需要的食材，并透过find_dish_in_bag函数知道食材在背包的位置。
发现食材被偷、移动食材或是拿去合成了，记得把背包中原先存放的用remove函数删除，参数为(类型，x，y)*/
/*

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
*/
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



/*
int main() {//测试函数功能
	bag test;
	cout << test.is_need(TomatoFriedEggNoodle, Wheat, 0, 0) << endl;
	test.update(Milk, 2, 0);
	test.update(Tomato, 1, 0);
	test.update(Egg, 3, 0);
	test.update(Noodle, 0, 0);
	cout << test.find_dish_in_bag(Milk) << endl;
	cout << test.get_synchronized_dish(TomatoFriedEggNoodle) << endl;
	cout << test.get_synchronized_dish(TomatoFriedEgg) << endl;
	test.remove(Noodle, 0, 0);
	cout << test.is_need(Cream, Milk) << endl;
}
*/
