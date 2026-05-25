#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <thread>
#include <chrono>
#include "tool.hpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/event.hpp>
using namespace ftxui;
using namespace std;
const int lim_row=6,lim_col=9;
long long sun_amount=0;
vector<vector<plant_father*>> earth(6,vector<plant_father*>(9));//map,从0开始
vector<shot*> shot_table;
//vector<plant_father*>plant_table;
vector<zombie_father*>zombie_table;
int begin_time=0;
int current_sun=0;
int cursor_row = 0;
int cursor_col = 0;
int current_select = 1;
int menu_index=0;
//bool game_over=false;
bool show_info = false;
bool running=true;
PII selected_coord = {0,0};
GameState game_state=MENU;
//检查坐标是否违法
bool check_coord(PII coord)
{
if((coord.first>=lim_row||coord.first<0)||(coord.second>=lim_col||coord.second<0))
{
    return false;
}
else
{
    return true;
}
}
//检查阳光是否充足  1为豌豆射手 100 2为向日葵 50 3为土豆地雷 25 4为樱桃炸弹 200 5为坚果 50
bool check_sun(int type)
{
if(type==1)
{
    if(current_sun>=100)
{
    current_sun-=100;
    return true;
}
else    return false;
}
else if(type==2)
{
    if(current_sun>=50)
{
    current_sun-=50;
    return true;
}
else    return false;
}
else if(type==3)
{
    if(current_sun>=25)
{
    current_sun-=25;
    return true;
}
else    return false;
}
else if(type==4)
{
    if(current_sun>=200)
{
    current_sun-=200;
    return true;
}
else    return false;
}
else if(type==5)
{
    if(current_sun>=50)
{
    current_sun-=50;
    return true;
}
else    return false;
}
return false;
}
//种植植物 0为空 1为豌豆射手 2为向日葵 3为土豆地雷 4为樱桃炸弹 5为坚果
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
else if(earth[coord.first][coord.second]==nullptr)
{
    if(type==1)
    {
       earth[coord.first][coord.second]=new PeaShooter(coord.first,coord.second);  
    }
    else if(type==2)
{
    earth[coord.first][coord.second]=new sunflower(coord.first,coord.second);
}
else if(type==5)
{
    earth[coord.first][coord.second]=new WallNut(coord.first,coord.second);
}
    // else if()//后续加模块
    // {

    // }
}
else
{
cout << "该位置已经有植物！" << endl;
}
}

