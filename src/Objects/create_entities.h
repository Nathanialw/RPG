#pragma once

#include "Graphics/XML_Parsers/texture_packer.h"
#include "Maps/Procedural_Generation/number_generator.h"
#include "Maps/World/cave.h"
#include "Objects/Collision/collision.h"
#include "Objects/Social/social_control.h"
#include "Objects/Stats/character_data.h"
#include "Objects/Stats/entity_data.h"
#include "Rendering_Components/rendering_functions.h"
#include "SQLite_spritesheets.h"
#include "SQLite_unit_data.h"
#include "action_components.h"
#include "building_components.h"
#include "components.h"
#include "graphics.h"
#include "load_zone.h"
#include "quad_tree.h"
#include "rendering_components.h"
#include "settings.h"
#include "utilities.h"

namespace Create_Entities {

  void Emplace_Interaction_Rect(entt::registry &zone, entt::entity &entity, Entity_Loader::Data &data, float &x, float &y) {
    if (data.whole_sprite == 1) {//if the interaction rect is the whole sprite
      zone.emplace_or_replace<Component::Interaction_Rect>(entity, (x - data.interact_r) * data.scale, (y - data.interact_h / 2.0f) * data.scale, (data.interact_r * 2.0f) * data.scale, data.interact_h * data.scale);
    } else {//if the interaction rect is a rect around it's position
      zone.emplace_or_replace<Component::Interaction_Rect>(entity, (x - data.interact_r) * data.scale, (y - data.interact_h / 2.0f) * data.scale, (data.interact_r * 2.0f) * data.scale, data.interact_h * data.scale, true);
    }
  }

  void Emplace_Interaction_Rect_Building(entt::registry &zone, entt::entity &entity, Entity_Loader::Building_Data &data, float &x, float &y, float &radius) {
    if (data.whole_sprite == 1) {//if the interaction rect is the whole sprite
      //      zone.emplace_or_replace<Component::Interaction_Rect>(entity, (x - radius), (y - 10.0f / 2.0f), (radius * 2.0f), 10.0f));
      zone.emplace_or_replace<Component::Interaction_Rect>(entity, (x - radius), (y - radius), (radius * 2.0f), radius * 2.0f);
    } else {//if the interaction rect is a rect around it's position
      zone.emplace_or_replace<Component::Interaction_Rect>(entity, (x - radius), (y - radius), (radius * 2.0f), radius * 2.0f, true);
    }
  }

