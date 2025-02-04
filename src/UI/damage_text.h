#pragma  once
#include "entt/entt.hpp"
#include "components.h"
#include "graphics.h"
#include "timer.h"
#include <vector>

namespace Damage_Text {

  std::vector<Component::Scrolling_Damage_Text> damageTextQueue;

  void Add_To_Scrolling_Damage(entt::registry& zone, entt::entity& entity, entt::entity& target, int& damage, Component::Attack_Type type, bool critical) {
    SDL_FPoint positionRect = { zone.get<Component::Position>(target).x, zone.get<Component::Position>(target).y - 60.0f };

    Component::Scrolling_Damage_Text text = { positionRect, std::to_string(damage), 1000, type, critical };
    damageTextQueue.emplace_back(text);
  }

  void Show_Damage(entt::registry& zone, Component::Camera& camera) {

    for (int j = 0; j < damageTextQueue.size(); j++) {
      Component::Scrolling_Damage_Text &i = damageTextQueue[j];
      SDL_Color color;

      if (i.type == Component::skill) {
	color = {250, 50, 55, 255};
      }
      else {
	color = {255, 255, 255, 255};
      }

      Graphics::Surface_Data itemTextBox = Graphics::Load_Text_Texture(i.damageText, color);

      SDL_FRect textBox = {};
      float textSize = 20.0f;
      if (i.critical) {
	textSize = 30.0f;
      }

      textBox.w = i.damageText.length() * (textSize / 2.0f);
      textBox.h = textSize;
      textBox.x = i.position.x - (textBox.w / 2.0f) - camera.screen.x;
      textBox.y = i.position.y - textSize - camera.screen.y;

      SDL_RenderCopyF(Graphics::renderer, itemTextBox.pTexture, &itemTextBox.k, &textBox);
      SDL_DestroyTexture(itemTextBox.pTexture);

      i.position.y -= 0.05f * Timer::timeStep;
      i.lingerTime -= Timer::timeStep;
      if (i.lingerTime <= 0) {
	damageTextQueue.erase(damageTextQueue.begin() + j);
      }
    }
  }
}
