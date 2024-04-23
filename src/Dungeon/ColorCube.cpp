#include "Dungeon/ColorCube.h"

namespace Dungeon {
ColorCube::ColorCube(CubeColor color)
    : m_Color(color) {
    SetColor(color);
    m_Available = true;
}

void ColorCube::Draw(const Core::Matrices& data) {
    if (m_ColorMap.find(m_Color) != m_ColorMap.end()) {
        m_ColorMap.at(m_Color)->Draw(data);
    }
}

glm::vec2 ColorCube::GetSize() const {
    if (m_ColorMap.find(m_Color) != m_ColorMap.end()) {
        return m_ColorMap.at(m_Color)->GetSize();
    }
    return glm::vec2(0.0f);
}

bool ColorCube::GetAvailable() const {
    return m_Available;
}

void ColorCube::SetColor(CubeColor color) {
    m_Color = color;
    if (!(m_ColorMap.find(m_Color) != m_ColorMap.end())) {
        m_ColorMap[m_Color] = ColorCubeFactory(m_Color);
    }
    m_Available = true;
}
Dungeon::CubeColor ColorCube::GetColor() const {
    return m_Color;
}

std::unique_ptr<Util::Image> ColorCube::ColorCubeFactory(CubeColor color) {
    switch (color) {
    case CubeColor::black:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_black.png"
        );
    case CubeColor::blood:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_blood.png"
        );
    case CubeColor::blue:
        return std::make_unique<Util::Image>(ASSETS_DIR
                                             "/particles/TEMP_particle_blue.png"
        );
    case CubeColor::confuse:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_confuse.png"
        );
    case CubeColor::crystal:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_crystal.png"
        );
    case CubeColor::darkpurple:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_darkpurple.png"
        );
    case CubeColor::dirt:
        return std::make_unique<Util::Image>(ASSETS_DIR
                                             "/particles/TEMP_particle_dirt.png"
        );
    case CubeColor::gray:
        return std::make_unique<Util::Image>(ASSETS_DIR
                                             "/particles/TEMP_particle_gray.png"
        );
    case CubeColor::green:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_green.png"
        );
    case CubeColor::heal:
        return std::make_unique<Util::Image>(ASSETS_DIR
                                             "/particles/TEMP_particle_heal.png"
        );
    case CubeColor::metrognome:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_metrognome.png"
        );
    case CubeColor::orange:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_orange.png"
        );
    case CubeColor::purple:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_purple.png"
        );
    case CubeColor::red:
        return std::make_unique<Util::Image>(ASSETS_DIR
                                             "/particles/TEMP_particle_red.png"
        );
    case CubeColor::spore:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_spore.png"
        );
    case CubeColor::stone:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_stone.png"
        );
    case CubeColor::wall:
        return std::make_unique<Util::Image>(ASSETS_DIR
                                             "/particles/TEMP_particle_wall.png"
        );
    case CubeColor::water:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_water.png"
        );
    case CubeColor::white:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_white.png"
        );
    case CubeColor::yellow:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_yellow.png"
        );
    case CubeColor::yellowgreen:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_yellowgreen.png"
        );
    default:
        return std::make_unique<Util::Image>(
            ASSETS_DIR "/particles/TEMP_particle_black.png"
        );
    }
}

}  // namespace Dungeon
