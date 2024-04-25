#ifndef I_LOVE_PTSD_BEATINDICATOR_H
#define I_LOVE_PTSD_BEATINDICATOR_H

#include <memory>

#include "GameElement.h"

namespace Display {
class BeatIndicator {
public:
    static void Init();

    static std::shared_ptr<GameElement> GetGameElement();

    static void Update();

private:
    static std::string                  m_IndicatorImagePath;
    static glm::vec2                    m_Position;
    static glm::vec2                    m_Scale;
    static float                        m_ZIndex;
    static std::shared_ptr<GameElement> m_GameElement;
};
}  // namespace Display

#endif  // I_LOVE_PTSD_BEATINDICATOR_H
