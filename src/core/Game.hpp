#pragma once

#include <SDL2/SDL.h>

#include <array>
#include <expected>
#include <memory>
#include <string>

#include "core/EntityManager.hpp"
#include "core/GameRenderer.hpp"
#include "core/GameState.hpp"
#include "input/InputHandler.hpp"
#include "levels/Level.hpp"
#include "ui/ScoreManager.hpp"

class Game
{
 public:
  Game() = default;
  ~Game() { SDL_Quit(); }

  std::expected<void, std::string> init();
  void run();

 private:
  using WindowPtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
  using RendererPtr = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;

  // SDL
  WindowPtr   window_{nullptr, SDL_DestroyWindow};
  RendererPtr renderer_{nullptr, SDL_DestroyRenderer};

  // Systems
  std::unique_ptr<GameRenderer> game_renderer_;
  EntityManager                 entities_;
  InputHandler                  input_;
  ScoreManager                  score_{"scores/highscore.txt"};

  // State
  GameState              state_ = GameState::MENU;
  bool                   running_ = true;
  int                    current_level_ = 0;
  std::unique_ptr<Level> level_;
  float                  state_timer_ = 0.0f;
  float                  escape_timer_ = 0.0f;

  static constexpr std::array<int, 3> LEVEL_NUMS{25, 50, 100};

  void fixedUpdate(float dt);
  void render();

  void updateMenu();
  void updatePlaying(float dt);
  void updateEscaping(float dt);
  void updateLevelComplete(float dt);
  void updateGameOver();
  void updateWin();

  void loadLevel(int idx);
};