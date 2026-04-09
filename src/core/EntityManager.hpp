#pragma once

#include <memory>
#include <vector>

#include "entities/Barrel.hpp"
#include "entities/DonkeyKong.hpp"
#include "entities/Mario.hpp"
#include "entities/Pauline.hpp"
#include "input/InputHandler.hpp"
#include "levels/Level.hpp"

class EntityManager
{
 public:
  std::unique_ptr<Mario>      mario;
  std::unique_ptr<DonkeyKong> dk;
  std::unique_ptr<Pauline>    pauline;
  std::vector<Barrel>         barrels;

  void reset(const Level& level);
  void updateAll(float dt, const InputHandler& input, Level& level);
  void spawnBarrel(const Level& level);
};