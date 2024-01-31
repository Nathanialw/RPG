#pragma once

#include "Rendering_Components/rendering_functions.h"
#include "SQLite_spritesheets.h"
#include "SQLite_unit_data.h"
#include "action_components.h"
#include "building_components.h"
#include "cave.h"
#include "character_data.h"
#include "collision.h"
#include "components.h"
#include "entity_data.h"
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

namespace Create_Entities {

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

  bool Polygon_Building(entt::registry &zone, int &state, float x, float y, std::string &name, std::string entity_class, std::string &filepath, Collision_Component::aabb &aabb, std::vector<std::vector<tmx::Vector2<float>>> &pointVecs, Component::Line_Segment &line, tmx::Vector2<float> imageOffset) {
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

      Collision::Set_Collision_Box(zone, state, entity, data.collider_type, position, aabb, pointVecs, line, data.radius);
      return true;
    }
    return false;
  }

  entt::entity Create_Render_Object(entt::registry &zone, float x, float y, std::string &templateName, int xmlIndex) {
    Entity_Loader::Building_Data data = Entity_Loader::Get_Building_Data(templateName);
    auto entity = zone.create();

    if (data.sprite_layout == "PVG") {
      ///get texture data
      Rendering::Sheet_Data sheetData = Rendering::Set_Rend(zone, entity, templateName, xmlIndex, data.img, data.xml);
      if (!sheetData.packerframeData) return entity;

      Rendering_Components::Sprite_Sheet_Data frame = sheetData.frame;
      auto &position = zone.emplace_or_replace<Component::Position>(entity, x, y);

      //attach collider
      Rendering_Components::Sprite_Offset offset = Rendering::Set_Offset(zone, entity, data.collider_type, position, data.x_offset, data.y_offset, frame);

      zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::object);
      zone.emplace_or_replace<Action_Component::Action>(entity, Action_Component::isStatic);

      SDL_Rect clipRect = sheetData.frame.clip;
      Emplace_Interaction_Rect_Building(zone, entity, data, x, y, data.radius, clipRect);

      Social_Control::Entity(zone, entity, data.race);
      zone.emplace_or_replace<Component::Scale>(entity, 1.0f);
      zone.emplace_or_replace<Component::Name>(entity, templateName);
      zone.emplace<Collision_Component::Collider_Data>(entity, templateName, offset, data.radius, position, data.collider_type);
    }
    return entity;
  }

  void Set_On_Mouse(entt::registry &zone, entt::entity &entity) {
    zone.emplace<Component::On_Mouse>(entity, entity, Component::Icon_Type::building);
    Mouse_Struct::mouseData.type = Component::Icon_Type::building;
    Mouse::mouseItem = entity;
    Mouse::itemCurrentlyHeld = true;
  }

  void Remove_From_Mouse(entt::registry &zone, entt::entity &entity) {
    zone.remove<Component::On_Mouse>(entity);
    Mouse::mouseItem = Mouse::cursor_ID;
    Mouse_Struct::mouseData.type = Component::Icon_Type::none;
    Mouse::itemCurrentlyHeld = false;
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

  entt::entity PVG_Building(entt::registry &zone, int state, float x, float y, float i, float j, std::string &templateName, int xmlIndex, Collision_Component::aabb &aabb, std::vector<std::vector<tmx::Vector2<float>>> &pointVecs, Component::Line_Segment &line) {
    entt::entity entity = Create_Entities::Create_Render_Object(zone, x, y, templateName, xmlIndex);
    if (!Cave::Set_As_Cave(zone, entity, templateName)) {
      ///used for object generation like blood so it isn't added to a tile index
      if (i != x && j != y) {
        zone.emplace_or_replace<Component::Tile_Index>(entity, (int) i, (int) j);
      }
    }
    Create_Entities::Create_Object(zone, state, entity);
    return entity;
  }

  void Set_Map_Texture(std::string &name, std::string imgpath) {
    int unit_ID = 0;
    unit_ID = Entity_Data::Check_For_Template_ID(name);
    Graphics::Create_Game_Object(unit_ID, imgpath.c_str());
  }

  Rendering_Components::Blend_Type Set_Texture_Components(entt::registry &zone, entt::entity &entity, db::Unit_Data &imgPaths) {
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
      zone.emplace_or_replace<Component::Melee_Damage>(entity, data.damage_min, data.damage_max, 25);
      zone.emplace_or_replace<Component::Attack_Speed>(entity, data.attack_speed, 0.0f);
    }
    zone.emplace_or_replace<Component::Melee_Range>(entity, ((data.radius + data.melee_range) * data.scale));
  }
  bool startup = true;

  void Create_Entity(entt::registry &zone, int &state, float x, float y, db::Unit_Data &imgPaths, bool player, Social_Component::Summon summon, Component::Unit_Index &unitIndex) {
    auto entity = zone.create();
    Entity_Loader::Data data;
    data = Entity_Loader::parse_data(imgPaths.name);
    Rendering_Components::Blend_Type blendType = Set_Texture_Components(zone, entity, imgPaths);
    Component::Position position = Add_Shared_Components(zone, entity, x, y, data, unitIndex);

    //dynamic entities
    if (data.body_type == 1) {
      zone.emplace_or_replace<Component::Direction>(entity, Component::Direction::S);
      bool yes = true;
      Collision::Create_Dynamic_Body(zone, state, entity, position.x, position.y, data.radius, data.mass, yes);
      //do not attach to non combat
      Add_Melee(zone, entity, data);
      //    needs to be copied for zone changes
      zone.emplace_or_replace<Rendering_Components::Buff_Sprites>(entity);
      zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::unit);
      zone.emplace_or_replace<Action_Component::Action>(entity, Action_Component::attack2);
      auto &velocity = zone.emplace_or_replace<Component::Velocity>(entity, 0.0f, 0.0f, 0.0f, 0.0f, data.speed * data.scale);

      if (!player) {
        zone.emplace_or_replace<Component::Name>(entity, imgPaths.name);
        if (!Social_Control::Summon(zone, entity, summon, blendType)) Social_Control::Entity(zone, entity, data.race);

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
