//#include "../SDL_FontCache.h"
#include "SDL2/SDL.h"
#include "entt/entt.hpp"
#include "components.h"
#include "graphics.h"
#include "timer.h"

namespace Text_Panels {

  struct Text_Box {
    float duration;
    // render  data
    SDL_Rect background;

    Component::Position position;

    SDL_Texture *backgroundTexture;
    std::string text;
    SDL_Rect source_rect;
    SDL_FRect destination_rect;
  };

  std::vector<Text_Box> currentTextBoxes;

  SDL_FRect Create_Box_over_Head() {

  }

  void Text_Box_At_Mouse(entt::registry &zone, std::string &text) {


  }

  void Text_Box_Over_Head(entt::registry &zone, Component::Camera &camera, std::string &text) {
    auto view = zone.view<Component::Interaction_Rect, Component::Renderable>();
    for (auto entity: view) {
      auto &box = view.get<Component::Interaction_Rect>(entity);
      Text_Box textBoxData;

      //get xy of the center and bottom of the box; just above the unit's head
      float y = box.rect.y;
      float x = box.rect.w / 2.0f;

      //add 100 ms for every letter in the text string
      textBoxData.duration = text.size() * 100;

      SDL_FRect textBox = {};
      textBox.w = text.length() * 5.0f;
      textBox.h = 10.0f;

      textBox.x = x - (textBox.w / 2.0f);
      textBox.y = y - 10.0f;

      textBox.x -= camera.screen.x;
      textBox.y -= camera.screen.y;
      SDL_Rect textBoxBackground = Utilities::SDL_FRect_To_SDL_Rect(textBox);
      textBoxBackground.x -= 5;
      textBoxBackground.w += 10;

//            textBox.texture =

      currentTextBoxes.emplace_back(textBoxData);
    }
  }

  void Remove_Text_Box() {
  }

  void Present_Text_Box() {
    for (int i = 0; i < currentTextBoxes.size(); i++) {
      auto box = currentTextBoxes[i];
      box.duration -= Timer::timeStep;
      if (box.duration <= 0) {
        currentTextBoxes.erase(currentTextBoxes.begin() + i);
      } else {
        SDL_RenderFillRect(Graphics::renderer, &box.background);
        FC_Draw(Graphics::fcFont, Graphics::renderer, box.position.x, box.position.y, box.text.c_str());
      }
    }
    currentTextBoxes.shrink_to_fit();
  }


}