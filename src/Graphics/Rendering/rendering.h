#pragma once

#include "Fog_Of_War/fog_of_war.h"
#include "Input/camera.h"
#include "Input/pause.h"
#include "Maps/Pathing/a_star.h"
#include "Maps/World/map.h"
#include "Maps/World/world_grid.h"
#include "UI/Debug_Panel/ui_debug.h"
#include "entt/entt.hpp"
#include "fog.h"
#include "frame_rendering.h"
#include "graphics.h"
#include "render_iso_tiles.h"
#include "rendering_components.h"
#include "sort.h"
#include "timer.h"
#include "tooltips.h"
#include "ui_resources.h"
#include "utilities.h"
#include <SDL2/SDL.h>
#include <vector>

namespace Rendering {

  using namespace Rendering_Components;

  namespace {
    bool showSpriteBox = false;
    bool debug = false;
  }// namespace

  void Render_UI(entt::registry &zone, int &state, SDL_Renderer *renderer, Component::Camera &camera) {
    auto view = zone.view<Component::Input>();
    for (auto player_ID: view) {

      Action_Bar::Render_Action_Bar(zone, state, camera);

      if (Bag_UI::bToggleCharacterUI) {
        //render UI
        Bag_UI::Character_UI = Camera_Control::Convert_FRect_To_Scale(Bag_UI::defaultScreenPosition, camera);
        Graphics::Render_FRect(Graphics::itsmars_Inventory, {255, 255, 255}, &Bag_UI::charui, &Bag_UI::Character_UI);
        //reder equipment slots
        Equipment_UI::Update_Equipment_Position(camera);
        Equipment_UI::Render_Equipment_Slot(zone, state, renderer, camera, player_ID);
        //render Items in bag
        Bag_UI::screenBag = Camera_Control::Convert_FRect_To_Scale(Bag_UI::bagRect, camera);
        Bag_UI::Render_Bag_Slot(zone, player_ID, state, renderer, camera);
      }
    }
  }

  void Display_Background_Objects(entt::registry &zone, Component::Camera &camera) {
    auto view1 = zone.view<Component::Renderable, Action_Component::Action, Component::Position, Rendering_Components::Sprite_Sheet_Info, Rendering_Components::Sprite_Offset, Component::Scale, Component::Entity_Type, Rendering_Components::Background>();

    for (auto entity: view1) {
      auto [renderable, action, position, sheetData, spriteOffset, scale, type] = view1.get(entity);

      if (sheetData.sheetData) {
        Render_Sprite(zone, entity, camera, scale, renderable, position, spriteOffset, sheetData);
      } else {
        Utilities::Log("Static_Animation_Frame() fallthrough error: all pointers NULL");
      }
    }
  }

  void Display_Foreground_Objects(entt::registry &zone, Component::Camera &camera) {
    auto view1 = zone.view<Component::Renderable, Component::Position, Rendering_Components::Sprite_Sheet_Info, Rendering_Components::Sprite_Offset, Component::Scale, Component::Entity_Type, Rendering_Components::Foreground>();

    for (auto entity: view1) {
      auto [renderable, position, sheetData, spriteOffset, scale, type] = view1.get(entity);

      if (sheetData.sheetData) {
        Render_Sprite(zone, entity, camera, scale, renderable, position, spriteOffset, sheetData);
      } else {
        Utilities::Log("Static_Animation_Frame() fallthrough error: all pointers NULL");
      }
    }
  }

  SDL_Rect Explosion_Frame_Update(Component::Sprite_Frames &frame) {
    /// reset X to zero and increment Y
    if (frame.frameX >= 8) {
      frame.frameX = 0;
      frame.frameY++;
    }
    /// set Output Rect values
    int row = frame.frameX * 128;
    int column = frame.frameY * 128;
    int width = 128;
    int height = 128;
    SDL_Rect rect = {row, column, width, height};
    ///increment X
    frame.frameX++;
    return rect;
  }

