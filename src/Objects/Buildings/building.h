#pragma once

namespace Build {

#include "Buildings/orc.h"

    void Clear_Objects() {
        //TODO: clear all objects that intersect with the building
    }

    bool Place_Building(entt::registry &zone, const int &state) {
        Clear_Objects();
        if (Mouse_Struct::mouseData.type == Component::Icon_Type::building) {
            auto &placeable = zone.get<Building_Component::Placement>(Mouse::mouseData.mouseItem);
            Utilities::Log(placeable.polygons.size());
            if (!placeable.obstructed) {
                auto &placement = zone.get<Building_Component::Placement>(Mouse::mouseData.mouseItem);
                auto &offset = zone.get<Rendering_Components::Sprite_Offset>(Mouse::mouseData.mouseItem);
                offset = placement.offset;
                zone.remove<Building_Component::Placement>(Mouse::mouseData.mouseItem);

                auto &sprite = zone.get<Rendering_Components::Sprite_Sheet_Info>(Mouse::mouseData.mouseItem);
                sprite.color = {200, 200, 200, SDL_ALPHA_OPAQUE};
                sprite.blendType = Rendering_Components::normal;
                //set building
                zone.get<Collision_Component::Collider_Data>(Mouse::mouseData.mouseItem).position = {Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse};
                if (Create_Entities::Create_Object(zone, state, Mouse::mouseData.mouseItem)) {
                    Mouse::Set_Cursor_As_Cursor(zone);
                    return true;
                };
            } else {
                Utilities::Log("building cannot be placed, something is in the way");
                return true;
            }
        }
        return false;
    }

    bool Rotate_Building(entt::registry &zone, const int &state) {
        //destroy the previous mouse entity
        zone.emplace_or_replace<Component::Destroyed>(Mouse::mouseData.mouseItem);
        //go into the db and get all with the icon name of the same,
        std::string name = Entity_Loader::Increment_Direction(Mouse_Struct::mouseData.name, Mouse_Struct::mouseData.direction);
        // plug it into Create_Render_Object()
        int xmlIndex = -1;
        auto mouseEntity = Create_Entities::Create_Render_Object(zone, state, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, name, xmlIndex);
        Mouse::Set_Cursor_As_Cursor(zone);
        Mouse::Set_Cursor_As_Entity(zone, mouseEntity, Component::Icon_Type::building);
        return true;
    }

    void Build() {
    }
};// namespace Build