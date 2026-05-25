#include "draw.hpp"

using namespace ftxui;
using namespace std;

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
                        text("1/2/3/4/5 选择植物") | center
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
                    else if(earth[i][j]->type==3)
                    {
                        icon="💣";
                        name="地雷";
                    }
                    else if(earth[i][j]->type==4)
                    {
                        icon="🍒";
                        name="炸弹";
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

    // =========================
    // 时间系统
    // =========================
    int second = begin_time / 10;

    string time_text =
        to_string(second / 60)
        + ":" +
        (second % 60 < 10 ? "0" : "")
        + to_string(second % 60);

    // =========================
    // 难度系统
    // =========================
    string difficulty = "简单";

    if(begin_time > 600)
    {
        difficulty = "普通";
    }

    if(begin_time > 1200)
    {
        difficulty = "困难";
    }

    if(begin_time > 2000)
    {
        difficulty = "地狱";
    }

    return vbox({

        hbox({

            text("时间: " + time_text)
            | bold,

            filler(),

            text("难度: " + difficulty)
            | bold,

            filler(),

            text("阳光: " + to_string(current_sun))
            | bold,

            filler(),

            text("僵尸: " +
            to_string(zombie_table.size()))
            | bold

        }),

        separator(),

        text("WASD移动  1/2/3/4/5选择植物  空格种植  X铲除  ESC暂停")
        | center
        | bold,

        separator(),

        vbox(rows)

    });
}

Element draw_info()
{
    vector<Element> info;

    info.push_back(text("植物列表") | bold | center);

    info.push_back(separator());

    info.push_back(vbox({
        text("🌱 豌豆射手"),
        text("☀100"),
        separator()
    }));

    info.push_back(vbox({
        text("🌻 向日葵"),
        text("☀50"),
        separator()
    }));

    info.push_back(vbox({
        text("💣 土豆地雷"),
        text("☀25"),
        separator()
    }));

    info.push_back(vbox({
        text("🍒 樱桃炸弹"),
        text("☀200"),
        separator()
    }));

    info.push_back(vbox({
        text("🧱 坚果墙"),
        text("☀50"),
        separator()
    }));

    string current_name;

    if(current_select==1)
        current_name="🌱 豌豆";

    else if(current_select==2)
        current_name="🌻 向日葵";

    else if(current_select==3)
        current_name="💣 土豆地雷";

    else if(current_select==4)
        current_name="🍒 樱桃炸弹";

    else if(current_select==5)
        current_name="🧱 坚果";

    info.push_back(text("当前选择:") | bold);

    info.push_back(text(current_name));

    info.push_back(separator());

    info.push_back(text("阳光: "+to_string(current_sun)));

    info.push_back(separator());

    info.push_back(text("WASD 移动"));
    info.push_back(text("1/2/3/4/5 选择植物"));
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
