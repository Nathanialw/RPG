#pragma once

#include "SQLite_spell_data.h"
#include "SQLite_spritesheets.h"
#include "SQLite_unit_data.h"
#include "collision.h"
#include "components.h"
#include "entity_data.h"
#include "movement_components.h"
#include "quad_tree.h"
#include "spell_components.h"
#include "texture_packer.h"

namespace Spells {

  void Spell_Linear_Target(entt::registry &zone, entt::entity &entity, float &x, float &y, float &sourceX, float &sourceY, float &range) {//sends spell to where the mouse is
    zone.emplace_or_replace<Component::Moving>(entity);
    zone.emplace_or_replace<Component::Linear_Move>(entity, x, y);
    zone.emplace_or_replace<Component::Spell_Range>(entity, sourceX, sourceY, range);
  }

  void Spell_Stack_Spells(entt::registry &zone, float &x, float &y) {//sends spell to where the mouse is
    auto view = zone.view<Component::Spell>();
    for (auto entity: view) {
      zone.emplace_or_replace<Component::Moving>(entity);
      zone.emplace_or_replace<Component::Mouse_Move>(entity, x, y);
    }
  }

  f2 Spell_Direction(Component::Position &pos, Component::Direction &direction, float &scale) {
    switch (direction) {
      case Component::Direction::N:
        return {pos.x, pos.y - (20.0f * scale)};
      case Component::Direction::S:
        return {pos.x, pos.y + (20.0f * scale)};
      case Component::Direction::E:
        return {pos.x + (20.0f * scale), pos.y};
      case Component::Direction::W:
        return {pos.x - (20.0f * scale), pos.y};
      case Component::Direction::NW:
        return {pos.x - (20.0f * scale), pos.y - (20.0f * scale)};
      case Component::Direction::NE:
        return {pos.x + (20.0f * scale), pos.y - (20.0f * scale)};
      case Component::Direction::SW:
        return {pos.x - (20.0f * scale), pos.y + (20.0f * scale)};
      case Component::Direction::SE:
        return {pos.x + (20.0f * scale), pos.y + (20.0f * scale)};
    }
    Utilities::Log("Spell_Direction() function fallthrough error");
    return {1.0f, 1.0f};
  }

  void Get_Spell_Texture(entt::registry &zone, entt::entity &entity, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, const char *spellname, std::string &xml, std::string &image) {
    std::string name = (std::string) spellname;

    int unit_ID = Entity_Data::Check_For_Template_ID(name);
    //    SQLite_Spell_Data::Spell_Data spellData = SQLite_Spell_Data::Spell_Loader(name);
    Graphics::Create_Game_Object(unit_ID, image.c_str());
    std::string sheetType = Entity_Loader::Get_Sprite_Sheet(name);
    SDL_SetTextureBlendMode(Graphics::unitTextures[unit_ID], SDL_BlendMode::SDL_BLENDMODE_ADD);

    //for grid sprite
    if (sheetType == "packer_linear") {
      std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerframeData = NULL;
      packerframeData = Texture_Packer::TexturePacker_Import_Linear(name, xml, Graphics::unitTextures[unit_ID]);
      auto &sprite = zone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(entity);

      sprite.sheetData = packerframeData;
      sprite.sheet_name = name;
      sprite.type = sheetType;

//      zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(entity, (sprite.sheetData->at(name).frameW / 2.0f), (sprite.sheetData->at(name).frameH / 1.25f));
    }
    //for packer sprites
    else {
      SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
      std::unordered_map<std::string, Rendering_Components::Sheet_Data_Flare> *flareSheetData = NULL;

      SQLite_Spritesheets::Get_Flare_From_DB(sheetType, sheetDataFlare);
      flareSheetData = Populate_Flare_SpriteSheet(name, sheetDataFlare, Graphics::unitTextures[unit_ID]);

      auto &sprite = zone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(entity);
      sprite.flareSpritesheet = flareSheetData;
      sprite.sheet_name = name;
      sprite.type = sheetDataFlare.sheet_type;

      zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(entity, sheetDataFlare.x_offset, sheetDataFlare.y_offset);
    }
  }

