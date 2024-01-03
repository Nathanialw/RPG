#pragma once

#include "FLARE_frames.h"
#include "PVG_frames.h"
#include "utilities.h"
#include <SDL2/SDL_pixels.h>

void Get_Spritesheet_Type(SDL_Rect &clipRect, Rendering_Components::Sprite_Sheet_Info &sheetData, Component::Direction &direction, Action_Component::Action &action) {
  /// get the height and width
  clipRect = {0, 0, sheetData.flareSpritesheet->at(sheetData.sheet_name).spriteWidth, sheetData.flareSpritesheet->at(sheetData.sheet_name).spriteHeight};

  if (sheetData.type == "flare") {
    Update_Frame_Flare(clipRect, action.frame, sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].startFrame, direction);
  } else if (sheetData.type == "pvg") {
    Update_Frame_PVG(clipRect, sheetData, action, direction);
  } else if (sheetData.type == "item") {
    /// assigned unused variables to store spritesheet indexes, currentFrame == row, frameIndex == column
    Update_Item_Frame(clipRect, action.frame, sheetData.frameIndex);
  }
}

SDL_FRect Position_For_Render(std::unordered_map<std::string, Rendering_Components::Sheet_Data> *sheetData, std::string &name, uint16_t frameIndex, Component::Position &position, Component::Camera &camera, Component::Scale &scale, Rendering_Components::Sprite_Offset &offset, SDL_Rect &clipRect, SDL_FRect &renderRect, Component::Interaction_Rect &interactionRect) {
  clipRect = sheetData->at(name).frameList[frameIndex].clip;
  renderRect = Utilities::Scale_Rect(clipRect, scale.scale);
  renderRect.x = sheetData->at(name).frameList[frameIndex].x_offset * scale.scale - offset.x + position.x;
  renderRect.y = sheetData->at(name).frameList[frameIndex].y_offset * scale.scale - offset.y + position.y;
  interactionRect.rect = renderRect;
  renderRect.x -= camera.screen.x;
  renderRect.y -= camera.screen.y;
  return renderRect;
}

void Render_Equipment(Rendering_Components::Equipment_Sprites &equipment, Component::Scale &scale, Rendering_Components::Sprite_Sheet_Info &sheetData, Component::Camera &camera, Component::Position &position, Component::Renderable &renderable, Rendering_Components::Sprite_Offset &spriteOffset) {
  SDL_Rect clipRect;
  for (auto item: equipment.sheet) {
    if (item.ItemSheetData) {
      Component::Interaction_Rect itemInteractionRect = {};
      SDL_FRect renderRect = Position_For_Render(item.ItemSheetData, item.name, sheetData.frameIndex, position, camera, scale, spriteOffset, clipRect, renderRect, itemInteractionRect);
      SDL_Texture *texture = item.ItemSheetData->at(item.name).texture;
      SDL_Color color = item.color;
      SDL_SetTextureAlphaMod(texture, renderable.alpha);
      Graphics::Render_FRect(texture, color, &clipRect, &renderRect);
    }
  }
}

void Render_Sprite(entt::registry &zone, entt::entity &entity, Component::Camera &camera, Component::Scale &scale, Component::Renderable &renderable, Component::Position &position, Rendering_Components::Sprite_Offset &spriteOffset, Rendering_Components::Sprite_Sheet_Info &sheetData) {
  SDL_Rect clipRect;
  SDL_FRect renderRect;

  if (zone.any_of<Component::Interaction_Rect>(entity)) {
    auto interactionRect = zone.get<Component::Interaction_Rect>(entity);
    renderRect = Position_For_Render(sheetData.sheetData, sheetData.sheet_name, sheetData.frameIndex, position, camera, scale, spriteOffset, clipRect, renderRect, interactionRect);
  } else {
    Component::Interaction_Rect interactionRect = {};
    renderRect = Position_For_Render(sheetData.sheetData, sheetData.sheet_name, sheetData.frameIndex, position, camera, scale, spriteOffset, clipRect, renderRect, interactionRect);
  }
  SDL_Texture *texture = sheetData.sheetData->at(sheetData.sheet_name).texture;
  SDL_SetTextureAlphaMod(texture, renderable.alpha);
  if (sheetData.blendType == Rendering_Components::ghost) {
    SDL_SetTextureBlendMode(texture, SDL_BlendMode::SDL_BLENDMODE_ADD);
    Graphics::Render_FRect(texture, sheetData.color, &clipRect, &renderRect);
    SDL_SetTextureBlendMode(texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
  }
  else if (sheetData.blendType == Rendering_Components::reanimated) {
    Graphics::Render_FRect(texture, {155,55,55}, &clipRect, &renderRect);
  }
  else {
    Graphics::Render_FRect(texture, sheetData.color, &clipRect, &renderRect);
  }
}

void Animation_Frame(entt::registry &zone, Component::Camera &camera) {//state

  auto view1 = zone.view<Component::Renderable, Action_Component::Action, Component::Position, Rendering_Components::Sprite_Sheet_Info, Component::Direction, Rendering_Components::Sprite_Offset, Component::Scale, Component::Entity_Type>();
  auto view = zone.view<Component::Renderable, Rendering_Components::Equipment_Sprites>();
  auto mounts = zone.view<Component::Renderable, Rendering_Components::Mount_Sprite>();

  Debug::settingsValue[Debug::NumRendered] = view1.size_hint();
  for (auto entity: view1) {
    auto [renderable, action, position, sheetData, direction, spriteOffset, scale, type] = view1.get(entity);
    //            std::cout << renderable.y << " ";
    SDL_Rect clipRect;
    SDL_FRect renderRect;
    SDL_Texture *texture;
    SDL_Color color;

    if (sheetData.flareSpritesheet) {
      //      get the next frame
      Update_Frame(zone, entity, scale, sheetData, direction, action);
      //      get/update the clip rect
      Get_Spritesheet_Type(clipRect, sheetData, direction, action);
      //      set the render rect size and position
      renderRect = Utilities::Scale_Rect(clipRect, scale.scale);
      renderRect.x = (position.x - camera.screen.x - spriteOffset.x);
      renderRect.y = (position.y - camera.screen.y - spriteOffset.y);
      //      render icons
      if (type == Component::Entity_Type::item) {
        renderRect.w = (spriteOffset.x * 2.0f);
        renderRect.h = (spriteOffset.y * 2.0f);
      }

      texture = sheetData.flareSpritesheet->at(sheetData.sheet_name).texture;
      color = sheetData.flareSpritesheet->at(sheetData.sheet_name).color;
      SDL_SetTextureAlphaMod(texture, renderable.alpha);
      Graphics::Render_FRect(texture, color, &clipRect, &renderRect);
    } else if (sheetData.sheetData) {
      if (sheetData.type == "packer_linear") {
        Update_Packer_Linear_Frame(sheetData, action);
        Render_Sprite(zone, entity, camera, scale, renderable, position, spriteOffset, sheetData);
      } else {
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
      }
    } else {
      Utilities::Log("Animation_Frame() fallthrough error: all pointers NULL");
    }
  }
  //        std::cout << "\n";
}
