#pragma once
#include "entt.hpp"
#include "components.h"

namespace Formation_Positions {



	void Update_Vector_From_Unit(entt::registry& zone) { // updates positions of soldiers stored in units for collision box
		auto view = zone.view<Component::Position, Component::Assigned_To_Formation, Component::Soldier>();
		for (auto entity : view) {
			auto& x = view.get<Component::Position>(entity);
			auto& y = view.get<Component::Position>(entity);
			auto& soldier = view.get<Component::Assigned_To_Formation>(entity);
			auto& squad = zone.get<Component::Squad>(soldier.iUnit_Assigned_To); // gets the squad that the soldier is attached to
			squad.fPX.at(soldier.iIndex) = x.x;
			squad.fPY.at(soldier.iIndex) = y.y;
		}
	}

	void Update_Company(entt::registry& zone) { // updates positions of soldiers stored in units for collision box
		auto company_view = zone.view<Component::Company>();
		for (auto companies : company_view) {
			auto& company = company_view.get<Component::Company>(companies);
			for (int i = 0; i < company.iSub_Units.size(); i++) {
				auto& platoon = zone.get<Component::Platoon>(company.iSub_Units[i]); // gets the squad that the soldier is attached to
				company.fPX[i] = platoon.sCollide_Box.x;
				company.fPY[i] = platoon.sCollide_Box.y;
				company.fPW[i] = platoon.sCollide_Box.x + platoon.sCollide_Box.w;
				company.fPH[i] = platoon.sCollide_Box.y + platoon.sCollide_Box.h;
			}
		}
	}
	void Update_Platoons(entt::registry& zone) { // updates positions of soldiers stored in units for collision box
		auto view = zone.view<Component::Platoon>();
		for (auto platoons : view) {
			auto& platoon = view.get<Component::Platoon>(platoons);
			for (int i = 0; i < platoon.iSub_Units.size(); i++) {
				auto& squad = zone.get<Component::Squad>(platoon.iSub_Units[i]); // gets the squad that the soldier is attached to
				platoon.fPX[i] = squad.sCollide_Box.x;
				platoon.fPY[i] = squad.sCollide_Box.y;
				platoon.fPW[i] = squad.sCollide_Box.x + squad.sCollide_Box.w;
				platoon.fPH[i] = squad.sCollide_Box.y + squad.sCollide_Box.h;
			}
		}
	}



	void Update_Squad_Box(entt::registry& zone) {
		auto view = zone.view<Component::Squad>();
		for (auto squads : view) { //update colliders
			auto& squad1 = view.get<Component::Squad>(squads);
			if (!squad1.iSub_Units.empty()) { //will crash if vector is empty
				float xMax = *std::max_element(squad1.fPX.begin(), squad1.fPX.end());
				float xMin = *std::min_element(squad1.fPX.begin(), squad1.fPX.end());
				float yMax = *std::max_element(squad1.fPY.begin(), squad1.fPY.end());
				float yMin = *std::min_element(squad1.fPY.begin(), squad1.fPY.end());
				float rMax = *std::max_element(squad1.fRadius.begin(), squad1.fRadius.end());
				squad1.sCollide_Box = { xMin - rMax, yMin - rMax, (xMax - xMin) + (rMax * 2), (yMax - yMin) + (rMax * 2) };
			}
		}
	}

	void Update_Platoon_Box(entt::registry& zone) {
		auto Platoon_View = zone.view<Component::Platoon>();
		for (auto platoons : Platoon_View) {
			auto& platoon1 = Platoon_View.get<Component::Platoon>(platoons);
			if (!platoon1.iSub_Units.empty()) { //will crash if vector is empty {
				float xMin = *std::min_element(platoon1.fPX.begin(), platoon1.fPX.end());
				float yMin = *std::min_element(platoon1.fPY.begin(), platoon1.fPY.end());
				float wMax = *std::max_element(platoon1.fPW.begin(), platoon1.fPW.end());
				float hMax = *std::max_element(platoon1.fPH.begin(), platoon1.fPH.end());
				platoon1.sCollide_Box = { xMin, yMin, wMax - xMin, hMax - yMin };
			}
		}
	}

	void Update_Company_Box(entt::registry& zone) {
		auto company_view = zone.view<Component::Company>();
		for (auto companies : company_view) {
			auto& company = company_view.get<Component::Company>(companies);
			if (!company.iSub_Units.empty()) { //will crash if vector is empty {
				float xMin = *std::min_element(company.fPX.begin(), company.fPX.end());
				float yMin = *std::min_element(company.fPY.begin(), company.fPY.end());
				float wMax = *std::max_element(company.fPW.begin(), company.fPW.end());
				float hMax = *std::max_element(company.fPH.begin(), company.fPH.end());
				company.sCollide_Box = { xMin, yMin, wMax - xMin, hMax - yMin };
			}
		}
	}






	void Update_Formation_Positions(entt::registry& zone) {
		Update_Vector_From_Unit(zone);
		
		Update_Platoons(zone);
		Update_Company(zone);

		Update_Squad_Box(zone);
		Update_Platoon_Box(zone);
		Update_Company_Box(zone);
	}



}