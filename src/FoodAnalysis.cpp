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

// �����Ѻϲ���player.cpp


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
