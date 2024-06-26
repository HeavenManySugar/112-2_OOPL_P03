//
// Created by adven on 2024/5/12.
//

#include "Game/Systems/HandItem.h"
#include "Game/Graphs/Coin.h"
#include "Game/Warehouse/Food_1.h"
#include "Game/Warehouse/Food_2.h"

namespace Game::Systems {

HandItem::HandItem(
    const std::shared_ptr<Dungeon::Map>& map,
    const std::shared_ptr<Player>&       player
)
    : m_DungeonMap(map),
      m_MainCharacter(player) {}

void HandItem::DispatchByMI(
    const std::shared_ptr<Dungeon::Item>& equip,
    std::size_t                           nextPos
) {
    std::function<void(std::size_t)> fptr = nullptr;
    switch (HashConvert(equip->GetClassName())) {
    case "BlackChest"_hash:
        fptr = [this](std::size_t pos) { BlackChest(pos); };
        break;
    case "Spear"_hash: fptr = [this](std::size_t pos) { Spear(pos); }; break;
    case "Dagger"_hash: fptr = [this](std::size_t pos) { Dagger(pos); }; break;
    case "Food_1"_hash: Food_1(nextPos); break;
    case "Food_2"_hash: Food_2(nextPos); break;
    case "Coin"_hash: Coin(nextPos); break;
    default: throw std::runtime_error("Can not Hand Item.");
    }

    if (fptr) {
        fptr(nextPos);
    }
}

void HandItem::BlackChest(std::size_t nextPos) {
    const auto& test = std::static_pointer_cast<Game::Graphs::BlackChest>(
        m_DungeonMap->GetMapData()->GetItem(nextPos)
    );

    const auto& [a, b] = test->GetContent();
    m_MainCharacter->GetToolMod()->RemoveTool("WEAPON", "Spear");
    m_MainCharacter->GetToolMod()->AddTool(a, b);
    const auto& _throw = std::make_shared<Game::Warehouse::Throw>(
        Config::IMAGE_DAGGER_PATH,
        "Dagger"
    );

    m_MainCharacter->GetToolMod()->AddTool(_throw, "THROW", "Dagger");

    m_DungeonMap->RemoveItem(nextPos);
}

void HandItem::Spear(std::size_t nextPos) {
    m_DungeonMap->RemoveItem(nextPos);
    m_MainCharacter->GetToolMod()->DisappearTool(true, "WEAPON", "Spear");
}

void HandItem::Dagger(std::size_t nextPos) {
    m_DungeonMap->RemoveItem(nextPos);
    m_MainCharacter->GetToolMod()->DisappearTool(true, "WEAPON", "Dagger");
}

void HandItem::Food_1(std::size_t nextPos) {
    m_DungeonMap->RemoveItem(nextPos);
    const auto& obj = std::make_shared<Game::Warehouse::Food_1>();
    m_MainCharacter->GetToolMod()->AddTool(obj, obj->GetName(), obj->GetType());
}

void HandItem::Food_2(std::size_t nextPos) {
    m_DungeonMap->RemoveItem(nextPos);
    const auto& obj = std::make_shared<Game::Warehouse::Food_2>();
    m_MainCharacter->GetToolMod()->AddTool(obj, obj->GetName(), obj->GetType());
}

void HandItem::Coin(std::size_t nextPos) {
    const auto& coinNumber = std::static_pointer_cast<Game::Graphs::Coin>(
                                 m_DungeonMap->GetMapData()->GetItem(nextPos)
    )
                                 ->GetNumber();

    m_DungeonMap->RemoveItem(nextPos);
    m_MainCharacter->gainCoin(coinNumber);
}

}  // namespace Game::Systems
