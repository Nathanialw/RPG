#pragma once
#include "Graphics/XML_Parsers/texture_packer.h"
#include "collision_components.h"

namespace Collision {
  void Load() {
    Texture_Packer::TexturePacker_Import_Collision("assets/sprites/buildings/medieval/collision/interiors.xml", Collision_Component::polygonColliders);
    Texture_Packer::TexturePacker_Import_Collision("assets/sprites/buildings/medieval/collision/cave_entrances.xml", Collision_Component::polygonColliders);
    Texture_Packer::TexturePacker_Import_Collision("assets/sprites/buildings/medieval/collision/labyrinth0.xml", Collision_Component::polygonColliders);
    Texture_Packer::TexturePacker_Import_Collision("assets/sprites/buildings/medieval/collision/labyrinth0_objects.xml", Collision_Component::polygonColliders);
    //    Collision_Component::Update_Offsets();
  }
}// namespace Collision
