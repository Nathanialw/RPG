#pragma once

#include "Objects/Social/social_control.h"
#include "camera.h"
#include "components.h"
#include "debug_components.h"
#include "debug_system.h"
#include "graphics.h"
#include "mouse_control.h"
#include "ui_frames.h"
#include "ui_resources.h"
#include "ui_unit_control.hpp"
#include "unit_frames.h"
#include <SDL2/SDL.h>

namespace Interface {

  namespace {
    int iMousePollRate;
    Graphics::Surface_Data mouseX;
    Graphics::Surface_Data mouseY;
    int gridDepth = -1;
  }// namespace

  void Update_Zoom(entt::registry &zone, const int zoom) {
    auto view = zone.view<Component::Camera>();
    for (auto focus: view) {
      auto &x = view.get<Component::Camera>(focus).scale;
      //      if (e.wheel.y > 0) {
      if (zoom > 0) {
        x.x *= 1.1f;
        x.y *= 1.1f;
        if (x.y > 2.0f) {
          x.x = 2.0f;
          x.y = 2.0f;
        }
        SDL_RenderSetScale(Graphics::renderer, x.x, x.y);
      }
      //      if (e.wheel.y < 0) {
      if (zoom < 0) {
        x.x *= 0.9f;
        x.y *= 0.9f;
        if (x.y < 0.75f) {
          //          x.x = 0.75f;
          //          x.y = 0.75f;
        }
        SDL_RenderSetScale(Graphics::renderer, x.x, x.y);
      }
    }
  }

  void Display_Military_Groups(entt::registry &zone) {
    auto camera_view = zone.view<Component::Camera>();
    for (auto cameras: camera_view) {
      auto &cam = camera_view.get<Component::Camera>(cameras);

      auto squad_view = zone.view<Component::Squad>();
      for (auto squads: squad_view) {
        SDL_FRect o = squad_view.get<Component::Squad>(squads).sCollide_Box;
        o.x -= cam.screen.x;
        o.y -= cam.screen.y;
        //SDL_SetRenderDrawColor(Graphics::renderer, 155, 55, 255, 255);
        //SDL_RenderDrawRectF(Graphics::renderer, &o);
      }

      auto platoon_view = zone.view<Component::Platoon>();
      for (auto platoons: platoon_view) {
        auto &platoon = platoon_view.get<Component::Platoon>(platoons);
        SDL_FRect o = platoon.sCollide_Box;
        o.x = platoon.sCollide_Box.x - cam.screen.x;
        o.y = platoon.sCollide_Box.y - cam.screen.y;
        //SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 0, 255);
        //SDL_RenderDrawRectF(Graphics::renderer, &o);
      }

      auto company_view = zone.view<Component::Company>();
      for (auto companies: company_view) {
        auto &company = company_view.get<Component::Company>(companies);
        SDL_FRect o = company.sCollide_Box;
        o.x = company.sCollide_Box.x - cam.screen.x;
        o.y = company.sCollide_Box.y - cam.screen.y;
        //SDL_SetRenderDrawColor(Graphics::renderer, 0, 200, 255, 255);
        //SDL_RenderDrawRectF(Graphics::renderer, &o);
      }
    }
  }

  void Display_Unit_Formations(entt::registry &zone, Component::Camera &camera) {
    int i = 0;
    auto unitFormations_view = zone.view<Test::Unit_Formation_Data>();
    for (auto unitFormation: unitFormations_view) {
      auto &formation = unitFormations_view.get<Test::Unit_Formation_Data>(unitFormation);

      switch (formation.formationType) {
        case Test::Formation_Type::squad: {
          SDL_SetRenderDrawColor(Graphics::renderer, 0, 55, 255, 125);
        };
            break;
        case Test::Formation_Type::platoon: {
          SDL_SetRenderDrawColor(Graphics::renderer, 0, 55, 155, 125);
        };
            break;
        case Test::Formation_Type::company: {
          SDL_SetRenderDrawColor(Graphics::renderer, 0, 55, 55, 125);
        };
            break;
        case Test::Formation_Type::battalion: {
          SDL_SetRenderDrawColor(Graphics::renderer, 0, 255, 0, 125);
        };
            break;
        case Test::Formation_Type::brigade: {
          SDL_SetRenderDrawColor(Graphics::renderer, 0, 155, 0, 125);
        };
            break;
        case Test::Formation_Type::division: {
          SDL_SetRenderDrawColor(Graphics::renderer, 55, 55, 0, 125);
        };
            break;
        case Test::Formation_Type::corps: {
          SDL_SetRenderDrawColor(Graphics::renderer, 155, 55, 0, 125);
        };
            break;
        case Test::Formation_Type::army: {
          SDL_SetRenderDrawColor(Graphics::renderer, 255, 55, 0, 125);
        };
            break;
      }

      SDL_FRect renderRectScreen = formation.sCollide_Box;
      renderRectScreen.x -= camera.screen.x;
      renderRectScreen.y -= camera.screen.y;
      SDL_RenderDrawRectF(Graphics::renderer, &renderRectScreen);
      i++;
    }
    //std::cout << i << std::endl;
  }

