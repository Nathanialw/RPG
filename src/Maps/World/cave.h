#pragma once

#include "components.h"
#include "world.h"
#include "world_update.h"

namespace Cave {

    bool Set_As_Cave(entt::registry &zone, entt::entity &entity, std::string &templateName) {
        if (templateName == "CaveEntrance1" ||
            templateName == "CaveEntrance2" ||
            templateName == "CaveEntrance3" ||
            templateName == "Chest1_2") {

            World::increment_Zone();
            auto &dungeon = zone.emplace_or_replace<Component::Dungeon>(entity);
            dungeon.instance = World::Zone_Count;
            dungeon.tilesetName = "labyrinth";
            zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::portal);
            return true;
        }
        return false;
    }

    void Surface(int &state) {
        World::currentZone.next = 2;
        World::world[World::currentZone.next].previousZoneIndex = state;
    }

    void Load_Zone(entt::registry &zone, entt::entity &targetID, int &state) {
        //get the new zone index
        auto newZone = zone.get<Component::Dungeon>(targetID);
        //save the old index go back to it later
        World::world[newZone.instance].previousZoneIndex = state;
        World::world[newZone.instance].tilesetName = newZone.tilesetName;
        World::currentZone.next = newZone.instance;
        World_Update::Clear_Tiles_Array(zone);
    }

}// namespace Cave