//绘制地图
Element draw_map()
{
    // =========================
    // 主菜单
    // =========================
    if(game_state == MENU)
    {
        Element start = text("开始游戏");
        Element help = text("帮助");
        Element quit = text("退出游戏");

        if(menu_index == 0)
            start = start | inverted | bold;

        if(menu_index == 1)
            help = help | inverted | bold;

        if(menu_index == 2)
            quit = quit | inverted | bold;

        return vbox({
            filler(),

            hbox({
                filler(),

                window(
                    text("Plants VS Zombies") | bold | center,

                    vbox({
                        separator(),
                        start | center,
                        separator(),
                        help | center,
                        separator(),
                        quit | center,
                        separator(),
                        text("W/S 选择") | center,
                        text("Enter 确认") | center
                    })

                ) | size(WIDTH, EQUAL, 40),

                filler()
            }),

            filler()
        });
    }

    // =========================
    // 暂停菜单
    // =========================
    if(game_state == PAUSE)
    {
        return vbox({

            filler(),

            hbox({
                filler(),

                window(
                    text("游戏暂停") | bold | center,

                    vbox({
                        separator(),
                        text("ESC 继续游戏") | center,
                        separator(),
                        text("Q 退出游戏") | center,
                        separator(),
                        text("WASD 移动") | center,
                        separator(),
                        text("1/2/5 选择植物") | center
                    })

                ) | size(WIDTH, EQUAL, 40),

                filler()
            }),

            filler()

        });
    }
    // =========================
    // 游戏结束
    // =========================
    if(game_state == GAME_OVER)
    {
        return vbox({

            filler(),

            text("你的脑子被僵尸吃掉了")
            | bold
            | center,

            text("GAME OVER")
            | bold
            | center,

            text("按 Q 退出")
            | center,

            filler()

        }) | borderDouble;
    }

    // =========================
    // 游戏地图
    // =========================
    vector<Element> rows;

    for(int i=0;i<lim_row;i++)
    {
        vector<Element> cols;

        for(int j=0;j<lim_col;j++)
        {
            string icon=" ";
            string name="草地";

            bool has_zombie=false;

            // 僵尸
            for(int k=0;k<zombie_table.size();k++)
            {
                if(zombie_table[k]!=nullptr &&
                   zombie_table[k]->coord.first==i &&
                   zombie_table[k]->coord.second==j)
                {
                    icon="🧟";
                    name="普僵";
                    has_zombie=true;
                    break;
                }
            }

            // 子弹
            if(!has_zombie)
            {
                bool has_shot=false;

                for(int k=0;k<shot_table.size();k++)
                {
                    if(shot_table[k]!=nullptr &&
                       shot_table[k]->coord.first==i &&
                       shot_table[k]->coord.second==j)
                    {
                        icon="•";
                        name="子弹";
                        has_shot=true;
                        break;
                    }
                }

                // 植物
                if(!has_shot &&
                   earth[i][j]!=nullptr)
                {
                    if(earth[i][j]->type==1)
                    {
                        icon="🌱";
                        name="豌豆";
                    }
                    else if(earth[i][j]->type==2)
                    {
                        icon="🌻";
                        name="向日葵";
                    }
                    else if(earth[i][j]->type==5)
                    {
                        icon="🧱";
                        name="坚果";
                    }
                }
            }

            Element cell =
                window(
                    text(icon),

                    vbox({
                        text(icon) | center,
                        separator(),
                        text(name) | center
                    })
                )
                | size(WIDTH,EQUAL,12);

            // 光标高亮
            if(i==cursor_row &&
               j==cursor_col)
            {
                cell =
                    cell
                    | borderDouble
                    | bold;
            }

            cols.push_back(cell);
        }

        rows.push_back(hbox(cols));
    }

    return vbox({

        text("WASD移动  1/2/5选择植物  空格种植  X铲除  ESC暂停")
        | center
        | bold,

        separator(),

        vbox(rows)

    });
}

// void draw()
// {
// cout<<"sun:"<<current_sun<<endl;
// for(int i=0;i<lim_row;i++)
// {

// for(int j=0;j<lim_col;j++)
// {
//     int size = 0;
//    for(int k=0;k<zombie_table.size();k++)
// {
// if(zombie_table[k]!=nullptr)
// {
// if((zombie_table[k]->coord.first==i)
// &&(zombie_table[k]->coord.second==j))
// {
// cout<<"Z";
// cout<<zombie_table[k]->hp/30;
// cout<<" ";

//         size=1;
//     }
// }

// }


// if(size==1)
// {
//     continue;
// }

// bool has_shot=false;

// for(int k=0;k<shot_table.size();k++)
// {
//     if(shot_table[k]!=nullptr)
//     {
//         if(shot_table[k]->coord.first==i &&
//            shot_table[k]->coord.second==j)
//         {
//             cout<<"* ";
//             has_shot=true;
//             break;
//         }
//     }
// }

// if(has_shot)
// {
//     continue;
// }

//     if(earth[i][j]!=nullptr)
// {
// if(earth[i][j]->type==1)
// {
// cout<<"P";
// cout<<earth[i][j]->hp/100;
// cout<<" ";
// }

// else if(earth[i][j]->type==2)
// {
// cout<<"S";
// cout<<earth[i][j]->hp/100;
// cout<<" "
// }

// }
// else if(earth[i][j]->type==5)
// {
// cout<<"W";
// cout<<earth[i][j]->hp/500;
// cout<<" ";
// }
// else
// {
// cout<<". ";
// }
// }
// cout<<endl;
// }


// }//测试版
//生成僵尸
void creat_zombie()
{
zombie_father *z=new zombie_father;
zombie_table.push_back(z);
}

