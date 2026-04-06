#include "levels/Level50m.hpp"

#include "core/Constants.hpp"

Level50m::Level50m()
{
  const float W = Constants::WINDOW_WIDTH;
  const float H = Constants::WINDOW_HEIGHT;

  SDL_Color steel{60, 90, 140, 255};

  // Same vertical layout as 25m, steel blue theme + conveyors
  platforms.push_back({0, H - 32, W, 32, 0.0f, steel});           // ground
  platforms.push_back({60, H - 128, W - 110, 16, 60.0f, steel});  // row1 — belt right
  platforms.push_back({0, H - 224, W - 110, 16, -60.0f, steel});  // row2 — belt left
  platforms.push_back({60, H - 320, W - 110, 16, 60.0f, steel});  // row3 — belt right
  platforms.push_back({0, H - 416, W - 110, 16, -60.0f, steel});  // row4 — belt left
  platforms.push_back({0, H - 512, 440, 16, 0.0f, steel});        // top

  ladders.push_back({640, H - 128, 20, 96});
  ladders.push_back({100, H - 224, 20, 96});
  ladders.push_back({570, H - 320, 20, 96});
  ladders.push_back({100, H - 416, 20, 96});
  ladders.push_back({380, H - 512, 20, 96});

  // Two fire enemies patrolling row1 and row3
  fire_enemies.emplace_back(200, H - 128 - Constants::FIRE_H, 60, 560);
  fire_enemies.emplace_back(400, H - 320 - Constants::FIRE_H, 60, 560);

  dk_x = 20;
  dk_y = H - 512 - Constants::DK_H;
  pauline_x = 340;
  pauline_y = H - 512 - Constants::PAULINE_H;
  mario_start_x = W - 80 - Constants::MARIO_W;
  mario_start_y = H - 32 - Constants::MARIO_H;
}

void Level50m::renderBackground(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 5, 10, 30, 255);
  SDL_RenderClear(renderer);
}