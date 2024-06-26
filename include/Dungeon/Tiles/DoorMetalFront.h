#ifndef DOORMETALFRONT_H
#define DOORMETALFRONT_H

#include "Dungeon/Tiles/GeneralDoor.h"

namespace Dungeon {
namespace Tiles {
class DoorMetalFront final : public GeneralDoor {
public:
    explicit DoorMetalFront(const s_Tile& _Tile);
    virtual ~DoorMetalFront() override = default;
};
}  // namespace Tiles
}  // namespace Dungeon

#endif  // DOORMETALFRONT_H