  void Render_Explosions(entt::registry &zone, Component::Camera &camera) {
    SDL_Rect xClipPos;
    float sx;
    float sy;
    auto view = zone.view<Component::Explosion, Component::Position, Component::Frame_Delay, Component::Texture, Component::Sprite_Frames>();
    for (auto spell: view) {
      auto &anim = view.get<Component::Explosion>(spell);
      auto &x = view.get<Component::Position>(spell);
      auto &y = view.get<Component::Position>(spell);
      auto &texture = view.get<Component::Texture>(spell);
      auto &frames = view.get<Component::Sprite_Frames>(spell);
      auto &delay = view.get<Component::Frame_Delay>(spell);
      delay.currentFrameTime += Timer::timeStep;
      if (delay.currentFrameTime >= delay.timeBetweenFrames) {
        if (frames.currentFrame <= frames.maxFrames) {// if there are still frames remaining
          /// only fire this at 60 frames/sec
          xClipPos = Explosion_Frame_Update(frames);                       //get state and direction state sprite draw from
          anim.renderPosition = Utilities::SDL_Rect_To_SDL_FRect(xClipPos);//save sprite for vector
          texture.clippedSpriteFrame = xClipPos;                           //save position for renderer
          frames.currentFrame++;
        } else {
          /// remove explosion from scene and free the entity
          zone.destroy(spell);
        }
        delay.currentFrameTime = 0;
      }
      sx = x.x - camera.screen.x - anim.offsetToAlignWithFireball.x;
      sy = y.y - camera.screen.y - anim.offsetToAlignWithFireball.y;
      anim.renderPosition.x = sx - anim.positionOffset.x;
      anim.renderPosition.y = sy - anim.positionOffset.y;
      SDL_RenderCopyF(Graphics::renderer, texture.pTexture, &texture.clippedSpriteFrame, &anim.renderPosition);
      if (showSpriteBox) {
        SDL_RenderDrawRectF(Graphics::renderer, &anim.renderPosition);
      }
    }
  }

  void Render_Mouse_Item(entt::registry &zone, Component::Camera &camera) {
    if (Mouse::mouseData.type == Component::Icon_Type::item) {
      const auto &icon = zone.get<Component::Icon>(Mouse::mouseData.mouseItem);
      SDL_FRect DisplayRect = {};
      DisplayRect.w = icon.renderRectSize.x / camera.scale.x;
      DisplayRect.h = icon.renderRectSize.y / camera.scale.y;
      DisplayRect.x = Mouse::iXMouse - (DisplayRect.w / 2.0f);
      DisplayRect.y = Mouse::iYMouse - (DisplayRect.h / 2.0f);
      SDL_RenderCopyF(Graphics::renderer, icon.pBackground, &icon.clipIcon, &DisplayRect);
      SDL_RenderCopyF(Graphics::renderer, icon.pTexture, &icon.clipSprite, &DisplayRect);
      SDL_RenderCopyF(Graphics::renderer, icon.pIconRarityBorder, &icon.clipIcon, &DisplayRect);
      if (showSpriteBox) {
        //SDL_RenderDrawRect(Graphics::renderer, &DisplayRect);
      }
    }
    if (Mouse_Struct::mouseData.type == Component::Icon_Type::spell) {
      //      const auto &icon = UI_Spellbook::spellbook.Skill_Trees[UI_Spellbook::fire][0].icon;
      const auto &icon = UI_Spellbook::spellbook.Skill_Trees[Mouse_Struct::mouseData.tree][Mouse_Struct::mouseData.index].icon;
      //      const auto &icon = Mouse::ss.spell;
      SDL_FRect DisplayRect = {};
      DisplayRect.w = icon.renderRectSize.x / camera.scale.x;
      DisplayRect.h = icon.renderRectSize.y / camera.scale.y;
      DisplayRect.x = Mouse::iXMouse - (DisplayRect.w / 2.0f);
      DisplayRect.y = Mouse::iYMouse - (DisplayRect.h / 2.0f);
      //std::cout << "x: " << DisplayRect.x << " y: " << DisplayRect.y << " w: " << DisplayRect.w << " h: " << DisplayRect.h << std::endl;
      SDL_RenderCopyF(Graphics::renderer, icon.pBackground, NULL, &DisplayRect);
      SDL_RenderCopyF(Graphics::renderer, icon.pTexture, NULL, &DisplayRect);
      SDL_RenderCopyF(Graphics::renderer, icon.pIconBorder, NULL, &DisplayRect);
    }
  }

  int Set_Render_Position_Alpha(float &screenEdge, float &renderEdge, float &yPosition) {
    if (yPosition <= screenEdge) {
      return 255;
    } else {
      float x = yPosition - screenEdge;
      float edgeBuffer = renderEdge - screenEdge;
      float y = edgeBuffer / x;
      int alpha = 255 - (255 / (int) y);
      return alpha;
    }
  }

