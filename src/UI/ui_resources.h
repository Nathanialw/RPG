#pragma once
#include "camera.h"
#include "graphics.h"


namespace UI_Resources {
    //health is health

    //mana used for neutral magic

    //shows Faith resource / demon resource,
    //piety increases faith decreases the demon resource and vice versa
    //faith used to heal and damage undead and demons, demon resource for demon magic and summoning

    //high stamina increases your speed and damage
    //as it reduces your become weaker
    //it is the only resources that naturally replenishes

    void Display_Stamina(entt::registry &zone, Component::Camera &camera) {
        //high stamina increases your speed and damage
        //as it reduces your become weaker
        //it is the only resources that naturally replenishes
        auto view = zone.view<Component::Input, Component::Health>();
        for (auto player : view) {
            auto& current = view.get<Component::Health>(player).currentHealth;
            auto& max = view.get<Component::Health>(player).maxHealth;
            std::string currentHealth = std::to_string(current);
            std::string maxHealth = std::to_string(max);

            std::string healthText = currentHealth + " / " + maxHealth;

            SDL_Color blue = { 212, 175, 55, 255 };
            Graphics::Surface_Data currentHealthBox = Graphics::Load_Text_Texture(healthText, blue);;

            SDL_FRect textBox = {};
            float textSize = 20.0f;

            textBox.w = (healthText.length() * (textSize / 2.0f));
            textBox.h = (textSize);

            SDL_Rect rect = Utilities::SDL_FRect_To_SDL_Rect(textBox);
            textBox = UI::Center_Rect(rect);
            textBox.y = textBox.y + 50.0f;
            textBox = UI::Update_Scale(camera.scale, textBox);

            SDL_RenderCopyF(Graphics::renderer, currentHealthBox.pTexture, &currentHealthBox.k, &textBox);
            SDL_DestroyTexture(currentHealthBox.pTexture);
        }
    }

    void Render_Resources (entt::registry &zone, Component::Camera &camera) {
        Display_Stamina(zone, camera);
    }



}
