#include "Objects/Stats/entity_data.h"
#include "SQLite_spritesheets.h"
#include "entt/entt.hpp"
#include "texture_packer.h"

namespace Rendering {

  struct Sheet_Data {
    Rendering_Components::Sprite_Sheet_Info frameData;
    //    std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerframeData = NULL;
    //    std::unordered_map<std::string, Rendering_Components::Image_Data> *imageData = NULL;
    Rendering_Components::Sprite_Sheet_Data frame;
  };

  Sheet_Data Set_Render_Interior(entt::registry &zone, const entt::entity &entity, std::string &templateName, int xmlIndex, const std::string img, const std::string xml) {
    ///get texture data
    SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
    int textureIndex = Entity_Data::Check_For_Template_ID(templateName);
    std::string sheetName;

    if (xmlIndex == -1) {
      xmlIndex = Game_Objects_Lists::objectIndexes[templateName];
    }

    Texture_Packer::Sheet_Data sheetData = Texture_Packer::Get_Texture_Data(textureIndex, templateName, img, xml, sheetName);
    auto &sprite = zone.emplace_or_replace<Rendering_Components::Interior_Sheet_Info>(entity);
    auto &interior = sprite.interior;
    interior.sheet_name = sheetName;
    interior.type = "RPG_Tools";
    interior.frameIndex = xmlIndex;

    if (!sheetData.packerData) {
      std::cout << "failed to load PVG_Building() for: " << templateName << std::endl;
      //      return {NULL};
      interior.imageData = sheetData.imageData;
      return {interior, {}};
    }
    std::cout << "BIG BUG PVG_Building() for: " << templateName << std::endl;
    return {interior, {}};
  }

  Sheet_Data Set_Rend(entt::registry &zone, const entt::entity &entity, std::string &templateName, int xmlIndex, const std::string img, const std::string xml) {
    ///get texture data
    SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
    int textureIndex = Entity_Data::Check_For_Template_ID(templateName);
    std::string sheetName;

    Texture_Packer::Sheet_Data sheetData = Texture_Packer::Get_Texture_Data(textureIndex, templateName, img, xml, sheetName);

    if (xmlIndex == -1) {
      xmlIndex = Game_Objects_Lists::objectIndexes[templateName];
    }

    auto &sprite = zone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(entity);
    sprite.sheet_name = sheetName;
    sprite.type = "RPG_Tools";
    sprite.frameIndex = xmlIndex;

    if (!sheetData.packerData) {
      std::cout << "Loading image data for: " << templateName << std::endl;

      sprite.imageData = sheetData.imageData;
      zone.emplace_or_replace<Component::Icon>(entity, sprite.imageData->at(sheetName).texture);
      return {sprite};
    }

    sprite.sheetData = sheetData.packerData;
    zone.emplace_or_replace<Component::Icon>(entity, sprite.sheetData->at(sheetName).texture);
    return {sprite, sprite.sheetData->at(sheetName).frameList[xmlIndex]};
  }

  Rendering_Components::Offsets Set_Offset(entt::registry &zone, const entt::entity &entity, const std::string colliderType, const float xOffset, const float yOffset, const Rendering_Components::Sprite_Sheet_Data &frame) {
    Rendering_Components::Sprite_Offset offset = {};
    if (colliderType == "rect") {
      offset = {xOffset, yOffset / 2.0f};
      zone.emplace_or_replace<Component::Direction>(entity, Component::Direction::W);
    } else if (colliderType == "polygon") {
      offset = {((float) xOffset), (float) yOffset / 2.0f};
      zone.emplace_or_replace<Component::Direction>(entity, Component::Direction::W);
    } else if (colliderType == "round") {
      offset = {xOffset, yOffset};
      zone.emplace_or_replace<Component::Direction>(entity, Component::Direction::W);
    } else if (colliderType == "none") {
      offset = {xOffset, yOffset};
      zone.emplace_or_replace<Component::Direction>(entity, Component::Direction::W);
    } else if (colliderType == "background") {
      offset.x = 0.0f;
      offset.y = 0.0f;
      offset = {(float) frame.clip.w / 2.0f, (float) frame.clip.h / 2.0f};
      zone.emplace_or_replace<Rendering_Components::Background>(entity);
    } else if (colliderType == "foreground") {
      offset.x = 0.0f;
      offset.y = 0.0f;
      offset = {(float) frame.clip.w / 2.0f, (float) frame.clip.h / 2.0f};
      zone.emplace_or_replace<Rendering_Components::Foreground>(entity);
    }
    // if object is a  background sprite DO NOT set Direction component
    return {offset, {fabs(((float) frame.clip.w / 2.0f) - offset.x), (float) frame.clip.h / 2.0f}};
  }
}// namespace Rendering