#include <iostream>
#include <deque>
#include <vector>
#include <list>
#include <map>
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
typedef int ToolType;

enum ObjType { //可能出现在地图上的各种物品
	People = 0,  //人
	Block = 1,   //墙体
	Dish = 2,   //食材
	Tool = 3,    //道具
	Trigger = 4, //触发器
	ObjTypeSize = 5
};

class Obj
{
public:
	ObjType objType;//物品所属类型
	DishType dish;//如果物品为食材，所属的食材类型；如果物品为人，拥有的食材类型
	ToolType tool;//如果物品为道具，所属的道具类型；如果物品为人，拥有的道具类型
	int trigger;//如果物品为触发器，所属的触发器类型
	Obj(ObjType type, DishType dish, ToolType tool) {
		this->objType = type;
		this->dish = dish;
		this->tool = tool;
	}
	Obj() {};
};

struct playerInfo
{
	int talent;//玩家的天赋
	DishType dish;//玩家手上拿的食材，只能同时拿一个
	ToolType tool; //玩家手上拿的道具，只能同时拿一个
};

playerInfo PlayerInfo;

struct DishProperty
{
	int Score;
	int CookTime;
	int TaskTime;
};

const map<int, DishProperty> DishInfo =
{
{int(Wheat), {0,0,0} },
{int(Rice), {0,0,0} },
{int(Tomato), {0,0,0} },
{int(Egg), {0,0,0} },
{int(Milk), {0,0,0} },
{int(Apple), {0,0,0} },
{int(Banana), {0,0,0} },
{int(DarkPlum), {0,0,0} },
{int(Hawthorn), {0,0,0} },
{int(Strawberry), {0,0,0} },
{int(Beef), {20,0,0} },
{int(Pork), {20,0,0} },
{int(ChickenMeat), {20,0,0} },
{int(Potato), {20,0,0} },
{int(Lettuce), {20,0,0} },
{int(Agaric), {20,0,0} },
{int(NeedleMushroom), {20,0,0} },
{int(Cabbage), {20,0,0} },
{int(Flour), {0,10000,0} },
{int(Noodle), {0,10000,0} },
{int(Bread), {0,10000,0} },
{int(CookedRice), {0,10000,0} },
{int(Ketchup), {0,10000,0} },
{int(Cream), {0,10000,0} },
{int(TomatoFriedEgg), {50,10000,60000} },
{int(TomatoFriedEggNoodle), {100,15000,90000} },
{int(BeefNoodle), {80,20000,90000} },
{int(OverRice), {90,20000,90000} },
{int(YellowPheasant), {100,20000,90000} },
{int(Barbecue), {55,20000,90000} },
{int(FrenchFries), {60,15000,90000} },
{int(PlumJuice), {50,10000,90000} },
{int(Hamburger), {110,20000,100000} },
{int(StrawberryIcecream), {60,10000,90000} },
{int(PopcornChicken), {60,15000,90000} },
{int(AgaricFriedEgg), {50,15000,90000} },
{int(Cake), {160,30000,120000} },
{int(SugarCoatedHaws), {20,10000,60000} },
{int(FruitSalad), {100,20000,120000} },
{int(SpicedPot), {0,60000,300000} },
{int(DarkDish), {-10,60000,30000} },
{int(OverCookedDish), {-10,60000,300000} },
};

std::vector<DishType> target_dish;

//********************以上为游戏内定义结构******************

