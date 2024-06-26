#include "Dungeon/Enemies/Skeleton.h"

#include "Dungeon/MapData.h"

namespace Dungeon {
Enemies::Skeleton::Skeleton(
    const s_Enemy&                 _Enemy,
    const std::shared_ptr<MapData> mapData
)
    : Enemy(_Enemy, mapData) {
    if (_Enemy.type == 4) {
        m_NormalFrames = {0, 1, 2, 3};
        m_AttackFrames = {4, 5, 6, 7};
        m_ShadowFrames = {17, 18, 19, 20};
        m_ShadowAttackFrames = {21, 22, 23, 24};
        m_SpriteSheet = std::make_shared<SpriteSheet>(
            ASSETS_DIR "/entities/skeleton_yellow.png",
            m_FrameSize,
            m_ShadowFrames,
            true,
            100,
            true,
            100
        );
        InitHealth(4);  // 2 hearts
        SetDamage(3);   // 1.5 hearts
        SetCoin(2);
        m_CanDropHead = true;
    } else if (_Enemy.type == 5) {
        m_NormalFrames = {0, 1, 2, 3};
        m_AttackFrames = {4, 5, 6, 7};
        m_ShadowFrames = {17, 18, 19, 20};
        m_ShadowAttackFrames = {21, 22, 23, 24};
        m_SpriteSheet = std::make_shared<SpriteSheet>(
            ASSETS_DIR "/entities/skeleton_black.png",
            m_FrameSize,
            m_ShadowFrames,
            true,
            100,
            true,
            100
        );
        InitHealth(6);  // 3 hearts
        SetDamage(4);   // 2 hearts
        SetCoin(4);
        m_CanDropHead = true;
    } else {
        // Default Skeleton type = 3
        m_NormalFrames = {0, 1, 2, 3};
        m_AttackFrames = {4, 5, 6, 7};
        m_ShadowFrames = {16, 17, 18, 19};
        m_ShadowAttackFrames = {20, 21, 22, 23};

        m_SpriteSheet = std::make_shared<SpriteSheet>(
            ASSETS_DIR "/entities/skeleton.png",
            m_FrameSize,
            m_NormalFrames,
            true,
            100,
            true,
            100
        );
        InitHealth(2);  // 1 heart
        SetDamage(1);   // 0.5 heart
        SetCoin(2);
        m_CanDropHead = false;
    }

    m_SkeletonNormalFrames = m_NormalFrames;
    m_SkeletonShadowFrames = m_ShadowFrames;

    m_Drawable = m_SpriteSheet;
    m_WillMovePosition = GetGamePosition();

    m_Attack = false;
}
}  // namespace Dungeon

namespace Dungeon::Enemies {
void Skeleton::Move() {
    if (m_Attack) {
        m_WillMovePosition = FindNextToPlayer();
        auto direction = m_WillMovePosition - GetGamePosition();

        if (IsVaildMove(m_WillMovePosition)) {
            // Set Animation
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
    m_Attack = !m_Attack;
    UpdateProperties();
}

void Skeleton::UpdateProperties() {
    if (m_Attack) {
        m_NormalFrames = m_AttackFrames;
        m_ShadowFrames = m_ShadowAttackFrames;
    } else {
        m_NormalFrames = m_SkeletonNormalFrames;
        m_ShadowFrames = m_SkeletonShadowFrames;
    }
    m_SpriteSheet->SetFrames(GetShadow() ? m_ShadowFrames : m_NormalFrames);
}
}  // namespace Dungeon::Enemies
