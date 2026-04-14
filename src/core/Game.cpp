#include "core/Game.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
#include "core/CollisionSystem.hpp"
#include "core/Constants.hpp"
#include "core/GameState.hpp"
#include "geometry/Platform.hpp"
#include "levels/Level100m.hpp"
#include "levels/Level25m.hpp"
#include "levels/Level50m.hpp"

std::expected<void, std::string> Game::init()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    return std::unexpected{"SDL_Init failed: " + std::string{SDL_GetError()}};

  window_.reset(SDL_CreateWindow("Donkey Kong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT,
                                 SDL_WINDOW_SHOWN));
  if (!window_)
    return std::unexpected{"SDL_CreateWindow failed: " + std::string{SDL_GetError()}};

  renderer_.reset(
      SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
  if (!renderer_)
    return std::unexpected{"SDL_CreateRenderer failed: " + std::string{SDL_GetError()}};

  game_renderer_ = std::make_unique<GameRenderer>(renderer_.get());
  return {};
}

void Game::run()
{
  Uint64 prev = SDL_GetPerformanceCounter();
  float  freq = static_cast<float>(SDL_GetPerformanceFrequency());
  float  accum = 0.0f;

  while (running_)
  {
    Uint64 now = SDL_GetPerformanceCounter();
    float  dt = static_cast<float>(now - prev) / freq;
    prev = now;

    if (dt > Constants::MAX_ACCUM)
      dt = Constants::MAX_ACCUM;
    accum += dt;

    input_.update();
    if (input_.quitRequested())
      running_ = false;

    while (accum >= Constants::FIXED_DT)
    {
      fixedUpdate(Constants::FIXED_DT);
      input_.clearPressed();
      accum -= Constants::FIXED_DT;
    }

    render();
  }
}

void Game::fixedUpdate(float dt)
{
  switch (state_)
  {
    case GameState::MENU:
      updateMenu();
      break;
    case GameState::PLAYING:
      updatePlaying(dt);
      break;
    case GameState::DK_ESCAPING:
      updateEscaping(dt);
      break;
    case GameState::LEVEL_COMPLETE:
      updateLevelComplete(dt);
      break;
    case GameState::GAME_OVER:
      updateGameOver();
      break;
    case GameState::WIN:
      updateWin();
      break;
  }
}

void Game::updateMenu()
{
  if (input_.justPressed(SDL_SCANCODE_RETURN))
  {
    current_level_ = 0;
    score_.reset();
    loadLevel(current_level_);
    state_ = GameState::PLAYING;
  }
}

void Game::updatePlaying(float dt)
{
  if (!entities_.mario || !entities_.dk || !level_)
    return;

  entities_.updateAll(dt, input_, *level_);
  entities_.spawnBarrel(*level_);

  if (!entities_.mario->isDead() &&
      CollisionSystem::checkEnemyHit(*entities_.mario, entities_.barrels, level_->fire_enemies))
  {
    score_.loseLife();
    entities_.mario->kill();
  }

  // Conveyor belt
  if (entities_.mario->on_ground)
  {
    for (const auto& p : level_->platforms)
    {
      if (p.conveyor_speed == 0.0f)
        continue;
      SDL_FRect pr = p.rect();
      SDL_FRect mr = entities_.mario->rect();
      if (mr.x + mr.w > pr.x && mr.x < pr.x + pr.w &&
          entities_.mario->y + entities_.mario->height >= p.y &&
          entities_.mario->y + entities_.mario->height <= p.y + p.height + 2)
      {
        entities_.mario->x += p.conveyor_speed * Constants::FIXED_DT;
      }
    }
  }

  // Tick hole open delays
  for (auto& p : level_->platforms)
    if (p.hole_open_delay > 0.0f)
      p.hole_open_delay -= dt;

  // Rivet collection
  for (auto& r : level_->rivets)
  {
    if (!r.collected && entities_.mario->x + entities_.mario->width > r.x &&
        entities_.mario->x < r.x + r.width && entities_.mario->y + entities_.mario->height >= r.y &&
        entities_.mario->y < r.y + r.height)
    {
      r.collected = true;
      score_.addPoints(Constants::POINTS_RIVET);
      if (r.platform_idx >= 0 && r.platform_idx < static_cast<int>(level_->platforms.size()))
      {
        auto& p = level_->platforms[r.platform_idx];
        if (!p.hole1)
          p.hole1 = Platform::Hole(r.x, r.width);
        else
          p.hole2 = Platform::Hole(r.x, r.width);
        p.hole_open_delay = 1.2f;
      }
    }
  }

  // Win condition
  if (level_->usesRivets())
  {
    if (level_->allRivetsGone())
    {
      score_.addPoints(Constants::POINTS_LEVEL_CLEAR);
      state_timer_ = Constants::LEVEL_COMPLETE_DELAY;
      state_ = GameState::LEVEL_COMPLETE;
    }
  }
  else
  {
    if (entities_.mario->overlaps(*entities_.pauline))
    {
      score_.addPoints(Constants::POINTS_LEVEL_CLEAR);
      escape_timer_ = 0.0f;
      entities_.dk->startEscape();
      state_ = GameState::DK_ESCAPING;
    }
  }

  // Death
  if (entities_.mario->deathDone())
  {
    if (score_.isGameOver())
      state_ = GameState::GAME_OVER;
    else
      entities_.reset(*level_);
  }
}

void Game::updateEscaping(float dt)
{
  constexpr float ESCAPE_SPEED = 80.0f;

  entities_.dk->y -= ESCAPE_SPEED * dt;
  entities_.pauline->x =
      entities_.dk->x + entities_.dk->width * 0.5f - entities_.pauline->width * 0.5f;
  entities_.pauline->y = entities_.dk->y - entities_.pauline->height;

  escape_timer_ += dt;
  if (escape_timer_ >= 2.0f)
  {
    state_timer_ = Constants::LEVEL_COMPLETE_DELAY;
    state_ = GameState::LEVEL_COMPLETE;
  }
}

void Game::updateLevelComplete(float dt)
{
  state_timer_ -= dt;
  if (state_timer_ <= 0.0f)
  {
    ++current_level_;
    if (current_level_ >= 3)
      state_ = GameState::WIN;
    else
    {
      loadLevel(current_level_);
      state_ = GameState::PLAYING;
    }
  }
}

void Game::updateGameOver()
{
  if (input_.justPressed(SDL_SCANCODE_RETURN))
  {
    current_level_ = 0;
    score_.reset();
    loadLevel(current_level_);
    state_ = GameState::PLAYING;
  }
}

void Game::updateWin()
{
  if (input_.justPressed(SDL_SCANCODE_RETURN))
    state_ = GameState::MENU;
}

void Game::render()
{
  game_renderer_->render(state_, level_.get(), entities_, score_, current_level_, LEVEL_NUMS);
}

void Game::loadLevel(int index)
{
  switch (index)
  {
    case 0:
      level_ = std::make_unique<Level25m>();
      break;
    case 1:
      level_ = std::make_unique<Level50m>();
      break;
    case 2:
      level_ = std::make_unique<Level100m>();
      break;
    default:
      return;
  }
  entities_.reset(*level_);
}