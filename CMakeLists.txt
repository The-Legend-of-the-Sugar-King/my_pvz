cmake_minimum_required(VERSION 3.14)

project(world)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(/home/mon/FTXUI FTXUI_build)

add_executable(world

    main.cpp
    world.cpp

    ui/draw.cpp
    ui/input.cpp
)

target_link_libraries(world PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
)