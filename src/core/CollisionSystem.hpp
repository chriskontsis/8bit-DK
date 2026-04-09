#pragma once
#include <algorithm>
#include <vector>

#include "entities/Barrel.hpp"
#include "entities/FireEnemy.hpp"
#include "entities/Mario.hpp"

namespace CollisionSystem
{

inline bool checkEnemyHit(const Mario& mario, const std::vector<Barrel>& barrels,
                          const std::vector<FireEnemy>& fire_enemies)
{
  auto hitsMario = [&](const auto& e) { return e.active && mario.overlaps(e); };
  return std::ranges::any_of(barrels, hitsMario) || std::ranges::any_of(fire_enemies, hitsMario);
}

}  // namespace CollisionSystem
