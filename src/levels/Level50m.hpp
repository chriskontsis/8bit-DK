#pragma once

#include "levels/Level.hpp"

class Level50m : public Level
{
 public:
  Level50m();
  void renderBackground(SDL_Renderer* renderer) override;
  std::string name() const override { return "50m"; }
};