  float placeRenderable = 0.0f;

  void Add_Remove_Renderable_Component(entt::registry &zone, int &state, Component::Camera &camera) {
    placeRenderable += Timer::timeStep;
    if (placeRenderable >= 150.0f) {
      placeRenderable -= 150.0f;
      SDL_FRect renderRect = {
          camera.screen.x - 550.0f,
          camera.screen.y - 550.0f,
          camera.screen.w + 1100.0f,
          camera.screen.h + 1100.0f};

      SDL_FRect despawnRect = {
          camera.screen.x - 500.0f,
          camera.screen.y - 500.0f,
          camera.screen.w + 1000.0f,
          camera.screen.h + 1000.0f};
      auto objectsView = zone.view<Component::Position, Component::Interaction_Rect, Component::Name, Component::Is_Inside>();
      //if you add Item_Component::Item_Type to this list it will not show ground items, instead I can give a graphic to a ground item
      float bottomOfScreenEdge = camera.screen.y + camera.screen.h;
      float bottomOfRenderRect = renderRect.y + renderRect.h;
      //    Debug::renderChecks = objectsView.size_hint();
      int i = 0;
      for (auto entity: objectsView) {
        i++;
        auto &position = objectsView.get<Component::Position>(entity);
        auto &name = objectsView.get<Component::Name>(entity).first;
        auto &inside = objectsView.get<Component::Is_Inside>(entity).inside;
        //        auto &inside = objectsView.get<Component::Is_Inside>(entity).interior;
        SDL_FPoint point = {position.x, position.y};
        if (Utilities::bFPoint_FRectIntersect(point, renderRect)) {
          if (zone.any_of<Component::Renderable>(entity)) {
            int alpha = Set_Render_Position_Alpha(bottomOfScreenEdge, bottomOfRenderRect, position.y);
            auto &renderable = zone.get<Component::Renderable>(entity);
            renderable.alpha = alpha;
            renderable.point.x = position.x;
            renderable.point.y = position.y;
            renderable.name = name;
            renderable.inside = inside;
          }

          else {
            int alpha = Set_Render_Position_Alpha(bottomOfScreenEdge, bottomOfRenderRect, position.y);
            auto &renderable = zone.emplace_or_replace<Component::Renderable>(entity);
            renderable.alpha = alpha;
            renderable.point.x = position.x;
            renderable.point.y = position.y;
            renderable.name = name;
            renderable.inside = inside;

            if (zone.any_of<Component::Saved_Line_Segments>(entity)) {
              auto lines = zone.get<Component::Saved_Line_Segments>(entity).lineSegment;
              auto index = zone.get<Component::Saved_Line_Segments>(entity).name;
              for (auto line: lines) {
                renderable.lineSegment.emplace_back(line);
                renderable.name = index;
              }
            }
          }
        }

        else if (!Utilities::bFPoint_FRectIntersect(point, despawnRect)) {
          if (zone.any_of<Component::Renderable>(entity)) {
            zone.remove<Component::Renderable>(entity);

            if (zone.any_of<Component::Tile_Index>(entity)) {
              auto &rect = objectsView.get<Component::Interaction_Rect>(entity);
              zone.emplace_or_replace<Component::Remove_From_Object_Tree>(entity, rect.rect);
            }
          }
        }
      }
      Debug::settingsValue[Debug::RenderChecks] = i;
    }
  }

