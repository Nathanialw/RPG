

namespace Building_Functions {

  void Show_Interior(entt::registry &zone, entt::entity building_ID) {
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

  void Show_Exterior(entt::registry &zone, entt::entity building_ID) {
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

}// namespace Building_Functions