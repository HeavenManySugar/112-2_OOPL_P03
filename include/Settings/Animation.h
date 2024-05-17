#ifndef ANIMATION_H
#define ANIMATION_H

#include "pch.hpp"  // IWYU pragma: export

class Animation final {
public:
    Animation(const glm::vec2& animationPosition);

    void MoveByTime(
        const unsigned long duringTimeMs,
        const glm::vec2&    destination,
        const uint16_t      direction
    );
    void MoveByTime(
        const unsigned long duringTimeMs,
        const glm::vec2&    destination
    );
    void      UpdateAnimation(const bool isDirection = false);
    bool      IsAnimating();
    glm::vec2 GetAnimationPosition();
    float     GetAnimationZIndex();
    void      UpdateGamePosition(const glm::vec2& gamePosition);

    glm::vec2     GetAnimationDestination();
    unsigned long GetAnimationStartMs();
    unsigned long GetAnimationDuringTimeMs();
    void          SetAnimationStop();

private:
    float CalculateZIndex(const glm::vec2& position);

    std::vector<glm::vec2> m_MoveAnimation;

    bool          m_IsAnimating = false;
    bool          m_IsAnimatingInternal = false;
    unsigned long m_AnimationStartMs = 0;
    unsigned long m_AnimationDuringTimeMs = 0;
    glm::vec2     m_AnimationDestination = {1e9, 1e9};
    uint16_t      m_AnimationDirection;
    glm::vec2     m_AnimationPosition = {-1e9, -1e9};
    float         m_AnimationZIndex = 0.0f;
};

#endif
