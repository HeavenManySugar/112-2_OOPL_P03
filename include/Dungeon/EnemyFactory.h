#ifndef ENEMIESFACTORY_H
#define ENEMIESFACTORY_H

#include "DragonGreen.h"
#include "Dungeon/Enemies/Bat.h"
#include "Dungeon/Enemies/BlueSlime.h"
#include "Dungeon/Enemies/Ghost.h"
#include "Dungeon/Enemies/GreenSlime.h"
#include "Dungeon/Enemies/Minotaur.h"
#include "Dungeon/Enemies/Monkey.h"
#include "Dungeon/Enemies/OrangeSlime.h"
#include "Dungeon/Enemies/Shopkeeper.h"
#include "Dungeon/Enemies/Skeleton.h"
#include "Dungeon/Enemies/Wraith.h"
#include "Dungeon/Enemies/Zombie.h"

namespace Dungeon {
class EnemyFactory final {
public:
    static std::shared_ptr<Enemy> CreateEnemy(
        const s_Enemy&                 _Enemy,
        const std::shared_ptr<MapData> mapData
    ) {
        // clang-format off
        switch (_Enemy.type) {
        case 0: return  std::make_shared<Enemies::GreenSlime>(_Enemy, mapData);
        case 1: return  std::make_shared<Enemies::BlueSlime>(_Enemy, mapData);
        case 2: return  std::make_shared<Enemies::OrangeSlime>(_Enemy, mapData);
        case 3: return  std::make_shared<Enemies::Skeleton>(_Enemy, mapData);
        case 4: return  std::make_shared<Enemies::Skeleton>(_Enemy, mapData);
        case 5: return  std::make_shared<Enemies::Skeleton>(_Enemy, mapData);
        case 6: return  std::make_shared<Enemies::Bat>(_Enemy, mapData);
        case 7: return  std::make_shared<Enemies::Bat>(_Enemy, mapData);
        case 8: return  std::make_shared<Enemies::Bat>(_Enemy, mapData);
        case 9: return  std::make_shared<Enemies::Monkey>(_Enemy, mapData);
        case 10: return std::make_shared<Enemies::Monkey>(_Enemy, mapData);
        case 11: return std::make_shared<Enemies::Ghost>(_Enemy, mapData);
        case 12: return std::make_shared<Enemies::Zombie>(_Enemy, mapData);
        case 13: return std::make_shared<Enemies::Wraith>(_Enemy, mapData);
        case 400: return std::make_shared<Enemies::Bat>(_Enemy, mapData);
        case 401: return std::make_shared<Enemies::Bat>(_Enemy, mapData);
        case 402: return std::make_shared<Enemies::DragonGreen>(_Enemy, mapData);
        case 407: return std::make_shared<Enemies::Minotaur>(_Enemy, mapData);
        case 408: return std::make_shared<Enemies::Minotaur>(_Enemy, mapData);
        case 600: return std::make_shared<Enemies::Shopkeeper>(_Enemy, mapData);
        default: return std::make_shared<Enemies::GreenSlime>(_Enemy, mapData);
        }
        // clang-format on
    }
};
}  // namespace Dungeon

#endif  // ENEMIESFACTORY_H
