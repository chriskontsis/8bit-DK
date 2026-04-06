#include "levels/Level100m.hpp"

#include "core/Constants.hpp"

Level100m::Level100m()
{
    const float W = Constants::WINDOW_WIDTH;
    const float H = Constants::WINDOW_HEIGHT;

    SDL_Color concrete{80, 80, 80, 255};

    platforms.push_back({0,   H - 32,  W,       32,  0.0f, concrete});
    platforms.push_back({60,  H - 128, W - 110, 16,  0.0f, concrete});
    platforms.push_back({0,   H - 224, W - 110, 16,  0.0f, concrete});
    platforms.push_back({60,  H - 320, W - 110, 16,  0.0f, concrete});
    platforms.push_back({0,   H - 416, W - 110, 16,  0.0f, concrete});
    platforms.push_back({0,   H - 512, 440,     16,  0.0f, concrete});

    ladders.push_back({640, H - 128, 20, 96});
    ladders.push_back({100, H - 224, 20, 96});
    ladders.push_back({570, H - 320, 20, 96});
    ladders.push_back({100, H - 416, 20, 96});
    ladders.push_back({380, H - 512, 20, 96});

    // 6 rivets spread across platforms
    rivets.push_back({150,  H - 128 - 14, 14, 14, false});
    rivets.push_back({400,  H - 128 - 14, 14, 14, false});
    rivets.push_back({200,  H - 224 - 14, 14, 14, false});
    rivets.push_back({500,  H - 320 - 14, 14, 14, false});
    rivets.push_back({150,  H - 416 - 14, 14, 14, false});
    rivets.push_back({300,  H - 512 - 14, 14, 14, false});

    // Two fire enemies patrolling
    fire_enemies.emplace_back(300, H - 224 - Constants::FIRE_H, 60, 560);
    fire_enemies.emplace_back(200, H - 416 - Constants::FIRE_H, 60, 460);

    dk_x          = 20;
    dk_y          = H - 512 - Constants::DK_H;
    pauline_x     = 340;
    pauline_y     = H - 512 - Constants::PAULINE_H;
    mario_start_x = W - 80 - Constants::MARIO_W;
    mario_start_y = H - 32  - Constants::MARIO_H;
}

void Level100m::renderBackground(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
    SDL_RenderClear(renderer);
}
