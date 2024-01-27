#include "SQLite_spritesheets.h"
#include "entity_data.h"
#include "entt/entt.hpp"
#include "texture_packer.h"

namespace Rendering {

  struct Sheet_Data {
    std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerframeData = NULL;
    Rendering_Components::Sprite_Sheet_Data frame;
  };

  Sheet_Data Set_Rend(entt::registry &zone, entt::entity &entity, std::string &templateName, int &xmlIndex, std::string img, std::string xml) {
    ///get texture data
    SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
    std::unordered_map<std::string, Rendering_Components::Sheet_Data_Flare> *flareSheetData = NULL;
    std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerframeData = NULL;
    int textureIndex = Entity_Data::Check_For_Template_ID(templateName);
    std::string sheetName;

    packerframeData = Texture_Packer::Get_Texture_Data(textureIndex, templateName, img, xml, sheetName);

    if (!packerframeData) {
      std::cout << "failed to load PVG_Building() for: " << templateName << std::endl;
      return {NULL};
    }
    auto &sprite = zone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(entity);
    sprite.sheetData = packerframeData;
    sprite.sheet_name = sheetName;
    sprite.type = "RPG_Tools";
    sprite.frameIndex = xmlIndex;

    zone.emplace_or_replace<Component::Icon>(entity, sprite.sheetData->at(sheetName).texture);
    return {sprite.sheetData, sprite.sheetData->at(sheetName).frameList[xmlIndex]};
  }

  void Set_Offset(entt::registry &zone, entt::entity &entity, std::string &colliderType, Component::Position &position, float &xOffset, float &yOffset, Rendering_Components::Sprite_Sheet_Data &frame) {
    auto &offset = zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(entity, 0.0f, 0.0f);
    if (colliderType == "rect") {
      offset = {((float) frame.clip.w / 2.0f), (float) frame.clip.h};
      position.y -= (float) frame.clip.h / 2.0f;
      zone.emplace_or_replace<Component::Direction>(entity, Component::Direction::W);
    } else if (colliderType == "polygon") {
      offset = {((float) frame.clip.w / 2.0f), (float) frame.clip.h / 2.0f};
      position.y -= (float) frame.clip.h / 2.0f;
      zone.emplace_or_replace<Component::Direction>(entity, Component::Direction::W);
    } else if (colliderType == "round") {
      offset = {xOffset, yOffset};
      position.x -= frame.x_offset;
      position.y -= frame.y_offset;
      zone.emplace_or_replace<Component::Direction>(entity, Component::Direction::W);
    } else if (colliderType == "none") {
      offset = {xOffset, yOffset};
      position.x -= frame.x_offset;
      position.y -= frame.y_offset;
      zone.emplace_or_replace<Component::Direction>(entity, Component::Direction::W);
    } else if (colliderType == "background") {
      offset = {frame.clip.w / 2.0f, frame.clip.h / 2.0f};
      position.x -= offset.x;
      position.y -= offset.y;
      offset.x = 0.0f;
      offset.y = 0.0f;
      zone.emplace_or_replace<Rendering_Components::Background>(entity);
    } else if (colliderType == "foreground") {
      offset = {frame.clip.w / 2.0f, frame.clip.h / 2.0f};
      position.x -= offset.x;
      position.y -= offset.y;
      offset.x = 0.0f;
      offset.y = 0.0f;
      zone.emplace_or_replace<Rendering_Components::Foreground>(entity);
    }
    // if object is a  background sprite DO NOT set Direction component
  }

  void Attach_Components(entt::registry &zone, entt::entity &entity) {
  }

}// namespace Rendering