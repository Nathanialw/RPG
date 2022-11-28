#pragma once
#include "entt/entt.hpp"

namespace Social_Component {

    struct Race {
        int race;
    };

    struct Relationships {
        int races[14];
        std::unordered_map<entt::entity, int> connections;
    };

}

namespace  Social_Control {

    bool Check_Relationship (entt::registry &zone, entt::entity &unitID, entt::entity &targetID) {
        int race = zone.get<Social_Component::Race>(targetID).race;
        int state = zone.get<Social_Component::Relationships>(unitID).races[race];
//        zone.get<Social_Component::Relationships>(unitID).races.at(targetID);

        if (state < 33) {
            return true;
        }

        return false;
    }


    void Interact (entt::registry &zone, entt::entity &unitID, entt::entity &targetID) {
        auto &sheetData = zone.get<Component::Sprite_Sheet_Info>(unitID);
        auto &action = zone.get<Component::Action>(unitID);
        action.state = Component::talk;
        sheetData.currentFrame = 0;
        /*
         *  open interactioon menu
         *  or use some audio
         *  or use a text box
         *  I don't know something like that
         */

        Utilities::Log("talking to a friend!");
    }



}