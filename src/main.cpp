#define STB_IMAGE_IMPLEMENTATION
#define STB_PERLIN_IMPLEMENTATION
#include "utils/stb_image.h"
#include "utils/stb_perlin.h"
#include "display/game_window.hpp"
#include <iostream>

int main() {
    std::cout << "LAUNCHING" << std::endl;
    // Create the game window with the specified size and title
    GameWindow gw = GameWindow{ 1728, 972, "opengl-imgui-cmake-template by @dcronqvist" };
    return gw.Run();
}