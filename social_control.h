#pragma once
#include "entt/entt.hpp"
#include "dialogue.h"

namespace Social_Component {

    struct Race {
        int race;
    };

    struct Relationships {
        int races[14];
        std::unordered_map<entt::entity, int> connections;
    };

    struct Speaking {
        std::string text;
        uint64_t duration = 0;
        uint64_t count = 1;
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

    void Show_Dialogue (entt::registry& zone, Component::Camera& camera) {
        auto view = zone.view<Component::Unit, Component::Interaction_Rect, Component::Renderable, Social_Component::Speaking>();
        for (auto entity : view) {
            auto &interactionRect = view.get<Component::Interaction_Rect>(entity);
            auto &speaking = view.get<Social_Component::Speaking>(entity);

            Component::Position itemPosition = {interactionRect.rect.x + (interactionRect.rect.w / 2.0f), interactionRect.rect.y};
            Graphics::Text_Box_Data itemTextBox = Graphics::Create_Text_Background(camera, {255, 255, 255}, speaking.text, itemPosition);

            SDL_RenderFillRect(Graphics::renderer, &itemTextBox.textBoxBackground);
            SDL_RenderCopyF(Graphics::renderer, itemTextBox.textdata.pTexture, &itemTextBox.textdata.k, &itemTextBox.highlightBox);
            SDL_DestroyTexture(itemTextBox.textdata.pTexture);

            speaking.count += Timer::timeStep;
            if (speaking.count >= speaking.duration) {
                zone.remove<Social_Component::Speaking>(entity);
            }
        }
    }

    void Interact(entt::registry &zone, entt::entity &unitID, std::string text_type){
        if (!zone.any_of<Social_Component::Speaking>(unitID)) {
            auto &text = zone.emplace<Social_Component::Speaking>(unitID);

            //set text
            text.text = Dialogue::Get_Random_Dialogue(text_type);

            //set duration
            text.duration = 2000 + (text.text.length() * 100);
        }
    }

    void Greet (entt::registry &zone, entt::entity &unitID, entt::entity &targetID) {
        auto &sheetData = zone.get<Component::Sprite_Sheet_Info>(unitID);
        auto &action = zone.get<Component::Action>(unitID);
        action.state = Component::talk;
        sheetData.currentFrame = 0;

        std::string text_type = "greeting";
        Interact(zone, unitID, text_type);

    }


}