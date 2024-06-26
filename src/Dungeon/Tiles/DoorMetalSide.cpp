#include "Dungeon/Tiles/DoorMetalSide.h"

#include "Settings/Window.hpp"

namespace Dungeon {
namespace Tiles {
DoorMetalSide::DoorMetalSide(const s_Tile& _Tile)
    : GeneralDoor(_Tile, false) {
    m_MagicNumber = 15;
    UpdateDoorMetalSideDrawable();
}

void DoorMetalSide::UpdateDoorMetalSideDrawable() {
    UpdateScale();
    UpdateTranslation();
    m_Transform.translation = {
      m_Transform.translation.x - Window::TileWidth,
      m_Transform.translation.y
    };
    m_SpriteSheet->SetDrawRect(
        {static_cast<int>(
             m_ImgSize.x * (m_Index % static_cast<size_t>(m_TileSize.x))
         ),
         static_cast<int>(
             m_ImgSize.y
             * static_cast<size_t>(m_Index / static_cast<float>(m_TileSize.x))
         ),
         static_cast<int>(m_ImgSize.x),
         static_cast<int>(m_ImgSize.y - m_OffSetY)}
    );
    m_Drawable = m_SpriteSheet;
}

void DoorMetalSide::UpdateDrawable() {
    UpdateDoorMetalSideDrawable();
}

}  // namespace Tiles
}  // namespace Dungeon
