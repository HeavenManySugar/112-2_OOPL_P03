//
// Created by adven on 2024/5/10.
//

#include <future>
#include <thread>
#include "App.hpp"

#include "Music/IndicatorBar.h"
#include "Util/Logger.hpp"
#include "eventpp/utilities/argumentadapter.h"
#include "eventpp/utilities/conditionalfunctor.h"

#include <Util/Input.hpp>
#include "Display/BeatHeart.h"
#include "Dungeon/Enemy.h"
#include "Event/Event.h"
#include "Music/Player.h"
#include "Music/Tempo.h"
#include "Settings/DeadScreen.h"
#include "System.h"

namespace Update {
auto musicTime = []() {
    return static_cast<std::size_t>(Music::Player::GetMusicTime() * 1000)
           % static_cast<std::size_t>(Music::Player::GetMusicLength() * 1000);
};
}  // namespace Update

void App::Update() {
    m_EventHandler.Update();

    if (Music::Tempo::IsSwitch()) {
        m_DungeonMap->TempoTrigger(Music::Tempo::GetBeatIdx());
        Display::BeatHeart::SwitchHeart(100);
        Event::SetAttackPlayer(true);

        SetDeadScreen(m_MainCharacter->GetHealth() == 0);
    }
    if (Util::Input::IfExit()) { m_CurrentState = State::END; }

    Game::System::Update();
    Display::BeatHeart::Update();
    // Display::BeatIndicator::Update();
    Music::IndicatorBar::Update();
    Music::Tempo::Update(Update::musicTime(), 0u, Music::Player::LoopCounter());

    m_MainCharacter->Update();
    m_Camera->Update();
}

std::shared_ptr<Dungeon::Map> App::m_DungeonMap = nullptr;
