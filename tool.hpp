#pragma once
#include <vector>
#include <utility>
#include <ftxui/dom/elements.hpp>
#include <iostream>
using namespace std;
typedef pair<int ,int> PII;


enum GameState
{
    MENU,
    PLAYING,
    GAME_OVER
};

//前向声明区
class shot;
class plant_father;
class zombie_father;

//变量声明区
extern const int lim_row,lim_col;
extern long long sun_amount;
extern vector<vector<plant_father*>> earth;//map,从0开始
extern vector<zombie_father*>zombie_table;
extern vector<shot*>shot_table;
extern int begin_time;
extern int current_sun;
extern int cursor_row;
extern int cursor_col;
extern int current_select;
extern bool show_info;
extern PII selected_coord;
//extern vector<plant_father*>plant_table;
//extern index;
//extern bool game_over;
extern GameState game_state;
extern int menu_index;
extern bool running;

//函数声明区
bool check_coord(PII coord);
bool check_sun(int type);
void plant(int type ,PII coord);
void draw();
void creat_zombie();
void update_all();
bool judge();
void player_input();
//void game_clock();
void clear_dead();
void remove_plant(PII coord);



ftxui::Element draw_map();

ftxui::Element draw_info();


//类声明区
class plant_father//创建时new
{
public:
//构造函数
plant_father(int row,int col)
{
coord.first=row;
coord.second=col;
statu=true;
}

//属性
int hp;
int price;

float cd;
int index;
int type;
PII coord;
int attack_power;
bool statu;//0死，1活
//行为
virtual void update()
{

}

bool is_dead()
{
if(hp<=0)   return true;
else        return false;
}

void takeDamage(int d)
{
hp-=d;
}
};

class zombie_father
{
public:

    zombie_father()
    {
        this->coord.second=lim_col-1;
        this->coord.first=rand()%lim_row;

        this->hp=180;

        this->attack_power=20;

        this->speed=1;

        this->last_move_time=0;

        statu=true;
    }

    int hp;

    int attack_power;

    int speed;

    int last_move_time;

    int index;

    PII coord;

    bool statu;

    bool is_dead()
    {
        return hp<=0;
    }

    void takeDamage()
    {
        hp-=30;
    }

    void move()
    {
        coord.second-=speed;
    }

    void attack()
    {
        int next_col = coord.second - 1;

        if(next_col>=0 &&
           earth[coord.first][next_col]!=nullptr)
        {
            earth[coord.first][next_col]
            ->takeDamage(attack_power);
        }
    }

    void update()
    {
        int next_col = coord.second - 1;

        if(next_col>=0 &&
           earth[coord.first][next_col]!=nullptr)
        {
            attack();
        }
        else
        {
            if(begin_time-last_move_time>=18)
            {
                move();

                last_move_time=begin_time;
            }
        }
    }
};


class shot//子弹
{
public:
//构造函数
shot(int row,int col,int power)
{
    this->coord.first=row;
    this->coord.second=col;
    this->attack_power=power;
    this->speed=1;
    this->hp=1;
    shot_table.push_back(this);
}
//属性
int attack_power;
int speed;
int index;
int hp;
PII coord;
bool statu;//0死，1活

//行为
bool is_dead()
{
    if(hp<=0)
    {
        for(int k=0;k<shot_table.size();k++)
        {
            if(shot_table[k]!=nullptr &&shot_table[k]->coord==this->coord)
            {
                shot_table[k]=nullptr;
            }
        }

        return true;
    }

    return false;
}

void move()
{
coord.second+=speed;
}

bool check_zombie()
{
for(int k=0;k<zombie_table.size();k++)
{
    if((zombie_table[k]!=nullptr)&&(*(zombie_table[k])).coord==this->coord)
    {
        return true;
    }
}
return false;
}

void attack()
{
for(int k=0;k<zombie_table.size();k++)
{
    if(zombie_table[k]!=nullptr &&zombie_table[k]->coord==this->coord)
    {
        this->hp=0;
        (*(zombie_table[k])).hp-=attack_power;
    }
}

}

void check_coord()
{
    if(this->coord.second>=lim_col)
    {
        this->hp=0;
    }
}

void update()
{
move();

 check_coord();

if(check_zombie())
{
    attack();
}
}

};


class PeaShooter : public plant_father
{
public:

int last_attack_time;

PeaShooter(int row,int col)
: plant_father(row,col)
{
this->type=1;
this->cd=10;
this->price=100;
this->hp=300;
this->attack_power=45;
this->last_attack_time=0;
}

void shoot()
{
shot* peashooter_shot=new shot(this->coord.first,this->coord.second+1,attack_power);
}

bool check_zombie()
{
for(int i=0;i<zombie_table.size();i++)
{
if((zombie_table[i] != nullptr)&&((*(zombie_table[i])).coord.first==this->coord.first))
{
return true;
}
}

return false;
}

void update() override
{
if(check_zombie())
{
if(begin_time-last_attack_time>=cd)
{
shoot();

    last_attack_time=begin_time;
}

}
}
};

class sunflower:public plant_father
{
public:
int last_make_time;
//行为

sunflower(int row,int col)
: plant_father(row,col)
{
this->type=2;
this->cd=50;
this->price=50;
this->hp=250;
this->attack_power=0;
this->last_make_time=0;
}

void make_sun()
{
    if(begin_time-last_make_time>cd)
    {
        current_sun+=50;
        last_make_time=begin_time;
    }
}

void update() override
{
    make_sun();
}
};
class WallNut : public plant_father
{
public:

WallNut(int row,int col)
: plant_father(row,col)
{
    this->type=5;

    this->hp=4000;

    this->price=50;

    this->attack_power=0;

    this->cd=0;
}

void update() override
{

}
};