  Mouse_Struct::Entity_ID_Direction Create_Render_Tile_Object(entt::registry &zone, int state, int x, int y, const std::string &templateName) {
    //    Entity_Loader::Building_Data data = Entity_Loader::Get_Tile_Data(templateName);
    auto entity = zone.create();

    zone.emplace_or_replace<Component::Position>(entity, (float) x * 512.0f, (float) y * 512.0f);
    auto offset = zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(entity, 256.0f, -256.0f);

    //    Collision::Save_Line_Segment(zone, entity, templateName);
    //    zone.emplace_or_replace<Component::Interaction_Rect>(entity, (x - (float) frameData.frameData.imageData->at(frameData.frameData.sheet_name).w / 2.0f), (y - (float) frameData.frameData.imageData->at(frameData.frameData.sheet_name).h / 2.0f), (float) frameData.frameData.imageData->at(frameData.frameData.sheet_name).w, (float) frameData.frameData.imageData->at(frameData.frameData.sheet_name).h, false);

    if (Collision_Component::polygonColliders.contains(templateName)) {
      //only use this interaction rect if the building doesn't have an interior
      zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::tile);
      zone.emplace<Collision_Component::Collider_Data>(entity, templateName, offset, 0.0f, (float) x * 512.0f, (float) y * 512.0f, "polygon", 0.0f);
      //      Emplace_Interaction_Rect_Building(zone, entity, data, x, y, data.radius);

      return {entity, templateName};
    }
  }

  bool Create_Tile_Object(entt::registry &zone, int state, entt::entity &entity) {
    auto colliderData = zone.get<Collision_Component::Collider_Data>(entity);
    zone.remove<Collision_Component::Collider_Data>(entity);
    Collision::Attach_Components(zone, state, entity, colliderData);

    zone.emplace_or_replace<Component::Radius>(entity, colliderData.radius);
    return true;
  }

  entt::entity Create_Tile(entt::registry &zone, int state, int x, int y, const std::string &templateName) {
    entt::entity entity = Create_Entities::Create_Render_Tile_Object(zone, state, x, y, templateName).entity;
    ///used for object generation like blood so it isn't added to a tile index
    zone.emplace_or_replace<Component::Tile_Index>(entity, x, y);
    Create_Tile_Object(zone, state, entity);
    return entity;
  }

  Mouse_Struct::Entity_ID_Direction Create_Render_Object(entt::registry &zone, int state, float x, float y, std::string &templateName, int xmlIndex) {
    Entity_Loader::Building_Data data = Entity_Loader::Get_Building_Data(templateName);
    auto entity = zone.create();

    if (data.sprite_layout == "PVG") {
      ///get texture data
      Rendering::Sheet_Data frameData = Rendering::Set_Rend(zone, entity, templateName, xmlIndex, data.img, data.xml);

      zone.emplace_or_replace<Component::Position>(entity, x, y);
      Rendering_Components::Offsets offsets = {};
      if (frameData.frameData.sheetData) {
        offsets = Rendering::Set_Offset(zone, entity, data.collider_type, data.x_offset, data.y_offset, frameData.frame);
      } else {
        offsets = Rendering::Set_Offset(zone, entity, data.collider_type, data.x_offset, (float) frameData.frameData.imageData->at(frameData.frameData.sheet_name).h, frameData.frame);
      }
      zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(entity, offsets.offset);

      if (!data.interior.empty()) {
        //interior sprite needs to be centered on the exterior sprite fot them to line up
        Entity_Loader::Interior_Building_Data interiorData = Entity_Loader::Get_Interior_Building_Data(data.interior);
        Rendering::Sheet_Data interiorFrameData = Rendering::Set_Render_Interior(zone, entity, data.interior, xmlIndex, interiorData.img, interiorData.xml);
        Rendering_Components::Sprite_Sheet_Data interiorFrame = interiorFrameData.frame;

        auto &interiorSheetData = zone.get<Rendering_Components::Interior_Sheet_Info>(entity);
        interiorSheetData.collisionBocArrayIndex = data.interior;
        Rendering::Set_Offset(zone, entity, "rect", interiorData.x_offset, interiorData.y_offset, interiorFrame);
        interiorSheetData.offset = {interiorData.x_offset, interiorData.y_offset};
        offsets.colliderOffset = {interiorData.x_collision_offset, interiorData.y_collision_offset};
        zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::building);
        //position is replaced by mouse position with building placement
        zone.emplace<Collision_Component::Collider_Data>(entity, data.interior, offsets.colliderOffset, data.radius, x, y, data.collider_type, ((float) frameData.frameData.imageData->at(frameData.frameData.sheet_name).h / 2.0f));
        Collision::Save_Line_Segment(zone, entity, templateName);

        zone.emplace_or_replace<Component::Interaction_Rect>(entity, (x - (float) frameData.frameData.imageData->at(frameData.frameData.sheet_name).w / 2.0f), (y - (float) frameData.frameData.imageData->at(frameData.frameData.sheet_name).h / 2.0f), (float) frameData.frameData.imageData->at(frameData.frameData.sheet_name).w, (float) frameData.frameData.imageData->at(frameData.frameData.sheet_name).h, false);

        if (Collision_Component::polygonColliders.contains(data.interior)) {
          Building_Component::Polygon treePolygon;
          for (auto polygon: Collision_Component::polygonColliders.at(data.interior).placementBox.pointVecs) {
            treePolygon.push_back({polygon.x + offsets.colliderOffset.x, polygon.y - offsets.colliderOffset.y});
          }
          std::vector<Building_Component::Polygon> polygons;
          polygons.emplace_back(treePolygon);
          zone.emplace_or_replace<Building_Component::Placement>(entity, polygons);
        }
      } else {
        //only use this interaction rect if the building doesn't have an interior
        zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::object);
        zone.emplace<Collision_Component::Collider_Data>(entity, templateName, offsets.colliderOffset, data.radius, x, y, data.collider_type, 0.0f);
        Emplace_Interaction_Rect_Building(zone, entity, data, x, y, data.radius);
      }

      zone.emplace_or_replace<Component::Is_Inside>(entity);
      zone.emplace_or_replace<Action_Component::Action>(entity, Action_Component::isStatic);
      Social_Control::Entity(zone, entity, data.race);
      zone.emplace_or_replace<Component::Scale>(entity, 1.0f);
      zone.emplace_or_replace<Component::Name>(entity, templateName);
    }
    return {entity, templateName, data.direction};
  }

  bool Create_Object(entt::registry &zone, int state, entt::entity &entity) {
    auto colliderData = zone.get<Collision_Component::Collider_Data>(entity);
    zone.remove<Collision_Component::Collider_Data>(entity);
    Collision::Attach_Components(zone, state, entity, colliderData);

    zone.emplace_or_replace<Component::Radius>(entity, colliderData.radius);
    zone.emplace_or_replace<Component::Alive>(entity, true);
    zone.emplace_or_replace<Rendering_Components::Unit_Frame_Portrait>(entity);
    zone.emplace_or_replace<Component::Health>(entity, 100, 100);
    return true;
  }

  entt::entity PVG_Building(entt::registry &zone, int state, float x, float y, float i, float j, std::string &templateName, int xmlIndex) {
    entt::entity entity = Create_Entities::Create_Render_Object(zone, state, x, y, templateName, xmlIndex).entity;
    if (!Cave::Set_As_Cave(zone, entity, templateName)) {
      ///used for object generation like blood so it isn't added to a tile index
      if (i != x && j != y) {
        zone.emplace_or_replace<Component::Tile_Index>(entity, (int) i, (int) j);
      }
    }
    Create_Entities::Create_Object(zone, state, entity);
    return entity;
  }

  Rendering_Components::Blend_Type Set_Texture_Components(entt::registry &zone, entt::entity &entity, db::Unit_Data &imgPaths, const std::string &unity, const bool &hexDir) {
    int unit_ID = 0;
    unit_ID = Entity_Data::Check_For_Template_ID(imgPaths.name);
    Graphics::Texture texture = {};

    imgPaths.imgPath = "assets/" + imgPaths.imgPath;
    imgPaths.portraitPath = "assets/" + imgPaths.portraitPath;
    imgPaths.bodyPath = "assets/" + imgPaths.bodyPath;
    texture.texture = Graphics::Create_Game_Object(unit_ID, imgPaths.imgPath.c_str());
    texture.portrait = zone.emplace_or_replace<Rendering_Components::Portrait>(entity, Graphics::Load_Portrait(unit_ID, imgPaths.portraitPath.c_str())).texture;
    texture.body = zone.emplace_or_replace<Rendering_Components::Body>(entity, Graphics::Load_Body(unit_ID, imgPaths.bodyPath.c_str())).texture;


    SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
    std::string sheetname = Entity_Loader::Get_Sprite_Sheet(imgPaths.name);
    std::unordered_map<std::string, Rendering_Components::Sheet_Data_Flare> *flareSheetData = nullptr;
    std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerframeData = nullptr;

    //stores the index for a static frame in spritesheet xml
    if (sheetname == "texture_packer") {
      ///run texture packer
      packerframeData = Texture_Packer::TexturePacker_Import(imgPaths.name, texture.texture);
    } else {
      ///get sheet data for new pointer to map
      SQLite_Spritesheets::Get_Flare_From_DB(sheetname, sheetDataFlare);
      flareSheetData = Populate_Flare_SpriteSheet(imgPaths.name, sheetDataFlare, texture.texture);
    }
    if (!texture.texture) {
      std::cout << "texture is NULL for: " << imgPaths.name << std::endl;
    }

    zone.emplace_or_replace<Rendering_Components::Unit_Frame_Portrait>(entity, texture.portrait);
    zone.emplace_or_replace<Rendering_Components::Body_Frame>(entity, texture.body);

    auto &sprite = zone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(entity);

    //if RTP_pieces type
    if (packerframeData) {
      sprite.sheetData = packerframeData;
      sprite.sheet_name = imgPaths.name;
      sprite.type = "RPG_Tools";
      sprite.hexDir = hexDir;
      if (unity == "Unity" || unity == "dwarf_female" || unity == "elf_female" || unity == "human_female") sprite.unity = true;
    } else {
      sprite.flareSpritesheet = flareSheetData;
      sprite.sheet_name = imgPaths.name;
      sprite.type = sheetDataFlare.sheet_type;
    }
    return sprite.blendType;
  }

  Component::Position Add_Shared_Components(entt::registry &zone, entt::entity &entity, float x, float y, Entity_Loader::Data &data, Component::Unit_Index &unitIndex) {
    //Add shared components
    auto &position = zone.emplace_or_replace<Component::Position>(entity, x, y);
    Emplace_Interaction_Rect(zone, entity, data, x, y);
    zone.emplace_or_replace<Component::Scale>(entity, data.scale);
    zone.emplace_or_replace<Component::Radius>(entity, (data.radius * data.scale));
    zone.emplace_or_replace<Component::Unit_Index>(entity, unitIndex);
    zone.emplace_or_replace<Component::Mass>(entity, data.mass * data.scale);
    zone.emplace_or_replace<Component::Alive>(entity, true);
    zone.emplace_or_replace<Component::Unit>(entity);
    zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(entity, data.x_offset * data.scale, data.y_offset * data.scale);
    return position;
  }

  void Add_Melee(entt::registry &zone, entt::entity &entity, Entity_Loader::Data &data) {
    if (data.temp_type_name != "non-combat") {
      zone.emplace_or_replace<Component::Melee_Damage>(entity, (int) (data.damage_min * data.scale), (int) (data.damage_max * data.scale), 25);
      zone.emplace_or_replace<Component::Attack_Speed>(entity, data.attack_speed, 0.0f);
    }
    zone.emplace_or_replace<Component::Melee_Range>(entity, ((data.radius * data.scale) + (data.melee_range * data.scale)));
  }
  bool startup = true;

  void Create_Entity(entt::registry &zone, int &state, float x, float y, db::Unit_Data &imgPaths, bool player, Social_Component::Summon summon, Component::Unit_Index &unitIndex) {
    auto entity = zone.create();
    Entity_Loader::Data data;
    data = Entity_Loader::parse_data(imgPaths.name);
    if (data.hexDir) {
      Utilities::Log(data.hexDir);
    }
    Rendering_Components::Blend_Type blendType = Set_Texture_Components(zone, entity, imgPaths, data.equip_type, data.hexDir);
    Component::Position position = Add_Shared_Components(zone, entity, x, y, data, unitIndex);

    //dynamic entities
    if (data.body_type == 1) {
      zone.emplace_or_replace<Component::Direction>(entity, Component::Direction::S);
      bool yes = true;
      Collision::Create_Dynamic_Body(zone, state, entity, position.x, position.y, (data.radius * data.scale), (data.mass * data.scale), yes);
      //do not attach to non combat
      Add_Melee(zone, entity, data);
      //    needs to be copied for zone changes
      zone.emplace_or_replace<Rendering_Components::Buff_Sprites>(entity);
      zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::unit);
      zone.emplace_or_replace<Action_Component::Action>(entity, Action_Component::attack2);
      zone.emplace_or_replace<Component::Is_Inside>(entity);

      auto &velocity = zone.emplace_or_replace<Component::Velocity>(entity, 0.0f, 0.0f, 0.0f, 0.0f, data.speed * data.scale, 0.0f, data.hexDir);

      if (!player) {
        zone.emplace_or_replace<Component::Name>(entity, imgPaths.name);
        if (!Social_Control::Summon(zone, entity, summon, blendType)) {
          Social_Control::Entity(zone, entity, data.race);
          zone.emplace_or_replace<Component::Spawn_Location>(entity, position);
        }

        zone.emplace_or_replace<Component::Health>(entity, (int) (data.health * data.scale), (int) (data.health * data.scale));
        Item_Component::Unit_Equip_Type equip_type = Item_Component::Get_Unit_Equip_Type(data.equip_type);

        if (equip_type != Item_Component::Unit_Equip_Type::none) {
          Item_Component::Emplace_Equipment(zone, state, entity, equip_type);
          zone.emplace_or_replace<Rendering_Components::Equipment_Sprites>(entity);
        }
      }
      // if this is the first run make these, otherwise copy them
      else if (startup) {
        startup = false;
        UI::Bag_UI::Create_Bag_UI(zone, entity, state);
        zone.emplace_or_replace<Social_Component::Race>(entity, Social_Control::Get_Race(data.race));
        auto raceData = Social_Control::Get_Race_Relationsips(data.race);
        auto &relationships = zone.emplace_or_replace<Social_Component::Relationships>(entity);
        for (int i = 0; i < raceData.size(); i++) {
          relationships.races[i] = raceData[i + 1];
        }
        Item_Component::Unit_Equip_Type equip_type = Item_Component::Get_Unit_Equip_Type(data.equip_type);
        if (equip_type != Item_Component::Unit_Equip_Type::none) {
          Item_Component::Emplace_Equipment(zone, state, entity, equip_type);
          zone.emplace_or_replace<Rendering_Components::Equipment_Sprites>(entity);
        }
        zone.emplace_or_replace<Component::Health>(entity, (int) (data.health * data.scale), (int) (data.health * data.scale));
      } else {
        Load::Copy_Player(zone, World::world[World::world[state].previousZoneIndex].zone, state, World::world[state].previousZoneIndex, entity);
      }

      zone.emplace_or_replace<Component::Soldier>(entity);
      zone.emplace_or_replace<Component::Commandable>(entity);
      zone.emplace_or_replace<Component::Spellbook>(entity);

      if (player) {
        Component::Add_Input_Component(zone, velocity, entity);
        SDL_DisplayMode dm;
        SDL_GetWindowDisplayMode(Graphics::window, &dm);
        zone.emplace_or_replace<Component::Camera>(entity, 0.0f, 0.0f, (float) dm.w, (float) dm.h, Settings::cameraScale, Settings::cameraScale);
        zone.emplace_or_replace<Component::Target_Range>(entity, 2000.0f * data.scale, position.x - (2000.0f / 2.0f * data.scale), position.y - (2000.0f / 2.0f * data.scale), 2000.0f * data.scale, 2000.0f * data.scale);
        auto &full_name = zone.emplace_or_replace<Component::Name>(entity);
        Character_Data::Get_Name(full_name);
      } else {
        zone.emplace_or_replace<Component::Sight_Range>(entity, data.sight_radius * data.scale, position.x - (data.sight_radius / 2.0f * data.scale), position.y - (data.sight_radius / 2.0f * data.scale), data.sight_radius * data.scale, data.sight_radius * data.scale);
      }
    }
    //static entities
    //if it is an aabb building
    else if (data.body_type == 0) {
      /// static objects must be set to west as it is the 0 position in the enumeration, ugh yeah I know
      zone.emplace_or_replace<Component::Direction>(entity, Component::Direction::W);
      Collision::Create_Static_Body(zone, state, entity, position.x, position.y, (data.radius * data.scale));
      zone.emplace_or_replace<Action_Component::Action>(entity, Action_Component::isStatic);
      zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::foliage);
    }
  }
}// namespace Create_Entities
