#pragma once

#include "levels/Level.hpp"

class Level100m : public Level
{
public:
    Level100m();
    void renderBackground(SDL_Renderer* renderer) override;
    std::string name() const override { return "100m"; }
};
