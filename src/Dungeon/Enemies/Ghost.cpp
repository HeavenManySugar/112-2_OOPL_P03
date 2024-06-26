#include "Dungeon/Enemies/Ghost.h"

#include "Dungeon/MapData.h"

namespace Dungeon {
Enemies::Ghost::Ghost(
    const s_Enemy&                 _Enemy,
    const std::shared_ptr<MapData> mapData
)
    : Enemy(_Enemy, mapData) {
    m_NormalFrames = {0, 1};
    m_ShadowFrames = {2, 3};
    m_SpriteSheet = std::make_shared<SpriteSheet>(
        ASSETS_DIR "/entities/ghost.png",
        m_FrameSize,
        m_NormalFrames,
        true,
        100,
        true,
        100
    );
    m_Drawable = m_SpriteSheet;
    m_WillMovePosition = GetGamePosition();

    InitHealth(2);  // 1 heart
    SetDamage(1);   // 0.5 heart
    SetCoin(2);

    m_AnimationType = 4;
}
}  // namespace Dungeon

namespace Dungeon::Enemies {
void Ghost::SetTransparent(bool transparent) {
    if (m_Transparent == transparent) { return; }
    m_Transparent = transparent;
    if (m_Transparent) {
        m_SpriteSheet->SetAlpha(128);
    } else {
        m_SpriteSheet->SetAlpha(255);
    }
}
void Ghost::Struck(const std::size_t damage) {
    if (m_Transparent) { return; }
    Enemy::Struck(damage);
}
void Ghost::Move() {
    auto relativeDirectionSet = GetRelativeDirectionSet(
        GetGamePosition() - GetPlayerPosition()
    );
    if (relativeDirectionSet.count(m_MapData->GetPlayer()->GetDirection())) {
        SetTransparent(true);
    } else {
        if (m_Transparent) {
            SetTransparent(false);
            return;
        }
    }

    if (!m_Transparent) {
        m_WillMovePosition = FindNextToPlayer();
        auto direction = m_WillMovePosition - GetGamePosition();

        if (IsVaildMove(m_WillMovePosition)) {
            if (direction.x > 0) {
                SetFace(false);
                m_AnimationType = 1;
            } else if (direction.x < 0) {
                SetFace(true);
                m_AnimationType = 3;
            } else if (direction.y > 0) {
                m_AnimationType = 0;
            } else if (direction.y < 0) {
                m_AnimationType = 2;
            }
            CanMove();
        }
    }
}

void Ghost::AttackPlayer() {
    if (m_Transparent) {
        m_WillMovePosition = GetGamePosition();
        return;
    }
    Enemy::AttackPlayer();
}
}  // namespace Dungeon::Enemies
