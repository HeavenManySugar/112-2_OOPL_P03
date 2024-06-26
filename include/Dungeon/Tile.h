#ifndef TILE_H
#define TILE_H

#include <memory>
#include <string>

#include "Util/GameObject.hpp"
#include "Util/SpriteSheet.hpp"

#include "Dungeon/Elements.h"
#include "Event/Event.h"
#include "Settings/SpriteSheet.hpp"

namespace Dungeon {

class Tile : public Util::GameObject {
public:
    Tile(const s_Tile& _Tile);
    Tile(const s_Tile& _Tile, const std::string& filepath);
    void Initialize();
    virtual ~Tile() override = default;

    void         SetIndex(std::size_t index);
    void         SetCloseDisplayB24(bool closeDisplayb24);
    void         SetRotation(float rotation);
    virtual void SetOverlay(bool visible);
    std::size_t  GetIndex();
    s_Tile       GetTile();
    bool         GetOverlay();
    virtual bool IsWall() = 0;
    virtual bool IsDoor() = 0;

    virtual void UpdateDrawable();

    bool GetSeen() const;

    void        SetCameraUpdate(bool cameraUpdate);
    std::string m_Filepath;

protected:
    void         UpdateScale();
    virtual void UpdateTranslation();

    void SetTorch(bool torch);

    s_Tile m_Tile;

    glm::vec2                          m_TileSize;
    glm::vec2                          m_ImgSize;
    int8_t                             m_MagicNumber = 0;
    std::size_t                        m_Index = 0;
    bool                               m_CloseDisplayb24 = false;
    std::size_t                        m_OffSetY = 0;
    std::shared_ptr<Util::SpriteSheet> m_SpriteSheet;

    bool m_Seen = false;
    bool m_IsOverlay = true;

    std::shared_ptr<SpriteSheet>      m_TorchAnimation;
    std::shared_ptr<Util::GameObject> m_Torch;

    Event::Remover m_Event;

    SDL_Color m_Color = {0, 0, 0, 255};
};
}  // namespace Dungeon

#endif  // TILE_HPP
