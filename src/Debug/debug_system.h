#pragma once

#include "debug_components.h"
#include "scene.h"
#include "camera.h"

using namespace Scene;

namespace Debug_System {
  float iFramePollRate = 0.0f;

  void Entity_Data_Debug(float x, float y, float sx, float sy, Component::Camera &camera) {
    //displays :
    //x,y position
    //collision radius
    //	mass
    //SDL_Rect j = { sx, sy, 30, 30 };
    ////SDL_RenderDrawRect(Graphics::renderer, &j);
    Graphics::Surface_Data x_Position = Graphics::Load_Text_Texture(std::to_string(int(x)), {255, 0, 133});
    Graphics::Surface_Data y_Position = Graphics::Load_Text_Texture(std::to_string(int(y)), {255, 133, 0});
    //Surface_Data collision_Radius = Graphics::Load_Text_Texture(std::to_string(mas), { 133,133,133 }, renderer);
    //Surface_Data mass = Graphics::Load_Text_Texture(std::to_string(radius), { 133,133,133 }, renderer);

    //SDL_Rect e = { sx - 5, sy - 5, 27, 50 };
    //SDL_RenderDrawRect(renderer, &e);
    ////SDL_RenderCopy(renderer, mass.pTexture, , );

    SDL_FRect a = {sx + 25, sy - 20, 15, 15};
    SDL_FRect c = Camera_Control::Convert_Rect_To_Screen_Coods(a, camera);
    SDL_RenderCopyF(Graphics::renderer, x_Position.pTexture, &x_Position.k, &c);
    SDL_FRect b = {sx + 25, sy - 10, 15, 15};
    SDL_FRect s = Camera_Control::Convert_Rect_To_Screen_Coods(b, camera);
    SDL_RenderCopyF(Graphics::renderer, y_Position.pTexture, &y_Position.k, &s);
    //SDL_Rect c = { sx, sy+20, 15, 10 };
    //SDL_RenderCopy(renderer, collision_Radius.pTexture, &collision_Radius.k, &c);
    //SDL_Rect d = { sx, sy+30, 15, 10 };
    //SDL_RenderCopy(renderer, mass.pTexture, &mass.k, &d);
    //SDL_DestroyTexture(collision_Radius.pTexture);
    //SDL_DestroyTexture(mass.pTexture);
    SDL_DestroyTexture(x_Position.pTexture);
    SDL_DestroyTexture(y_Position.pTexture);
  }

  Graphics::Surface_Data framerate;
  Graphics::Surface_Data timeStep;
  Graphics::Surface_Data loopTextures[Timer::SIZE];
  Graphics::Surface_Data numObjects;

  void Show_Settings(Component::Camera &camera) {
    float yPosition = 256.0f;
    for (int i = 1; i < Debug::Settings::SIZE; ++i) {
      yPosition += 48.0f ;//(float) numObjects.k.h;
      if (Debug::settings[i]) {
        SDL_DestroyTexture(numObjects.pTexture);
        std::string text = Debug::labels[i] + ":  " + std::to_string(Debug::settingsValue[i]);
        numObjects = Graphics::Load_Text_Texture(text, {133, 255, 133});
        SDL_FRect d = {0 / camera.scale.x, yPosition / camera.scale.y, 256.0f / camera.scale.x, 48.0f / camera.scale.y};
        SDL_RenderCopyF(Graphics::renderer, numObjects.pTexture, &numObjects.k, &d);
      }
    }

    if (Debug::settings[Debug::Settings::loopTimers]) {
      for (int i = 0; i < Timer::SIZE; ++i) {
        if (iFramePollRate >= 400.0f) {
          SDL_DestroyTexture(loopTextures[i].pTexture);
          std::string text = Timer::GameStateText[i] + ":  " + std::to_string(Timer::GameStateValue[i]);
          loopTextures[i] = Graphics::Load_Text_Texture(text, {133, 255, 133});
        }
        yPosition += 48.0f;
        SDL_FRect d = {0 / camera.scale.x, yPosition / camera.scale.y, 256.0f / camera.scale.x, 48.0f / camera.scale.y};
        SDL_RenderCopyF(Graphics::renderer, loopTextures[i].pTexture, &loopTextures[i].k, &d);
      }
    }
  }

  void Update_Settings(entt::registry &zone) {
    Debug::settingsValue[Debug::EntityCount] = zone.capacity();
    Debug::settingsValue[Debug::RenderComponent] = zone.view<Component::Renderable>().size();
    Debug::settingsValue[Debug::TreeSize] = zone.view<Component::Interaction_Rect>().size();
  }

  void Framerate(entt::registry &zone, Component::Camera &camera) {
    Update_Settings(zone);
    if (Debug::settings[Debug::Settings::Framerate]) {
      iFramePollRate += Timer::timeStep;
      if (iFramePollRate >= 400.0f) {
        SDL_DestroyTexture(framerate.pTexture);
        framerate = Graphics::Load_Text_Texture(std::to_string((int) Timer::fps_avgFPS), {133, 255, 133});
        SDL_DestroyTexture(timeStep.pTexture);
        timeStep = Graphics::Load_Text_Texture(std::to_string(Timer::timeStep), {133, 255, 133});
      }
      SDL_FRect c = {0.0f, 0.0f, 96.0f / camera.scale.x, 32.0f / camera.scale.y};
      SDL_RenderCopyF(Graphics::renderer, framerate.pTexture, &framerate.k, &c);

      SDL_FRect d = {128.0f / camera.scale.x, 0.0f, 128.0f / camera.scale.x, 32.0f / camera.scale.y};
      SDL_RenderCopyF(Graphics::renderer, timeStep.pTexture, &framerate.k, &d);
    }

    Show_Settings(camera);
    if (iFramePollRate >= 400.0f) {
      iFramePollRate = 0.0f;
    }
  }

  void Debug_Positions(entt::registry &zone) {
    if (Debug::settings[Debug::Settings::entityPositions]) {
      auto view1 = zone.view<Component::Camera>();
      auto view = zone.view<Component::Position, Component::Renderable>();

      for (auto focus: view1) {
        auto &camera = view1.get<Component::Camera>(focus);
        for (auto entity: view) {
          auto &position = view.get<Component::Position>(entity);
          Component::Radius radius;
          radius.fRadius = 20.0f;
          SDL_Color color = {55, 255, 55};

          SDL_FRect frect = {position.x - radius.fRadius, position.y, radius.fRadius * 2, radius.fRadius};
          SDL_RenderDrawRectF(Graphics::renderer, &frect);
          Debug_System::Entity_Data_Debug(position.x, position.y, position.x, position.y, camera);
        }
      }
    }
  }

  void Debugger() {
    //Framerate();
  }
}// namespace Debug_System
