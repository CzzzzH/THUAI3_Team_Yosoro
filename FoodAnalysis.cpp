#include <iostream>
#include <deque>
#include <vector>
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
/*
class Obj
{
public:
	XYPosition position;//��Ʒλ��
	ObjType objType;//��Ʒ��������
	BlockType blockType;//�����ƷΪǽ�壬������ǽ������
	DishType dish;//�����ƷΪʳ�ģ�������ʳ�����ͣ������ƷΪ�ˣ�ӵ�е�ʳ������
	ToolType tool;//�����ƷΪ���ߣ������ĵ������ͣ������ƷΪ�ˣ�ӵ�еĵ�������
	TriggerType trigger;//�����ƷΪ�������������Ĵ���������
	Direction facingDiretion;//�����ƷΪ�ˣ��泯�ķ���
};
*/
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




class bag {
public:
	deque< deque<DishType> > gridient; //��֪����
	bag() {
		gridient.resize(4, deque<DishType>{});
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
		gridient[x].push_back(t);
	}
	
	void update_stove(int x, vector<Obj> finding) {//������̨x�ϵ�ʳ��
		gridient[x].clear();
		for (int i = 0; i < finding.size(); ++i) {
			gridient[x].push_back(finding[i].dish);
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
		for (int i = 0; i < gridient[x].size(); ++i) {
			if (gridient[x][i] == t) {
				gridient[x].erase(gridient[x].begin() + i);
				return true;
			}
		}
		return false; //ɾ��ʧ��
	}

	int find_dish_in_bag(DishType t, int x) { //�ҵ������ڱ����е�x����̨��λ��(��0��ʼ��,���û�У�����-1
		for (int i = 0; i < gridient[x].size(); ++i) {
			if (t == gridient[x][i]) return i;
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

	DishType get_synchronized_dish(DishType goal, int x) {//���Ŀ���Ʒ�����м��Ʒ�Ƿ���Ա���̨x�ϳɣ����ص�һ���ҵ��Ŀ��ԺϳɵĲ�Ʒ���м��Ʒ
		if (is_synchronized(goal, x)) return goal; //��Ʒ���Ա���̨x�ϳ�
		for (int i = 0; i < table[goal].size(); ++i) {
			if (get_synchronized_dish(table[goal][i], x) != -1) return table[goal][i]; //�м���Ͽ��Ա���̨x�ϳ�
		}
		return (DishType)-1;//û�пɺϳɲ�Ʒ���м��Ʒ
	}


};




int main() {//���Ժ�������
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
}
