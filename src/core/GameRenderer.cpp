#include "core/GameRenderer.hpp"

#include <array>
#include <memory>

#include "SDL2/SDL_render.h"
#include "core/EntityManager.hpp"
#include "core/GameState.hpp"
#include "ui/ScoreManager.hpp"

GameRenderer::GameRenderer(SDL_Renderer* renderer)
    : renderer_(renderer), ui_(std::make_unique<UI>(renderer))
{
}

void GameRenderer::render(GameState state, Level* level, const EntityManager& entities,
                          const ScoreManager& score, int current_level,
                          const std::array<int, 3>& level_nums)
{
  if (state == GameState::MENU)
  {
    ui_->renderMenu(renderer_);
    SDL_RenderPresent(renderer_);
    return;
  }

  if (level)
    level->renderBackground(renderer_);

  if (state == GameState::PLAYING || state == GameState::DK_ESCAPING)
  {
    for (const auto& p : level->platforms)
      p.render(renderer_);
    for (const auto& l : level->ladders)
      l.render(renderer_);
    for (const auto& r : level->rivets)
      r.render(renderer_);

    entities.dk->render(renderer_);
    entities.pauline->render(renderer_);
    for (const auto& b : entities.barrels)
      b.render(renderer_);
    for (const auto& f : level->fire_enemies)
      f.render(renderer_);
    entities.mario->render(renderer_);

    ui_->renderHUD(score, level_nums[current_level]);

    if (state == GameState::LEVEL_COMPLETE)
    {
      int next = (current_level + 1 < 3) ? level_nums[current_level + 1] : 0;
      ui_->renderLevelComplete(renderer_, next);
    }
  }
  else if (state == GameState::GAME_OVER)
    ui_->renderGameOver(renderer_);
  else if (state == GameState::WIN)
    ui_->renderWin(renderer_, score.score);

  SDL_RenderPresent(renderer_);
}