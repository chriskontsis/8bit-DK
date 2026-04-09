#pragma once

#include <SDL2/SDL.h>

#include <array>
#include <memory>

#include "SDL2/SDL_render.h"
#include "core/EntityManager.hpp"
#include "core/GameState.hpp"
#include "levels/Level.hpp"
#include "ui/ScoreManager.hpp"
#include "ui/UI.hpp"

class GameRenderer
{
 public:
  explicit GameRenderer(SDL_Renderer* renderer);

  void render(GameState state, Level* level, const EntityManager& entities,
              const ScoreManager& score, int current_level, const std::array<int, 3>& level_nums);

 private:
  SDL_Renderer*       renderer_;
  std::unique_ptr<UI> ui_;
};