#pragma once

#include "../tool.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

ftxui::Component CreateGameComponent(
ftxui::Component renderer,
ftxui::ScreenInteractive& screen
);
