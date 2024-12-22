#pragma once

#include "Audio/effects.h"
#include "action_components.h"
#include "combat_graphics.h"
#include "components.h"
#include "damage_text.h"
#include "movement_components.h"
#include "utilities.h"

namespace Combat_Control {

  int Calculate_Damage(Component::Damage &damageRange) {
    if (damageRange.minDamage == 0) {
      std::cout << "function Calculate_Damage() min damage range 0, divide by zero error" << std::endl;
      //			return 0;
    }

    int damage = Utilities::Get_Random_Number(damageRange.minDamage, damageRange.maxDamage);
    int checkCrit = Utilities::Get_Random_Number(1, 100);

    if (checkCrit <= damageRange.critChance) {
      damage *= 2;
      damageRange.critical = true;
    }
    return damage;
  }

  void Attack_Cast(entt::registry &zone) {
    auto view = zone.view<Component::Direction, Component::Position, Action_Component::Action, Component::Attack, Component::Velocity, Component::Attack_Speed>(entt::exclude<Component::Attacking>);
    for (auto entity: view) {
      //initiates a point and click attack
      auto &attackSpeed = view.get<Component::Attack_Speed>(entity);
      auto &action = view.get<Action_Component::Action>(entity);
      ///ensures it attacks at the end of the last frame of the attack
      /// if current attackSpeed >= attackSpeed value then subtract the attackSpeed value from the current attackSpeed counter
      if (action.state != Action_Component::attack && action.state != Action_Component::struck && action.state != Action_Component::block && attackSpeed.counter <= 0.0f) {
        zone.emplace_or_replace<Component::In_Combat>(entity, true);
        auto &direction = view.get<Component::Direction>(entity);
        auto &velocity = view.get<Component::Velocity>(entity);
        auto &target = view.get<Component::Attack>(entity);
        auto &position = view.get<Component::Position>(entity);

        //reset keyboard movement
        velocity.magnitude.x = 0.0f;
        velocity.magnitude.y = 0.0f;

        direction = Movement_Component::Look_At_Target(position.x, position.y, target.targetX, target.targetY, velocity.angle, velocity.hexDir);
        attackSpeed.counter = attackSpeed.period;

        //        if (action.weaponType == Weapon_Type::spear || action.weaponType == Weapon_Type::staff) {
        //          Action_Component::Set_State(action, Action_Component::attackPolearm);
        //        }
        if (action.weaponType == Weapon_Type::bow) {
          Action_Component::Set_State(action, Action_Component::ranged);
        } else {
          Action_Component::Set_State(action, Action_Component::attack);
        }

        zone.emplace_or_replace<Component::Attacking>(entity, target.target_ID);
        zone.remove<Component::Attack>(entity);
      } else {
        zone.remove<Component::Attack>(entity);
      }
    }
    //create SDL_FRect in front of unit, size from a size component on weapon
  }

  void Queue_Hit(entt::registry &zone, entt::entity entity, entt::entity target_ID, Component::Melee_Damage meleeDamage, Component::Attack_Type type, float mod, Component::Bonus_Damage_Type bonusType) {
    Component::Damage damageRange = {meleeDamage.minDamage, meleeDamage.maxDamage, meleeDamage.critChance};

    switch (bonusType) {
      case Component::Bonus_Damage_Type::add: {
        damageRange.minDamage += mod;
        damageRange.maxDamage += mod;
        break;
      }
      case Component::Bonus_Damage_Type::multiply: {
        damageRange.minDamage *= mod;
        damageRange.maxDamage *= mod;
        break;
      }
    }

    int damage = Calculate_Damage(damageRange);

    if (zone.any_of<Component::Input>(entity)) {
      Damage_Text::Add_To_Scrolling_Damage(zone, entity, target_ID, damage, type, damageRange.critical);
    }

    auto &struck = zone.get_or_emplace<Component::Struck>(target_ID);
    struck.struck += damage;

    auto &targetAction = zone.get_or_emplace<Action_Component::Action>(target_ID);
    if (damageRange.critical) {
      struck.critical = true;
      Action_Component::Set_State(targetAction, Action_Component::struck);
    } else if (targetAction.state == Action_Component::idle) {
      Action_Component::Set_State(targetAction, Action_Component::combatIdle);
    }

    //get weapon type
    Effects::Play(meleeDamage.weaponType);
  }

  void Attack_Target(entt::registry &zone) {
    auto view = zone.view<Component::Attacking, Action_Component::Action, Component::Melee_Damage>();
    for (auto entity: view) {
      auto &action = view.get<Action_Component::Action>(entity);
      ///possibly not necessary
      if (action.state == Action_Component::attack || action.state == Action_Component::attackPolearm || action.state == Action_Component::ranged) {
        ///ensures it attacks at the end of the last frame of the attack
        if (action.frameState == Action_Component::Action_Progress::hit) {
          //executes a point and click attack
          auto &target_ID = view.get<Component::Attacking>(entity).target_ID;
          auto &meleeDamage = view.get<Component::Melee_Damage>(entity);
          /// calculate damage and show for player
          Queue_Hit(zone, entity, target_ID, meleeDamage, Component::normal, 0.0f, Component::Bonus_Damage_Type::add);
          zone.remove<Component::Attacking>(entity);
        }
      } else {
        zone.remove<Component::Attacking>(entity);
      }
    }
  }