struct ObjValue {
	Obj object;
	int value;
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

deque<int> tool_score = //10000代表马上、一定要捡的东西，包括立即使用物品、陷阱，百利而无害
{
	0, //没有物品
	1000, //虎头鞋
	10000, //加速
	10000, //加力量
	1000, //望远镜
	1000, //调料
	1000, //肥料
	1000, //护心镜
	10000, //传送门
	10000, //胶水
	10000, //地雷
	10000, //陷阱
	10000, //闪光炸弹
	10000, //锤子
	10000, //弓箭
};

deque<int> one_time_use = { 2,3,8,9,10,11,12,13,14 }; //一次性道具

deque< deque<int> > occupation_tool = {
	{},//空
	{ //跑者道具分数加成
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
	{ //力量男道具分数加成
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
		10, //锤子
		10, //弓箭
	},
	{ //厨师道具分数加成
		0, //没有物品
		1, //虎头鞋
		1, //加速
		1, //加力量
		1, //望远镜
		10, //调料
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
	{ //技工道具分数加成
		0, //没有物品
		1, //虎头鞋
		1, //加速
		1, //加力量
		1, //望远镜
		1, //调料
		1, //肥料
		1, //护心镜
		10, //传送门
		1, //胶水
		1, //地雷
		1, //陷阱
		1, //闪光炸弹
		1, //锤子
		1, //弓箭
	},
	{ //幸运男孩道具分数加成
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
	}
};

/*
物品比较
思路：
道具优先级高于食材，一次性道具优于常驻型道具，如果返回发现是一次性道具（调用函数is_one_time_use）
周围还有其它道具，就捡起来把它用掉再捡其它道具，如果返回 < 0，代表不要捡
*/



class bag {
public:
	deque< deque<DishType> > ingredient; //已知材料
	bag() {
		ingredient.resize(4, deque<DishType>{});//四个炉子
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
		ingredient[x].push_back(t);
	}

	void update_stove(int x, vector<Obj> finding) {//更新灶台x上的食材
		ingredient[x].clear();
		for (int i = 0; i < finding.size(); ++i) {
			ingredient[x].push_back(finding[i].dish);
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
		for (int i = 0; i < ingredient[x].size(); ++i) {
			if (ingredient[x][i] == t) {
				ingredient[x].erase(ingredient[x].begin() + i);
				return true;
			}
		}
		return false; //删除失败
	}

	int find_dish_in_bag(DishType t, int x) { //找到材料在背包中第x个灶台的位置(从0开始）,如果没有，返回-1
		for (int i = 0; i < ingredient[x].size(); ++i) {
			if (t == ingredient[x][i]) return i;
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

	vector<DishType> get_what_is_rudundant(DishType goal, int x) {//检查灶台x上是否有对于goal来说多余的食材
		vector<DishType> rc;//返回值
		for (int i = 0; i < ingredient[x].size(); ++i) {
			bool redundant = true;
			for (int j = 0; j < table[goal].size(); ++j) {
				if (table[goal][j] == ingredient[x][i]) {
					redundant = false;
				}
			}
			if (redundant) {
				rc.push_back(ingredient[x][i]);
			}
		}
		return rc;
	}

	DishType get_synchronized_dish(DishType goal, int x) {//检查目标菜品或其中间产品是否可以被灶台x合成，返回第一个找到的可以合成的菜品或中间产品
		if (is_synchronized(goal, x)) return goal; //菜品可以被灶台x合成
		for (int i = 0; i < table[goal].size(); ++i) {
			if (get_synchronized_dish(table[goal][i], x) != -1) return table[goal][i]; //中间材料可以被灶台x合成
		}
		return (DishType)-1;//没有可合成菜品或中间产品
	}

	/*
	分析食物的分数
	每个炉子中任务列表中的食物分数 * dish由多少食材合成 / 所需食材数量
	*/
	int parse_food(DishType dish) {
		bool needed = false;
		for (int i = 0; i < target_dish.size(); ++i) {
			if (is_need(target_dish[i], dish)) {
				needed = true;
			}
		}
		if (!needed) return -1;
		int rc = 0;
		int importance = table[dish].size() + 1;
		for (int i = 0; i < target_dish.size(); ++i) {
			for (int j = 0; j < 4; ++j) {
				if (is_need(target_dish[i], dish)) {
					int source_num = get_what_is_need(target_dish[i], j).size();
					rc += DishInfo.at(target_dish[i]).Score * importance / source_num;
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
	int parse_tool(ToolType tool) {
		return  tool_score[tool] + occupation_tool[PlayerInfo.talent][tool];
	}
	/*
	分析物品
	如果小于0则不捡
	*/
	ObjValue parseObject(std::vector<Obj> obj_list) {
		deque<int> score;
		score.resize(obj_list.size(), 0);
		ObjValue best = ObjValue{};
		best.value = -1;
		int cur_food = parse_food(PlayerInfo.dish);
		int cur_tool = parse_tool(PlayerInfo.tool);
		for (int i = 0; i < obj_list.size(); ++i) {
			if (obj_list[i].objType == 2) {
				score[i] += parse_food(obj_list[i].dish);
				if (cur_food > score[i]) score[i] = -1;
			}
			else if (obj_list[i].objType == 3) {
				score[i] += parse_tool(obj_list[i].tool);
				if (cur_tool < 10000) { //是常备道具
					if (score[i] < cur_tool) score[i] = -1;
				}
			}
			if (score[i] > best.value) {
				best.object = obj_list[i];
				best.value = score[i];
			}
		}
		return best;
	}

	bool is_one_time_use(ToolType tool) {//是否为一次性道具
		for (int i = 0; i < one_time_use.size(); ++i) {
			if (tool == one_time_use[i]) return true;
		}
		return false;
	}
};




int main() {//测试函数功能
	/*
	bag test{};
	test.update(Noodle, 0);
	auto a = test.get_what_is_need(TomatoFriedEggNoodle, 0);
	for (int i = 0; i < a.size(); ++i) {
		cout << a[i] << endl;
	}
	cout << test.is_need(TomatoFriedEggNoodle, Egg) << endl;
	cout << test.is_need(TomatoFriedEggNoodle, Beef) << endl;
	test.update(Beef, 0);
	test.update(Milk, 0);
	auto b = test.get_what_is_need(BeefNoodle, 0);
	for (int i = 0; i < b.size(); ++i) {
		cout << b[i] << endl;
	}
	cout << test.is_synchronized(BeefNoodle, 0) << endl << endl;
	auto c = test.get_what_is_rudundant(TomatoFriedEggNoodle, 0);
	for (int i = 0; i < c.size(); ++i) {
		cout << c[i] << endl;
	}
	test.update(Tomato, 0);
	test.update(Egg, 0);
	cout << test.get_synchronized_dish(TomatoFriedEggNoodle, 0) << endl;
	test.update(Flour, 0);
	cout << test.get_synchronized_dish(TomatoFriedEggNoodle, 0) << endl;
	test.remove(Flour, 0);
	*/
	bag test{};
	PlayerInfo.talent = 1;
	test.update(Wheat, 2);
	target_dish.push_back(OverRice);
	vector<Obj> obj_list;
	obj_list.push_back(Obj{ ObjType(2),Rice,0});
	obj_list.push_back(Obj{ ObjType(2),Beef,0 });
	obj_list.push_back(Obj{ ObjType(3), DishType(0), 1 });
	ObjValue best = test.parseObject(obj_list);
	cout << best.object.dish << " " << best.value;
}
