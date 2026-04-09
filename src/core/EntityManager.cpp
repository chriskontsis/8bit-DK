#include "core/EntityManager.hpp"

#include "core/Constants.hpp"

void EntityManager::reset(const Level& level)
{
  barrels.clear();
  mario = std::make_unique<Mario>(level.mario_start_x, level.mario_start_y);
  dk = std::make_unique<DonkeyKong>(level.dk_x, level.dk_y);
  pauline = std::make_unique<Pauline>(level.pauline_x, level.pauline_y);
}

void EntityManager::updateAll(float dt, const InputHandler& input, Level& level)
{
  mario->update(dt, input, level.platforms, level.ladders);
  dk->update(dt);
  pauline->update(dt);

  for (auto& b : barrels)
    b.update(dt, level.platforms, level.ladders);
  for (auto& f : level.fire_enemies)
    f.update(dt);

  std::erase_if(barrels, [](const Barrel& b) { return !b.active; });
}

void EntityManager::spawnBarrel(const Level& level)
{
  if (level.usesRivets() || !dk->readyToSpawn())
    return;

  float  spawn_y = level.platforms.back().y - Constants::BARREL_H;
  Barrel b(dk->spawnX(), spawn_y, 1.0f);
  b.on_ground = true;
  b.prev_on_ground = true;
  barrels.push_back(std::move(b));
}