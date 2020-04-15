#include <iostream>
#include <deque>
using namespace std;

/*
ʹ�÷�����
��Ϊһ��ֻ�ܼ���һ��ʳ�ģ�������һ�������Ľṹ�洢�Ѿ��ҵ���ʳ�ĵ�������λ��(x,y)
ÿ���ҵ��µ�ʳ�ģ�����not_found����ȷ�ϲ���֮ǰ�Ѿ��ҵ���ʳ�ģ�Ȼ��ʹ��is_need����ȷ�ϵ�ǰҪ���Ĳ�Ʒ�Ƿ���Ҫ��ʳ��
�����µ�ʳ�ģ���update���������ҵ���ʳ�ļ��뵽�����У����ҿ���get_synchronized_dish������鱳�����Ƿ��Ѿ����Ժϳɵ�ǰĿ���Ʒ�����м��Ʒ(����У����ظò�Ʒ���ͣ����򷵻�-1)
�ɸ��ݲ�Ʒ���ȥ����table����Ҫ��ʳ�ģ���͸��find_dish_in_bag����֪��ʳ���ڱ�����λ�á�
����ʳ�ı�͵���ƶ�ʳ�Ļ�����ȥ�ϳ��ˣ��ǵðѱ�����ԭ�ȴ�ŵ���remove����ɾ��������Ϊ(���ͣ�x��y)*/
/*

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


class element { //�����ڲ�������
public:
	DishType type; //��������
	int x, y; //��������λ��
	element(int x, int y, DishType t) {
		this->x = x;
		this->y = y;
		this->type = t;
	}
};

class bag {
public:
	deque<element> gridient; //��֪����
	int not_found(DishType t, int x, int y) {//ȷ��ʳ���Ƿ���֮ǰ�Ѿ��ҵ���ʳ��
		for (int i = 0; i < gridient.size(); ++i) {
			if (gridient[i].type == t && gridient[i].x == x && gridient[i].y == y) {
				return 0; //�ҵ���֪ʳ��, ����0
			}
		}
		return 1; //�ҵ�δ֪ʳ�ģ�����1
	}

	/*
	���ò����Ƿ�Ϊ��ǰ��Ʒ�������,�ҷ���֪ʳ��
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

	void update(DishType t, int x, int y) { //�������ҵ�ʳ�ĵ�������
		gridient.push_back(element{ x, y, t });
	}

	bool remove(DishType t, int x, int y) { //�Ƴ����������Ʒ��������Ʒ���͡����꣨��Ϊ���������ظ����ϣ�
		for (int i = 0; i < gridient.size(); ++i) {
			if (gridient[i].type == t && gridient[i].x == x && gridient[i].y == y) {
				gridient.erase(gridient.begin() + i);
				return true; //ɾ���ɹ�
			}
		}
		return false; //ɾ��ʧ��
	}

	int find_dish_in_bag(DishType t) { //�ҵ������ڱ����е�λ��(��0��ʼ��,���û�У�����-1
		for (int i = 0; i < gridient.size(); ++i) {
			if (t == gridient[i].type) return i;
		}
		return -1;
	}

	bool is_synchronized(DishType t) { //���t�Ƿ���Ա��ϳ�
		if (table[t].empty()) return false; //ԭ�ϣ����úϳ�
		for (int i = 0; i < table[t].size(); ++i) { //�Ƿ�������Ҫ���϶��ڱ�����
			if (find_dish_in_bag(table[t][i]) == -1) {
				return false;
			}
		}
		return true;
	}

	DishType get_synchronized_dish(DishType goal) {//���Ŀ���Ʒ�����м��Ʒ�Ƿ���Ա��ϳɣ����ؿ��ԺϳɵĲ�Ʒ���м��Ʒ
		if (is_synchronized(goal)) return goal; //��Ʒ���Ա��ϳ�
		for (int i = 0; i < table[goal].size(); ++i) {
			if (get_synchronized_dish(table[goal][i])) return table[goal][i]; //�м���Ͽ��Ա��ϳ�
		}
		return (DishType)-1;//û�пɺϳɲ�Ʒ���м��Ʒ
	}


};



/*
int main() {//���Ժ�������
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
