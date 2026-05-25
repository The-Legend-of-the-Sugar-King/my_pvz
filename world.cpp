#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <thread>
#include <chrono>

#include "tool.hpp"

using namespace std;
using namespace ftxui;

// =========================
// 全局变量
// =========================

const int lim_row=6,lim_col=9;

long long sun_amount=0;

vector<vector<plant_father*>> earth(
6,
vector<plant_father*>(9)
);

vector<shot*> shot_table;

vector<zombie_father*> zombie_table;

int begin_time=0;

int current_sun=0;

int cursor_row = 0;

int cursor_col = 0;

int current_select = 1;

int menu_index=0;

bool show_info = false;

bool running=true;

PII selected_coord = {0,0};

GameState game_state=MENU;

// =========================
// 检查坐标
// =========================

bool check_coord(PII coord)
{
if((coord.first>=lim_row||coord.first<0)
|| (coord.second>=lim_col||coord.second<0))
{
return false;
}


return true;


}

// =========================
// 检查阳光
// =========================

bool check_sun(int type)
{
if(type==1)
{
if(current_sun>=100)
{
current_sun-=100;
return true;
}


    return false;
}

else if(type==2)
{
    if(current_sun>=50)
    {
        current_sun-=50;
        return true;
    }

    return false;
}
else if(type==3)
{
    if(current_sun>=150)
    {
        current_sun-=150;
        return true;
    }

    return false;
}
else if(type==4)
{
    if(current_sun>=50)
    {
        current_sun-=50;
        return true;
    }

    return false;
}
else if(type==5)
{
    if(current_sun>=50)
    {
        current_sun-=50;
        return true;
    }

    return false;
}

return false;


}

// =========================
// 种植植物
// =========================

void plant(int type ,PII coord)
{
if(!check_coord(coord))
{
cout<<"非法坐标"<<endl;
}


else if(earth[coord.first][coord.second]!=nullptr)
{
    cout<<"已有植物"<<endl;
}

else if(!check_sun(type))
{
    cout<<"阳光不足"<<endl;
}

else
{
    if(type==1)
    {
        earth[coord.first][coord.second]
        =new PeaShooter(
            coord.first,
            coord.second
        );
    }

    else if(type==2)
    {
        earth[coord.first][coord.second]
        =new sunflower(
            coord.first,
            coord.second
        );
    }
    else if(type==3)
{
    earth[coord.first][coord.second]=new PotatoMine(coord.first,coord.second);
}
    else if(type==4)
{
    earth[coord.first][coord.second]=new CherryBomb(coord.first,coord.second);
}
    else if(type==5)
    {
        earth[coord.first][coord.second]
        =new WallNut(
            coord.first,
            coord.second
        );
    }
}


}

// =========================
// 生成僵尸
// =========================

void creat_zombie()
{
    zombie_father *z = new zombie_father;

    // =========================
    // 难度成长系统
    // =========================

    // 时间越久血越厚
    int hp_bonus = begin_time / 20;

    // 上限防止太变态
    if(hp_bonus > 500)
    {
        hp_bonus = 500;
    }

    z->hp += hp_bonus;

    // 后期速度也增加
    if(begin_time > 1500)
    {
        z->speed = 2;
    }

    // 地狱模式
    if(begin_time > 3000)
    {
        z->attack_power += 20;
    }

    zombie_table.push_back(z);
}


// =========================
// 判定系统
// =========================

bool judge()
{
for(int i=0;i<zombie_table.size();i++)
{
if(zombie_table[i]!=nullptr)
{
if(zombie_table[i]->coord.second<=0)
{
game_state = GAME_OVER;


            return false;
        }
    }
}

return true;


}

// =========================
// 清理死亡对象
// =========================

void clear_dead()
{
// 子弹
for(int i=0;i<shot_table.size();i++)
{
if(shot_table[i]==nullptr)
{
shot_table.erase(
shot_table.begin()+i
);


        i--;
    }
}

// 僵尸
for(int i=0;i<zombie_table.size();i++)
{
    if(zombie_table[i]==nullptr)
    {
        zombie_table.erase(
            zombie_table.begin()+i
        );

        i--;
    }
}


}

// =========================
// 更新系统
// =========================

void update_all()
{
// 更新植物
for(int i=0;i<lim_row;i++)
{
for(int j=0;j<lim_col;j++)
{
if(earth[i][j]!=nullptr)
{
earth[i][j]->update();


            if(earth[i][j]->is_dead())
            {
                delete earth[i][j];

                earth[i][j]=nullptr;
            }
        }
    }
}

// 更新子弹
for(int i=0;i<shot_table.size();i++)
{
    if(shot_table[i]!=nullptr)
    {
        shot_table[i]->update();

        if(shot_table[i]->is_dead())
        {
            delete shot_table[i];

            shot_table[i]=nullptr;
        }
    }
}

// 更新僵尸
for(int i=0;i<zombie_table.size();i++)
{
    if(zombie_table[i]!=nullptr)
    {
        zombie_table[i]->update();

        if(zombie_table[i]->is_dead())
        {
            delete zombie_table[i];

            zombie_table[i]=nullptr;
        }
    }
}

clear_dead();


}

// =========================
// 铲除植物
// =========================

void remove_plant(PII coord)
{
if(check_coord(coord)
&& earth[coord.first][coord.second]!=nullptr)
{
delete earth[coord.first][coord.second];


    earth[coord.first][coord.second]=nullptr;
}


}