  void Alert_When_Struck(entt::registry &zone) {
    auto view = zone.view<Component::Sight_Range, Component::Struck>(entt::exclude<Component::In_Combat, Component::Alert>);
    //expands sight range
    for (auto entity: view) {
      auto &sight = view.get<Component::Sight_Range>(entity);
      zone.emplace_or_replace<Component::Alert>(entity, sight.range);
      sight.range *= 2.0f;
    }
  }

  void Reset_Alert(entt::registry &zone) {
    auto view = zone.view<Component::Alert, Component::In_Combat, Component::Sight_Range>();
    for (auto entity: view) {
      //resets sight range after entering combat
      auto &sight = view.get<Component::Sight_Range>(entity);
      auto &alert = view.get<Component::Alert>(entity);
      sight.range = alert.sightRange;
      zone.remove<Component::Alert>(entity);
    }
  }

  //take input of unit Radius and Weapon_Size to fill this out for any wielder and any weapon
  SDL_FRect Create_Attack_Box(Component::Position &position, Component::Direction &direction) {
    switch (direction) {
      case Component::Direction::N:
        return {position.x - 15, position.y - 55, 30, 40};
      case Component::Direction::S:
        return {position.x - 15, position.y + 15, 30, 40};
      case Component::Direction::E:
        return {position.x + 15, position.y - 15, 40, 30};
      case Component::Direction::W:
        return {position.x - 55, position.y - 15, 40, 30};
      case Component::Direction::NW:
        return {position.x - 45, position.y - 45, 30, 30};
      case Component::Direction::NE:
        return {position.x + 15, position.y - 45, 30, 30};
      case Component::Direction::SW:
        return {position.x - 45, position.y + 15, 30, 30};
      case Component::Direction::SE:
        return {position.x + 15, position.y + 15, 30, 30};
    }
    std::cout << "Weapons::Attack_Direction() passthrough error" << std::endl;
    return {0, 0, 0, 0};
  }

  void Attack_Box_Destroy(entt::registry &zone) {
    auto view = zone.view<Component::Attack_Box_Duration>();
    for (auto entity: view) {
      auto &time = view.get<Component::Attack_Box_Duration>(entity).lifeTime;
      auto &count = view.get<Component::Attack_Box_Duration>(entity).count;
      count += Timer::timeStep;
      if (count >= time) {
        zone.destroy(entity);
      }
    }
  }

  void Create_AoE_Attack(entt::registry &zone, Component::Position &position, Component::Direction &direction) {
    SDL_FRect attackRect = Create_Attack_Box(position, direction);

    auto aoeAttack = zone.create();
    zone.emplace_or_replace<Component::Damage>(aoeAttack, 1, 10);
    zone.emplace_or_replace<Component::Attack_Box_Duration>(aoeAttack, 0.0f, 0.0f);
    zone.emplace_or_replace<Component::Mass>(aoeAttack, 500.0f);
    zone.emplace_or_replace<Component::Weapon_Size>(aoeAttack, attackRect.x, attackRect.y, attackRect.w, attackRect.h);//set x, y to in front of char when he attacks
    zone.emplace_or_replace<Component::Position>(aoeAttack, position);
    zone.emplace_or_replace<Component::Alive>(aoeAttack, true);
    zone.emplace_or_replace<Component::Entity_Type>(aoeAttack, Component::Entity_Type::spell);
  }

  void AttackSpeed_Updater(entt::registry &zone) {
    auto view = zone.view<Component::Attack_Speed, Component::Renderable>();
    for (auto entity: view) {
      auto &attackSpeed = view.get<Component::Attack_Speed>(entity);
      if (attackSpeed.counter >= 0.0f) {
        attackSpeed.counter -= Timer::timeStep;
      }
    }
  }

  void Struck_Updater(entt::registry &zone) {
    auto view = zone.view<Component::Struck, Action_Component::Action, Rendering_Components::Sprite_Sheet_Info>();
    for (auto entity: view) {
      auto &action = view.get<Action_Component::Action>(entity);
      auto &sheetData = view.get<Rendering_Components::Sprite_Sheet_Info>(entity);
      if (action.state == Action_Component::dying || action.state == Action_Component::dead) {
        zone.remove<Component::Struck>(entity);
      }
      if (sheetData.flareSpritesheet) {
        if (action.frameState == Action_Component::last) {
          ///should not return to idle, should go into an "idle-combat" mode
          zone.remove<Component::Struck>(entity);
        }
      } else if (sheetData.sheetData) {
        if (action.frameState == Action_Component::last) {
          ///should not return to idle, should go into an "idle-combat" mode
          zone.remove<Component::Struck>(entity);
        }
      } else {
        Utilities::Log("Struck_Updater(entt::registry &zone) - Both NULL passthrough error");
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
        if (targetData.b && caster_ID != targetData.entity_ID && targetData.entity_ID != entity && !zone.any_of<Item_Component::Ground_Item>(targetData.entity_ID)) {
          alive = false;
          Spell_Hit(zone, entity, targetData.entity_ID);
          zone.remove<Component::Linear_Move>(entity);
          zone.remove<Component::Mouse_Move>(entity);
        }
      }
    }
  }


  void Attack_Sounds() {
  }

  void Update_Attacks(entt::registry &zone, int &state) {
    AttackSpeed_Updater(zone);
    Check_Spell_Collide(zone, state);
    Struck_Updater(zone);
    Attack_Cast(zone);
    Attack_Target(zone);
    Reset_Alert(zone);
    Alert_When_Struck(zone);
  }
}// namespace Combat_Control
