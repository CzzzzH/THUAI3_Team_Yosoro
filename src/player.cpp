#include "API.h"
#include "Constant.h"
#include "player.h"
#include <iostream>
#include "OS_related.h"
#include <deque>
#include <vector>

#define PLYAER 0 // 0为正常策略，1为强盗策略
#define SLEEP_TIME 50 // 休眠时间

using namespace THUAI3;
Protobuf::Talent initTalent = Protobuf::Talent::Runner;//指定人物天赋。选手代码必须定义此变量，否则报错

/* ------------------------------------- FoodAnalysis.cpp ------------------------------------- */

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

class Bag {
public:
    deque< deque<DishType> > gridient; //已知材料
    Bag() {
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

/* ------------------------------------- mainLogic ------------------------------------- */

enum Action {findFood, setFood, pushMission};
Action now_action;
std::vector<Obj> target_food, target_tool;
std::vector<int> target_cooker;
XYPosition now_pos, target_pos;
Bag now_bag;

bool food_compare(Obj a, Obj b)
{
    return true;
}

bool tool_compare(Obj a, Obj b)
{
    return true;
}

bool cooker_compare(int a, int b)
{
    return true;
}

bool check_need(DishType target_dish)
{
    for (auto it = task_list.begin(); it != task_list.end(); ++it)
    {
        auto need_list = now_bag.get_what_is_need(*it, target_cooker[0]);
        if (find(need_list.begin(), need_list.end(), target_dish) != need_list.end())
            return true;
    }
    return false;
}


void update_view()
{
    target_food.clear();
    target_tool.clear();
    for (int x = 0; x < 50; ++x)
        for (int y = 0; y < 50; ++y)
        {
            auto obj_list = MapInfo::get_mapcell(x, y);

            // ***添加当前位置为炉子的判定
            for (auto it = obj_list.begin(); it != obj_list.end(); ++it)
            {   
                if (it->objType == Dish && check_need(it->dish))
                {
                    
                }
            }
        }
    std::sort(target_food.begin(), target_food.end(), food_compare);
    std::sort(target_tool.begin(), target_tool.end(), tool_compare);
}

void play()
{
    bool is_act;
    while (true)
    {
        is_act = false;
        now_pos = PlayerInfo.position;
        update_view();
        if (PlayerInfo.dish != DishEmpty && !check_need(PlayerInfo.dish))
        {
            put(3, PlayerInfo.facingDirection, true);
            now_action = Action::findFood;
            is_act = true;
        }
        if (now_action == Action::findFood && !is_act)
        {

        }
        if (now_action == Action::setFood && !is_act)
        {

        }
        if (now_action == Action::pushMission && !is_act)
        {
            
        }
        Sleep(SLEEP_TIME); // 等待下一帧到来
    }
}