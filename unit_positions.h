#pragma once
#include "entt/entt.hpp"
#include "components.h"

namespace Unit_Position {


	void Update_Formation_Unit_Positions(entt::registry& zone) { // updates positions of soldiers stored in units for collision box
		auto view = zone.view<Component::Position, Component::Assigned_To_Formation, Component::Alive, Component::Soldier>();
		
		for (auto entity : view) {
			auto& x = view.get<Component::Position>(entity);
			auto& y = view.get<Component::Position>(entity);
			auto& soldier = view.get<Component::Assigned_To_Formation>(entity);
			//auto& alive = view.get<Component::Alive>(entity).bIsAlive;
			
			auto& formation = zone.get<Test::Soldiers_Assigned_List>(soldier.iUnit_Assigned_To); // gets the squad that the soldier is attached to
			formation.unitData.at(soldier.iIndex).x = x.x;
			formation.unitData.at(soldier.iIndex).y = y.y;
		}
	}

	//updates the formation rect from the positions of the soldiers assigned to it
	void Update_Soldier_Formation_Rect(entt::registry& zone) {
		auto formation_view = zone.view<Test::Unit_Formation_Data, Test::Soldiers_Assigned_List>();

		for (auto formation_ID : formation_view) { //update colliders
			auto& formation = formation_view.get<Test::Unit_Formation_Data>(formation_ID);
			auto& soldierList = formation_view.get<Test::Soldiers_Assigned_List>(formation_ID);
			

			
			if (!soldierList.unitData.empty()) { //will crash if vector is empty
				
				std::vector<float> elementList;
				elementList.resize(soldierList.unitData.size());
				
				
				for (int i = 0; i < soldierList.unitData.size(); i++) {
					elementList.at(i) = soldierList.unitData.at(i).radius;
				}
				float radius = *std::max_element(elementList.begin(), elementList.end());

				//get rect max and min points
				for (int i = 0; i < soldierList.unitData.size(); i++) {
					elementList.at(i) = soldierList.unitData.at(i).x;
				}
				float x = *std::min_element(elementList.begin(), elementList.end());
				formation.sCollide_Box.x = x - radius;

				for (int i = 0; i < soldierList.unitData.size(); i++) {
					elementList.at(i) = soldierList.unitData.at(i).y;
				}
				float y = *std::min_element(elementList.begin(), elementList.end());
				formation.sCollide_Box.y = y - radius;

				for (int i = 0; i < soldierList.unitData.size(); i++) {
					elementList.at(i) = soldierList.unitData.at(i).x;
				}
				float w = *std::max_element(elementList.begin(), elementList.end());
				formation.sCollide_Box.w = w - x + (radius * 2.0f);

				for (int i = 0; i < soldierList.unitData.size(); i++) {
					elementList.at(i) = soldierList.unitData.at(i).y;
				}
				float h = *std::max_element(elementList.begin(), elementList.end());
				formation.sCollide_Box.h = h - y + (radius * 2.0f);
			}
		}
	}


	void Update_Subformation(entt::registry &zone, Test::Unit_Formation_Data &formation) {
		
		for (int i = 0; i < formation.subformationData.size(); i++) {
			auto& eachFormationInSubformation = zone.get<Test::Unit_Formation_Data>(formation.subformationData.at(i).formation_ID);
			formation.subformationData.at(i) = eachFormationInSubformation;
		}
	}
	void Update_CollideBox_From_Subformation(SDL_FRect & formationRect, const std::vector <Test::Unit_Formation_Data> & subformation) {
		
		if (!subformation.empty()) { //will crash if vector is empty
				
			std::vector<float> elementList;
			elementList.resize(subformation.size());

			//get rect max and min points
			for (int i = 0; i < subformation.size(); i++) {
				elementList.at(i) = subformation.at(i).sCollide_Box.x, i;
			}
			float x = *std::min_element(elementList.begin(), elementList.end());
			formationRect.x = x;

			for (int i = 0; i < subformation.size(); i++) {
				elementList.at(i) = subformation.at(i).sCollide_Box.y;
			}
			float y = *std::min_element(elementList.begin(), elementList.end());
			formationRect.y = y;

			for (int i = 0; i < subformation.size(); i++) {
				elementList.at(i) = subformation.at(i).sCollide_Box.x + subformation.at(i).sCollide_Box.w;
			}

			float w = *std::max_element(elementList.begin(), elementList.end());
			formationRect.w = w - x;

			for (int i = 0; i < subformation.size(); i++) {
				elementList.at(i) = subformation.at(i).sCollide_Box.y + subformation.at(i).sCollide_Box.h;
			}

			float h = *std::max_element(elementList.begin(), elementList.end());
			formationRect.h = h - y;
		}		
	}

	//updates the formation rect from the rects of its subformations
	void Update_Formation_Rect(entt::registry& zone) {
		auto view = zone.view<Test::Unit_Formation_Data>(entt::exclude<Test::Soldiers_Assigned_List>);
		for (auto formation_ID : view) { //update colliders
			auto& formation = view.get<Test::Unit_Formation_Data>(formation_ID);
			const auto& subformation = view.get<Test::Unit_Formation_Data>(formation_ID).subformationData;
			
			Update_Subformation(zone, formation);
			Update_CollideBox_From_Subformation(formation.sCollide_Box, subformation);
		}
	}

	/*
	
	A really janky way to quickly grab the right grouping level	
	*/
	void Set_Component(entt::registry & zone) {
		auto formation_view = zone.view<Test::Unit_Formation_Data>(entt::exclude<Test::Grouped>);
		for (auto formation_ID : formation_view) { //update colliders
			auto& formationType = formation_view.get<Test::Unit_Formation_Data>(formation_ID).formationType;	


			switch (formationType) {
				//case Test::Formation_Type::soldier:	{	zone.emplace<Test::Soldier>(formation_ID); }; break;
				case Test::Formation_Type::squad:	{	zone.emplace<Test::Squad>(formation_ID); }; break;
				case Test::Formation_Type::platoon: { 	zone.emplace<Test::Platoon>(formation_ID); }; break;
				case Test::Formation_Type::company: { 	zone.emplace<Test::Company>(formation_ID); }; break;
				case Test::Formation_Type::battalion: { zone.emplace<Test::Battalion>(formation_ID); }; break;
				case Test::Formation_Type::brigade: { 	zone.emplace<Test::Brigade>(formation_ID); }; break;
				case Test::Formation_Type::division: { 	zone.emplace<Test::Division>(formation_ID); }; break;
				case Test::Formation_Type::corps: { 	zone.emplace<Test::Corps>(formation_ID); }; break;
				case Test::Formation_Type::army: { 		zone.emplace<Test::Army>(formation_ID); }; break;
			}
			zone.emplace<Test::Grouped>(formation_ID);

		
		}
	}
	

	void Update_Formation_Positions(entt::registry& zone) {
		Set_Component(zone);
		Update_Formation_Unit_Positions(zone);
		Update_Soldier_Formation_Rect(zone);
		Update_Formation_Rect(zone);
	}

}