  void Update_Cursor(Component::Camera &camera) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    Mouse::iXMouse = (float) mx;
    Mouse::iYMouse = (float) my;
    Mouse::iXWorld_Mouse = (Mouse::iXMouse / camera.scale.x) + camera.screen.x;//getting mouse world Position corrected for scale
    Mouse::iYWorld_Mouse = (Mouse::iYMouse / camera.scale.y) + camera.screen.y;//getting mouse world Position corrected for scale
    Mouse::iXMouse = Mouse::iXMouse / camera.scale.x;                          // getting the screen mouse position corrected for scale
    Mouse::iYMouse = Mouse::iYMouse / camera.scale.y;                          // getting the screen mouse position corrected for scale
    Mouse::mousePoint = {(float) mx, (float) my};
    Mouse::screenMousePoint = Camera_Control::Convert_FPoint_To_Scale(Mouse::mousePoint, camera);
  }

  void Update_Camera_And_Mouse(entt::registry &zone) {
    auto focus_view = zone.view<Component::Camera, Component::Position>();
    for (auto player: focus_view) {
      auto &camera = focus_view.get<Component::Camera>(player);
      auto &position = focus_view.get<Component::Position>(player);
      Camera_Control::Update_Camera_Follow(camera, position);
      Update_Cursor(camera);
    }
  }

  void Remove_Entities_From_Registry(entt::registry &zone, int &state) {
    auto view = zone.view<Component::Destroyed, Component::Interaction_Rect>();
    for (auto entity: view) {
      if (zone.any_of<Component::In_Object_Tree>(entity)) {
        auto &rect = view.get<Component::Interaction_Rect>(entity).rect;
        zone.emplace_or_replace<Component::Remove_From_Object_Tree>(entity, rect);
        Utilities::Log("item was still has In_Object_Tree");
        Quad_Tree::Remove_Entity_From_Tree(zone, entity, state);
      }
      if (zone.any_of<Component::Body>(entity)) {
        auto &body = zone.get<Component::Body>(entity).body;
        Collision::collisionList[state]->DestroyBody(body);
        zone.remove<Component::Body>(entity);
      }
      zone.destroy(entity);
      Utilities::Log("object destroyed");
      zone.compact<>();
    }
  }

  void Render_Map(entt::registry &zone, int &state, Component::Camera &camera) {
    SDL_RenderClear(Graphics::renderer);

    Maps::Render(zone, state, camera);
    Display_Background_Objects(zone, camera);
    //   need a render dead render routine so the foreground objects are on top
    //   alternatively we could sort all the background and foreground objects by WHEN they spawned rather than simple y position
    Display_Foreground_Objects(zone, camera);
    Interface::Background(zone);
    Animation_Frame(zone, camera);
    Render_Explosions(zone, camera);
  }

  bool Rendering(entt::registry &zone, int &state) {
    Update_Camera_And_Mouse(zone);
    SDL_FPoint mouse = {Mouse::iXMouse, Mouse::iYMouse};

    auto camera_view = zone.view<Component::Camera>();
    for (auto entity: camera_view) {
      auto &camera = camera_view.get<Component::Camera>(entity);
      //			SDL_RenderClear(Graphics::renderer);
      Add_Remove_Renderable_Component(zone, state, camera);
      Sort::Sort_Positions(zone);
      Render_Map(zone, state, camera);
      Remove_Entities_From_Registry(zone, state);// cannot be done before clearing the entities from the quad tree
                                                 //            RenderLine(zone, camera);
      A_Star::Draw(zone, camera);
      Fog_Of_War::Render(camera);
      Items::Show_Ground_Items(zone, camera);
      Items::Unit_Name_On_Mouseover(zone, camera);
      Social_Control::Show_Dialogue(zone, camera);
      Items::Name_On_Mouseover(zone, camera);
      Fog::Update_Fog(camera);
      UI_Resources::Show_Frames(zone, camera);
      Unit_Frames::Show_Frames(zone, camera);
      Render_UI(zone, state, Graphics::renderer, camera);
      Character_Stats::Render_Character_Stats(camera);
      Items::Update_Mouse_Slot_Position(zone, state, Mouse::mouseData.mouseItem, Mouse::mouseData.itemCurrentlyHeld, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
      Damage_Text::Show_Damage(zone, camera);
      UI_Spellbook::Draw_Spellbook(camera);
      UI_Debug::Draw(zone, camera);
      UI_Info::Draw_Attributes(camera);
      Pause::Pause_Control(camera);
      if (!Menu::Render_Menu(zone, state, camera)) {
        return false;
      }
      //      Sort::RenderLines();
      //Mouse
      Interface::Foreground(zone, state, camera);
      //on top of mouse
      Tooltip::Show_Item_Tooltip(zone, entity, state, mouse, camera);
      Render_Mouse_Item(zone, camera);
      SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
      if (Items::showGroundItems) {//****//search quad tree instead
        auto view = zone.view<Ground_Item, Component::Renderable>();
        for (auto item: view) {
          auto &box = zone.get<Ground_Item>(item);
          SDL_RenderDrawRectF(Graphics::renderer, &box.box);
        }
      }
    }
    return true;
  }

  void Present() {
    SDL_RenderPresent(Graphics::renderer);
  }
}// namespace Rendering
