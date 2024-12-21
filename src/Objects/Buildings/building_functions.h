

namespace Building_Functions {

    bool Check_For_Interior(entt::registry &zone, const entt::entity building_ID) {
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
			return true;
		    }
		}
	    }
	}
	return false;
    }

    void Flag_Inside(entt::registry &zone, const entt::entity entity) {
	if (zone.any_of<Component::Renderable>(entity)) {
	    zone.get<Component::Renderable>(entity).inside = true;
	}
	if (zone.any_of<Component::Is_Inside>(entity)) {
	    zone.get<Component::Is_Inside>(entity).inside = true;
	}
    }

    void Show_Interior(const b2Contact *contact) {
	entt::registry &zone = World::world[World::currentZone.current].zone;
	entt::entity entity1 = (entt::entity) contact->GetFixtureA()->GetBody()->GetUserData().entity_ID;
	entt::entity entity2 = (entt::entity) contact->GetFixtureB()->GetBody()->GetUserData().entity_ID;

	if (contact->GetFixtureB()->IsSensor()) {
	    if (zone.any_of<Component::Input>(entity1)) {
		if (Check_For_Interior(zone, entity2))
		    Flag_Inside(zone, entity1);
	    } else {
		Flag_Inside(zone, entity1);
	    }
	} else if (contact->GetFixtureA()->IsSensor()) {
	    if (zone.any_of<Component::Input>(entity2)) {
		if (Check_For_Interior(zone, entity1))
		    Flag_Inside(zone, entity2);
	    } else {
		Flag_Inside(zone, entity2);
	    }
	}
    }

    bool Check_For_Exterior(entt::registry &zone, const entt::entity building_ID) {
	if (zone.any_of<Rendering_Components::Interior_Sheet_Info>(building_ID)) {
	    auto &sprite = zone.get<Rendering_Components::Sprite_Sheet_Info>(building_ID);
	    auto &offset = zone.get<Rendering_Components::Sprite_Offset>(building_ID);
	    auto &buildingPosition = zone.get<Component::Position>(building_ID);
	    auto &interior = zone.get<Rendering_Components::Interior_Sheet_Info>(building_ID);
	    buildingPosition.y -= interior.offset.y;
	    buildingPosition.y += offset.y;
	    sprite.interior = false;
	    zone.remove<Rendering_Components::Showing_Interior>(building_ID);
	    return true;
	}
	return false;
    }

    void Flag_Outside(entt::registry &zone, const entt::entity entity) {
	if (zone.any_of<Component::Renderable>(entity)) {
	    zone.get<Component::Renderable>(entity).inside = false;
	}
	if (zone.any_of<Component::Is_Inside>(entity)) {
	    zone.get<Component::Is_Inside>(entity).inside = false;
	}
    }

    void Show_Exterior(const b2Contact *contact) {
	entt::registry &zone = World::world[World::currentZone.current].zone;
	entt::entity entity1 = (entt::entity) contact->GetFixtureA()->GetBody()->GetUserData().entity_ID;
	entt::entity entity2 = (entt::entity) contact->GetFixtureB()->GetBody()->GetUserData().entity_ID;

	if (contact->GetFixtureB()->IsSensor()) {
	    if (zone.any_of<Component::Input>(entity1)) {
		if (Check_For_Exterior(zone, entity2))
		    Flag_Outside(zone, entity1);
	    } else {
		Flag_Outside(zone, entity1);
	    }
	} else if (contact->GetFixtureA()->IsSensor()) {
	    if (zone.any_of<Component::Input>(entity2)) {
		if (Check_For_Exterior(zone, entity1))
		    Flag_Outside(zone, entity2);
	    } else {
		Flag_Outside(zone, entity2);
	    }
	}
    }
}// namespace Building_Functions