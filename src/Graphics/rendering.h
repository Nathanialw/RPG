#pragma once

#include "camera.h"
#include "entt/entt.hpp"
#include "fog.h"
#include "frame_rendering.h"
#include "graphics.h"
#include "pause.h"
#include "render_iso_tiles.h"
#include "rendering_components.h"
#include "timer.h"
#include "tooltips.h"
#include "ui_resources.h"
#include "utilities.h"
#include "world_grid.h"
#include <SDL2/SDL.h>
#include <vector>

namespace Rendering {

  using namespace Rendering_Components;

  namespace {
    bool showSpriteBox = false;
    bool debug = false;
  }// namespace

  void gg() {
    auto &zone = World::world[World::currentZone.current].zone;

    auto view = zone.view<Component::Renderable>();
    auto camera_view = zone.view<Component::Camera>();

    for (auto camera: camera_view) {
      auto &screen = camera_view.get<Component::Camera>(camera).screen;
      for (auto entity: view) {
        auto &lines = view.get<Component::Renderable>(entity).lineSegment;
        for (auto line: lines) {
          SDL_RenderDrawLineF(Graphics::renderer, line.start.x - screen.x, line.start.y - screen.y, line.end.x - screen.x, line.end.y - screen.y);
        }
      }
    }
  }

  int ComparePointAndLine(Component::Position point, Component::Line_Segment line) {
    float pointY = point.y;
    if (pointY > line.start.y && pointY > line.end.y) {
      return 0;
    } else if (pointY < line.start.y && pointY < line.end.y) {
      return 1;
    } else {
      float slope = (line.end.y - line.start.y) / (line.end.x - line.start.x);
      float intercept = line.start.y - (slope * line.start.x);
      float yOnLineForPoint = (slope * point.x) + intercept;
      return yOnLineForPoint > point.y ? 1 : 0;
    }
  }

  int Sort(const Component::Renderable &lhs, const Component::Renderable &rhs) {
    if (!lhs.lineSegment.empty()) {
      for (auto line: lhs.lineSegment)
        if (ComparePointAndLine(rhs.point, line)) {
          Utilities::Log("collision 1");
          return 0;
        } else {
          Utilities::Log("collision 2");
          return 1;
        };
    }

    if (!rhs.lineSegment.empty()) {
      for (auto line: rhs.lineSegment) {
        if (ComparePointAndLine(lhs.point, line)) {
          Utilities::Log("collision 3");
          return 1;
        } else {
          Utilities::Log("collision 4");
          return 0;
        };
      }
    }

    Utilities::Log("collision 5");
    return lhs.point.y < rhs.point.y;
  }

  void Sort_Positions(entt::registry &zone) {
    zone.sort<Component::Renderable>([](const Component::Renderable &lhs, const Component::Renderable &rhs) {
      return Sort(lhs, rhs);
    });
  }

