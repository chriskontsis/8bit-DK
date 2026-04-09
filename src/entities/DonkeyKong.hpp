#pragma once

#include "entities/Entity.hpp"

class DonkeyKong : public Entity
{
 public:
  float barrel_timer = 0.0f;
  float anim_timer = 0.0f;
  bool  beating_chest = false;
  bool  escape_mode = false;

  DonkeyKong(float x, float y);

  void update(float dt);
  void render(SDL_Renderer* renderer) const override;
  // ret true 1 per spawn interval - game uses to create barrel
  bool readyToSpawn();
  void startEscape() { escape_mode = true; }
  float spawnX() const { return x + width + 4.0f; }
  float spawnY() const { return y + height * 0.5f; }
};