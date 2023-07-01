#pragma once
#include "entt/entt.hpp"
#include "components.h"


namespace Test_Units {

	// "squad" type unit_formation has a Test::Soldiers component to hold the unit data
	// All other units do not have this component

	entt::entity Create_New_Formation(entt::registry& zone) {
		auto formation_ID = zone.create();
		auto& formation = zone.emplace<Test::Unit_Formation_Data>(formation_ID);
		formation.formation_ID = formation_ID;
		return formation_ID;
	}

	bool Is_ParentFormation_One_Level_Above(Test::Unit_Formation_Data &parentFormationData, Test::Unit_Formation_Data& subformationData) {
		int subformationType = (int)parentFormationData.formationType - 1;
		if (subformationType == (int)(subformationData.formationType)) {
			return true;
		}
		return false;
	}
	void Set_ParentFormation_Formation_Type(Test::Unit_Formation_Data &parentFormationData, Test::Unit_Formation_Data& subformationData) {
		int parentFormationType_int = (int)subformationData.formationType + 1;		
		parentFormationData.formationType = (Test::Formation_Type)parentFormationType_int;
	}

	bool Replace_Subformation_In_Formation(entt::registry& zone, entt::entity& parentFormation_ID, Test::Unit_Formation_Data& parentFormationData, entt::entity& subformation_ID, Test::Unit_Formation_Data & subformationData) {

		for (int i = 0; i < parentFormationData.subformationData.size(); i++) {
			if (parentFormationData.subformationData.at(i).bAlive == false) {
				zone.emplace_or_replace<Component::Assigned_To_Formation>(subformation_ID, i, parentFormation_ID);
				
				parentFormationData.subformationData.at(i) = subformationData;
				return true;
			}
		}
		return false;
	}
	bool Emplace_Subformation_In_Formation(entt::registry& zone, entt::entity& parentFormation_ID, Test::Unit_Formation_Data& parentFormationData, entt::entity& subformation_ID, Test::Unit_Formation_Data& subformationData) {
		
		if (parentFormationData.subformationData.size() < parentFormationData.size) {
			auto& subformation = zone.emplace_or_replace<Component::Assigned_To_Formation>(subformation_ID, 0, parentFormation_ID);
			subformation.iIndex = (int)parentFormationData.subformationData.size();
			parentFormationData.subformationData.emplace_back(subformationData);
			return true;
		}		
		return false;
	}

	bool Assign_Selected_Formations_To_ParentFormations(entt::registry& zone, entt::entity& subformation_ID, Test::Unit_Formation_Data& subformationData) {

		// get the selected formations => check which type they are => add them to the parent formation type

		auto parentFormations_view = zone.view<Test::Unit_Formation_Data>();

		//So that it doesn't place it within intself
		for (auto parentFormation_ID : parentFormations_view) {
			if (parentFormation_ID != subformation_ID) {
				Test::Unit_Formation_Data& parentFormationData = zone.get<Test::Unit_Formation_Data>(parentFormation_ID);
				if (Is_ParentFormation_One_Level_Above(parentFormationData, subformationData)) {
					if (Replace_Subformation_In_Formation(zone, parentFormation_ID, parentFormationData, subformation_ID, subformationData)) {
						return true;
					}
				}
			}
			//search exist formations for space and insert the subformation data
			for (auto parentFormation_ID : parentFormations_view) {
				if (parentFormation_ID != subformation_ID) {
					Test::Unit_Formation_Data& parentFormationData = zone.get<Test::Unit_Formation_Data>(parentFormation_ID);
					if (Is_ParentFormation_One_Level_Above(parentFormationData, subformationData)) {
						if (Emplace_Subformation_In_Formation(zone, parentFormation_ID, parentFormationData, subformation_ID, subformationData)) {
							return true;
						}
					}
				}
			}
		}
	

		//so it doesn't recursively grouop forever
		
		if (subformationData.formationType != Test::Formation_Type::army){
			//create a new formation and insert the subformation data
			entt::entity new_parentFormation_ID = Create_New_Formation(zone);
			//set the formation type one level above the subformation being inserted
			Test::Unit_Formation_Data& parentFormationData = zone.get<Test::Unit_Formation_Data>(new_parentFormation_ID);
			Set_ParentFormation_Formation_Type(parentFormationData, subformationData);
			//assign subformation to parent formation
			if (Emplace_Subformation_In_Formation(zone, new_parentFormation_ID, parentFormationData, subformation_ID, subformationData)) {
			
				return true;
			}
			return false;
		}
		return false;
	};

	void Create_Formation(entt::registry& zone) {
		auto subformations_view = zone.view<Test::Unit_Formation_Data>(entt::exclude<Component::Assigned_To_Formation, Test::Army>); // this should be only the selected formations I think
		for (auto subformation_ID : subformations_view) {
			auto & subformationData = subformations_view.get<Test::Unit_Formation_Data>(subformation_ID);
			Assign_Selected_Formations_To_ParentFormations(zone, subformation_ID, subformationData);
		}	
	}


	

	//void Create_And_Fill_New_Squad(entt::registry& zone) {

	//	auto view = zone.view<Component::Position, Component::Radius, Component::Soldier>(entt::exclude<Component::Assigned_To>);

	//	for (auto unit_ID : view) {
	//		auto& x = view.get<Component::Position>(unit_ID).x;
	//		auto& y = view.get<Component::Position>(unit_ID).y;
	//		auto& radius = view.get<Component::Radius>(unit_ID).fRadius;
	//		SDL_FRect rect = Utilities::Get_FRect_From_Point_Radius(radius, x, y);

	//		Test::Unit_Formation_Data soldierData = {unit_ID, 1, true, Test::Formation_Type::soldier, rect};

	//		Assign_Selected_Units_To_Squad(zone, soldierData);
	//	}
	//};




	//bool Assign_All_Units_To_Squad(entt::registry& zone, entt::entity& squad_ID) {
	//	Test::Soldiers_Assigned_List& squad = zone.get<Test::Soldiers_Assigned_List>(squad_ID);
	//	int iUnit = 0;

	//	auto view = zone.view<Component::Position, Component::Radius, Component::Mass, Component::Alive, Component::Soldier>(entt::exclude<Component::Assigned_To_Formation>);
	//	for (auto entity : view) {
	//		auto& x = view.get<Component::Position>(entity).x;
	//		auto& y = view.get<Component::Position>(entity).y;
	//		auto& m = view.get<Component::Mass>(entity).fKilos;
	//		auto& r = view.get<Component::Radius>(entity).fRadius;
	//		auto& alive = view.get<Component::Alive>(entity).bIsAlive;
	//		
	//		Test::Soldier_Data soldier_data = {entity, x, y, m, r, 0, alive};
	//		squad.unitData.emplace_back(soldier_data);
	//		
	//		auto& soldier = zone.emplace_or_replace<Component::Assigned_To_Formation>(entity, 0, squad_ID);
	//		soldier.iIndex = squad.unitData.size() - 1;

	//		iUnit++;
	//		if (iUnit >= squad.size) {
	//			return true;
	//		}
	//	}
	//	return false;
	//}
	//void Create_Squads_From_All_Unassigned(entt::registry& zone) {
	//	entt::entity squad_ID;
	//	bool units = true;

	//	while (units) {
	//		squad_ID = Create_New_Squad(zone);
	//		units = Assign_All_Units_To_Squad(zone, squad_ID);;
	//	}
	//}


	//void Assign_Soldiers_On_Spawn(entt::registry& zone) {
	//	Create_Squads_From_All_Unassigned(zone);		
	//}


	
}