  void Update_Interaction_Box(entt::registry &zone) {
    auto view = zone.view<Component::Interaction_Rect, Component::Position, Component::Radius, Component::Renderable>(entt::exclude<Building_Component::Placement, Building_Component::Set_Placement>);

    for (auto entity: view) {
      auto &position = view.get<Component::Position>(entity);
      auto &radius = view.get<Component::Radius>(entity);
      auto &rect = view.get<Component::Interaction_Rect>(entity);
      if (rect.tall) {// if the unit is tall
        rect.rect = {position.x - radius.fRadius, position.y - (radius.fRadius * 3.0f), radius.fRadius * 2.0f, radius.fRadius * 4.0f};
      } else {
        rect.rect = {position.x - radius.fRadius, position.y - radius.fRadius, radius.fRadius * 2.0f, radius.fRadius * 2.0f};
      }
    }
  }

  void Display_Selected(entt::registry &zone) {
    auto view1 = zone.view<Component::Camera>();
    auto view = zone.view<Component::Selected, Component::Position, Component::Radius>();

    for (auto focus: view1) {
      auto &camera = view1.get<Component::Camera>(focus);
      for (auto entity: view) {
        auto &position = view.get<Component::Position>(entity);
        auto &radius = view.get<Component::Radius>(entity);
        auto &selector = view.get<Component::Selected>(entity);

        SDL_Color color;

        if (Social_Control::Check_Relationship(zone, focus, entity)) {
          //	  red if enemy
          color = {255, 55, 55};
        } else {
          //	  green if ally
          color = {55, 255, 55};
        }

        SDL_FRect frect = {position.x - radius.fRadius, position.y, radius.fRadius * 2, radius.fRadius};
        UI_Unit_Control::Loop_Sprite(selector, frect, color, camera);
      }
    }
  }

  void Display_Selection_Box() {
    if (Mouse::bLeft_Mouse_Pressed && ((Mouse::iXMouse - Mouse::Mouse_Selection_Box_x_Display) > 5.0f || (Mouse::iYMouse - Mouse::Mouse_Selection_Box_y_Display) > 5.0f)) {
      SDL_SetRenderDrawColor(Graphics::renderer, 55, 255, 55, 255);
      SDL_FRect p = {Mouse::Mouse_Selection_Box_x_Display, Mouse::Mouse_Selection_Box_y_Display, Mouse::iXMouse - Mouse::Mouse_Selection_Box_x_Display, Mouse::iYMouse - Mouse::Mouse_Selection_Box_y_Display};
      SDL_RenderDrawRectF(Graphics::renderer, &p);
    }
  }

  void Display_Mouse(entt::registry &zone) {
    auto view = zone.view<Component::Camera>();
    for (auto focus: view) {
      auto &componentCamera = view.get<Component::Camera>(focus);

      SDL_Rect srcRect = {0, 0, 32, 32};
      SDL_FRect d = {Mouse::iXMouse, Mouse::iYMouse, 32.0f / componentCamera.scale.x, 32.0f / componentCamera.scale.y};
      if (zone.any_of<Component::Icon>(Mouse::mouseData.mouseItem)) {
        SDL_RenderCopyF(Graphics::renderer, zone.get<Component::Icon>(Mouse::mouseData.mouseItem).pBackground, &srcRect, &d);
      }
    }
  }