  //  void Create_Spell(entt::entity caster_ID, entt::entity &entity, Component::Position &pos, Component::Direction &direction, const char *spellname, float &targetX, float &targetY) {
  void Create_Spell_object(entt::registry &zone, entt::entity &entity, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, const char *spellname) {
    if (spellname == "") {
      return;
    }
    float scale = 1.0f;
    Entity_Loader::Data data = Entity_Loader::parse_data(spellname);

    if (!zone.any_of<Rendering_Components::Sprite_Offset>(entity)) {
      zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(entity, data.x_offset, data.y_offset);
    }
    zone.emplace_or_replace<Component::Scale>(entity, scale);
    zone.emplace_or_replace<Action_Component::Action>(entity, Action_Component::walk);
    ///spell data
    zone.emplace_or_replace<Component::Radius>(entity, data.radius * scale);
    zone.emplace_or_replace<Component::Velocity>(entity, 0.0f, 0.0f, 0.0f, 0.0f, data.speed, 0.0f);
    zone.emplace_or_replace<Component::Mass>(entity, data.mass * scale);
    zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::spell);
    ///default data

    zone.emplace_or_replace<Component::Interaction_Rect>(entity, (data.radius * 1.1f), ((data.radius * 1.1f) * 2.0f));
    zone.emplace_or_replace<Component::Direction>(entity, direction);//match Direction of the caster
    zone.emplace_or_replace<Component::Alive>(entity, true);
    zone.emplace_or_replace<Component::Caster>(entity, caster_ID);
  }


  void Create_Spell(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Spells::Hit &hitEffect, Component::Casting casting, float &targetX, float &targetY) {
    if (casting.name == "") {
      return;
    }
    auto entity = zone.create();
    SQLite_Spell_Data::Spell_Data spellData = SQLite_Spell_Data::Spell_Loader(casting.name);
    Get_Spell_Texture(zone, entity, state, caster_ID, position, direction, casting.name, spellData.xmlPath, spellData.imagePath);

    Create_Spell_object(zone, entity, state, caster_ID, position, direction, casting.name);
    float scale = 1.0f;
    Entity_Loader::Data data = Entity_Loader::parse_data(casting.name);
    f2 spelldir = Spell_Direction(position, direction, scale);

    zone.emplace_or_replace<Component::Position>(entity, spelldir.x, spelldir.y);
    zone.emplace_or_replace<Component::Damage>(entity, 2, 4, 5);
    ///default data
    zone.emplace_or_replace<Component::Spell>(entity, casting.hit);
    zone.emplace_or_replace<Spells::Hit_Effect>(entity, hitEffect);

    bool yes = true;
    Collision::Create_Dynamic_Body(zone, state, entity, spelldir.x, spelldir.y, data.radius, data.mass, yes);
    Spell_Linear_Target(zone, entity, targetX, targetY, spelldir.x, spelldir.y, spellData.range);
  }

  void Spell_Cast_Effect(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, const char *spellname, float &targetX, float &targetY) {
    if (spellname == "") {
      return;
    }
    auto entity = zone.create();
    SQLite_Spell_Data::Spell_Data spellData = SQLite_Spell_Data::Spell_Loader(spellname);
    Get_Spell_Texture(zone, entity, state, caster_ID, position, direction, spellname, spellData.casting_xml_path, spellData.casting_image_path);
    Create_Spell_object(zone, entity, state, caster_ID, position, direction, spellname);

    zone.emplace_or_replace<Component::Position>(entity, position.x, position.y + 0.1f);
    zone.emplace_or_replace<Component::Particle>(entity);
    ////to prevent the spell from being added into the quad tree
    zone.emplace_or_replace<Component::In_Object_Tree>(entity);
  }

  void Spell_Hit_Effect(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, const char *spellname, float &targetX, float &targetY) {
    if (spellname == "") {
      return;
    }
    auto entity = zone.create();
    float scale = 1.0f;
    Entity_Loader::Data data = Entity_Loader::parse_data(spellname);
    SQLite_Spell_Data::Spell_Data spellData = SQLite_Spell_Data::Spell_Loader(spellname);
    Get_Spell_Texture(zone, entity, state, caster_ID, position, direction, spellname, spellData.hit_xml_path, spellData.hit_image_path);
    Create_Spell_object(zone, entity, state, caster_ID, position, direction, spellname);

    zone.emplace_or_replace<Component::Position>(entity, position.x, (position.y + 0.1f));
    zone.emplace_or_replace<Component::Particle>(entity);
    zone.emplace_or_replace<Component::In_Object_Tree>(entity);
  }

  int Cast_Template(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Spells::Hit &hitEffect, Component::Casting &casting, float &targetX, float &targetY) {
    Spells::Spell_Cast_Effect(zone, state, caster_ID, position, direction, casting.effect, casting.x, casting.y);
    return 1;
  }

  int Efect_Template(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Component::Casting &casting, float &targetX, float &targetY) {
    Spells::Spell_Cast_Effect(zone, state, caster_ID, position, direction, casting.effect, casting.x, casting.y);
    return 1;
  }

  struct Cast_Effect {
    castEffect casting = Efect_Template;
    Cast create = Cast_Template;
    Hit hit = Hit_Template;
  };

  void Cast_Spell(entt::registry &zone, int &state) {
    auto view = zone.view<Cast_Effect, Component::Direction, Action_Component::Action, Component::Position, Component::Casting, Component::Velocity>();
    for (auto caster_ID: view) {
      auto &casting = view.get<Component::Casting>(caster_ID);
      auto &position = view.get<Component::Position>(caster_ID);
      auto &velocity = view.get<Component::Velocity>(caster_ID);
      auto &direction = view.get<Component::Direction>(caster_ID);
      auto &action = view.get<Action_Component::Action>(caster_ID);
      auto &spell = view.get<Cast_Effect>(caster_ID);

      ///look at target but only once
      if (casting.counter >= casting.castTime) {
        direction = Movement_Component::Look_At_Target(position.x, position.y, casting.x, casting.y, velocity.angle);
        ////create cast particle object
        if (spell.casting(zone, state, caster_ID, position, direction, casting, casting.x, casting.x) == 0) {
          zone.remove<Component::Casting>(caster_ID);
          zone.remove<Spells::Cast_Effect>(caster_ID);
        };
      }
      //            Utilities::Log((int)direction);
      casting.counter -= Timer::timeStep;
      if (casting.counter <= 0) {
        auto &target = zone.emplace_or_replace<Component::Cast>(caster_ID, casting.x, casting.y);

        ///set into casting mode
        if (action.state == Action_Component::casting) {
          Action_Component::Set_State(action, Action_Component::Action_State::cast);
        }

        ///cast spell
        if (action.frameState == Action_Component::last) {
          direction = Movement_Component::Look_At_Target(position.x, position.y, casting.x, casting.y, velocity.angle);
          spell.create(zone, state, caster_ID, position, direction, spell.hit, casting, target.targetX, target.targetY);
          zone.remove<Component::Casting>(caster_ID);
          zone.remove<Spells::Cast_Effect>(caster_ID);
        }
      } else if (zone.any_of<Component::Moving>(caster_ID)) {
        Action_Component::Set_State(action, Action_Component::idle);
        zone.remove<Component::Casting>(caster_ID);
        zone.remove<Spells::Cast_Effect>(caster_ID);
      }
    }
  }

  void Destroy_Particle_Spells(entt::registry &zone) {
    auto view = zone.view<Component::Particle, Rendering_Components::Sprite_Sheet_Info, Action_Component::Action>();
    for (auto entity: view) {
      auto &action = view.get<Action_Component::Action>(entity);
      auto &sheetData = view.get<Rendering_Components::Sprite_Sheet_Info>(entity);
      if (sheetData.frameIndex >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames) {
        zone.destroy(entity);
      }
    }
  }

  void Create_Explosion(entt::registry &zone, float &x, float y) {//creates the explosion for fireballs
    auto explosion = zone.create();
    zone.emplace_or_replace<Component::Position>(explosion, x, y);
    zone.emplace_or_replace<Component::Sprite_Frames>(explosion, 63, 0, 0, 0);
    zone.emplace_or_replace<Component::Texture>(explosion, Graphics::fireball_explosion_0, 0, 0, 128, 128);
    zone.emplace_or_replace<Component::Frame_Delay>(explosion, 16.0f, 0.0f);
    zone.emplace_or_replace<Component::Explosion>(explosion, 0, 0, 0.0f, 0.0f, 128.0f, 128.0f, 64.0f, 100.0f);
  }

  void Destroy_NonMoving_Spells(entt::registry &zone, int &state) {
    auto view = zone.view<Spells::Hit_Effect, Component::Spell, Component::Body, Component::Position, Component::Interaction_Rect, Component::In_Object_Tree>(entt::exclude<Component::Linear_Move, Component::Explosion, Component::Particle>);
    for (auto entity: view) {
      auto &position = view.get<Component::Position>(entity);
      auto &body = view.get<Component::Body>(entity);
      auto &rect = view.get<Component::Interaction_Rect>(entity).rect;
      auto &hitParticle = view.get<Component::Spell>(entity).hit;
      auto &hitEffect = view.get<Spells::Hit_Effect>(entity).hit;
      ///create explosion
      Component::Direction direction = Component::Direction::W;
      float targetX = 0.0f;
      float targetY = 0.0f;
      hitEffect(zone, state, entity, position, direction, hitParticle, targetX, targetY);
      ///destroy box2d body
      auto world = Collision::collisionList[state];
      world->DestroyBody(body.body);
      zone.remove<Component::Body>(entity);
      ///set to remove from quad tree on update

      zone.emplace_or_replace<Component::Remove_From_Object_Tree>(entity, rect);//goto: Dynamic_Quad_Tree::Remove_From_Tree_And_Registry()
      zone.emplace_or_replace<Component::Destroyed>(entity, rect);              //goto: Dynamic_Quad_Tree::Remove_From_Tree_And_Registry()
    }
  }

  void Clear_Collided_Spells(entt::registry &zone, int &state) {
    auto view = zone.view<Component::Spell, Component::Position, Component::Alive, Component::Body, Component::Radius, Component::Interaction_Rect, Component::In_Object_Tree>(entt::exclude<Component::Mouse_Move, Component::Linear_Move, Component::Explosion>);
    for (auto entity: view) {
      if (!view.get<Component::Alive>(entity).bIsAlive) {
        auto &position = view.get<Component::Position>(entity);
        auto &radius = view.get<Component::Radius>(entity).fRadius;
        auto &body = view.get<Component::Body>(entity);
        auto &inTree = view.get<Component::In_Object_Tree>(entity).inTree;
        auto &rect = view.get<Component::Interaction_Rect>(entity).rect;
        ///create explosion
        Create_Explosion(zone, position.x, position.y);
        ///destroy box2d body
        auto world = Collision::collisionList[state];
        world->DestroyBody(body.body);
        zone.remove<Component::Body>(entity);
        ///set to remove from quad tree on update
        zone.emplace_or_replace<Component::Remove_From_Object_Tree>(entity, rect);//goto: Dynamic_Quad_Tree::Remove_From_Tree_And_Registry()
        zone.emplace_or_replace<Component::Destroyed>(entity, rect);              //goto: Dynamic_Quad_Tree::Remove_From_Tree_And_Registry()
      }
    }
  }

  void Casting_Updater(entt::registry &zone) {
    auto view = zone.view<Component::Cast, Action_Component::Action>();
    for (auto entity: view) {
      auto &action = view.get<Action_Component::Action>(entity);
      if (action.frameState == Action_Component::last) {
        zone.remove<Component::Cast>(entity);
      }
    }
  }


  void Update_Spells(entt::registry &zone, int &state) {
    Destroy_Particle_Spells(zone);
    Destroy_NonMoving_Spells(zone, state);
    Clear_Collided_Spells(zone, state);
    Cast_Spell(zone, state);
    Casting_Updater(zone);
  }


}// namespace Spells
