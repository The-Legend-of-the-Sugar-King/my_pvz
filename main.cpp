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

    if(begin_time%150==0)
    {
        creat_zombie();
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