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

    void Init(entt::registry &zone, int &state) {
//        player position: 1660, 8924
        std::cout << "Town Init level: " << state << std::endl;
        if (state == 2) {
            std::cout << "Town Init" << std::endl;

            //Create_Entities::Create_Entity(zone, state, 1660, 8924, data, true, summon, unitIndex);
            Component::Unit_Index unitIndex = {"demons", 0};

            Social_Component::Summon summon;
            Character_Options::Customization options;
            db::Unit_Data data = Entity_Loader::Get_Character_Create(Character_Options::Get_Character(options));

            Create_Entities::Create_Entity(zone, state, 1700, 8950, data, false, summon, unitIndex);
            Create_Entities::Create_Entity(zone, state, 1700, 8900, data, false, summon, unitIndex);
            Create_Entities::Create_Entity(zone, state, 1650, 9000, data, false, summon, unitIndex);


            //Create_Entities::Create_Building(zone, state, 1660, 8924, data, true, summon, unitIndex);
            //Create_Entities::Create_Building(zone, state, 1660, 8924, data, true, summon, unitIndex);
            //Create_Entities::Create_Building(zone, state, 1660, 8924, data, true, summon, unitIndex);
            //Create_Entities::Create_Building(zone, state, 1660, 8924, data, true, summon, unitIndex);
        }
        else {
            std::cout << "In cave" << std::endl;
        }
    }

}