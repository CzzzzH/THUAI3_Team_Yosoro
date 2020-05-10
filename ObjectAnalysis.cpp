#include <iostream>
#include <deque>
#include <vector>
#include <list>
#include <map>
using namespace std;
enum DishType {//ͬԭ��Ԥ��enum
	DishEmpty = 0,
	//����Ϊʳ��
	Wheat = 1,	         //����
	Rice = 2,            //ˮ��
	Tomato = 3,          //����
	Egg = 4,             //����
	Milk = 5,            //ţ��
	Apple = 6,           //ƻ��
	Banana = 7,          //�㽶
	DarkPlum = 8,        //��÷
	Hawthorn = 9,        //ɽ�
	Strawberry = 10,     //��ݮ
	Beef = 11,           //ţ��
	Pork = 12,           //����
	ChickenMeat = 13,    //����
	Potato = 14,         //����
	Lettuce = 15,        //����
	Agaric = 16,         //ľ��
	NeedleMushroom = 17, //���빽
	Cabbage = 18,        //����
	DishSize1 = 19,
	//����Ϊ�м����
	Flour = 20,      //���
	Noodle = 21,     //����
	Bread = 22,      //���Ƭ
	CookedRice = 23, //�׷�
	Ketchup = 24,    //���ѽ�
	Cream = 25,      //����
	//����Ϊ��Ʒ
	TomatoFriedEgg = 26,       //���ѳ���
	TomatoFriedEggNoodle = 27, //������������
	BeefNoodle = 28,           //����ţ��
	OverRice = 29,             //�ǽ���
	YellowPheasant = 30,       //���˼��׷�
	Barbecue = 31,             //����
	FrenchFries = 32,          //����
	PlumJuice = 33,            //��÷��
	Hamburger = 34,            //����
	StrawberryIcecream = 35,   //��ݮʥ��
	PopcornChicken = 36,       //���׻�
	AgaricFriedEgg = 37,       //ľ������
	Cake = 38,                 //����
	SugarCoatedHaws = 39,      //���Ǻ�«
	FruitSalad = 40,           //ˮ��ɫ��
	SpicedPot = 41,
	SpicedPot3 = 42,
	SpicedPot4 = 43,
	SpicedPot5 = 44,
	SpicedPot6 = 45,
	SpicedPot7 = 46,
	SpicedPot8 = 47, //���
	DishSize2 = 48,
	//����Ϊ����
	OverCookedDish = 49,
	DarkDish = 50, //�ڰ�����
	DishSize3 = 51
};
typedef int ToolType;

enum ObjType { //���ܳ����ڵ�ͼ�ϵĸ�����Ʒ
	People = 0,  //��
	Block = 1,   //ǽ��
	Dish = 2,   //ʳ��
	Tool = 3,    //����
	Trigger = 4, //������
	ObjTypeSize = 5
};

class Obj
{
public:
	ObjType objType;//��Ʒ��������
	DishType dish;//�����ƷΪʳ�ģ�������ʳ�����ͣ������ƷΪ�ˣ�ӵ�е�ʳ������
	ToolType tool;//�����ƷΪ���ߣ������ĵ������ͣ������ƷΪ�ˣ�ӵ�еĵ�������
	int trigger;//�����ƷΪ�������������Ĵ���������
	Obj(ObjType type, DishType dish, ToolType tool) {
		this->objType = type;
		this->dish = dish;
		this->tool = tool;
	}
	Obj() {};
};

