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

// 代码已合并至player.cpp


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
