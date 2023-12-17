#pragma once

#include "components.h"
#include "graphics.h"
#include "SQLite_unit_data.h"
#include "SQLite_spritesheets.h"
#include "character_data.h"
#include "action_components.h"
#include "collision.h"
#include "rendering_components.h"
#include "social_control.h"
#include "item_components.h"
#include "spellbook.h"
#include "texture_packer.h"
#include "ui_actionbar.h"
#include "procedural_generator.h"
#include "utilities.h"
#include "settings.h"
#include <stddef.h>

namespace Create_Entities {

  int template_ID = 0;
  std::map<std::string, int> savedTemplate_IDs;

  int Check_For_Template_ID(std::string &name) {
    if (savedTemplate_IDs.count(name) == 0) {
      savedTemplate_IDs[name] = template_ID;
      int unit_ID = template_ID;
      template_ID++;
      //  std::cout << "created and assigned template key: " << unit_ID << std::endl;
      return unit_ID;
    } else {
      // std::cout << "assigned existing template key: " << savedTemplate_IDs[name] << std::endl;
      return savedTemplate_IDs[name];
    }
    std::cout << "failed to assign template key for: " << name << std::endl;
  }

  int Get_Existing_Template_ID(std::string &name, std::string &entity_class) {
    if (savedTemplate_IDs.count(name) == 0) {
      //if it is not found save the class as the name (the class will always have the name of the base unit of the class and thus will default to the base unit if the random unit is not found)
      name = entity_class;
      if (savedTemplate_IDs.count(name) == 0) {
        savedTemplate_IDs[name] = template_ID;
        int unit_ID = template_ID;
        template_ID++;
        //  std::cout << "created and assigned template key: " << unit_ID << std::endl;
        return unit_ID;
      }
      return savedTemplate_IDs[name];
    } else {
      // std::cout << "assigned existing template key: " << savedTemplate_IDs[name] << std::endl;
      return savedTemplate_IDs[name];
    }
    std::cout << "failed to assign template key for: " << name << std::endl;
  }