struct playerInfo
{
	int talent;//��ҵ��츳
	DishType dish;//��������õ�ʳ�ģ�ֻ��ͬʱ��һ��
	ToolType tool; //��������õĵ��ߣ�ֻ��ͬʱ��һ��
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

//********************����Ϊ��Ϸ�ڶ���ṹ******************

struct ObjValue {
	Obj object;
	int value;
};


deque< deque<DishType> > table = { //��Ʒ�ϳ��б���i��Ϊ��i���Ʒ(��ʳ��)�������
	{}, //��
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

deque<int> tool_score = //10000�������ϡ�һ��Ҫ��Ķ�������������ʹ����Ʒ�����壬�������޺�
{
	0, //û����Ʒ
	1000, //��ͷЬ
	10000, //����
	10000, //������
	1000, //��Զ��
	1000, //����
	1000, //����
	1000, //���ľ�
	10000, //������
	10000, //��ˮ
	10000, //����
	10000, //����
	10000, //����ը��
	10000, //����
	10000, //����
};

deque<int> one_time_use = { 2,3,8,9,10,11,12,13,14 }; //һ���Ե���

deque< deque<int> > occupation_tool = {
	{},//��
	{ //���ߵ��߷����ӳ�
		0, //û����Ʒ
		1, //��ͷЬ
		1, //����
		1, //������
		1, //��Զ��
		1, //����
		1, //����
		1, //���ľ�
		1, //������
		1, //��ˮ
		1, //����
		1, //����
		1, //����ը��
		1, //����
		1, //����
	},
	{ //�����е��߷����ӳ�
		0, //û����Ʒ
		1, //��ͷЬ
		1, //����
		1, //������
		1, //��Զ��
		1, //����
		1, //����
		1, //���ľ�
		1, //������
		1, //��ˮ
		1, //����
		1, //����
		1, //����ը��
		10, //����
		10, //����
	},
	{ //��ʦ���߷����ӳ�
		0, //û����Ʒ
		1, //��ͷЬ
		1, //����
		1, //������
		1, //��Զ��
		10, //����
		1, //����
		1, //���ľ�
		1, //������
		1, //��ˮ
		1, //����
		1, //����
		1, //����ը��
		1, //����
		1, //����
	},
	{ //�������߷����ӳ�
		0, //û����Ʒ
		1, //��ͷЬ
		1, //����
		1, //������
		1, //��Զ��
		1, //����
		1, //����
		1, //���ľ�
		10, //������
		1, //��ˮ
		1, //����
		1, //����
		1, //����ը��
		1, //����
		1, //����
	},
	{ //�����к����߷����ӳ�
		0, //û����Ʒ
		1, //��ͷЬ
		1, //����
		1, //������
		1, //��Զ��
		1, //����
		1, //����
		1, //���ľ�
		1, //������
		1, //��ˮ
		1, //����
		1, //����
		1, //����ը��
		1, //����
		1, //����
	}
};

/*
��Ʒ�Ƚ�
˼·��
�������ȼ�����ʳ�ģ�һ���Ե������ڳ�פ�͵��ߣ�������ط�����һ���Ե��ߣ����ú���is_one_time_use��
��Χ�����������ߣ��ͼ����������õ��ټ��������ߣ�������� < 0������Ҫ��
*/



class bag {
public:
	deque< deque<DishType> > ingredient; //��֪����
	bag() {
		ingredient.resize(4, deque<DishType>{});//�ĸ�¯��
	}

	/*
	���ò����Ƿ�Ϊ��ǰ��Ʒ�������
	����ҵ��µ���Ҫʳ�ģ�����1
	����ҵ��µĲ���Ҫʳ�ģ�����0
	*/
	int is_need(DishType goal, DishType t) {
		if (goal == t) return 1;
		for (int i = 0; i < table[goal].size(); ++i) {
			if (is_need(table[goal][i], t) == 1) {
				return 1;
			}
		}
		return 0;//�ҵ��µĲ���Ҫʳ��
	}

	void update(DishType t, int x) { //�������ҵ�ʳ�ĵ������е�x����̨
		ingredient[x].push_back(t);
	}

	void update_stove(int x, vector<Obj> finding) {//������̨x�ϵ�ʳ��
		ingredient[x].clear();
		for (int i = 0; i < finding.size(); ++i) {
			ingredient[x].push_back(finding[i].dish);
		}
	}

	vector<DishType> get_what_is_need(DishType goal, int x) {//����¯��x��ȱʲôʳ��,����м�ʳ��û�У�������м�ʳ�ļ���ϳ�����ʳ��
		vector<DishType> rc; //����ֵ
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

	bool remove(DishType t, int x) { //�Ƴ��������x����̨����Ʒ��typeΪt��ע�⣺ֻ��ɾ����һ��tʳ��
		for (int i = 0; i < ingredient[x].size(); ++i) {
			if (ingredient[x][i] == t) {
				ingredient[x].erase(ingredient[x].begin() + i);
				return true;
			}
		}
		return false; //ɾ��ʧ��
	}

	int find_dish_in_bag(DishType t, int x) { //�ҵ������ڱ����е�x����̨��λ��(��0��ʼ��,���û�У�����-1
		for (int i = 0; i < ingredient[x].size(); ++i) {
			if (t == ingredient[x][i]) return i;
		}
		return -1;
	}

	bool is_synchronized(DishType t, int x) { //���t�Ƿ���Ա���̨x�ϳ�
		if (table[t].empty()) return false; //ԭ�ϣ����úϳ�
		for (int i = 0; i < table[t].size(); ++i) { //�Ƿ�������Ҫ���϶�����̨x��
			if (find_dish_in_bag(table[t][i], x) == -1) {
				return false;
			}
		}
		return true;
	}

	vector<DishType> get_what_is_rudundant(DishType goal, int x) {//�����̨x���Ƿ��ж���goal��˵�����ʳ��
		vector<DishType> rc;//����ֵ
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

	DishType get_synchronized_dish(DishType goal, int x) {//���Ŀ���Ʒ�����м��Ʒ�Ƿ���Ա���̨x�ϳɣ����ص�һ���ҵ��Ŀ��ԺϳɵĲ�Ʒ���м��Ʒ
		if (is_synchronized(goal, x)) return goal; //��Ʒ���Ա���̨x�ϳ�
		for (int i = 0; i < table[goal].size(); ++i) {
			if (get_synchronized_dish(table[goal][i], x) != -1) return table[goal][i]; //�м���Ͽ��Ա���̨x�ϳ�
		}
		return (DishType)-1;//û�пɺϳɲ�Ʒ���м��Ʒ
	}

	/*
	����ʳ��ķ���
	ÿ��¯���������б��е�ʳ����� * dish�ɶ���ʳ�ĺϳ� / ����ʳ������
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
	����������Ҫ��
	�����һ���Ե��ߣ����ȼ����������õ�����ñ����������ã�
	ע�����������һ���Ե���Ҫ���Ͱ���ʹ�õ�
	*/
	int parse_tool(ToolType tool) {
		return  tool_score[tool] + occupation_tool[PlayerInfo.talent][tool];
	}
	/*
	������Ʒ
	���С��0�򲻼�
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
				if (cur_tool < 10000) { //�ǳ�������
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

	bool is_one_time_use(ToolType tool) {//�Ƿ�Ϊһ���Ե���
		for (int i = 0; i < one_time_use.size(); ++i) {
			if (tool == one_time_use[i]) return true;
		}
		return false;
	}
};




int main() {//���Ժ�������
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
