#include "Dungeon/Enemies/Skeleton.h"

namespace Dungeon {
Enemies::Skeleton::Skeleton(const s_Enemy &u_Enemy,
                            const std::shared_ptr<SimpleMapData> &simpleMapData)
    : Enemy(u_Enemy, simpleMapData),
      Animation(ToolBoxs::GamePostoPos(GetGamePosition())) {
    m_NormalFrames = {0, 1, 2, 3};
    m_ShadowFrames = {16, 17, 18, 19};
    m_SpriteSheet = std::make_shared<SpriteSheet>(
        ASSETS_DIR "/entities/skeleton.png", m_FrameSize, m_NormalFrames, true,
        100, true, 100);
    m_Drawable = m_SpriteSheet;
    m_WillMovePosition = GetGamePosition();

    SetHealth(2); // 1 heart
    SetDamage(1); // 0.5 heart
    SetCoin(2);
    m_Attack = false;
}
} // namespace Dungeon

namespace Dungeon::Enemies {
void Skeleton::Move() {
    if (m_Attack) {
        m_WillMovePosition = FindNextToPlayer();
        auto direction = m_WillMovePosition - GetGamePosition();
        LOG_INFO("Skeleton: Move to ({0}, {1})", m_WillMovePosition.x,
                 m_WillMovePosition.y);

        if (IsVaildMove(m_WillMovePosition)) {
            m_CanMove = true;
            SetGamePosition(m_WillMovePosition);
            if (direction.x > 0) {
                SetFace(false);
                m_AnimationType = 1;
            }
            else if (direction.x < 0) {
                SetFace(true);
                m_AnimationType = 3;
            }
            else if (direction.y > 0) {
                m_AnimationType = 0;
            }
            else if (direction.y < 0) {
                m_AnimationType = 2;
            }
        }
        else {
            m_CanMove = false;
        }
    }
    m_Attack = !m_Attack;
} // Green_Slime does not move
void Skeleton::Update() {
    if (m_Attack) {
        m_SpriteSheet->SetFrames(GetShadow() ? m_ShadowAttackFrames
                                             : m_AttackFrames);
    }
    else {
        m_SpriteSheet->SetFrames(GetShadow() ? m_ShadowFrames : m_NormalFrames);
    }

    // Collision
    if (m_CanMove && !m_IsAnimating) {
        MoveByTime(200, ToolBoxs::GamePostoPos(m_WillMovePosition),
                   m_AnimationType);
        m_CanMove = false;
    }
    UpdateAnimation(true);
    if (m_IsAnimating || m_AnimationPosition == m_AnimationDestination) {
        m_Transform.translation = m_AnimationPosition;
    }
    SetZIndex(m_AnimationZIndex);
}
} // namespace Dungeon::Enemies