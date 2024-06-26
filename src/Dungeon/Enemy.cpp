#include "Dungeon/Enemy.h"

#include <memory>
#include <vector>

#include "Dungeon/AStar.h"
#include "Dungeon/MapData.h"
#include "Dungeon_config.h"
#include "Event/Event.h"
#include "Event/EventArgs.h"
#include "Settings/ToolBoxs.h"
#include "Settings/Window.hpp"
#include "UGameElement.h"

namespace Dungeon {

Enemy::Enemy(const s_Enemy& _Enemy, const std::shared_ptr<MapData> mapData)
    : m_MapData(mapData),
      m_ID(_Enemy.type),
      m_BeatDelay(_Enemy.beatDelay),
      m_Lord(_Enemy.lord == 1),
      m_DrawableUpdate(Event::EventQueue) {
    m_Transform.scale = Window::GetScale();
    SetGamePosition({_Enemy.x, _Enemy.y});
    m_Animation = std::make_unique<Animation>(
        ToolBoxs::GamePostoPos(m_GamePosition)
    );
    m_Transform.translation = m_Animation->GetAnimationPosition();
    SetZIndex(m_Animation->GetAnimationZIndex());

    m_FullHeart = std::make_shared<Util::Image>(
        Dungeon::config::IMAGE_FULL_HEART_SM.data()
    );
    m_EmptyHeart = std::make_shared<Util::Image>(
        Dungeon::config::IMAGE_EMPTY_HEART_SM.data()
    );
    m_DrawableUpdate.appendListener(
        EventType::DrawableUpdate,
        [this](const Object*, const EventArgs&) { Update(); }
    );
}

void Enemy::SetShadow(const bool shadow) {
    if (!m_Seen) {
        if (!shadow) {
            m_Seen = !shadow;
        } else {
            m_Visible = false;
            return;
        }
    }
    if (m_Shadow == shadow) { return; }
    m_Shadow = shadow;
    m_SpriteSheet->SetFrames(shadow ? m_ShadowFrames : m_NormalFrames);
}

void Enemy::SetGamePosition(const glm::vec2& gamePosition) {
    m_GamePosition = gamePosition;
    m_WillMovePosition = gamePosition;

    // drawable would be updated depending on the enemy derived class
    // m_Transform.translation = ToolBoxs::GamePostoPos(gamePosition);
    // SetZIndex(m_GamePosition.y + float(0.25));
}

void Enemy::SetLord(const bool lord) {
    m_Lord = lord;
    if (m_Lord) {
        m_Transform.scale += m_Transform.scale;
        m_Damage += m_Damage;
        m_Health += m_Health;
        m_Coin = 24;
    }
}

void Enemy::TempoMove() {
    // if (GetVisible() == false || m_Seen == false) {
    //     return;
    // }
    if (m_BeatDelay > 0) {
        m_BeatDelay--;
        return;
    }
    Move();
}

bool Enemy::IsVaildMove(const glm::vec2& position) {
    return m_MapData->IsPositionWalkable(position);
}

glm::vec2 Enemy::FindNextToPlayer(bool isExtraDirections) {
    if (GetPlayerPosition() == GetGamePosition()) { return GetGamePosition(); }
    auto path = Dungeon::AStar::FindPath(
        GetGamePosition(),
        GetPlayerPosition(),
        m_MapData,
        10,
        isExtraDirections
    );
    if (path.empty() || path.size() == 1) {
        return GetGamePosition();
    } else {
        return path[1];
    }
}

void Enemy::AttackPlayer() {
    if (GetPlayerPosition() == m_WillMovePosition) {
        Event::EventQueue.dispatch(this, AttackPlayerEventArgs(GetDamage()));
        Event::SetAttackPlayer(false);
        m_WillMovePosition = GetGamePosition();
        m_Animation->MoveByTime(
            200,
            ToolBoxs::GamePostoPos(GetGamePosition()),
            m_AnimationType + 5
        );
    }
}

void Enemy::UpdateAnimationType(const glm::vec2& direction) {
    if (direction.x > 0) {
        m_AnimationType = 1;
    } else if (direction.x < 0) {
        m_AnimationType = 3;
    } else if (direction.y > 0) {
        m_AnimationType = 0;
    } else if (direction.y < 0) {
        m_AnimationType = 2;
    }
}

void Enemy::SetCameraUpdate(bool cameraUpdate) {
    SetVisible(cameraUpdate);
    for (auto& child : m_Children) {
        child->SetVisible(cameraUpdate);
    }
}

void Enemy::Struck(const std::size_t damage) {
    m_IsBeAttacked = true;
    if (m_Health >= damage) {
        m_Health -= damage;
    } else {
        m_Health = 0;
    }
    if (m_Health == 0) {
        m_Dead = true;
        return;
    }
    if (m_KnockbackAble) {
        auto delta = GetGamePosition() - GetPlayerPosition();
        auto willMove = GetGamePosition() + delta;
        if (IsVaildMove(willMove)) {
            m_WillMovePosition = willMove;
            m_AnimationType = 4;
            m_UnnecssaryAnimation = true;
            CanMove();
        }
    }
};

void Enemy::Update() {
    if (m_Dead && (!m_Animation->IsAnimating() || m_UnnecssaryAnimation)) {
        Event::EventQueue.dispatch(
            this,
            EnemyRemoveEventArgs(GamePostion2MapIndex(GetGamePosition()))
        );
        return;
    }

    // Update animation
    if (m_Animation->UpdateAnimation()) {
        m_Transform.translation = m_Animation->GetAnimationPosition();
    } else {
        m_UnnecssaryAnimation = false;
    }

    // Update z index
    SetZIndex(m_Animation->GetAnimationZIndex());

    UpdateHeart(m_Transform.translation);
}

void Enemy::CanMove() {
    if (m_Dead || m_WillMovePosition == GetGamePosition()) { return; }
    if (m_WillMovePosition == GetPlayerPosition()) {
        AttackPlayer();
        return;
    }
    if (!m_Animation->IsAnimating()) {
        Event::EventQueue.dispatch(
            this,
            EnemyMoveEventArgs(GamePostion2MapIndex(m_WillMovePosition))
        );
        SetGamePosition(m_WillMovePosition);
        m_Animation->MoveByTime(
            200,
            ToolBoxs::GamePostoPos(m_WillMovePosition),
            m_AnimationType
        );
    }
}

void Enemy::InitHealthBarImage(const glm::vec2& pixelPos) {
    const auto& hp = m_Health;

    float zindex = 0.01;
    for (std::size_t ii = 0; ii < hp; ii += 2) {
        const auto obj = std::make_shared<Util::GameElement>();
        //        obj->SetDrawable(Dungeon::config::PTR_IMAGE_FULL_HEART_SM);
        obj->SetDrawable(m_FullHeart);
        obj->SetZIndex(99.0f + zindex);
        obj->SetPosition(pixelPos);
        obj->SetVisible(false);
        obj->SetScale(Window::GetScale());

        m_HealthBar->AddChild(obj);

        zindex = zindex + 0.01;
    }
}

void Enemy::UpdateHeart(const glm::vec2& pixelPos) {
    const auto m_HeartList = m_HealthBar->GetChildren();
    const auto numberOfHeart = m_HeartList.size();

    auto setVisible = [&](bool visible) {
        for (const auto& elem : m_HeartList) {
            elem->SetVisible(visible);
        }
    };

    if (m_Health == 0 || numberOfHeart == 0 || GetShadow() == true
        || GetVisible() == false || m_IsBeAttacked == false) {
        setVisible(false);
        return;
    }
    setVisible(true);

    int       startIdx = numberOfHeart * -1 + 1;
    auto      pos = pixelPos + glm::vec2{0.0f, 40.0f};
    glm::vec2 x_offset;

    for (std::size_t ii = 0; ii < numberOfHeart; ii++) {
        x_offset = {startIdx * 18 + pos.x, pos.y};
        std::dynamic_pointer_cast<Util::GameElement>(m_HeartList[ii])
            ->SetPosition(x_offset);
        startIdx += 2;
    }

    for (std::size_t i = 0; i < numberOfHeart - m_Health / 2; i++) {
        m_HeartList[numberOfHeart - i - 1]->SetDrawable(m_EmptyHeart);
    }
}

glm::vec2 Enemy::GetPlayerPosition() const {
    return m_MapData->GetPlayerPosition();
}

std::size_t Enemy::GamePostion2MapIndex(const glm::ivec2& position) const {
    return m_MapData->GamePosition2MapIndex(position);
}

float Enemy::Heuristic(const glm::vec2& start, const glm::vec2& end) {
    return m_MapData->Heuristic(start, end);
}
void Enemy::InitHealth(const std::size_t health) {
    RemoveChild(m_HealthBar);
    m_HealthBar = std::make_shared<Util::GameObject>();
    AddChild(m_HealthBar);
    m_Health = health;
    InitHealthBarImage(m_Transform.translation);
}

std::set<Player::Direction> Enemy::GetRelativeDirectionSet(
    const glm::vec2& direction
) {
    std::set<Player::Direction> relativeDirection;
    if (direction.x > 0) {
        relativeDirection.insert(Player::Direction::RIGHT);
    } else if (direction.x < 0) {
        relativeDirection.insert(Player::Direction::LEFT);
    }
    if (direction.y > 0) {
        relativeDirection.insert(Player::Direction::DOWN);
    } else if (direction.y < 0) {
        relativeDirection.insert(Player::Direction::UP);
    }
    return relativeDirection;
}

void Enemy::ChangeHealthBar(const std::size_t health) {
    m_Health = health;
    UpdateHeart(m_Transform.translation);
}

void Enemy::SetFace(bool faceTo) {
    m_Transform.scale.x = faceTo ? Window::Scale : -Window::Scale;
}

}  // namespace Dungeon
