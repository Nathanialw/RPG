#pragma once

#include "SQLite_spritesheets.h"
#include "SQLite_unit_data.h"
#include "action_components.h"
#include "character_data.h"
#include "collision.h"
#include "components.h"
#include "graphics.h"
#include "load_zone.h"
#include "procedural_generator.h"
#include "quad_tree.h"
#include "rendering_components.h"
#include "settings.h"
#include "social_control.h"
#include "texture_packer.h"
#include "utilities.h"
#include <stddef.h>
#include "entity_data.h"

namespace Create_Entities {

  void Set_Collision_Box(entt::registry &zone, int &state, entt::entity &entity, std::string &entity_class, Component::Position &position, Collision::aabb &aabb, std::vector<std::vector<tmx::Vector2<float>>> &pointVecs, Component::Line_Segment &line, float &radius) {
    if (entity_class == "polygon") {
      Collision::Create_Static_Body_Polygon(zone, state, entity, position.x, position.y, pointVecs);
      zone.emplace_or_replace<Action_Component::Action>(entity, Action_Component::isStatic);
      zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::object);
      zone.emplace_or_replace<Component::Line_Segment>(entity, line);
      zone.emplace_or_replace<Social_Component::Race>(entity, Social_Component::Race::neutral);
    } else if (entity_class == "rect") {
      Collision::Create_Static_Body_Rect(zone, state, entity, position.x, position.y, aabb);
      zone.emplace_or_replace<Action_Component::Action>(entity, Action_Component::isStatic);
      zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::object);
      zone.emplace_or_replace<Social_Component::Race>(entity, Social_Component::Race::neutral);
    } else if (entity_class == "round") {
      Collision::Create_Static_Body(zone, state, entity, position.x, position.y, radius);
      zone.emplace_or_replace<Action_Component::Action>(entity, Action_Component::isStatic);
      zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::object);
      zone.emplace_or_replace<Social_Component::Race>(entity, Social_Component::Race::neutral);
    } else {
      //        no collision box
      zone.emplace_or_replace<Action_Component::Action>(entity, Action_Component::isStatic);
      zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::object);
      zone.emplace_or_replace<Social_Component::Race>(entity, Social_Component::Race::neutral);
    }
  }

  void Emplace_Interaction_Rect(entt::registry &zone, entt::entity &entity, Entity_Loader::Data &data, float &x, float &y) {
    if (data.whole_sprite == 1) {//if the interaction rect is the whole sprite
      zone.emplace_or_replace<Component::Interaction_Rect>(entity, (x - data.interact_r) * data.scale, (y - data.interact_h / 2.0f) * data.scale, (data.interact_r * 2.0f) * data.scale, data.interact_h * data.scale);
    } else {//if the interaction rect is a rect around it's position
      zone.emplace_or_replace<Component::Interaction_Rect>(entity, (x - data.interact_r) * data.scale, (y - data.interact_h / 2.0f) * data.scale, (data.interact_r * 2.0f) * data.scale, data.interact_h * data.scale, true);
    }
  }

  void Emplace_Interaction_Rect_Building(entt::registry &zone, entt::entity &entity, Entity_Loader::Building_Data &data, float &x, float &y, float &radius, SDL_Rect &clipRect) {
    if (data.whole_sprite == 1) {//if the interaction rect is the whole sprite
      //      zone.emplace_or_replace<Component::Interaction_Rect>(entity, (x - radius), (y - 10.0f / 2.0f), (radius * 2.0f), 10.0f));
      zone.emplace_or_replace<Component::Interaction_Rect>(entity, (x - radius), (y - radius), (radius * 2.0f), radius * 2.0f);

    } else {//if the interaction rect is a rect around it's position
      zone.emplace_or_replace<Component::Interaction_Rect>(entity, (x - radius), (y - radius), (radius * 2.0f), radius * 2.0f, true);

    }
  }

  bool Polygon_Building(entt::registry &zone, int &state, float x, float y, std::string &name, std::string entity_class, std::string &filepath, Collision::aabb &aabb, std::vector<std::vector<tmx::Vector2<float>>> &pointVecs, Component::Line_Segment &line, tmx::Vector2<float> imageOffset) {
    /// if it is a building
    Entity_Loader::Building_Data data = Entity_Loader::Get_Building_Data(name);

    if (data.sprite_layout == "static") {
      //std::cout << "loading Flare: " << name << std::endl;
      auto entity = zone.create();
      int unit_ID = Entity_Data::Check_For_Template_ID(name);
      Graphics::Create_Game_Object(unit_ID, filepath.c_str());

      SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
      std::unordered_map<std::string, Rendering_Components::Sheet_Data_Flare> *flareSheetData = NULL;
      std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerframeData = NULL;

      ///get sheet data for new pointer to map
      SQLite_Spritesheets::Get_Flare_Building_From_DB(name, data.sprite_layout, sheetDataFlare);
      flareSheetData = Populate_Flare_SpriteSheet(name, sheetDataFlare, Graphics::unitTextures[unit_ID]);

      //Add shared components
      auto &position = zone.emplace_or_replace<Component::Position>(entity, x, y);
      auto &scale = zone.emplace_or_replace<Component::Scale>(entity, 1.0f);
      auto &radius = zone.emplace_or_replace<Component::Radius>(entity, 1.0f);

      /// static objects must be set to west as it is the 0 position in the enumeration, ugh yeah I know
      zone.emplace_or_replace<Component::Direction>(entity, Component::Direction::W);
      zone.emplace_or_replace<Component::Name>(entity, name);
      zone.emplace_or_replace<Component::Mass>(entity, 100.0f);
      zone.emplace_or_replace<Component::Alive>(entity, true);

      auto &sprite = zone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(entity);
      sprite.flareSpritesheet = flareSheetData;
      sprite.sheet_name = name;
      sprite.type = sheetDataFlare.sheet_type;
      zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(entity, sheetDataFlare.x_offset, sheetDataFlare.y_offset);

      Set_Collision_Box(zone, state, entity, data.collider_type, position, aabb, pointVecs, line, data.radius);
      return true;
    }
    return false;
  }

  entt::entity PVG_Building(entt::registry &zone, int state, float x, float y, float i, float j, std::string &templateName, int xmlIndex, Collision::aabb &aabb, std::vector<std::vector<tmx::Vector2<float>>> &pointVecs, Component::Line_Segment &line) {
    /// if it is a building
    Entity_Loader::Building_Data data = Entity_Loader::Get_Building_Data(templateName);
    auto entity = zone.create();

    if (data.sprite_layout == "PVG") {
      ///get texture data
      SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
      std::unordered_map<std::string, Rendering_Components::Sheet_Data_Flare> *flareSheetData = NULL;
      std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerframeData = NULL;
      int textureIndex = Entity_Data::Check_For_Template_ID(templateName);
      std::string tilesetName;

      packerframeData = Texture_Packer::Get_Texture_Data(textureIndex, templateName, data, tilesetName);
      if (packerframeData == NULL) {
        //	I think returning true will just make it do nothing because of how it is called in map.h
        std::cout << "failed to load PVG_Building() for: " << templateName << std::endl;
        return entity;
      }
      auto &sprite = zone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(entity);
      sprite.sheetData = packerframeData;
      sprite.sheet_name = tilesetName;
      sprite.type = "RPG_Tools";
      sprite.frameIndex = xmlIndex;
      Rendering_Components::Sprite_Sheet_Data frame = sprite.sheetData->at(tilesetName).frameList.at(xmlIndex);
      auto &position = zone.emplace_or_replace<Component::Position>(entity, x, y);

      auto &offset = zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(entity, 0.0f, 0.0f);
      if (data.collider_type == "rect") {
        offset = {((float) frame.clip.w / 2.0f), (float) frame.clip.h};
        position.y -= (float) frame.clip.h / 2.0f;
        Set_Collision_Box(zone, state, entity, data.collider_type, position, aabb, pointVecs, line, data.radius);
      } else if (data.collider_type == "polygon") {
        offset = {((float) frame.clip.w / 2.0f), (float) frame.clip.h / 2.0f};
        Set_Collision_Box(zone, state, entity, data.collider_type, position, aabb, pointVecs, line, data.radius);
        position.y -= (float) frame.clip.h / 2.0f;
      } else if (data.collider_type == "round") {
        offset = {data.x_offset, data.y_offset};
        position.x -= frame.x_offset;
        position.y -= frame.y_offset;
        Set_Collision_Box(zone, state, entity, data.collider_type, position, aabb, pointVecs, line, data.radius);
      } else if (data.collider_type == "none") {
        offset = {data.x_offset, data.y_offset};
        position.x -= frame.x_offset;
        position.y -= frame.y_offset;
        Set_Collision_Box(zone, state, entity, data.collider_type, position, aabb, pointVecs, line, data.radius);
      } else {
        Utilities::Log("PVG_Building() no collider_type found for templateName, also prints for blood spawning");
        Set_Collision_Box(zone, state, entity, data.collider_type, position, aabb, pointVecs, line, data.radius);
        //        std::cout << templateName << " PVG_Buliding() trying to add collider, not found in db" << std::endl;
      }

      //Add shared components
      SDL_Rect clipRect = sprite.sheetData->at(tilesetName).frameList[xmlIndex].clip;
      auto &radius = zone.emplace_or_replace<Component::Radius>(entity, data.radius);
      Emplace_Interaction_Rect_Building(zone, entity, data, x, y, radius.fRadius, clipRect);

      if (templateName == "Rock_3_1" ||
          templateName == "Rock_3_2" ||
          templateName == "Rock_3_3" ||
          templateName == "Rock_3_4" ||
          templateName == "Rock_3_5" ||
          templateName == "Rock_3_6" ||
          templateName == "Rock_3_7" ||
          templateName == "Rock_3_8" ||
          templateName == "Rock_2_1" ||
          templateName == "Rock_2_2" ||
          templateName == "Rock_2_3" ||
          templateName == "Rock_2_4" ||
          templateName == "Rock_2_5" ||
          templateName == "Rock_2_6" ||
          templateName == "Rock_2_7" ||
          templateName == "Rock_2_8") {

        World::increment_Zone();
        auto &dungeon = zone.emplace_or_replace<Component::Dungeon>(entity);
        dungeon.instance = World::Zone_Count;
        dungeon.tilesetName = "hell";
        zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::portal);
      }
      else {
        if (i != x && j != y) {
          //        std::cout << i << " " << j << std::endl;
          zone.emplace_or_replace<Component::Tile_Index>(entity, (int) i, (int) j);
        }
      }

      auto raceData = Entity_Loader::Get_Race_Relationsips(data.race);
      auto &relationships = zone.emplace_or_replace<Social_Component::Relationships>(entity);
      for (int q = 0; q < raceData.size(); q++) {
        relationships.races[q] = raceData[q + 1];
      }
      zone.emplace_or_replace<Component::Scale>(entity, 1.0f);
      zone.emplace_or_replace<Component::Name>(entity, templateName);
      zone.emplace_or_replace<Component::Mass>(entity, 100.0f);
      zone.emplace_or_replace<Component::Alive>(entity, true);
      zone.emplace_or_replace<Rendering_Components::Unit_Frame_Portrait>(entity);
      zone.emplace_or_replace<Component::Health>(entity, 100, 100);



      //std::cout << templateName << "Xo: " << frame.x_offset << " Cw: " << frame.clip.w << " IXo: " << imageOffset.x << " Yo: " << frame.y_offset << " Ch: " << frame.clip.h << " IYo: " << imageOffset.y << std::endl;

      //should place the tiled position on the point
      if (data.collider_type == "background") {
        offset = {frame.clip.w / 2.0f, frame.clip.h / 2.0f};
        position.x -= offset.x;
        position.y -= offset.y;
        offset.x = 0.0f;
        offset.y = 0.0f;
        zone.emplace_or_replace<Rendering_Components::Background>(entity);
      } else if (data.collider_type == "foreground") {
        offset = {frame.clip.w / 2.0f, frame.clip.h / 2.0f};
        position.x -= offset.x;
        position.y -= offset.y;
        offset.x = 0.0f;
        offset.y = 0.0f;
        zone.emplace_or_replace<Rendering_Components::Foreground>(entity);
      }
      // if object is a  background sprite DO NOT set Direction component
      else {
        zone.emplace_or_replace<Component::Direction>(entity, Component::Direction::W);
      }
      return entity;
    }
    return entity;
  }

  void Set_Map_Texture(std::string &name, std::string imgpath) {
    int unit_ID = 0;
    unit_ID = Entity_Data::Check_For_Template_ID(name);
    Graphics::Create_Game_Object(unit_ID, imgpath.c_str());
  }

  Procedural_Components::Seed seed;
  bool startup = true;

  void Create_Entity(entt::registry &zone, int &state, float x, float y, std::string entity_class, bool is_random, db::Unit_Data &imgPaths, bool player, Social_Component::Summon summon, Component::Unit_Index &unitIndex) {
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
      unit_ID = Entity_Data::Get_Existing_Template_ID(imgPaths.name, entity_class);
      texture.texture = Graphics::Create_Game_Object(unit_ID, imgPaths.imgPath.c_str());
      data = Entity_Loader::parse_data(imgPaths.name);//
      ////randomEntity must be converted into a std::string//
      //auto& scale = zone.get<Component::Scale>(entity);
      //auto& radius = zone.get<Component::Radius>(entity);//
      ////random number between 0.25 and 2
      //int rand_scale = 0.5;
      //scale.scale = rand_scale;
    } else {
      unit_ID = Entity_Data::Check_For_Template_ID(imgPaths.name);
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
    auto &position = zone.emplace_or_replace<Component::Position>(entity, x, y);
    auto &scale = zone.emplace_or_replace<Component::Scale>(entity, data.scale);
    //            randomly scale trees
    if (is_random == 1) {
      seed.seed = Procedural_Generation::Create_Initial_Seed(position.x, position.y);
      float type = Procedural_Generation::Random_Int(0.5f, 1.5f, seed);
      scale.scale = type;
      data.scale = type;
    }
    //
    auto &radius = zone.emplace_or_replace<Component::Radius>(entity, (data.radius * data.scale));
    Emplace_Interaction_Rect(zone, entity, data, x, y);

    zone.emplace_or_replace<Component::Unit_Index>(entity, unitIndex);
    zone.emplace_or_replace<Component::Mass>(entity, data.mass * data.scale);
    zone.emplace_or_replace<Component::Alive>(entity, true);
    zone.emplace_or_replace<Component::Unit>(entity);
    zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(entity, data.x_offset * data.scale, data.y_offset * data.scale);

    texture.portrait = zone.emplace_or_replace<Rendering_Components::Portrait>(entity, Graphics::Load_Portrait(unit_ID, imgPaths.portraitPath.c_str())).texture;
    texture.body = zone.emplace_or_replace<Rendering_Components::Body>(entity, Graphics::Load_Body(unit_ID, imgPaths.bodyPath.c_str())).texture;

    zone.emplace_or_replace<Rendering_Components::Unit_Frame_Portrait>(entity, texture.portrait);
    zone.emplace_or_replace<Rendering_Components::Body_Frame>(entity, texture.body);

    auto &sprite = zone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(entity);

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
      zone.emplace_or_replace<Component::Direction>(entity, Component::Direction::S);
      bool yes = true;
      Collision::Create_Dynamic_Body(zone, state, entity, position.x, position.y, radius.fRadius, data.mass, yes);
      zone.emplace_or_replace<Collision_Component::Dynamic_Collider>(entity);

      //do not attach to non combat
      if (data.temp_type_name != "non-combat") {
        zone.emplace_or_replace<Component::Melee_Damage>(entity, data.damage_min, data.damage_max, 25);
        zone.emplace_or_replace<Component::Attack_Speed>(entity, data.attack_speed, 0.0f);
      }

      zone.emplace_or_replace<Component::Melee_Range>(entity, ((data.radius + data.melee_range) * data.scale));
      zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::unit);
      zone.emplace_or_replace<Action_Component::Action>(entity, Action_Component::idle);
      auto &velocity = zone.emplace_or_replace<Component::Velocity>(entity, 0.0f, 0.0f, 0.0f, 0.0f, data.speed * data.scale);

      if (!player) {
        zone.emplace_or_replace<Component::Name>(entity, imgPaths.name);
        if (summon.summon) {
          if (summon.blendType == Social_Component::ghost) {
            sprite.blendType = Rendering_Components::ghost;
            auto &relationships = zone.emplace_or_replace<Social_Component::Relationships>(entity);
            zone.emplace_or_replace<Social_Component::Race>(entity, summon.race);
            relationships.races = summon.relationships.races;
          } else if (summon.blendType == Social_Component::reanimated) {
            sprite.blendType = Rendering_Components::reanimated;
            auto &relationships = zone.emplace_or_replace<Social_Component::Relationships>(entity);
            zone.emplace_or_replace<Social_Component::Race>(entity, summon.race);
            relationships.races = summon.relationships.races;
          }
          else {
            sprite.blendType = Rendering_Components::normal;
            auto &relationships = zone.emplace_or_replace<Social_Component::Relationships>(entity);
            zone.emplace_or_replace<Social_Component::Race>(entity, summon.race);
            relationships.races = summon.relationships.races;
          }
        }
        else {
          zone.emplace_or_replace<Social_Component::Race>(entity, Social_Control::Get_Race(data.race));
          auto &relationships = zone.emplace_or_replace<Social_Component::Relationships>(entity);
          auto raceData = Entity_Loader::Get_Race_Relationsips(data.race);
          for (int i = 0; i < raceData.size(); i++) {
            relationships.races[i] = raceData[i + 1];
          }
        }


        Item_Component::Unit_Equip_Type equip_type = Item_Component::Get_Unit_Equip_Type(data.equip_type);
        if (equip_type != Item_Component::Unit_Equip_Type::none) {
          Item_Component::Emplace_Equipment(zone, state, entity, equip_type);
          zone.emplace_or_replace<Rendering_Components::Equipment_Sprites>(entity);
        }
        zone.emplace_or_replace<Component::Health>(entity, int(data.health * data.scale), int(data.health * data.scale));
      }
      // if this is the first run make these, otherwise copy them
      else if (startup) {
        startup = false;
        UI::Bag_UI::Create_Bag_UI(zone, entity, state);
        zone.emplace_or_replace<Social_Component::Race>(entity, Social_Control::Get_Race(data.race));
        auto raceData = Entity_Loader::Get_Race_Relationsips(data.race);
        auto &relationships = zone.emplace_or_replace<Social_Component::Relationships>(entity);
        for (int i = 0; i < raceData.size(); i++) {
          relationships.races[i] = raceData[i + 1];
        }
        Item_Component::Unit_Equip_Type equip_type = Item_Component::Get_Unit_Equip_Type(data.equip_type);
        if (equip_type != Item_Component::Unit_Equip_Type::none) {
          Item_Component::Emplace_Equipment(zone, state, entity, equip_type);
          zone.emplace_or_replace<Rendering_Components::Equipment_Sprites>(entity);
        }
        zone.emplace_or_replace<Component::Health>(entity, int(data.health * data.scale), int(data.health * data.scale));
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
        auto &camera = zone.emplace_or_replace<Component::Camera>(entity, 0.0f, 0.0f, (float) dm.w, (float) dm.h, Settings::cameraScale, Settings::cameraScale);
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