  void Render_UI(entt::registry &zone, int &state, SDL_Renderer *renderer, Component::Camera &camera) {
    auto view = zone.view<Component::Input>();
    for (auto player_ID: view) {

      Action_Bar::Render_Action_Bar(zone, state, camera);

      if (UI::bToggleCharacterUI) {
        //render UI
        UI::Character_UI = Camera_Control::Convert_FRect_To_Scale(UI::defaultScreenPosition, camera);
        Graphics::Render_FRect(Graphics::itsmars_Inventory, {255, 255, 255}, &UI::charui, &UI::Character_UI);
        //reder equipment slots
        UI::Equipment_UI::Update_Equipment_Position(camera);
        UI::Equipment_UI::Render_Equipment_Slot(zone, state, renderer, camera, player_ID);
        //render Items in bag
        UI::Bag_UI::screenBag = Camera_Control::Convert_FRect_To_Scale(UI::Bag_UI::bagRect, camera);
        UI::Bag_UI::Render_Bag_Slot(zone, player_ID, state, renderer, camera);
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

  void Render_Dead(entt::registry &zone, Component::Camera &camera) {//state

    auto view1 = zone.view<Component::Renderable, Action_Component::Action, Component::Position, Sprite_Sheet_Info, Component::Direction, Sprite_Offset, Component::Scale, Component::Entity_Type, Component::Dead>();
    auto view = zone.view<Component::Renderable, Rendering_Components::Equipment_Sprites>();
    auto mounts = zone.view<Component::Renderable, Mount_Sprite>();

    //        std::cout << "----------------------" << std::endl;

    for (auto entity: view1) {
      auto [renderable, action, position, sheetData, direction, spriteOffset, scale, type, dead] = view1.get(entity);
      //            std::cout << renderable.y << " ";
      SDL_Rect clipRect;
      SDL_FRect renderRect;
      SDL_Texture *texture;
      SDL_Color color;

      if (sheetData.flareSpritesheet) {
        /// get the next frame
        Update_Frame(zone, entity, scale, sheetData, direction, action);
        /// get/update the clip rect
        Get_Spritesheet_Type(clipRect, sheetData, direction, action);
        /// set the render rect size and position
        renderRect = Utilities::Scale_Rect(clipRect, scale.scale);
        renderRect.x = (position.x - camera.screen.x - spriteOffset.x);
        renderRect.y = (position.y - camera.screen.y - spriteOffset.y);

        //                render icons
        if (type == Component::Entity_Type::item) {
          renderRect.w = (spriteOffset.x * 2.0f);
          renderRect.h = (spriteOffset.y * 2.0f);
        }
        texture = sheetData.flareSpritesheet->at(sheetData.sheet_name).texture;
        color = sheetData.flareSpritesheet->at(sheetData.sheet_name).color;
        SDL_SetTextureAlphaMod(texture, renderable.alpha);
        Graphics::Render_FRect(texture, color, &clipRect, &renderRect);
      }
      // else if {
      //	sheetData
      //}

      else if (sheetData.sheetData) {
        if (mounts.contains(entity)) {
          //                render horse half behind unit
        }

        //                render unit
        Frame_Increment(zone, entity, scale, sheetData, action, direction);
        Render_Sprite(zone, entity, camera, scale, renderable, position, spriteOffset, sheetData);

        //                render equipment
        if (view.contains(entity)) {
          auto equipment = view.get<Rendering_Components::Equipment_Sprites>(entity);
          Render_Equipment(equipment, scale, sheetData, camera, position, renderable, spriteOffset);
        }

        if (mounts.contains(entity)) {
          //                render horse half in front unit
        }
      } else {
        Utilities::Log("Animation_Frame() fallthrough error: all pointers NULL");
        return;
      }
    }
    //        std::cout << "\n";
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

  void Render_Ortho_Tiles(entt::registry &zone, tmx::Map &map, Component::Camera &camera) {
    float originX = 0.0f;
    float originY = 0.0f;
    SDL_Rect tileSpriteRect = {0, 0, (int) map.getTileSize().x, (int) map.getTileSize().y};
    SDL_FRect renderPosition = {0.0f, 0.0f, (float) map.getTileSize().x, (float) map.getTileSize().y};
    auto &numOfTiles = map.getTileCount();
    auto &layers = map.getLayers();
    const auto tileWidth = (float) map.getTileSize().x;
    const auto tileHeight = (float) map.getTileSize().y;
    int newX = (int) camera.screen.x;
    int newY = (int) camera.screen.y;

    int X = (newX / tileWidth);
    int Y = (newY / tileHeight);

    int width = X + (camera.screen.w / tileWidth);
    int height = Y + (camera.screen.h / tileHeight);

    int g = 0;
    //int o = 0;
    auto &tilesets = Maps::map.getTilesets();
    int p = Maps::map.getTilesets().size() - 1;
    for (int i = 0; i < layers.size(); i++) {
      if (layers[i]->getType() == tmx::Layer::Type::Tile) {
        const auto &tiles = layers[i]->getLayerAs<tmx::TileLayer>().getTiles();
        if (i == 2) {
          /// renders all objects just above the 2nd layer but below the 3rd in tiled
        }
        for (int x = X; x < width; x++) {
          if (x < 0) { x = 0; }
          for (int y = Y; y < height; y++) {
            //h++;
            if (y < 0) { y = 0; }
            if (x >= numOfTiles.x) { break; }
            if (y >= numOfTiles.y) { break; }
            renderPosition.x = originX + (x * tileWidth);
            renderPosition.y = originY + (y * tileHeight);
            renderPosition.x -= camera.screen.x;
            ;
            renderPosition.y -= camera.screen.y;
            ;

            int k = (numOfTiles.x * y) + x;
            auto id = tiles[k].ID;

            for (int tilesetCount = p; tilesetCount >= 0; --tilesetCount) {
              const tmx::Tileset *tileset = &tilesets[tilesetCount];
              if (tileset->getFirstGID() - 1 <= id) {
                id -= tilesets[tilesetCount].getFirstGID() - 1;
                std::string name = tileset->getName();
                int tileCount = tileset->getColumnCount();
                SDL_Rect data = getTexture(id, tileCount, tileSpriteRect);
                if (Graphics::pTexture[name] != NULL) {
                  Graphics::Render_FRect(Graphics::pTexture[name], {255, 255, 255}, &data, &renderPosition);
                }
                //h++;
                break;
              }
            }
          }
          g++;
        }
        if (i == 1) {
        }
      }
    }
  }

  void Render_Mouse_Item(entt::registry &zone, Component::Camera &camera) {
    SDL_FRect DisplayRect = {};
    auto view = zone.view<Component::Position, Component::Icon, Component::On_Mouse>();
    for (auto item: view) {
      const auto &mouse = view.get<Component::On_Mouse>(item);
      const auto &x = view.get<Component::Position>(item).x;
      const auto &y = view.get<Component::Position>(item).y;
      if (mouse.type == Component::Icon_Type::item) {
        const auto &icon = view.get<Component::Icon>(item);
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
    }
    if (Mouse_Struct::mouseData.type == Component::Icon_Type::spell) {
      //      const auto &icon = UI_Spellbook::spellbook.Skill_Trees[UI_Spellbook::fire][0].icon;
      const auto &icon = UI_Spellbook::spellbook.Skill_Trees[Mouse_Struct::mouseData.tree][Mouse_Struct::mouseData.index].icon;
      //      const auto &icon = Mouse::ss.spell;

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
          camera.screen.x - 500.0f,
          camera.screen.y - 500.0f,
          camera.screen.w + 1000.0f,
          camera.screen.h + 1000.0f};

      SDL_FRect despawnRect = {
          camera.screen.x - 1000.0f,
          camera.screen.y - 1000.0f,
          camera.screen.w + 2000.0f,
          camera.screen.h + 2000.0f};
      auto objectsView = zone.view<Component::Position, Component::Interaction_Rect>();
      //if you add Item_Component::Item_Type to this list it will not show ground items, instead I can give a graphic to a ground item
      float bottomOfScreenEdge = camera.screen.y + camera.screen.h;
      float bottomOfRenderRect = renderRect.y + renderRect.h;
      //    Debug::renderChecks = objectsView.size_hint();
      int i = 0;
      for (auto entity: objectsView) {
        i++;
        auto &position = objectsView.get<Component::Position>(entity);
        SDL_FPoint point = {position.x, position.y};
        if (Utilities::bFPoint_FRectIntersect(point, renderRect)) {
          if (zone.any_of<Component::Renderable>(entity)) {
            //  update renderable values
            int alpha = Set_Render_Position_Alpha(bottomOfScreenEdge, bottomOfRenderRect, position.y);
            auto &renderable = zone.get<Component::Renderable>(entity);
            renderable.alpha = alpha;
            //if (zone.all_of<Component::Line_Segment>(entity) == false) {
            renderable.point.x = position.x;
            renderable.point.y = position.y;
            //}
          } else {
            //                    emplace and initialize renderable
            int alpha = Set_Render_Position_Alpha(bottomOfScreenEdge, bottomOfRenderRect, position.y);
            auto &renderable = zone.emplace_or_replace<Component::Renderable>(entity);
            renderable.alpha = alpha;
            // if (zone.all_of<Component::Line_Segment>(entity) == false) {
            renderable.point.x = position.x;
            renderable.point.y = position.y;
            if (zone.any_of<Rendering_Components::Interior_Sheet_Info>(entity)) {
              auto name = zone.get<Rendering_Components::Interior_Sheet_Info>(entity).collisionBocArrayIndex;
              auto ff = Collision_Component::houseColliders[name].lineSegment;
              Component::Line_Segment line = {{Mouse::iXWorld_Mouse + ff.start.x, Mouse::iYWorld_Mouse + ff.start.y}, {Mouse::iXWorld_Mouse + ff.end.x, Mouse::iYWorld_Mouse + ff.end.y}};
              renderable.lineSegment.emplace_back(line);
            }
            //	  }
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
      zone.destroy(entity);
      Utilities::Log("object destroyed");
    }
    zone.compact<>();
  }

  void Render_Map(entt::registry &zone, int &state, Component::Camera &camera) {
    SDL_RenderClear(Graphics::renderer);

    Maps::Render(zone, state, camera);
    //        Render_Iso_Tiles(zone, Maps::map, camera);
    //        Render_Ortho_Tiles(zone, Maps::map, camera);
    Display_Background_Objects(zone, camera);
    //   need a render dead render routine so the foreground objects are on top
    //   alternatively we could sort all the background and foreground objects by WHEN they spawned rather than simple y position
    //Render_Dead(zone, camera);
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
      Sort_Positions(zone);
      Render_Map(zone, state, camera);
      Remove_Entities_From_Registry(zone, state);// cannot be done before clearing the entities from the quad tree
      //            RenderLine(zone, camera);
      Items::Show_Ground_Items(zone, camera);
      Items::Unit_Name_On_Mouseover(zone, camera);
      Social_Control::Show_Dialogue(zone, camera);
      Items::Name_On_Mouseover(zone, camera);
      Fog::Update_Fog(camera);
      UI_Resources::Show_Frames(zone, camera);
      Unit_Frames::Show_Frames(zone, camera);
      Render_UI(zone, state, Graphics::renderer, camera);
      Character_Stats::Render_Character_Stats(camera);
      Items::Update_Mouse_Slot_Position(zone, state, Mouse::mouseItem, Mouse::itemCurrentlyHeld, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
      Damage_Text::Show_Damage(zone, camera);
      UI_Spellbook::Draw_Spellbook(camera);
      UI_Info::Draw_Attributes(camera);
      Pause::Pause_Control(camera);
      if (!Menu::Render_Menu(zone, state, camera)) {
        return false;
      }
      gg();
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
