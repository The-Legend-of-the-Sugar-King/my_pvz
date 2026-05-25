#include "tool.hpp"

#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;
using namespace std;

int main()
{
    srand(time(nullptr));

    current_sun=500;

    auto screen = ScreenInteractive::Fullscreen();

    auto renderer = Renderer([&] {

        return hbox({
            draw_map(),
            draw_info()
        });

    });

    auto component = CatchEvent(renderer,[&](Event event) {
// =========================
// 菜单控制
// =========================
   // =========================
    // ESC暂停
    // =========================
    if(event == Event::Escape)
    {
        if(game_state == PLAYING)
        {
            game_state = PAUSE;
        }
        else if(game_state == PAUSE)
        {
            game_state = PLAYING;
        }

        return true;
    }
if(game_state==MENU)
{
    if(event == Event::Character("w"))
    {
        if(menu_index>0)
        {
            menu_index--;
        }

        return true;
    }

    if(event == Event::Character("s"))
    {
        if(menu_index<2)
        {
            menu_index++;
        }

        return true;
    }

    if(event == Event::Return)
    {
        // 开始游戏
        if(menu_index==0)
        {
            game_state=PLAYING;
        }

        // 帮助
        else if(menu_index==1)
        {

        }

        // 退出
    else if(menu_index==2)
    {
        running=false;

        screen.ExitLoopClosure()();
    }
        return true;
    }

    return false;
}

    // =========================
    // 暂停状态
    // =========================
    if(game_state == PAUSE)
    {
        // Q退出
        if(event == Event::Character("q"))
        {
            running=false;

            screen.ExitLoopClosure()();

            return true;
        }

        return true;
    }

        // 游戏结束后按Q退出
       if(game_state == GAME_OVER)
        {
         if(event == Event::Character("q"))
        {
            running=false;

            screen.ExitLoopClosure()();

            return true;
        }

            return false;
        }
        // WASD移动
        if(event == Event::Character("w"))
        {
            if(cursor_row>0)
                cursor_row--;

            return true;
        }

        if(event == Event::Character("s"))
        {
            if(cursor_row<lim_row-1)
                cursor_row++;

            return true;
        }

        if(event == Event::Character("a"))
        {
            if(cursor_col>0)
                cursor_col--;

            return true;
        }

        if(event == Event::Character("d"))
        {
            if(cursor_col<lim_col-1)
                cursor_col++;

            return true;
        }

        // 选择植物
        if(event == Event::Character("1"))
        {
            current_select=1;
            return true;
        }

        if(event == Event::Character("2"))
        {
            current_select=2;
            return true;
        }
        if(event == Event::Character("3"))
        {
            current_select=3;
            return true;
        }

        if(event == Event::Character("4"))
        {
            current_select=4;
            return true;
        }
        if(event == Event::Character("5"))
        {
            current_select=5;
            return true;
        }

        // 空格种植
        if(event == Event::Character(" "))
        {
            plant(current_select,{
                cursor_row,
                cursor_col
            });

            return true;
        }

        // X铲除
        if(event == Event::Character("x"))
        {
            remove_plant({
                cursor_row,
                cursor_col
            });

            return true;
        }

        // Enter查看属性
        if(event == Event::Return)
        {
            show_info=!show_info;

            selected_coord={
                cursor_row,
                cursor_col
            };

            return true;
        }

        return false;
    });

    thread game_loop([&] {

while(running)
{
if(game_state == PLAYING)
{
begin_time++;

    update_all();

int zombie_cd=150;

// 1分钟后
if(begin_time>600)
{
    zombie_cd=120;
}

// 2分钟后
if(begin_time>1200)
{
    zombie_cd=90;
}

// 3分钟后
if(begin_time>1800)
{
    zombie_cd=60;
}

// 4分钟后
if(begin_time>2400)
{
    zombie_cd=40;
}

// 5分钟后
if(begin_time>3000)
{
    zombie_cd=25;
}

// 刷怪
if(begin_time%zombie_cd==0)
{
    int count=1;

    if(begin_time>1200)
        count=2;

    if(begin_time>2400)
        count=3;

    if(begin_time>3600)
        count=5;

    for(int i=0;i<count;i++)
    {
        creat_zombie();
    }
}

    if(!judge())
    {
        game_state = GAME_OVER;
    }
}

screen.PostEvent(Event::Custom);

this_thread::sleep_for(
    chrono::milliseconds(100));

}


    });

    screen.Loop(component);

    game_loop.join();

    return 0;
}