  void Set_Collision_Box(entt::registry &zone, entt::entity &entity, std::string &entity_class, Component::Position &position, Collision::aabb &aabb, std::vector<std::vector<tmx::Vector2<float>>> &pointVecs, Component::Line_Segment &line, float &radius) {
    if (entity_class == "polygon") {
      Collision::Create_Static_Body_Polygon(zone, entity, position.x, position.y, pointVecs);
      zone.emplace<Action_Component::Action>(entity, Action_Component::isStatic);
      zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::object);
      zone.emplace<Component::Line_Segment>(entity, line);
      zone.emplace<Social_Component::Race>(entity, Social_Component::Race::neutral);
    } else if (entity_class == "rect") {
      Collision::Create_Static_Body_Rect(zone, entity, position.x, position.y, aabb);
      zone.emplace<Action_Component::Action>(entity, Action_Component::isStatic);
      zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::object);
      zone.emplace<Social_Component::Race>(entity, Social_Component::Race::neutral);
    } else if (entity_class == "round") {
      Collision::Create_Static_Body(zone, entity, position.x, position.y, radius);
      zone.emplace<Action_Component::Action>(entity, Action_Component::isStatic);
      zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::object);
      zone.emplace<Social_Component::Race>(entity, Social_Component::Race::neutral);
    } else {
      //        no collision box
      zone.emplace<Action_Component::Action>(entity, Action_Component::isStatic);
      zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::object);
      zone.emplace<Social_Component::Race>(entity, Social_Component::Race::neutral);
    }
  }

  bool Polygon_Building(entt::registry &zone, float x, float y, std::string &name, std::string entity_class, std::string &filepath, Collision::aabb &aabb, std::vector<std::vector<tmx::Vector2<float>>> &pointVecs, Component::Line_Segment &line, tmx::Vector2<float> imageOffset) {
    /// if it is a building
    Entity_Loader::Building_Data data = Entity_Loader::Get_Building_Data(name);

    if (data.sprite_layout == "static") {
      //std::cout << "loading Flare: " << name << std::endl;
      auto entity = zone.create();
      int unit_ID = Check_For_Template_ID(name);
      Graphics::Create_Game_Object(unit_ID, filepath.c_str());

      SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
      std::unordered_map<std::string, Rendering_Components::Sheet_Data_Flare> *flareSheetData = NULL;
      std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerframeData = NULL;

      ///get sheet data for new pointer to map
      SQLite_Spritesheets::Get_Flare_Building_From_DB(name, data.sprite_layout, sheetDataFlare);
      flareSheetData = Populate_Flare_SpriteSheet(name, sheetDataFlare, Graphics::unitTextures[unit_ID]);

      //Add shared components
      auto &position = zone.emplace<Component::Position>(entity, x, y);
      auto &scale = zone.emplace<Component::Scale>(entity, 1.0f);
      auto &radius = zone.emplace<Component::Radius>(entity, 1.0f);

      /// static objects must be set to west as it is the 0 position in the enumeration, ugh yeah I know
      zone.emplace<Component::Direction>(entity, Component::Direction::W);
      zone.emplace<Component::Name>(entity, name);
      zone.emplace<Component::Mass>(entity, 100.0f);
      zone.emplace<Component::Alive>(entity, true);

      auto &sprite = zone.emplace<Rendering_Components::Sprite_Sheet_Info>(entity);
      sprite.flareSpritesheet = flareSheetData;
      sprite.sheet_name = name;
      sprite.type = sheetDataFlare.sheet_type;
      zone.emplace<Rendering_Components::Sprite_Offset>(entity, sheetDataFlare.x_offset, sheetDataFlare.y_offset);

      Set_Collision_Box(zone, entity, data.collider_type, position, aabb, pointVecs, line, data.radius);
      return true;
    }
    return false;
  }

  bool PVG_Building(entt::registry &zone, float x, float y, float i, float j, std::string &templateName, int xmlIndex, Collision::aabb &aabb, std::vector<std::vector<tmx::Vector2<float>>> &pointVecs, Component::Line_Segment &line) {
    /// if it is a building
    Entity_Loader::Building_Data data = Entity_Loader::Get_Building_Data(templateName);

    if (data.sprite_layout == "PVG") {
      ///get texture data
      SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
      std::unordered_map<std::string, Rendering_Components::Sheet_Data_Flare> *flareSheetData = NULL;
      std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerframeData = NULL;
      int textureIndex = Check_For_Template_ID(templateName);
      std::string tilesetName;

      packerframeData = Texture_Packer::Get_Texture_Data(textureIndex, templateName, data, tilesetName);
      if (packerframeData == NULL) {
        //	I think returning true will just make it do nothing because of how it is called in map.h
        std::cout << "failed to load PVG_Building() for: " << templateName << std::endl;
        return true;
      }
      auto entity = zone.create();
      auto &sprite = zone.emplace<Rendering_Components::Sprite_Sheet_Info>(entity);
      sprite.sheetData = packerframeData;
      sprite.sheet_name = tilesetName;
      sprite.type = "RPG_Tools";
      sprite.frameIndex = xmlIndex;
      Rendering_Components::Sprite_Sheet_Data frame = sprite.sheetData->at(tilesetName).frameList.at(xmlIndex);
      auto &position = zone.emplace<Component::Position>(entity, x, y);

      auto &offset = zone.emplace<Rendering_Components::Sprite_Offset>(entity, 0.0f, 0.0f);
      if (data.collider_type == "rect") {
        offset = {((float) frame.clip.w / 2.0f), (float) frame.clip.h};
        position.y -= (float) frame.clip.h / 2.0f;
        Set_Collision_Box(zone, entity, data.collider_type, position, aabb, pointVecs, line, data.radius);
      } else if (data.collider_type == "polygon") {
        offset = {((float) frame.clip.w / 2.0f), (float) frame.clip.h / 2.0f};
        Set_Collision_Box(zone, entity, data.collider_type, position, aabb, pointVecs, line, data.radius);
        position.y -= (float) frame.clip.h / 2.0f;
      } else if (data.collider_type == "round") {
        offset = {data.x_offset, data.y_offset};
        position.x -= frame.x_offset;
        position.y -= frame.y_offset;
        Set_Collision_Box(zone, entity, data.collider_type, position, aabb, pointVecs, line, data.radius);
      } else if (data.collider_type == "none") {
        offset = {data.x_offset, data.y_offset};
        position.x -= frame.x_offset;
        position.y -= frame.y_offset;
        Set_Collision_Box(zone, entity, data.collider_type, position, aabb, pointVecs, line, data.radius);
      } else {
        Utilities::Log("PVG_Building() no collider_type found for templateName, also prints for blood spawning");
        Set_Collision_Box(zone, entity, data.collider_type, position, aabb, pointVecs, line, data.radius);
//        std::cout << templateName << " PVG_Buliding() trying to add collider, not found in db" << std::endl;
      }

      //Add shared components
      auto &radius = zone.emplace<Component::Radius>(entity, data.radius);
      zone.emplace<Component::Interaction_Rect>(entity, (x - radius.fRadius), (y - 10.0f / 2.0f), (radius.fRadius * 2.0f), 10.0f);

      zone.emplace<Component::Scale>(entity, 1.0f);
      zone.emplace<Component::Name>(entity, templateName);
      zone.emplace<Component::Mass>(entity, 100.0f);
      zone.emplace<Component::Alive>(entity, true);
      zone.emplace<Component::Health>(entity, 100, 100);

      if (i != x && j != y) {
//        std::cout << i << " " << j << std::endl;
        zone.emplace<Component::Tile_Index>(entity, (int)i, (int)j);
      }

      //std::cout << templateName << "Xo: " << frame.x_offset << " Cw: " << frame.clip.w << " IXo: " << imageOffset.x << " Yo: " << frame.y_offset << " Ch: " << frame.clip.h << " IYo: " << imageOffset.y << std::endl;

      //should place the tiled position on the point
      if (data.collider_type == "background") {
        offset = {frame.clip.w/2.0f, frame.clip.h/2.0f};
        position.x -= offset.x;
        position.y -= offset.y;
        offset.x = 0.0f;
        offset.y = 0.0f;
        zone.emplace<Rendering_Components::Background>(entity);
      } else if (data.collider_type == "foreground") {
        offset = {frame.clip.w/2.0f, frame.clip.h/2.0f};
        position.x -= offset.x;
        position.y -= offset.y;
        offset.x = 0.0f;
        offset.y = 0.0f;
        zone.emplace<Rendering_Components::Foreground>(entity);
      }
        // if object is a  background sprite DO NOT set Direction component
      else {
        zone.emplace<Component::Direction>(entity, Component::Direction::W);
      }
      return true;
    }
    return false;
  }

  void Set_Map_Texture(std::string &name, std::string imgpath) {
    int unit_ID = 0;
    unit_ID = Check_For_Template_ID(name);
    Graphics::Create_Game_Object(unit_ID, imgpath.c_str());
  }

  Procedural_Components::Seed seed;

  void Create_Entity(entt::registry &zone, float x, float y, std::string entity_class, bool is_random, db::Unit_Data &imgPaths, bool player) {
    auto entity = zone.create();
    Entity_Loader::Data data;
    int unit_ID = 0;

    Graphics::Texture texture;
    //std::cout << "loading Unit: " << templateName << std::endl;

    //get the entity_class from the template in the tiled map
    //get the is_random from the template in the tiled map
    //if it is random, grab a random entry of the same class from the DB table, including the key name
    //
    //scaling only applies to trees
    imgPaths.imgPath = "assets/" + imgPaths.imgPath;
    imgPaths.portraitPath = "assets/" + imgPaths.portraitPath;
    imgPaths.bodyPath = "assets/" + imgPaths.bodyPath;

    if (is_random == 1) {
      imgPaths.name = Entity_Loader::Get_All_Of_Class(entity_class);
      //check if the random name has a tamplate ID, if it doesn't revert to default name
      unit_ID = Get_Existing_Template_ID(imgPaths.name, entity_class);
      texture.texture = Graphics::Create_Game_Object(unit_ID, imgPaths.imgPath.c_str());
      data = Entity_Loader::parse_data(imgPaths.name);//
      ////randomEntity must be converted into a std::string//
      //auto& scale = zone.get<Component::Scale>(entity);
      //auto& radius = zone.get<Component::Radius>(entity);//
      ////random number between 0.25 and 2
      //int rand_scale = 0.5;
      //scale.scale = rand_scale;
    } else {
      unit_ID = Check_For_Template_ID(imgPaths.name);
      texture.texture = Graphics::Create_Game_Object(unit_ID, imgPaths.imgPath.c_str());
      data = Entity_Loader::parse_data(imgPaths.name);
    }

    SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
    std::string sheetname = Entity_Loader::Get_Sprite_Sheet(imgPaths.name);
    std::unordered_map<std::string, Rendering_Components::Sheet_Data_Flare> *flareSheetData = NULL;
    std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerframeData = NULL;

    //stores the index for a static frame in spritesheet xml
    if (sheetname == "texture_packer") {
      ///run texture packer
      packerframeData = Texture_Packer::TexturePacker_Import(imgPaths.name, sheetname, texture.texture);
    } else {
      ///get sheet data for new pointer to map
      SQLite_Spritesheets::Get_Flare_From_DB(sheetname, sheetDataFlare);
      flareSheetData = Populate_Flare_SpriteSheet(imgPaths.name, sheetDataFlare, texture.texture);
    }

    if (texture.texture == NULL) {
      std::cout << "texture is NULL for: " << imgPaths.name << std::endl;
    }

    //Add shared components
    auto &position = zone.emplace<Component::Position>(entity, x, y);
    auto &scale = zone.emplace<Component::Scale>(entity, data.scale);
    //            randomly scale trees
    if (is_random == 1) {
      seed.seed = Procedural_Generation::Create_Initial_Seed(position.x, position.y);
      float type = Procedural_Generation::Random_Int(0.5f, 1.5f, seed);
      scale.scale = type;
      data.scale = type;
    }
    //
    auto &radius = zone.emplace<Component::Radius>(entity, (data.radius * data.scale));
    zone.emplace<Component::Interaction_Rect>(entity, (x - data.interact_r) * data.scale, (y - data.interact_h / 2.0f) * data.scale, (data.interact_r * 2.0f) * data.scale, data.interact_h * data.scale);

    zone.emplace<Component::Mass>(entity, data.mass * data.scale);
    zone.emplace<Component::Alive>(entity, true);
    zone.emplace<Component::Unit>(entity);
    zone.emplace<Rendering_Components::Sprite_Offset>(entity, data.x_offset * data.scale, data.y_offset * data.scale);

    auto &full_name = zone.emplace<Component::Name>(entity);
    Character_Data::Get_Name(full_name);

    texture.portrait = zone.emplace<Rendering_Components::Portrait>(entity, Graphics::Load_Portrait(unit_ID, imgPaths.portraitPath.c_str())).texture;
    texture.body = zone.emplace<Rendering_Components::Body>(entity, Graphics::Load_Body(unit_ID, imgPaths.bodyPath.c_str())).texture;

    zone.emplace<Rendering_Components::Unit_Frame_Portrait>(entity, texture.portrait);
    zone.emplace<Rendering_Components::Body_Frame>(entity, texture.body);

    auto &sprite = zone.emplace<Rendering_Components::Sprite_Sheet_Info>(entity);

    //if RTP_pieces type
    if (packerframeData) {
      sprite.sheetData = packerframeData;
      sprite.sheet_name = imgPaths.name;
      sprite.type = "RPG_Tools";
    } else {
      sprite.flareSpritesheet = flareSheetData;
      sprite.sheet_name = imgPaths.name;
      sprite.type = sheetDataFlare.sheet_type;
    }

    //dynamic entities
    if (data.body_type == 1) {
      zone.emplace<Component::Direction>(entity, Component::Direction::S);

      auto raceData = Entity_Loader::Get_Race_Relationsips(data.race);
      zone.emplace<Social_Component::Race>(entity, Social_Control::Get_Race(data.race));
      auto &relationships = zone.emplace<Social_Component::Relationships>(entity);
      for (int i = 0; i < raceData.size(); i++) {
        relationships.races[i] = raceData[i + 1];
      }

      Item_Component::Unit_Equip_Type equip_type = Item_Component::Get_Unit_Equip_Type(data.equip_type);
      if (equip_type != Item_Component::Unit_Equip_Type::none) {
        zone.emplace<Item_Component::Equipment>(entity, equip_type);
        zone.emplace<Rendering_Components::Equipment_Sprites>(entity);
      }

      bool yes = true;
      Collision::Create_Dynamic_Body(zone, entity, position.x, position.y, radius.fRadius, data.mass, yes);
      zone.emplace<Collision_Component::Dynamic_Collider>(entity);

      //do not attach to non combat
      if (data.temp_type_name != "non-combat") {
        zone.emplace<Component::Melee_Damage>(entity, data.damage_min, data.damage_max, 25);
        zone.emplace<Component::Attack_Speed>(entity, data.attack_speed, 0.0f);
      }

      zone.emplace<Component::Melee_Range>(entity, ((data.radius + data.melee_range) * data.scale));
      zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::unit);
      zone.emplace<Action_Component::Action>(entity, Action_Component::idle);
      zone.emplace<Component::Velocity>(entity, 0.0f, 0.0f, 0.0f, 0.0f, data.speed * data.scale);
      auto &health = zone.emplace<Component::Health>(entity, int(data.health * data.scale), int(data.health * data.scale));
      zone.emplace<Component::Soldier>(entity);
      zone.emplace<Component::Commandable>(entity);
      zone.emplace<Component::Spellbook>(entity);

      if (player) {
        health.currentHealth += 200;
        health.maxHealth += 200;
        zone.emplace<Component::Input>(entity);
        SDL_DisplayMode dm;
        SDL_GetWindowDisplayMode(Graphics::window, &dm);
        auto &camera = zone.emplace<Component::Camera>(entity, 0.0f, 0.0f, (float) dm.w, (float) dm.h, Settings::cameraScale, Settings::cameraScale);

        zone.emplace<Component::Target_Range>(entity, 2000.0f * data.scale, position.x - (2000.0f / 2.0f * data.scale), position.y - (2000.0f / 2.0f * data.scale), 2000.0f * data.scale, 2000.0f * data.scale);
      }
      else {
        zone.emplace<Component::Sight_Range>(entity, data.sight_radius * data.scale, position.x - (data.sight_radius / 2.0f * data.scale), position.y - (data.sight_radius / 2.0f * data.scale), data.sight_radius * data.scale, data.sight_radius * data.scale);
      }
    }
      //static entities
      //if it is an aabb building
    else if (data.body_type == 0) {
      /// static objects must be set to west as it is the 0 position in the enumeration, ugh yeah I know
      zone.emplace<Component::Direction>(entity, Component::Direction::W);
      Collision::Create_Static_Body(zone, entity, position.x, position.y, (data.radius * data.scale));
      zone.emplace<Action_Component::Action>(entity, Action_Component::isStatic);
      zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::foliage);
    }
  }
}
