#include "components.h"
#include "create_entities.h"
#include "rendering_components.h"
#include "spells.h"

namespace Update_Spells {

  void Update_Metamorphosis(entt::registry &zone, int &state) {
    //    auto view1 = zone.view<Rendering_Components::Metamorphosis, Rendering_Components::Sprite_Sheet_Info, Rendering_Components::Sprite_Offset, Component::Interaction_Rect, Component::Radius>();
    auto view1 = zone.view<Rendering_Components::Metamorphosis, Component::Position, Component::Direction, Rendering_Components::Sprite_Sheet_Info, Rendering_Components::Sprite_Offset, Component::Interaction_Rect, Component::Radius>();
    for (auto caster_ID: view1) {
      auto &metamorphosis = view1.get<Rendering_Components::Metamorphosis>(caster_ID);
      if (metamorphosis.duration <= 0) {
        auto &sprite = view1.get<Rendering_Components::Sprite_Sheet_Info>(caster_ID);
        auto &offsets = view1.get<Rendering_Components::Sprite_Offset>(caster_ID);
        auto &rect = view1.get<Component::Interaction_Rect>(caster_ID);
        auto &radius = view1.get<Component::Radius>(caster_ID);

        sprite.sheetData = metamorphosis.sheetData;
        sprite.sheet_name = metamorphosis.sheet_name;
        offsets = metamorphosis.offsets;
        rect = metamorphosis.rect;
        radius = metamorphosis.radius;
        //set texture to new texture
        zone.emplace_or_replace<Rendering_Components::Equipment_Sprites>(caster_ID, metamorphosis.equipment);

        auto &position = view1.get<Component::Position>(caster_ID);
        auto &direction = view1.get<Component::Direction>(caster_ID);
        Spells::Spell_Cast_Effect(zone, state, caster_ID, position, direction, metamorphosis.spellEffect,
                                  position.x, position.y);

        zone.remove<Rendering_Components::Metamorphosis>(caster_ID);
      } else {
        metamorphosis.duration -= Timer::timeStep;
      }
    }

    auto view = zone.view<Rendering_Components::Metamorphosis, Component::Position, Rendering_Components::Sprite_Sheet_Info, Rendering_Components::Equipment_Sprites, Rendering_Components::Sprite_Offset, Component::Interaction_Rect, Component::Radius>();
    for (auto caster_ID: view) {
      auto &metamorphosis = view.get<Rendering_Components::Metamorphosis>(caster_ID);
      db::Unit_Data imgPaths = Game_Objects_Lists::units[metamorphosis.unit.type][metamorphosis.unit.index];
      Component::Unit_Index unitIndex = {metamorphosis.unit.type, metamorphosis.unit.index};

      imgPaths.imgPath = "assets/" + imgPaths.imgPath;
      imgPaths.portraitPath = "assets/" + imgPaths.portraitPath;
      imgPaths.bodyPath = "assets/" + imgPaths.bodyPath;
      Entity_Loader::Data data;
      data = Entity_Loader::parse_data(imgPaths.name);//

      auto &position = view.get<Component::Position>(caster_ID);
      auto &sprite = view.get<Rendering_Components::Sprite_Sheet_Info>(caster_ID);
      auto &equipment = view.get<Rendering_Components::Equipment_Sprites>(caster_ID);
      auto &offsets = view.get<Rendering_Components::Sprite_Offset>(caster_ID);
      auto &rect = view.get<Component::Interaction_Rect>(caster_ID);
      auto &radius = view.get<Component::Radius>(caster_ID);

      //save rendering data to component
      metamorphosis = {sprite.sheetData, sprite.sheet_name, equipment, offsets, rect, radius, metamorphosis.duration, metamorphosis.unit, metamorphosis.spellEffect};

      //set texture to new texture
      int unit_ID = 0;
      unit_ID = Entity_Data::Check_For_Template_ID(imgPaths.name);
      Graphics::Texture texture;
      texture.texture = Graphics::Create_Game_Object(unit_ID, imgPaths.imgPath.c_str());

      std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerframeData = NULL;
      std::string sheetname = Entity_Loader::Get_Sprite_Sheet(imgPaths.name);
      packerframeData = Texture_Packer::TexturePacker_Import(imgPaths.name, texture.texture);

      sprite.sheetData = packerframeData;
      sprite.sheet_name = imgPaths.name;
      zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(caster_ID, data.x_offset * data.scale,
                                                                   data.y_offset * data.scale);
      radius.fRadius = data.radius;
      Create_Entities::Emplace_Interaction_Rect(zone, caster_ID, data, position.x, position.y);
      zone.remove<Rendering_Components::Equipment_Sprites>(caster_ID);
    }
  }

