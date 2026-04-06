#include "levels/Level25m.hpp"

#include "core/Constants.hpp"

Level25m::Level25m()
{
  const float W = Constants::WINDOW_WIDTH;
  const float H = Constants::WINDOW_HEIGHT;

  SDL_Color brown{180, 120, 60, 255};

  platforms.push_back({0, H - 32, W, 32, 0.0f, brown, 0.0f});          // ground — flat
  platforms.push_back({60, H - 128, W - 60, 16, 0.0f, brown, 0.01f});  // row 1
  platforms.push_back({0, H - 224, W - 110, 16, 0.0f, brown, 0.01f});  // row 2
  platforms.push_back({60, H - 320, W - 60, 16, 0.0f, brown, 0.01f});  // row 3
  platforms.push_back({0, H - 416, W - 110, 16, 0.0f, brown, 0.01f});  // row 4
  platforms.push_back({0, H - 512, 440, 16, 0.0f, brown, 0.01f});      // top

  // Ground → Row 1
  ladders.push_back({100, H - 128, 20, 96});
  ladders.push_back({620, H - 128, 20, 96});
  // Row 1 → Row 2
  ladders.push_back({100, H - 224, 20, 96});
  ladders.push_back({570, H - 224, 20, 96});
  // Row 2 → Row 3
  ladders.push_back({200, H - 320, 20, 96});
  ladders.push_back({550, H - 320, 20, 96});
  // Row 3 → Row 4
  ladders.push_back({100, H - 416, 20, 96});
  ladders.push_back({500, H - 416, 20, 96});
  // Row 4 → Top
  ladders.push_back({280, H - 512, 20, 96});
  ladders.push_back({420, H - 512, 20, 96});

  dk_x = 20;
  dk_y = H - 512 - Constants::DK_H;
  pauline_x = 340;
  pauline_y = H - 512 - Constants::PAULINE_H;
  mario_start_x = W - 80 - Constants::MARIO_W;
  mario_start_y = H - 32 - Constants::MARIO_H;
}

void Level25m::renderBackground(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
}