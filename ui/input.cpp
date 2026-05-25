#include "input.hpp"

using namespace ftxui;
using namespace std;

Component CreateGameComponent(
    Component renderer,
    ScreenInteractive& screen
)
{
    return CatchEvent(renderer,[&](Event event) {

        // ESC暂停
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

        // 主菜单
        if(game_state == MENU)
        {
            if(event == Event::Character("w"))
            {
                if(menu_index > 0)
                {
                    menu_index--;
                }

                return true;
            }

            if(event == Event::Character("s"))
            {
                if(menu_index < 2)
                {
                    menu_index++;
                }

                return true;
            }

            if(event == Event::Return)
            {
                if(menu_index == 0)
                {
                    game_state = PLAYING;
                }
                else if(menu_index == 2)
                {
                    running = false;

                    screen.ExitLoopClosure()();
                }

                return true;
            }

            return false;
        }

        // 暂停
        if(game_state == PAUSE)
        {
            if(event == Event::Character("q"))
            {
                running = false;

                screen.ExitLoopClosure()();

                return true;
            }

            return true;
        }

        // 游戏结束
        if(game_state == GAME_OVER)
        {
            if(event == Event::Character("q"))
            {
                running = false;

                screen.ExitLoopClosure()();

                return true;
            }

            return false;
        }

        // WASD
        if(event == Event::Character("w"))
        {
            if(cursor_row > 0)
                cursor_row--;

            return true;
        }

        if(event == Event::Character("s"))
        {
            if(cursor_row < lim_row - 1)
                cursor_row++;

            return true;
        }

        if(event == Event::Character("a"))
        {
            if(cursor_col > 0)
                cursor_col--;

            return true;
        }

        if(event == Event::Character("d"))
        {
            if(cursor_col < lim_col - 1)
                cursor_col++;

            return true;
        }

        // 选择植物
        if(event == Event::Character("1"))
        {
            current_select = 1;
            return true;
        }

        if(event == Event::Character("2"))
        {
            current_select = 2;
            return true;
        }

        if(event == Event::Character("5"))
        {
            current_select = 5;
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

        // 查看属性
        if(event == Event::Return)
        {
            show_info = !show_info;

            selected_coord = {
                cursor_row,
                cursor_col
            };

            return true;
        }

        return false;
    });
}