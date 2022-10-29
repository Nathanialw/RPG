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

	void Display_Health(entt::registry &zone, Component::Camera &camera) {
		auto view = zone.view<Input, Health>();
		for (auto player : view) {

			auto& current = view.get<Health>(player).currentHealth;
			auto& max = view.get<Health>(player).maxHealth;
            std::string currentHealth = std::to_string(current);
            std::string maxHealth = std::to_string(max);

            std::string healthText = currentHealth + " / " + maxHealth;

			SDL_Color blue = { 212, 175, 55, 255 };
			Graphics::Surface_Data currentHealthBox = Graphics::Load_Text_Texture(healthText, blue);;

			SDL_FRect textBox = {};
			float textSize = 20.0f;

			textBox.w = (healthText.length() * (textSize / 2.0f));
			textBox.h = (textSize);
			textBox.x = (100.0f - (textBox.w / 2.0f));
			textBox.y = (1000.0f - textSize);
			
			textBox.x = textBox.x / camera.scale.x;
			textBox.y = textBox.y / camera.scale.y;
			textBox.w = textBox.w / camera.scale.x;
			textBox.h = textBox.h / camera.scale.y;

			SDL_RenderCopyF(Graphics::renderer, currentHealthBox.pTexture, &currentHealthBox.k, &textBox);
			SDL_DestroyTexture(currentHealthBox.pTexture);
		}
	}

	void Display_Mana(entt::registry &zone, Component::Camera &camera) {
        auto view = zone.view<Input, Health>();
        for (auto player : view) {
            auto& current = view.get<Health>(player).currentHealth;
            auto& max = view.get<Health>(player).maxHealth;
            std::string currentHealth = std::to_string(current);
            std::string maxHealth = std::to_string(max);

            std::string healthText = currentHealth + " / " + maxHealth;

            SDL_Color blue = { 212, 175, 55, 255 };
            Graphics::Surface_Data currentHealthBox = Graphics::Load_Text_Texture(healthText, blue);;

            SDL_FRect textBox = {};
            float textSize = 20.0f;

            textBox.w = (healthText.length() * (textSize / 2.0f));
            textBox.h = (textSize);
            textBox.x = (1800.0f - (textBox.w / 2.0f));
            textBox.y = (1000.0f - textSize);

            textBox.x = textBox.x / camera.scale.x;
            textBox.y = textBox.y / camera.scale.y;
            textBox.w = textBox.w / camera.scale.x;
            textBox.h = textBox.h / camera.scale.y;

            SDL_RenderCopyF(Graphics::renderer, currentHealthBox.pTexture, &currentHealthBox.k, &textBox);
            SDL_DestroyTexture(currentHealthBox.pTexture);
        }
        //mana used for neutral magic
	}

    void Display_Faith(entt::registry &zone, Component::Camera &camera) {
       //shows Faith resource / demon resource,
       // piety increases faith decreases the demon resource and vice versa
       // faith used to heal and damage undead and demons, demon resource for demon magic and summoning
        auto view = zone.view<Input, Health>();
        for (auto player : view) {
            auto& current = view.get<Health>(player).currentHealth;
            auto& max = view.get<Health>(player).maxHealth;
            std::string currentHealth = std::to_string(current);
            std::string maxHealth = std::to_string(max);

            std::string healthText = currentHealth + " / " + maxHealth;

            SDL_Color blue = { 212, 175, 55, 255 };
            Graphics::Surface_Data currentHealthBox = Graphics::Load_Text_Texture(healthText, blue);;

            SDL_FRect textBox = {};
            float textSize = 20.0f;

            textBox.w = (healthText.length() * (textSize / 2.0f));
            textBox.h = (textSize);
            textBox.x = (800.0f - (textBox.w / 2.0f));
            textBox.y = (1000.0f - textSize);

            textBox.x = textBox.x / camera.scale.x;
            textBox.y = textBox.y / camera.scale.y;
            textBox.w = textBox.w / camera.scale.x;
            textBox.h = textBox.h / camera.scale.y;

            SDL_RenderCopyF(Graphics::renderer, currentHealthBox.pTexture, &currentHealthBox.k, &textBox);
            SDL_DestroyTexture(currentHealthBox.pTexture);
        }
    }

    void Display_Stamina(entt::registry &zone, Component::Camera &camera) {
        //high stamina increases your speed and damage
        //as it reduces your become weaker
        //it is the only resources that naturally replenishes
        auto view = zone.view<Input, Health>();
        for (auto player : view) {
            auto& current = view.get<Health>(player).currentHealth;
            auto& max = view.get<Health>(player).maxHealth;
            std::string currentHealth = std::to_string(current);
            std::string maxHealth = std::to_string(max);

            std::string healthText = currentHealth + " / " + maxHealth;

            SDL_Color blue = { 212, 175, 55, 255 };
            Graphics::Surface_Data currentHealthBox = Graphics::Load_Text_Texture(healthText, blue);;

            SDL_FRect textBox = {};
            float textSize = 20.0f;

            textBox.w = (healthText.length() * (textSize / 2.0f));
            textBox.h = (textSize);
            textBox.x = (1000.0f - (textBox.w / 2.0f));
            textBox.y = (1000.0f - textSize);

            textBox.x = textBox.x / camera.scale.x;
            textBox.y = textBox.y / camera.scale.y;
            textBox.w = textBox.w / camera.scale.x;
            textBox.h = textBox.h / camera.scale.y;

            SDL_RenderCopyF(Graphics::renderer, currentHealthBox.pTexture, &currentHealthBox.k, &textBox);
            SDL_DestroyTexture(currentHealthBox.pTexture);
        }
    }

    void Render_Resources (entt::registry &zone, Camera &camera) {
        Display_Health(zone, camera);
        Display_Mana(zone, camera);
        Display_Faith(zone, camera);
        Display_Stamina(zone, camera);
    }



}
