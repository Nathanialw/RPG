//
// Created by desktop on 12/1/24.
//

#pragma once

#include "iostream"
#include "entt/entt.hpp"


///what should the initial town have?

//peasants? - mining? farming? lumberjacks?
//2

//merchant? - every time you come to town, he has new items?
//1 - mundane items

//buildings?
//4 - merchant, 2 peasant houses, 1 your house




namespace Town {
    void Building(entt::registry &zone, const int &state, const float &x, const float &y, std::string name = "Medieval RTP Building Exteriors Building1_1") {
        int xmlIndex = -1;

        auto building = Create_Entities::Create_Render_Object(zone, state, x, y, name, xmlIndex);
        auto &placement = zone.get<Building_Component::Placement>(building.entity);
        auto &offset = zone.get<Rendering_Components::Sprite_Offset>(building.entity);
        offset = placement.offset;
        zone.remove<Building_Component::Placement>(building.entity);
        Create_Entities::Create_Object(zone, state, building.entity);
    }


    void Init(entt::registry &zone, int &state) {
//        player position: 1660, 8924
        std::cout << "Town Init level: " << state << std::endl;
        if (state == 2) {
            std::cout << "Town Init" << std::endl;

            Component::Unit_Index unitIndex = {"demons", 0};

            Social_Component::Summon summon;
            Character_Options::Customization options;
            db::Unit_Data data = Entity_Loader::Get_Character_Create(Character_Options::Get_Character(options));

            //TODO offset the town from the cave entrance
            Create_Entities::Create_Entity(zone, state, 2215, 9050, data, false, summon, unitIndex); //peasant
            Create_Entities::Create_Entity(zone, state, 2265, 9115, data, false, summon, unitIndex); //peasant
            Create_Entities::Create_Entity(zone, state, 2350, 9860, data, false, summon, unitIndex); //merchant

            Building(zone, state, 500, 8550, "Medieval RTP Building Exteriors Building1_1"); //player house
            Building(zone, state, 2200, 8750, "Medieval RTP Building Exteriors Building1_6"); //peasant house
            Building(zone, state, 2400, 9500, "Medieval RTP Building Exteriors Building1_6"); //merchant house
        }
        else {
            std::cout << "In cave" << std::endl;
        }
    }

}
