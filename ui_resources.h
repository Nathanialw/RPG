#pragma once
#include "items.h"
#include "camera.h"
#include "graphics.h"


namespace UI_Resources {

	void Render_Health(entt::registry &zone, Component::Camera &camera) {
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

	void Render_Mana() {

	}





}
