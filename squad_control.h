#pragma once
#include "entt/entt.hpp"
#include "components.h"

namespace Squad_Control {

	entt::entity Create_New_Squad(entt::registry& zone) {
		auto new_squad_ID = zone.create();
		zone.emplace<Test::Soldiers_Assigned_List>(new_squad_ID);
		auto& squadData = zone.emplace<Test::Unit_Formation_Data>(new_squad_ID);
		squadData.formationType = Test::Formation_Type::squad;
		squadData.formation_ID = new_squad_ID;
		return new_squad_ID;
	}

	bool Replace_Umit_In_Squad(entt::registry& zone, entt::entity& squad_ID, Test::Soldiers_Assigned_List& squad, Test::Soldier_Data& soldierData) {

		for (int i = 0; i < squad.unitData.size(); i++) {
			if (squad.unitData.at(i).bAlive == false) {
				auto& soldier = zone.emplace<Component::Assigned_To_Formation>(soldierData.unit_ID, i, squad_ID);
				squad.unitData.at(i) = soldierData;
				return true;
			}
		}
		return false;
	}
	bool Emplace_Umit_In_Squad(entt::registry& zone, entt::entity& squad_ID, Test::Soldiers_Assigned_List& squad, Test::Soldier_Data& soldierData) {

		if (squad.unitData.size() < squad.size) {
			auto& soldier = zone.emplace<Component::Assigned_To_Formation>(soldierData.unit_ID, 0, squad_ID);
			soldier.iIndex = (int)squad.unitData.size();
			squad.unitData.emplace_back(soldierData);
			return true;
		}
		return false;
	}
	bool Assign_Selected_Units_To_Squad(entt::registry& zone, Test::Soldier_Data& soldierData) {

		auto view = zone.view<Test::Soldiers_Assigned_List>();

		for (auto squad_ID : view) {
			Test::Soldiers_Assigned_List& squad = view.get<Test::Soldiers_Assigned_List>(squad_ID);
			if (Replace_Umit_In_Squad(zone, squad_ID, squad, soldierData) == true) {
				return true;
			}
		}

		for (auto squad_ID : view) {
			Test::Soldiers_Assigned_List& squad = view.get<Test::Soldiers_Assigned_List>(squad_ID);
			if (Emplace_Umit_In_Squad(zone, squad_ID, squad, soldierData) == true) {
				return true;
			}
		}

		entt::entity new_squad_ID = Create_New_Squad(zone);
		Test::Soldiers_Assigned_List& squad = zone.get<Test::Soldiers_Assigned_List>(new_squad_ID);
		Emplace_Umit_In_Squad(zone, new_squad_ID, squad, soldierData);
		return true;
	}

	void Create_And_Fill_New_Squad(entt::registry& zone) {

		auto view = zone.view<Component::Position, Component::Radius, Component::Mass, Component::Soldier, Component::Alive>(entt::exclude<Component::Assigned_To_Formation>);

		for (auto unit_ID : view) {
			auto& x = view.get<Component::Position>(unit_ID).x;
			auto& y = view.get<Component::Position>(unit_ID).y;
			auto& mass = view.get<Component::Mass>(unit_ID).fKilos;
			auto& radius = view.get<Component::Radius>(unit_ID).fRadius;
			auto& alive = view.get<Component::Alive>(unit_ID).bIsAlive;
			Test::Soldier_Data soldierData = { unit_ID, x, y, mass, radius, alive };

			Assign_Selected_Units_To_Squad(zone, soldierData);
		}
	};
}