  void Aura_Damage(entt::registry &zone, int &state) {
    auto view = zone.view<Component::Aura_Damage, Component::Position, Component::Direction, Component::Alive, Component::Entity_Type>();
    for (auto caster_ID: view) {
      auto &casting = view.get<Component::Aura_Damage>(caster_ID);

      casting.count -= Timer::timeStep;
      if (casting.count <= 0.0f) {
        casting.count = casting.duration;

        auto &position = view.get<Component::Position>(caster_ID);
        SDL_FRect rect = {0.0f, 0.0f, 1000.0f, 1000.0f};
        SDL_FRect frect = Utilities::Centre_Rect_On_Position(rect, position.x, position.y);
        std::vector<entt::entity> entities = Quad_Tree::Get_Nearby_Entities(zone, frect, state);

        for (auto entity: entities) {
          auto &type = view.get<Component::Entity_Type>(entity);
          if (type == Component::Entity_Type::unit) {
            auto &alive = view.get<Component::Alive>(entity);
            if (alive.bIsAlive) {
              if (Social_Control::Check_Relationship(zone, caster_ID, entity)) {// if unit is friendly
                auto &targetPosition = view.get<Component::Position>(entity);
                auto &direction = view.get<Component::Direction>(entity);

                Spells::Spell_Cast_Effect(zone, state, entity, targetPosition, direction, casting.effect, targetPosition.x, targetPosition.y);

                Component::Melee_Damage meleeDamage = {1, 5, 20};
                Combat_Control::Queue_Hit(zone, caster_ID, entity, meleeDamage, Component::normal, 0.0f, Component::Bonus_Damage_Type::add);
              }
            }
          }
        }
      }
    }
  }

  void Might(entt::registry &zone, int &state) {
    auto view = zone.view<Component::Might, Component::Position, Component::Direction, Component::Alive, Component::Entity_Type, Rendering_Components::Buff_Sprites>();
    for (auto caster_ID: view) {
      auto &casting = view.get<Component::Might>(caster_ID);
      casting.count -= Timer::timeStep;
      if (casting.count <= 0.0f) {
        casting.count = casting.duration;

        auto &position = view.get<Component::Position>(caster_ID);
        SDL_FRect rect = {0.0f, 0.0f, 1000.0f, 1000.0f};
        SDL_FRect frect = Utilities::Centre_Rect_On_Position(rect, position.x, position.y);
        std::vector<entt::entity> entities = Quad_Tree::Get_Nearby_Entities(zone, frect, state);

        for (auto entity: entities) {
          auto &type = view.get<Component::Entity_Type>(entity);
          if (type == Component::Entity_Type::unit) {
            auto &alive = view.get<Component::Alive>(entity);
            if (alive.bIsAlive) {
              if (!Social_Control::Check_Relationship(zone, caster_ID, entity)) {// if unit is friendly
                                                                                 //            if the buff is already applied do nothing
                                                                                 //             maybe a hashmap instead I can look it up
                if (1) {
                  auto &targetPosition = view.get<Component::Position>(entity);
                  auto &direction = view.get<Component::Direction>(entity);
                  auto &sprites = view.get<Rendering_Components::Buff_Sprites>(entity);

                  for (auto &buff: sprites.sheet) {
                    if (buff.FrameIndex >= buff.ItemSheetData->at(buff.name).actionFrameData[Action_Component::walk].NumFrames) {
                      sprites.sheet.clear();
                    }
                  }

                  auto sprite = Spells::Spell_Cast_Effect(zone, state, entity, targetPosition, direction, casting.effect, targetPosition.x, targetPosition.y);
                  sprite.offset.y /= 2.0f;
                  zone.destroy(sprite.itemID);

                  sprites.sheet.emplace_back(sprite);

                  std::string name = "z";
                  if (sprites.buffs.contains(name)) {
                    return;
                  } else {
                    sprites.buffs[name] = sprite;
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  void Damage_Over_Time(entt::registry &zone, int &state) {
    auto view = zone.view<Component::Damage_Over_Time, Component::Position, Component::Direction>();
    for (auto entity: view) {
      auto &casting = view.get<Component::Damage_Over_Time>(entity);
      for (int i = 0; i < casting.debuffs.size(); ++i) {
        casting.debuffs[i].count -= Timer::timeStep;
        if (casting.debuffs[i].count <= 0.0f) {
          casting.debuffs[i].count = casting.debuffs[i].duration;
          casting.debuffs[i].ticks -= 1;
          auto &position = view.get<Component::Position>(entity);
          auto &direction = view.get<Component::Direction>(entity);

          Spells::Spell_Cast_Effect(zone, state, entity, position, direction, casting.debuffs[i].effect, position.x, position.y);

          Component::Melee_Damage meleeDamage = {1, 5, 20};
          Combat_Control::Queue_Hit(zone, entity, entity, meleeDamage, Component::normal, 0.0f, Component::Bonus_Damage_Type::add);
          if (casting.debuffs[i].ticks < 0) {
            casting.debuffs.erase(casting.debuffs.begin() + i);
          }
        }
      }
    }
  }

  void Update_Spells(entt::registry &zone, int &state) {
    Update_Metamorphosis(zone, state);
    Aura_Damage(zone, state);
    Might(zone, state);
    Damage_Over_Time(zone, state);
    Spells::Destroy_Particle_Spells(zone);
    Spells::Destroy_NonMoving_Spells(zone, state);
    Spells::Clear_Collided_Spells(zone, state);
    Spells::Cast_Spell(zone, state);
    Spells::Casting_Updater(zone);
  }

}// namespace Update_Spells
