//
// Created by 陳世昂 on 2024/3/25.
//

#ifndef FUCK_PTSD_HEART_H
#define FUCK_PTSD_HEART_H

#include <deque>

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "config.hpp"

#include "Settings/GameElement.h"

class Heart final {
public:
    explicit Heart();

    void plusHP(const std::size_t number);

    void minusHP(const std::size_t number);
    void minusHeart(const float number);

    void hitHeart(const std::size_t index);

    void gainHeart(const std::size_t number = 2);
    void UpdateHP();
    void resetHP();

    void SetHeartMaxHp(const std::size_t value);

    [[nodiscard]]
    std::vector<std::shared_ptr<Util::GameObject>> getGameObjects() const;

    [[nodiscard]]
    std::size_t GetHealth() const;

    std::shared_ptr<GameElement> GetGameObject() const;

    enum STATE {
        FULL = 0,
        HALF,
        EMPTY,
    };

private:
    void                         IsDead();
    std::shared_ptr<GameElement> generalHeart(
        STATE            state,
        const glm::vec2& position
    );
    std::size_t m_ZIndex = 100;
    // HP settings
    //
    std::size_t     m_MaxHp = 12;
    std::size_t     m_currentHP = 0;
    glm::vec2       m_FirstPosition;
    const glm::vec2 m_eachPositionDiff_X = {-80, 0};
    const glm::vec2 m_eachPositionDiff_Y = {0, -80};

    // Image Settings
    glm::vec2 m_Scale;

    std::shared_ptr<Util::Image> m_FullHPImage = std::make_shared<Util::Image>(
        ASSETS_DIR "/gui/heart.png"
    );

    std::shared_ptr<Util::Image> m_HalfHPImage = std::make_shared<Util::Image>(
        ASSETS_DIR "/gui/heart_half.png"
    );

    std::shared_ptr<Util::Image> m_EmptyHPImage = std::make_shared<Util::Image>(
        ASSETS_DIR "/gui/heart_empty.png"
    );

    // game object
    std::deque<std::shared_ptr<GameElement>> m_ElementList;

    std::shared_ptr<GameElement> m_OneHeart = std::make_shared<GameElement>();
    std::shared_ptr<GameElement> m_TwoHeart = std::make_shared<GameElement>();
    std::shared_ptr<GameElement> m_ThreeHeart = std::make_shared<GameElement>();

    std::shared_ptr<GameElement> m_Heart = std::make_shared<GameElement>();
};

#endif  // FUCK_PTSD_HEART_H
