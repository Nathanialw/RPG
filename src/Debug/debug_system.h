#pragma once

#include "components.h"
#include "graphics.h"
#include "scene.h"
#include "sstream"
#include "timer.h"
#include "mouse_control.h"
#include <SDL2/SDL_render.h>
#include <algorithm>
#include "camera.h"
#include "debug_components.h"

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
  Graphics::Surface_Data numObjects;
  Graphics::Surface_Data numRendered;
  Graphics::Surface_Data renderChecks;
  Graphics::Surface_Data collisionChecks;
  Graphics::Surface_Data renderComponent;
  Graphics::Surface_Data treeSize;
  bool frameRateMode = true;
  bool frameTimeMode = false;

  void Toggle_Frame_Rate_Mode() {
    if (frameRateMode) {
      frameRateMode = false;
      frameTimeMode = true;
    } else {
      frameRateMode = true;
      frameTimeMode = false;
    }
    iFramePollRate = 500.0f;
  }

  void Framerate(Component::Camera &camera) {
    if (Debug::settings[Debug::Settings::Framerate]) {
      iFramePollRate += Timer::timeStep;
      if (iFramePollRate >= 1000.0f) {
        iFramePollRate = 0.0f;
        if (frameRateMode) {
          SDL_DestroyTexture(framerate.pTexture);
          framerate = Graphics::Load_Text_Texture(std::to_string((int) Timer::fps_avgFPS), {133, 255, 133});

          SDL_DestroyTexture(timeStep.pTexture);
          timeStep = Graphics::Load_Text_Texture(std::to_string(Timer::timeStep), {133, 255, 133});
        }
      }
      SDL_FRect c = {0.0f, 0.0f, 96.0f / camera.scale.x, 64.0f / camera.scale.y};
      SDL_RenderCopyF(Graphics::renderer, framerate.pTexture, &framerate.k, &c);

      SDL_FRect d = {128.0f / camera.scale.x, 0.0f, 128.0f / camera.scale.x, 64.0f / camera.scale.y};
      SDL_RenderCopyF(Graphics::renderer, timeStep.pTexture, &framerate.k, &d);
    }
    if (Debug::settings[Debug::Settings::InteractionRects]) {
//      Debug::numEntities = World::zone.view<Component::Position>().size();
      Debug::numEntities = World::zone.capacity();

      SDL_DestroyTexture(numObjects.pTexture);
      numObjects = Graphics::Load_Text_Texture(std::to_string(Debug::numEntities), {133, 255, 133});
      SDL_FRect d = {256.0f / camera.scale.x, 0.0f, 128.0f / camera.scale.x, 64.0f / camera.scale.y};
      SDL_RenderCopyF(Graphics::renderer, numObjects.pTexture, &numObjects.k, &d);
    }

    if (Debug::settings[Debug::Settings::EntityCount]) {
      SDL_DestroyTexture(numRendered.pTexture);
      numRendered = Graphics::Load_Text_Texture(std::to_string(Debug::numRendered), {133, 255, 133});
      SDL_FRect d = {372.0f / camera.scale.x, 0.0f, 128.0f / camera.scale.x, 64.0f / camera.scale.y};
      SDL_RenderCopyF(Graphics::renderer, numRendered.pTexture, &numRendered.k, &d);
    }

    if (Debug::settings[Debug::Settings::RenderChecks]) {
      SDL_DestroyTexture(renderChecks.pTexture);
      renderChecks = Graphics::Load_Text_Texture(std::to_string(Debug::renderChecks), {133, 255, 133});
      SDL_FRect d = {512.0f / camera.scale.x, 0.0f, 128.0f / camera.scale.x, 64.0f / camera.scale.y};
      SDL_RenderCopyF(Graphics::renderer, renderChecks.pTexture, &renderChecks.k, &d);
    }

    if (Debug::settings[Debug::Settings::CollisionChecks]) {
      SDL_DestroyTexture(collisionChecks.pTexture);
      collisionChecks = Graphics::Load_Text_Texture(std::to_string(Debug::collisionChecks), {133, 255, 133});
      SDL_FRect d = {640.0f / camera.scale.x, 0.0f, 128.0f / camera.scale.x, 64.0f / camera.scale.y};
      SDL_RenderCopyF(Graphics::renderer, collisionChecks.pTexture, &collisionChecks.k, &d);
    }

    if (Debug::settings[Debug::Settings::RenderComponent]) {
      Debug::renderComponent = World::zone.view<Component::Renderable>().size();

      SDL_DestroyTexture(renderComponent.pTexture);
      renderComponent = Graphics::Load_Text_Texture(std::to_string(Debug::renderComponent), {133, 255, 133});
      SDL_FRect d = {768.0f / camera.scale.x, 0.0f, 128.0f / camera.scale.x, 64.0f / camera.scale.y};
      SDL_RenderCopyF(Graphics::renderer, renderComponent.pTexture, &renderComponent.k, &d);
    }

    if (Debug::settings[Debug::Settings::TreeSize]) {
      SDL_DestroyTexture(treeSize.pTexture);
      treeSize = Graphics::Load_Text_Texture(std::to_string(Debug::treeSize), {133, 255, 133});
      SDL_FRect d = {896.0f / camera.scale.x, 0.0f, 128.0f / camera.scale.x, 64.0f / camera.scale.y};
      SDL_RenderCopyF(Graphics::renderer, treeSize.pTexture, &treeSize.k, &d);
    }
  }

  void Debug_Positions() {
    if (Debug::settings[Debug::Settings::NumRendered]) {
      auto view1 = World::zone.view<Component::Camera>();
      auto view = World::zone.view<Component::Position, Component::Renderable>();

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
}





