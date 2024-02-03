

namespace Building_Functions {

  void Check_For_Interior(entt::registry &zone, const entt::entity building_ID) {
    if (!zone.any_of<Rendering_Components::Showing_Interior>(building_ID)) {
      if (zone.any_of<Component::Entity_Type>(building_ID)) {
        auto type = zone.get<Component::Entity_Type>(building_ID);
        if (type == Component::Entity_Type::building) {
          if (zone.any_of<Rendering_Components::Interior_Sheet_Info>(building_ID)) {
            auto &sprite = zone.get<Rendering_Components::Sprite_Sheet_Info>(building_ID);
            auto &offset = zone.get<Rendering_Components::Sprite_Offset>(building_ID);
            auto &targetPosition = zone.get<Component::Position>(building_ID);
            auto &interior = zone.get<Rendering_Components::Interior_Sheet_Info>(building_ID);
            targetPosition.y -= offset.y;
            targetPosition.y += interior.offset.y;
            sprite.interior = true;
            zone.emplace_or_replace<Rendering_Components::Showing_Interior>(building_ID);
          }
        }
      }
    }
  }

  void Show_Interior(const b2Contact *contact) {
    if (World::world[World::currentZone.current].zone.any_of<Component::Input>((entt::entity) contact->GetFixtureA()->GetBody()->GetUserData().entity_ID)) {
      if (contact->GetFixtureB()->IsSensor()) {
        Check_For_Interior(World::world[World::currentZone.current].zone, (entt::entity) contact->GetFixtureB()->GetBody()->GetUserData().entity_ID);
      }
    } else if (World::world[World::currentZone.current].zone.any_of<Component::Input>((entt::entity) contact->GetFixtureB()->GetBody()->GetUserData().entity_ID)) {
      if (contact->GetFixtureA()->IsSensor()) {
        Check_For_Interior(World::world[World::currentZone.current].zone, (entt::entity) contact->GetFixtureA()->GetBody()->GetUserData().entity_ID);
      }
    }
  }

  void Check_For_Exterior(entt::registry &zone, const entt::entity building_ID) {
    if (zone.any_of<Rendering_Components::Interior_Sheet_Info>(building_ID)) {
      auto &sprite = zone.get<Rendering_Components::Sprite_Sheet_Info>(building_ID);
      auto &offset = zone.get<Rendering_Components::Sprite_Offset>(building_ID);
      auto &buildingPosition = zone.get<Component::Position>(building_ID);
      auto &interior = zone.get<Rendering_Components::Interior_Sheet_Info>(building_ID);
      buildingPosition.y -= interior.offset.y;
      buildingPosition.y += offset.y;
      sprite.interior = false;
      zone.remove<Rendering_Components::Showing_Interior>(building_ID);
    }
  }

  void Show_Exterior(const b2Contact *contact) {
    if (World::world[World::currentZone.current].zone.any_of<Component::Input>((entt::entity) contact->GetFixtureA()->GetBody()->GetUserData().entity_ID)) {
      if (contact->GetFixtureB()->IsSensor()) {
        Check_For_Exterior(World::world[World::currentZone.current].zone, (entt::entity) contact->GetFixtureB()->GetBody()->GetUserData().entity_ID);
      }
    } else if (World::world[World::currentZone.current].zone.any_of<Component::Input>((entt::entity) contact->GetFixtureB()->GetBody()->GetUserData().entity_ID)) {
      if (contact->GetFixtureA()->IsSensor()) {
        Check_For_Exterior(World::world[World::currentZone.current].zone, (entt::entity) contact->GetFixtureA()->GetBody()->GetUserData().entity_ID);
      }
    }
  }

}// namespace Building_Functions