void player_input(ftxui::Event event)
{
    // ESC 暂停
    if(event == ftxui::Event::Escape)
    {
        if(game_state == PLAYING)
        {
            game_state = PAUSE;
        }
        else if(game_state == PAUSE)
        {
            game_state = PLAYING;
        }
    }

    // Q 退出
    if(event == ftxui::Event::Character('q'))
    {
        running = false;
    }

    // 只有游戏中才能操作
    if(game_state != PLAYING)
    {
        return;
    }

    // 光标移动
    if(event == ftxui::Event::ArrowUp)
    {
        cursor_row--;
    }

    if(event == ftxui::Event::ArrowDown)
    {
        cursor_row++;
    }

    if(event == ftxui::Event::ArrowLeft)
    {
        cursor_col--;
    }

    if(event == ftxui::Event::ArrowRight)
    {
        cursor_col++;
    }

    // 防止越界
    cursor_row=max(0,min(cursor_row,lim_row-1));
    cursor_col=max(0,min(cursor_col,lim_col-1));

    // 选择植物
    if(event == ftxui::Event::Character('1'))
    {
        current_select=1;
    }

    if(event == ftxui::Event::Character('2'))
    {
        current_select=2;
    }

    if(event == ftxui::Event::Character('5'))
    {
        current_select=5;
    }

    // 回车种植
    if(event == ftxui::Event::Return)
    {
        plant(current_select,{cursor_row,cursor_col});
    }
}


//时间系统
// void game_clock()
// {
// while(true)
// {
//     begin_time++;

//     system("clear");

//     draw();

//     player_input();

//     update_all();


// if(begin_time%50==0)
// {
//     creat_zombie();
// }

//     if(!judge())
//     {
//         break;
//     }

//     this_thread::sleep_for(chrono::milliseconds(100));
// }

// }

Element draw_info()
{
    vector<Element> info;

    info.push_back(text("植物列表") | bold | center);

    info.push_back(separator());

    info.push_back(vbox({
        text("🌱 豌豆射手"),
        text("☀100   CD:75"),
        separator()
    }));

    info.push_back(vbox({
        text("🌻 向日葵"),
        text("☀50   CD:50"),
        separator()
    }));

    info.push_back(vbox({
        text("🧱 坚果墙"),
        text("☀50   CD:0"),
        separator()
    }));

    string current_name;

    if(current_select==1)
        current_name="🌱 豌豆";
    else if(current_select==2)
        current_name="🌻 向日葵";
    else if(current_select==5)
        current_name="🧱 坚果";

    info.push_back(text("当前选择:") | bold);

    info.push_back(text(current_name));

    info.push_back(separator());

    info.push_back(text("阳光: "+to_string(current_sun)));

    info.push_back(separator());

    info.push_back(text("WASD 移动"));
    info.push_back(text("1/2/5 选择植物"));
    info.push_back(text("Space 种植"));
    info.push_back(text("X 铲除"));
    info.push_back(text("Enter 查看属性"));

    if(show_info)
    {
        int r = selected_coord.first;
        int c = selected_coord.second;

        if(earth[r][c]!=nullptr)
        {
            info.push_back(separator());

            info.push_back(text("当前属性") | bold);

            info.push_back(text(
                "HP: "+to_string(earth[r][c]->hp)
            ));

            info.push_back(text(
                "ATK: "+to_string(earth[r][c]->attack_power)
            ));

            info.push_back(text(
                "PRICE: "+to_string(earth[r][c]->price)
            ));
        }
    }

    return window(
        text("信息栏"),
        vbox(info)
    ) | size(WIDTH,EQUAL,30);
}

//阳光系统
void sun_system(int begin,int end)
{
    if(begin-end>=500)
    current_sun+=50;
}
void sun_system(bool a)
{
    if(a)
    {
        current_sun+=50;
    }
}

//判定系统
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


void clear_dead()
{
    // 清理子弹
    for(int i=0;i<shot_table.size();i++)
    {
        if(shot_table[i]==nullptr)
        {
            shot_table.erase(shot_table.begin()+i);
            i--;
        }
    }

    // 清理僵尸
    for(int i=0;i<zombie_table.size();i++)
    {
        if(zombie_table[i]==nullptr)
        {
            zombie_table.erase(zombie_table.begin()+i);
            i--;
        }
    }
}

//全流程系统
void update_all()
{

//更新植物
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

//更新子弹
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

//更新僵尸
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
void remove_plant(PII coord)
{
    if(check_coord(coord) &&
       earth[coord.first][coord.second]!=nullptr)
    {
        delete earth[coord.first][coord.second];

        earth[coord.first][coord.second]=nullptr;
    }
}