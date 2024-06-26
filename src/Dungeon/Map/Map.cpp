#include "Dungeon/Map.h"
#include <memory>

#include "Dungeon/Enemy.h"
#include "Dungeon/TileFactory.h"
#include "Settings/Window.hpp"

namespace Dungeon {
std::size_t Map::GetLevelNum() const {
    return m_LevelNum;
}

bool Map::CheckShowPosition(
    const glm::vec2& position1,
    const glm::vec2& position2
) {
    return (
        position1.x >= position2.x - (ALLOW_EXTRA_DRAW + Window::HalfColNumber)
        && position1.x
               <= position2.x + (ALLOW_EXTRA_DRAW + Window::HalfColNumber)
        && position1.y
               >= position2.y - (ALLOW_EXTRA_DRAW + Window::HalfRowNumber)
        && position1.y
               <= position2.y + (ALLOW_EXTRA_DRAW + Window::HalfRowNumber)
    );
}

std::size_t Map::GamePostion2MapIndex(const glm::ivec2& position) const {
    return m_MapData->GamePosition2MapIndex(position);
    // return (position.x - m_Level->GetLevelIndexMin().x + 1)
    //        + (position.y - m_Level->GetLevelIndexMin().y + 1) * m_Size.x;
}

glm::ivec2 Map::MapIndex2GamePosition(const std::size_t index) const {
    return m_MapData->MapIndex2GamePosition(index);
    // return {
    //   static_cast<int>(index % m_Size.x) + m_Level->GetLevelIndexMin().x - 1,
    //   static_cast<int>(index / m_Size.x) + m_Level->GetLevelIndexMin().y - 1
    // };
}

std::shared_ptr<MapData> Map::GetMapData() const {
    return m_MapData;
}

void Map::RemoveEnemy(const std::size_t position) {
    std::shared_ptr<Enemy> enemy = m_MapData->GetEnemy(position);
    m_EnemyHead->RemoveChild(enemy);
    m_MapData->RemoveEnemy(position);
}

void Map::RemoveWall(const std::size_t position) {
    auto tile = m_MapData->GetTile(position)->GetTile();

    if (tile.type == 102) { return; }
    m_TileHead->RemoveChild(m_MapData->GetTile(position));
    m_MapData->RemoveTile(position);
    m_MapData->AddTile(
        position,
        TileFactory::CreateTile(
            s_Tile{tile.x, tile.y, 0, tile.zone, 0, tile.cracked}
        )
    );
    m_TileHead->AddChild(m_MapData->GetTile(position));
}

void Map::OpenDoor(const std::size_t position) {
    auto tile = m_MapData->GetTile(position)->GetTile();
    auto doorType = tile.type;
    auto gamePosition = glm::vec2(tile.x, tile.y);

    if (doorType == 50 || doorType == 103 || doorType == 118) {
        m_TileHead->RemoveChild(m_MapData->GetTile(position));
        m_MapData->RemoveTile(position);
        m_MapData->AddTile(
            position,
            TileFactory::CreateTile(
                s_Tile{tile.x, tile.y, 0, tile.zone, 0, tile.cracked}
            )
        );
        m_TileHead->AddChild(m_MapData->GetTile(position));
        for (size_t i = 0; i < 4; i++) {
            auto tmpPosition = gamePosition + m_EnemyMove[i];
            auto tmpMapIndex = GamePostion2MapIndex(tmpPosition);
            if (m_MapData->IsPositionDoor(tmpPosition)
                && doorType
                       == m_MapData->GetTile(tmpMapIndex)->GetTile().type) {
                OpenDoor(tmpMapIndex);
                // m_TileHead->RemoveChild(m_MapData->GetTile(tmpMapIndex));
                // m_MapData->RemoveTile(tmpMapIndex);
                // m_MapData->AddTile(
                //     tmpMapIndex,
                //     TileFactory::CreateTile(s_Tile{
                //       static_cast<int>(tmpPosition.x),
                //       static_cast<int>(tmpPosition.y),
                //       0,
                //       tile.zone,
                //       0,
                //       tile.cracked
                //     })
                // );
                // m_TileHead->AddChild(m_MapData->GetTile(tmpMapIndex));
            }
        }
    }
}

bool Map::CanPlayerSeePosition(const glm::vec2& position) {
    // BresenhamLine Algorithm
    // glm::vec2 targetPosition = position;
    // glm::vec2 sourcePosition = m_MapData->GetPlayerPosition();

    // I don't know why but the target and source position are reversed
    // so I have to reverse them
    glm::ivec2 targetPosition = m_MapData->GetPlayerPosition();
    glm::ivec2 sourcePosition = position;

    std::vector<glm::ivec2> integerCoordinates;

    glm::ivec2 delta = targetPosition - sourcePosition;
    glm::ivec2 sign = glm::sign(delta);
    delta = glm::abs(delta);
    int        error = 0;
    glm::ivec2 current = sourcePosition;

    if (delta.x > delta.y) {
        for (int i = 0; i <= delta.x; ++i) {
            integerCoordinates.push_back(current);
            error -= delta.y;
            if (error < 0) {
                current.y += sign.y;
                error += delta.x;
            }
            current.x += sign.x;
        }
    } else {
        for (int i = 0; i <= delta.y; ++i) {
            integerCoordinates.push_back(current);
            error -= delta.x;
            if (error < 0) {
                current.x += sign.x;
                error += delta.y;
            }
            current.y += sign.y;
        }
    }

    std::size_t mapIndex;
    // because the integerCoordinates is from target to source
    // so we have to reverse it
    for (int i = integerCoordinates.size() - 1; i >= 0; --i) {
        mapIndex = m_MapData->GamePosition2MapIndex(integerCoordinates[i]);
        if (position == static_cast<glm::vec2>(integerCoordinates[i])) {
            m_ShadowRenderDP[mapIndex] = true;
            return true;
        }
        if (m_MapData->IsPositionWall(integerCoordinates[i])
            || m_MapData->IsPositionDoor(integerCoordinates[i])) {
            m_ShadowRenderDP[mapIndex] = false;
            return false;
        }
    }
    m_ShadowRenderDP[m_MapData->GamePosition2MapIndex(position)] = true;
    return true;
}

void Map::AddItem(
    const std::size_t           position,
    const std::shared_ptr<Item> item
) {
    if (m_MapData->IsItemEmpty(position)) {
        const auto& tile = GetMapData()->GetTile(position);
        item->m_Transform = tile->m_Transform;
        item->SetZIndex(tile->GetZIndex() + 0.01f);
        m_MapData->AddItem(position, item);
        m_ItemHead->AddChild(item);
    } else {
        LOG_CRITICAL("Item already exists at position: {0}", position);
    }
}

void Map::RemoveItem(const std::size_t position) {
    m_ItemHead->RemoveChild(m_MapData->GetItem(position));
    m_MapData->RemoveItem(position);
}
}  // namespace Dungeon

glm::ivec2 Dungeon::Map::m_Size = {0, 0};
