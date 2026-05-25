#pragma once

#include <vector>
#include <utility>
#include <iostream>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/event.hpp>

using namespace std;

typedef pair<int,int> PII;

// =========================
// 游戏状态
// =========================
enum GameState
{
    MENU,
    PLAYING,
    PAUSE,
    GAME_OVER
};

// =========================
// 前向声明
// =========================
class shot;
class plant_father;
class zombie_father;

// =========================
// 全局变量
// =========================
extern const int lim_row;
extern const int lim_col;

extern long long sun_amount;

extern vector<vector<plant_father*>> earth;

extern vector<shot*> shot_table;

extern vector<zombie_father*> zombie_table;

extern int begin_time;

extern int current_sun;

extern int cursor_row;
extern int cursor_col;

extern int current_select;

extern int menu_index;

extern bool show_info;

extern bool running;

extern PII selected_coord;

extern GameState game_state;

// =========================
// 函数声明
// =========================
bool check_coord(PII coord);

bool check_sun(int type);

void plant(int type,PII coord);

void creat_zombie();

void update_all();

bool judge();

void clear_dead();

void remove_plant(PII coord);

ftxui::Element draw_map();

ftxui::Element draw_info();

// =========================
// 植物父类
// =========================
class plant_father
{
public:

    plant_father(int row,int col)
    {
        coord.first=row;
        coord.second=col;

        statu=true;
    }

    int hp;

    int price;

    float cd;

    int index;

    int type;

    int attack_power;

    bool statu;

    PII coord;

    virtual void update()
    {

    }

    bool is_dead()
    {
        return hp<=0;
    }

    void takeDamage(int d)
    {
        hp-=d;
    }
};

// =========================
// 僵尸类
// =========================
class zombie_father
{
public:

    zombie_father()
    {
        coord.second=lim_col-1;

        coord.first=rand()%lim_row;

        hp=180;

        attack_power=20;

        speed=1;

        last_move_time=0;

        statu=true;
    }

    int hp;

    int attack_power;

    int speed;

    int last_move_time;

    int index;

    bool statu;

    PII coord;

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
        int next_col=coord.second-1;

        if(next_col>=0 &&
           earth[coord.first][next_col]!=nullptr)
        {
            earth[coord.first][next_col]
            ->takeDamage(attack_power);
        }
    }

    void update()
    {
        int next_col=coord.second-1;

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

// =========================
// 子弹类
// =========================
class shot
{
public:

    shot(int row,int col,int power)
    {
        coord.first=row;

        coord.second=col;

        attack_power=power;

        speed=1;

        hp=1;

        shot_table.push_back(this);
    }

    int attack_power;

    int speed;

    int index;

    int hp;

    bool statu;

    PII coord;

    bool is_dead()
    {
        if(hp<=0)
        {
            for(int k=0;k<shot_table.size();k++)
            {
                if(shot_table[k]!=nullptr &&
                   shot_table[k]->coord==coord)
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
            if(zombie_table[k]!=nullptr &&
               zombie_table[k]->coord==coord)
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
            if(zombie_table[k]!=nullptr &&
               zombie_table[k]->coord==coord)
            {
                hp=0;

                zombie_table[k]->hp-=attack_power;
            }
        }
    }

    void check_coord()
    {
        if(coord.second>=lim_col)
        {
            hp=0;
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

// =========================
// 豌豆射手
// =========================
class PeaShooter : public plant_father
{
public:

    int last_attack_time;

    PeaShooter(int row,int col)
    : plant_father(row,col)
    {
        type=1;

        cd=10;

        price=100;

        hp=300;

        attack_power=45;

        last_attack_time=0;
    }

    void shoot()
    {
        shot* pea=
        new shot(coord.first,
                 coord.second+1,
                 attack_power);
    }

    bool check_zombie()
    {
        for(int i=0;i<zombie_table.size();i++)
        {
            if(zombie_table[i]!=nullptr &&
               zombie_table[i]->coord.first==coord.first)
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

// =========================
// 向日葵
// =========================
class sunflower : public plant_father
{
public:

    int last_make_time;

    sunflower(int row,int col)
    : plant_father(row,col)
    {
        type=2;

        cd=50;

        price=50;

        hp=250;

        attack_power=0;

        last_make_time=0;
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

// =========================
// 坚果墙
// =========================
class WallNut : public plant_father
{
public:

    WallNut(int row,int col)
    : plant_father(row,col)
    {
        type=5;

        hp=4000;

        price=50;

        attack_power=0;

        cd=0;
    }

    void update() override
    {

    }
};
class PotatoMine : public plant_father
{
public:

bool armed;
int plant_time;

PotatoMine(int row,int col)
: plant_father(row,col)
{
    this->type=3;

    this->hp=150;

    this->price=25;

    this->attack_power=9999;

    this->cd=50;

    armed=false;

    plant_time=begin_time;
}

void explode()
{
    for(int i=0;i<zombie_table.size();i++)
    {
        if(zombie_table[i]!=nullptr &&
           zombie_table[i]->coord==this->coord)
        {
            delete zombie_table[i];

            zombie_table[i]=nullptr;
        }
    }

    this->hp=0;
}

void update() override
{
    // 3秒后激活
    if(begin_time-plant_time>=30)
    {
        armed=true;
    }

    if(!armed)
    {
        return;
    }

    for(int i=0;i<zombie_table.size();i++)
    {
        if(zombie_table[i]!=nullptr &&
           zombie_table[i]->coord==this->coord)
        {
            explode();
            break;
        }
    }
}
};
class CherryBomb : public plant_father
{
public:

bool exploded;

CherryBomb(int row,int col)
: plant_father(row,col)
{
    this->type=4;

    this->hp=999;

    this->price=200;

    this->attack_power=9999;

    this->cd=100;

    exploded=false;
}

void boom()
{
    for(int i=0;i<zombie_table.size();i++)
    {
        if(zombie_table[i]!=nullptr)
        {
            int zr=zombie_table[i]->coord.first;
            int zc=zombie_table[i]->coord.second;

            if(abs(zr-this->coord.first)<=1 &&
               abs(zc-this->coord.second)<=1)
            {
                delete zombie_table[i];

                zombie_table[i]=nullptr;
            }
        }
    }

    exploded=true;

    this->hp=0;
}

void update() override
{
    if(!exploded)
    {
        boom();
    }
}
};
