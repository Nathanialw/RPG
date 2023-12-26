#pragma once

#include "SQLite_spell_data.h"
#include "collision.h"
#include "combat_control.h"
#include "components.h"
#include "map.h"
#include "movement.h"
#include "quad_tree.h"

namespace Spells {

  void Spell_Move_Target(entt::registry &zone, entt::entity entity, float &x, float &y) {//sends spell to where the mouse is
    zone.emplace_or_replace<Component::Moving>(entity);
    zone.emplace_or_replace<Component::Mouse_Move>(entity, x, y);
    zone.remove<Component::Casted>(entity);
  }

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

  //  void Create_Spell(entt::entity caster_ID, entt::entity &entity, Component::Position &pos, Component::Direction &direction, const char *spellname, float &targetX, float &targetY) {
  void Create_Spell(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, const char *spellname, float &targetX, float &targetY) {
    auto entity = zone.create();
    float scale = 1.0f;
    Entity_Loader::Data data = Entity_Loader::parse_data(spellname);
    f2 spelldir = Spell_Direction(position, direction, scale);

    std::string name = (std::string) spellname;

    int unit_ID = Create_Entities::Check_For_Template_ID(name);
    SQLite_Spell_Data::Spell_Data spellData = SQLite_Spell_Data::Spell_Loader(name);
    spellData.path = "assets/" + spellData.path;
    Graphics::Create_Game_Object(unit_ID, spellData.path.c_str());

    SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
    std::string sheetname = Entity_Loader::Get_Sprite_Sheet(name);
    std::unordered_map<std::string, Rendering_Components::Sheet_Data_Flare> *flareSheetData = NULL;

    SQLite_Spritesheets::Get_Flare_From_DB(sheetname, sheetDataFlare);
    flareSheetData = Populate_Flare_SpriteSheet(name, sheetDataFlare, Graphics::unitTextures[unit_ID]);

    auto &sprite = zone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(entity);
    sprite.flareSpritesheet = flareSheetData;
    sprite.sheet_name = name;
    sprite.type = sheetDataFlare.sheet_type;
    zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(entity, sheetDataFlare.x_offset, sheetDataFlare.y_offset);

    zone.emplace_or_replace<Component::Scale>(entity, scale);

    zone.emplace_or_replace<Action_Component::Action>(entity, Action_Component::walk);
    ///positon data
    zone.emplace_or_replace<Component::Position>(entity, spelldir.x, spelldir.y);
    ///spell data
    zone.emplace_or_replace<Component::Radius>(entity, data.radius * scale);
    zone.emplace_or_replace<Component::Velocity>(entity, 0.0f, 0.0f, 0.0f, 0.0f, data.speed, 0.0f);
    zone.emplace_or_replace<Component::Mass>(entity, data.mass * scale);
    zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::spell);
    zone.emplace_or_replace<Component::Damage>(entity, 2, 4, 5);
    zone.emplace_or_replace<Component::Caster_ID>(entity, caster_ID);
    ///default data
    zone.emplace_or_replace<Component::Spell>(entity);
    zone.emplace_or_replace<Component::Interaction_Rect>(entity, (data.radius * 1.1f), ((data.radius * 1.1f) * 2.0f));
    zone.emplace_or_replace<Component::Direction>(entity, direction);//match Direction of the caster
    zone.emplace_or_replace<Component::Alive>(entity, true);
    zone.emplace_or_replace<Component::Caster>(entity, caster_ID);
    bool yes = true;
    Collision::Create_Dynamic_Body(zone, state, entity, spelldir.x, spelldir.y, data.radius, data.mass, yes);
    Spell_Linear_Target(zone, entity, targetX, targetY, spelldir.x, spelldir.y, spellData.range);
  }

  void Cast_Spell(entt::registry &zone, int &state) {
    auto view = zone.view<Component::Direction, Action_Component::Action, Component::Position, Component::Casting, Component::Velocity>();
    for (auto entity: view) {

      auto &casting = view.get<Component::Casting>(entity);
      auto &position = view.get<Component::Position>(entity);
      auto &velocity = view.get<Component::Velocity>(entity);
      auto &direction = view.get<Component::Direction>(entity);
      auto &action = view.get<Action_Component::Action>(entity);

      ///look at target but only once
      if (casting.counter >= casting.castTime) {
        direction = Movement::Look_At_Target(position.x, position.y, casting.x, casting.y, velocity.angle);
      }
      //            Utilities::Log((int)direction);
      casting.counter -= Timer::timeStep;
      if (casting.counter <= 0) {
        auto &target = zone.emplace_or_replace<Component::Cast>(entity, casting.x, casting.y);

        ///set into casting mode
        if (action.state == Action_Component::casting) {
          Action_Component::Set_State(action, Action_Component::Action_State::cast);
        }

        ///cast spell
        if (action.frameState == Action_Component::last) {
          direction = Movement::Look_At_Target(position.x, position.y, casting.x, casting.y, velocity.angle);
          Create_Spell(zone, state, entity, position, direction, casting.name, target.targetX, target.targetY);
          zone.remove<Component::Casting>(entity);
        }
      } else if (zone.any_of<Component::Moving>(entity)) {
        Action_Component::Set_State(action, Action_Component::idle);
        zone.remove<Component::Casting>(entity);
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
    auto view = zone.view<Component::Spell, Component::Body, Component::Position, Component::Interaction_Rect, Component::In_Object_Tree>(entt::exclude<Component::Linear_Move, Component::Explosion>);
    for (auto entity: view) {
      auto &position = view.get<Component::Position>(entity);
      auto &body = view.get<Component::Body>(entity);
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

  void Spell_Hit(entt::registry &zone, entt::entity spell_ID, entt::entity struck_ID) {
    Component::Damage damageRange = zone.get<Component::Damage>(spell_ID);

    int damage = Combat_Control::Calculate_Damage(damageRange);
    entt::entity player;
    auto view = zone.view<Component::Input>();
    for (auto input: view) {
      player = input;
    }
    if (zone.get<Component::Caster>(spell_ID).caster == player) {
      Damage_Text::Add_To_Scrolling_Damage(zone, spell_ID, struck_ID, damage, Component::fire, damageRange.critical);
    }
    auto &struck = zone.get_or_emplace<Component::Struck>(struck_ID);
    struck.struck += damage;
  }

  void Check_Spell_Collide(entt::registry &zone, int &state) {
    auto view = zone.view<Component::Spell, Component::Radius, Component::Position, Component::Alive, Component::Caster>();
    for (auto entity: view) {
      auto &alive = view.get<Component::Alive>(entity).bIsAlive;
      if (alive) {
        auto &radius = view.get<Component::Radius>(entity).fRadius;
        auto &position = view.get<Component::Position>(entity);
        auto &caster_ID = zone.get<Component::Caster>(entity).caster;

        SDL_FRect spellRect = Utilities::Get_FRect_From_Point_Radius(radius, position.x, position.y);
        Quad_Tree::Entity_Data targetData = Quad_Tree::Entity_vs_QuadTree_Collision(zone, spellRect, state);

        ///prevent spell from hitting itself or it's caster or a ground item
        if (targetData.b && caster_ID != targetData.entity_ID && targetData.entity_ID != entity && !zone.any_of<Ground_Item>(targetData.entity_ID)) {
          alive = false;
          Spell_Hit(zone, entity, targetData.entity_ID);
          zone.remove<Component::Linear_Move>(entity);
          zone.remove<Component::Mouse_Move>(entity);
        }
      }
    }
  }


  void Update_Spells(entt::registry &zone, int &state) {
    Destroy_NonMoving_Spells(zone, state);
    Clear_Collided_Spells(zone, state);
    Check_Spell_Collide(zone, state);
    Cast_Spell(zone, state);
    Casting_Updater(zone);
  }
}// namespace Spells