  void Show_Grid(entt::registry &zone, Map::Node3 &terrain) {
    SDL_SetRenderDrawColor(Graphics::renderer, 255, 155, 255, 255);

    if (gridDepth > 3) {
      gridDepth = 3;
    }
    if (gridDepth < -1) {
      gridDepth = -1;
    } else {
      auto view = zone.view<Component::Camera>();

      for (auto id: view) {
        auto &camera = view.get<Component::Camera>(id);
        SDL_FRect offset = camera.screen;
        SDL_FRect screen = {camera.screen.x - (camera.screen.w * 0.5f), camera.screen.y - (camera.screen.h * 0.5f), camera.screen.w * 2.0f, camera.screen.h * 2.0f};
        //SDL_FRect debug;


        if (Utilities::bFRect_Intersect(screen, terrain.sCollide_Box)) {// checks terrain for visibility like grass and such
          for (int i = 0; i < Map::size; i++) {
            if (gridDepth == 0) {
              SDL_FRect o = terrain.sCollide_Box;
              o.x -= offset.x;
              o.y -= offset.y;
              SDL_RenderDrawRectF(Graphics::renderer, &o);
            }
            if (Utilities::bFRect_Intersect(screen, terrain.nodes[i].sCollide_Box)) {
              for (int j = 0; j < Map::size; j++) {
                if (gridDepth == 1) {
                  SDL_FRect o = terrain.nodes[i].sCollide_Box;
                  o.x -= offset.x;
                  o.y -= offset.y;
                  SDL_RenderDrawRectF(Graphics::renderer, &o);
                }
                if (Utilities::bFRect_Intersect(screen, terrain.nodes[i].nodes[j].sCollide_Box)) {
                  for (int k = 0; k < Map::size; k++) {
                    if (Utilities::bFRect_Intersect(screen, terrain.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
                      for (int l = 0; l < Map::size; l++) {
                        if (gridDepth == 2) {
                          SDL_FRect o = terrain.nodes[i].nodes[j].nodes[k].sCollide_Box;
                          o.x -= offset.x;
                          o.y -= offset.y;
                          SDL_RenderDrawRectF(Graphics::renderer, &o);
                        }
                        if (Utilities::bFRect_Intersect(screen, terrain.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
                          for (int a = 0; a < terrain.nodes[i].nodes[j].nodes[k].cells[l].entities.size(); a++) {
                            if (gridDepth == 3) {
                              SDL_FRect o = terrain.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box;
                              o.x -= offset.x;
                              o.y -= offset.y;
                              SDL_RenderDrawRectF(Graphics::renderer, &o);
                            }
                          }
                          //std::cout << terrain.nodes[i].nodes[j].nodes[k].cells[l].entities.size() << std::endl;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  void Show_Attacks(entt::registry &zone) {
    SDL_SetRenderDrawColor(Graphics::renderer, 155, 155, 55, 255);
    auto weapon_view = zone.view<Component::Weapon_Size>();
    auto camera_view = zone.view<Component::Camera>();
    for (auto camera: camera_view) {
      auto &cam = camera_view.get<Component::Camera>(camera);
      SDL_FRect offset = cam.screen;
      for (auto entity: weapon_view) {
        auto &o = weapon_view.get<Component::Weapon_Size>(entity).attackArea;

        o.x -= offset.x;
        o.y -= offset.y;
        SDL_RenderDrawRectF(Graphics::renderer, &o);
      }
    }
  }

  void Unit_Arrive_UI(entt::registry &zone) {
    auto view = zone.view<Component::Mouse_Move>();
    auto view2 = zone.view<Component::Camera>();
    for (auto camera: view2) {
      auto &cam = view2.get<Component::Camera>(camera);
      for (auto entity: view) {
        auto &mov = view.get<Component::Mouse_Move>(entity);
        SDL_FRect o = {(mov.fX_Destination - 15.0f) - cam.screen.x, (mov.fY_Destination - 15.0f) - cam.screen.y, 30.0f, 30.0f};
        SDL_SetRenderDrawColor(Graphics::renderer, 155, 155, 255, 255);
        SDL_RenderFillRectF(Graphics::renderer, &o);
      }
    }
  }

  void Background(entt::registry &zone) {
    Display_Selected(zone);
  }

  void Init_UI(f2 scale) {
    // UI_Resources::Init_Frames ();
    // Unit_Frames::Init_Frames();
    //    UI_Frames::Init_Frames();
  }

  void Foreground(entt::registry &zone, int &state, Component::Camera &camera) {
    //    Show_Grid(Map::terrain);
    //Display_Military_Groups();
    //Display_Unit_Formations(camera);
    Update_Interaction_Box(zone);
    Loot_Panel::Render_Loot(zone, state, camera);
    UI_Frames::Show_Menu_Frame(zone, camera);
    Debug_System::Framerate(zone, camera);
    //Unit_Arrive_UI();
    Display_Mouse(zone);
    Display_Selection_Box();
    Debug_System::Debug_Positions(zone);
    //Show_Attacks();
  }
}// namespace Interface
