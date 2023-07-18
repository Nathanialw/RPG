#pragma  once
#include "entt/entt.hpp"
#include "components.h"
#include "graphics.h"
#include "timer.h"
#include <vector>


namespace Damage_Text {

  std::vector<Component::Scrolling_Damage_Text> damageTextQueue;

  void Add_To_Scrolling_Damage(entt::registry& zone, entt::entity& entity, entt::entity& target, int& damage, bool special) {
    SDL_FPoint positionRect = { zone.get<Component::Position>(target).x, zone.get<Component::Position>(target).y - 60.0f };

    Component::Scrolling_Damage_Text text = { positionRect, std::to_string(damage), 1000, special };
    damageTextQueue.emplace_back(text);
  }

  void Show_Damage(entt::registry& zone, Component::Camera& camera) {

    for (int j = 0; j < damageTextQueue.size(); j++) {
      auto &i = damageTextQueue[j];
      SDL_Color color;

      if (i.special) {
	color = {250, 50, 55, 255};
      }
      else {
	color = {255, 255, 255, 255};
      }

      Graphics::Surface_Data itemTextBox = Graphics::Load_Text_Texture(i.damageText, color);

      SDL_FRect textBox = {};
      float textSize = 20.0f;

      textBox.w = i.damageText.length() * (textSize / 2.0f);
      textBox.h = textSize;
      textBox.x = i.position.x - (textBox.w / 2.0f) - camera.screen.x;
      textBox.y = i.position.y - textSize - camera.screen.y;

      SDL_RenderCopyF(Graphics::renderer, itemTextBox.pTexture, &itemTextBox.k, &textBox);
      SDL_DestroyTexture(itemTextBox.pTexture);

      i.position.y -= 0.05f * (float)Timer::timeStep;
      i.lingerTime -= (int)Timer::timeStep;
      if (i.lingerTime <= 0) {
	damageTextQueue.erase(damageTextQueue.begin() + j);
      }
    }
  }
}
