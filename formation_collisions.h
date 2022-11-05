#pragma once
#include "entt/entt.hpp"
#include "components.h"

namespace Formation_Collision {
	
	int iActual_Resolves = 0;
	int iNumber = 0;
	int iunitGridCollisionCheck = 0;
	int stat = 0;

	int iArmy_Check = 0;
	int iCorps_Check = 0;
	int iDivision_Check = 0;
	int iBrigade_Check = 0;
	int iBattalion_Check = 0;
	int iCompany_Check = 0;
	int iPlatoon_Check = 0;
	int iSquad_Check = 0;
	int iSoldier_Check = 0;

	void Collisions_Text(entt::registry &zone) {
		if (1) {

			int iArmy = 0;
			int iCorps = 0;
			int iDivision = 0;
			int iBrigade = 0;
			int iBattalion = 0;
			int iCompany = 0;
			int iPlatoon = 0;
			int iSquad = 0;
			int iSoldier = 0;

			auto formation_view = zone.view<Test::Unit_Formation_Data>();
			for (auto formations : formation_view) {
				auto& formation = formation_view.get<Test::Unit_Formation_Data>(formations);
				switch (formation.formationType) {
				case Test::Formation_Type::squad: { iSquad++; }; break;
				case Test::Formation_Type::platoon: {iPlatoon++; }; break;
				case Test::Formation_Type::company: { iCompany++; }; break;
				case Test::Formation_Type::battalion: { iBattalion++; }; break;
				case Test::Formation_Type::brigade: { iBrigade++; }; break;
				case Test::Formation_Type::division: { iDivision++; }; break;
				case Test::Formation_Type::corps: { iCorps++; }; break;
				case Test::Formation_Type::army: { iArmy++; }; break;
				}
				
			}

			auto soldierList_view = zone.view<Test::Soldiers_Assigned_List>();
			for (auto list : soldierList_view) {
				auto& soldierList = soldierList_view.get<Test::Soldiers_Assigned_List>(list);
				iSoldier = iSoldier + (int)soldierList.unitData.size();
			}
			//system("CLS");
			std::cout << "Armies:       " << iArmy << "   checks:   " << iArmy_Check << std::endl;
			std::cout << "Corps:        " << iCorps << "   checks:   " << iCorps_Check << std::endl;
			std::cout << "Divisions:    " << iDivision << "   checks:   " << iDivision_Check << std::endl;
			std::cout << "Brigades:     " << iBrigade << "   checks:   " << iBrigade_Check << std::endl;
			std::cout << "Battalions:   " << iBattalion << "   checks:   " << iBattalion_Check << std::endl;
			std::cout << "Companies:    " << iCompany << "   checks:   " << iCompany_Check << std::endl;
			std::cout << "Platoons:     " << iPlatoon << "   checks:   " << iPlatoon_Check << std::endl;
			std::cout << "Squads:       " << iSquad << "   checks:   " << iSquad_Check << std::endl;
			std::cout << "Soldiers:     " << iSoldier << "   checks:   " << iSoldier_Check << std::endl;
			std::cout << "Resolves:     " << iActual_Resolves << std::endl;
			std::cout << "----------   " <<  std::endl;
			//std::cout << "iunitGridCollisionCheck:     " << iunitGridCollisionCheck << std::endl;
			std::cout << "----------   " <<  std::endl;
			
			iArmy_Check = 0;
			iCorps_Check = 0;
			iDivision_Check = 0;
			iBrigade_Check = 0;
			iBattalion_Check = 0;
			iCompany_Check = 0;
			iPlatoon_Check = 0;
			iSquad_Check = 0;
			iSoldier_Check = 0;

			iActual_Resolves = 0;
		}
	}

	void Add_Or_Update_Collidee(entt::registry& zone, entt::entity& entity, Component::Position& position, float& potentialX, float& potentialY, Component::Entity_Interaction& type) {
		if (zone.any_of<Component::Collided>(entity)) {
			auto& interaction = zone.get<Component::Collided>(entity);
			interaction.x += potentialX; // * normalized mass
			interaction.y += potentialY;
			interaction.InteractionList.emplace_back(type);
		}
		else {
			auto& interaction = zone.emplace<Component::Collided>(entity);
			interaction = { position.x += potentialX, position.y += potentialY };
			interaction.InteractionList.emplace_back(type);
		}
	}
	void Add_Or_Update_Collider(entt::registry& zone, entt::entity& entity, Component::Position& position, float& potentialX, float& potentialY, Component::Entity_Interaction& type) {
		if (zone.any_of<Component::Collided>(entity)) {
			auto& interaction = zone.get<Component::Collided>(entity);
			interaction.x -= potentialX; // * normalized mass
			interaction.y -= potentialY;
			interaction.InteractionList.emplace_back(type);
		}
		else {
			auto& interaction = zone.emplace<Component::Collided>(entity);
			interaction = { position.x -= potentialX, position.y -= potentialY };
			interaction.InteractionList.emplace_back(type);
		}
	}

	void Unit_Intersect(entt::registry &zone, Test::Soldier_Data& soldier1, Test::Soldier_Data & soldier2) {
								
		if (soldier1.bAlive == true && soldier2.bAlive == true) {
			if (soldier1.unit_ID != soldier2.unit_ID) {
	
			float fx = soldier1.x - soldier2.x;
			float fy = soldier1.y - soldier2.y;
			float fDistance = (fx * fx) + (fy * fy);
																		
				if (fDistance <= ((soldier1.radius + soldier2.radius) * (soldier1.radius + soldier2.radius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
					fDistance = sqrtf(fDistance);
					float fOverlap = fDistance - (soldier1.radius + soldier2.radius);
					DataTypes::f2d resolver = {};
					resolver.x = fOverlap * (soldier2.x - soldier1.x) / fDistance;
					resolver.y = fOverlap * (soldier2.y - soldier1.y) / fDistance;
					float fTotalmass = soldier1.mass + soldier2.mass;
					float fNomalizedMassA = (soldier1.mass / fTotalmass);
					float fNomalizedMassB = (soldier2.mass / fTotalmass);



                    Component::Entity_Interaction interaction1 = {  soldier1.unit_ID, Component::Entity_Type::unit };
					Component::Position position = { soldier2.x, soldier2.y };
					float resolverX = (resolver.x * fNomalizedMassA);
					float resolverY = (resolver.y * fNomalizedMassA);
					Add_Or_Update_Collider(zone, soldier2.unit_ID, position, resolverX, resolverY, interaction1);

                    Component::Entity_Interaction interaction2 = { soldier2.unit_ID, Component::Entity_Type::unit };
					position = { soldier1.x, soldier1.y };
					resolverX = (resolver.x * fNomalizedMassB); // * normalized mass
					resolverY = (resolver.y * fNomalizedMassB);
					Add_Or_Update_Collidee(zone, soldier1.unit_ID, position, resolverX, resolverY, interaction2);
					iActual_Resolves++;
				}
			}
		}
	}
	void Formation_vs_Formation_Collision(entt::registry& zone) {

		Test::Unit_Formation_Data armyData_1;
		Test::Unit_Formation_Data armyData_2;
		Test::Unit_Formation_Data corpData_1;
		Test::Unit_Formation_Data corpData_2;
		Test::Unit_Formation_Data divisionData_1;
		Test::Unit_Formation_Data divisionData_2;
		Test::Unit_Formation_Data brigadeData_1;
		Test::Unit_Formation_Data brigadeData_2;
		Test::Unit_Formation_Data battalionData_1;
		Test::Unit_Formation_Data battalionData_2;
		Test::Unit_Formation_Data companyData_1;
		Test::Unit_Formation_Data companyData_2;
		Test::Unit_Formation_Data platoonData_1;
		Test::Unit_Formation_Data platoonData_2;
		Test::Unit_Formation_Data squadData_1;
		Test::Unit_Formation_Data squadData_2;




		auto army_view = zone.view<Test::Army, Test::Unit_Formation_Data>();
			
		for (auto armies : army_view) {
			auto& army_1 = army_view.get<Test::Unit_Formation_Data>(armies);
			for (auto armies : army_view) {
				auto& army_2 = army_view.get<Test::Unit_Formation_Data>(armies);
				
				
				
				if (Utilities::bFRect_Intersect(army_1.sCollide_Box, army_2.sCollide_Box)) {
					iArmy_Check++;
					for (int corp_1 = 0; corp_1 < army_1.subformationData.size(); corp_1++) {
						for (int corp_2 = 0; corp_2 < army_2.subformationData.size(); corp_2++) {
							corpData_1 = army_1.subformationData[corp_1];
							corpData_2 = army_2.subformationData[corp_2];
							
							
							if (Utilities::bFRect_Intersect(corpData_1.sCollide_Box, corpData_2.sCollide_Box)) {
								iCorps_Check++;
								for (int division_1 = 0; division_1 < corpData_1.subformationData.size(); division_1++) {
									for (int division_2 = 0; division_2 < corpData_2.subformationData.size(); division_2++) {
										divisionData_1 = corpData_1.subformationData[division_1];
										divisionData_2 = corpData_1.subformationData[division_2];
										
										
										if (Utilities::bFRect_Intersect(divisionData_1.sCollide_Box, divisionData_2.sCollide_Box)) {
											iDivision_Check++;
											for (int brigade_1 = 0; brigade_1 < divisionData_1.subformationData.size(); brigade_1++) {
												for (int brigade_2 = 0; brigade_2 < divisionData_2.subformationData.size(); brigade_2++) {
													brigadeData_1 = divisionData_1.subformationData[division_1];
													brigadeData_2 = divisionData_2.subformationData[division_2];
													
													
													if (Utilities::bFRect_Intersect(brigadeData_1.sCollide_Box, brigadeData_2.sCollide_Box)) {
														iBrigade_Check++;
														for (int battalion_1 = 0; battalion_1 < brigadeData_1.subformationData.size(); battalion_1++) {
															for (int battalion_2 = 0; battalion_2 < brigadeData_2.subformationData.size(); battalion_2++) {
																battalionData_1 = brigadeData_1.subformationData[battalion_1];
																battalionData_2 = brigadeData_2.subformationData[battalion_2];
																
																
																if (Utilities::bFRect_Intersect(battalionData_1.sCollide_Box, battalionData_2.sCollide_Box)) {	
																	iBattalion_Check++;
																	for (int company_1 = 0; company_1 < battalionData_1.subformationData.size(); company_1++) {
																		for (int company_2 = 0; company_2 < battalionData_2.subformationData.size(); company_2++) {
																			companyData_1 = battalionData_1.subformationData[company_1];
																			companyData_2 = battalionData_2.subformationData[company_2];
																			
																			
																			if (Utilities::bFRect_Intersect(companyData_1.sCollide_Box, companyData_2.sCollide_Box)) {
																				iCompany_Check++;
																				for (int platoon_1 = 0; platoon_1 < companyData_1.subformationData.size(); platoon_1++) {
																					for (int platoon_2 = 0; platoon_2 < companyData_2.subformationData.size(); platoon_2++) {
																						platoonData_1 = companyData_1.subformationData[platoon_1];
																						platoonData_2 = companyData_2.subformationData[platoon_2];
																						
																						
																						if (Utilities::bFRect_Intersect(platoonData_1.sCollide_Box, platoonData_2.sCollide_Box)) {
																							iPlatoon_Check++;
																							for (int squad_1 = 0; squad_1 < platoonData_1.subformationData.size(); squad_1++) {
																								for (int squad_2 = 0; squad_2 < platoonData_2.subformationData.size(); squad_2++) {
																									squadData_1 = platoonData_1.subformationData[squad_1];
																									squadData_2 = platoonData_2.subformationData[squad_2];


																									if (Utilities::bFRect_Intersect(squadData_1.sCollide_Box, squadData_2.sCollide_Box)) {
																										iSquad_Check++;
																										auto& soldiersList_1 = zone.get<Test::Soldiers_Assigned_List>(squadData_1.formation_ID);
																										auto& soldiersList_2 = zone.get<Test::Soldiers_Assigned_List>(squadData_2.formation_ID);
																										for (int soldier_1 = 0; soldier_1 < soldiersList_1.unitData.size(); soldier_1++) {
																											for (int soldier_2 = 0; soldier_2 < soldiersList_2.unitData.size(); soldier_2++) {
																												iSoldier_Check++;
																												Test::Soldier_Data soldier1 = soldiersList_1.unitData[soldier_1];
																												Test::Soldier_Data soldier2 = soldiersList_2.unitData[soldier_2];

																												Unit_Intersect(zone, soldier1, soldier2);
																											}
																										}
																									}
																								}
																							}
																						}
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	void Entity_Unit_Intersect(entt::registry& zone, Test::Soldier_Data& soldier, Test::Soldier_Data &entity, Component::Entity_Type &type) {
		if (soldier.bAlive == true && entity.bAlive == true) {
			if (soldier.unit_ID != entity.unit_ID) {

				float fx = soldier.x - entity.x;
				float fy = soldier.y - entity.y;
				float fDistance = (fx * fx) + (fy * fy);

				if (fDistance <= ((soldier.radius + entity.radius) * (soldier.radius + entity.radius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
					fDistance = (float)sqrtf(fDistance);
					if (fDistance == 0.0f) { fDistance = 0.00001f; }
					float fOverlap = fDistance - (soldier.radius + entity.radius);
					DataTypes::f2d resolver = {};
					resolver.x = fOverlap * (entity.x - soldier.x) / fDistance;
					resolver.y = fOverlap * (entity.y - soldier.y) / fDistance;
					float fTotalmass = soldier.mass + entity.mass;
					float fNomalizedMassA = (soldier.mass / fTotalmass);
					float fNomalizedMassB = (entity.mass / fTotalmass);

					if (type == Component::Entity_Type::melee) {
						std::cout << "weapon" << std::endl;
					}

                    Component::Entity_Interaction interaction1 = { soldier.unit_ID, Component::Entity_Type::unit };
					Component::Position position = { entity.x, entity.y };
					float resolverX = (resolver.x * fNomalizedMassA);
					float resolverY = (resolver.y * fNomalizedMassA);
					Add_Or_Update_Collider(zone, entity.unit_ID, position, resolverX, resolverY, interaction1);


                    Component::Entity_Interaction interaction2 = { entity.unit_ID, type };
					position = { soldier.x, soldier.y };
					resolverX = (resolver.x * fNomalizedMassB); // * normalized mass
					resolverY = (resolver.y * fNomalizedMassB);
					Add_Or_Update_Collidee(zone, soldier.unit_ID, position, resolverX, resolverY, interaction2);
					iActual_Resolves++;
				}
			}
		}
	
	}
	void Entity_vs_Formation_Collision(entt::registry &zone) {

		Test::Unit_Formation_Data armyData;
		Test::Unit_Formation_Data corpData;
		Test::Unit_Formation_Data divisionData;
		Test::Unit_Formation_Data brigadeData;
		Test::Unit_Formation_Data battalionData;
		Test::Unit_Formation_Data companyData;
		Test::Unit_Formation_Data platoonData;
		Test::Unit_Formation_Data squadData;
		
		auto view = zone.view<Component::Radius, Component::Position, Component::Mass, Component::Alive, Component::Entity_Type>(entt::exclude<Component::Assigned_To_Formation>);
		auto army_view = zone.view<Test::Army, Test::Unit_Formation_Data>();
		auto soldiers_view = zone.view<Test::Soldiers_Assigned_List>();
		
		
		for (auto entity_ID : view) {
			auto& radius = view.get<Component::Radius>(entity_ID).fRadius;
			auto& position = view.get<Component::Position>(entity_ID);
			auto& mass = view.get<Component::Mass>(entity_ID).fKilos;
			auto& alive = view.get<Component::Alive>(entity_ID).bIsAlive;
			auto& type = view.get<Component::Entity_Type>(entity_ID);
			SDL_FRect entityColliderRect = Utilities::Get_FRect_From_Point_Radius(radius, position.x, position.y);

			for (auto armies : army_view) {
				armyData = army_view.get<Test::Unit_Formation_Data>(armies);
				if (Utilities::bFRect_Intersect(armyData.sCollide_Box, entityColliderRect)) {
					for (int corps = 0; corps < armyData.subformationData.size(); corps++) {
						corpData = armyData.subformationData[corps];
					
						if (Utilities::bFRect_Intersect(corpData.sCollide_Box, entityColliderRect)) {
							for (int division = 0; division < corpData.subformationData.size(); division++) {
							divisionData = corpData.subformationData[division];
								
								if (Utilities::bFRect_Intersect(divisionData.sCollide_Box, entityColliderRect)) {
									for (int brigade = 0; brigade < divisionData.subformationData.size(); brigade++) {
									brigadeData = divisionData.subformationData[brigade];
										
										if (Utilities::bFRect_Intersect(brigadeData.sCollide_Box, entityColliderRect)) {
											for (int battalion = 0; battalion < brigadeData.subformationData.size(); battalion++) {
												battalionData = brigadeData.subformationData[battalion];
												
												if (Utilities::bFRect_Intersect(battalionData.sCollide_Box, entityColliderRect)) {
													for (int company = 0; company < battalionData.subformationData.size(); company++) {
														companyData = battalionData.subformationData[company];

														if (Utilities::bFRect_Intersect(companyData.sCollide_Box, entityColliderRect)) {
															for (int platoon = 0; platoon < companyData.subformationData.size(); platoon++) {
																platoonData = companyData.subformationData[platoon];

																if (Utilities::bFRect_Intersect(platoonData.sCollide_Box, entityColliderRect)) {
																	for (int squad = 0; squad < platoonData.subformationData.size(); squad++) {
																		squadData = platoonData.subformationData[squad];

																		if (Utilities::bFRect_Intersect(squadData.sCollide_Box, entityColliderRect)) {
																			auto& soldiersList = soldiers_view.get<Test::Soldiers_Assigned_List>(squadData.formation_ID);
																			for (int soldier = 0; soldier < soldiersList.unitData.size(); soldier++) {
																				Test::Soldier_Data unitData = soldiersList.unitData[soldier];
																				Test::Soldier_Data entity = { entity_ID, position.x, position.y, mass, radius, alive };

																				Entity_Unit_Intersect(zone, unitData, entity, type);
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}				
			}				
		}	
	}






	void Test_Collision(entt::registry &zone) {
		Formation_vs_Formation_Collision(zone);
		Entity_vs_Formation_Collision(zone);
		//CollisionsT(zone);
	}



	//void Formation_vs_Grid_collision(entt::registry& zone, Map::Node3& map) { //waaaaay too slow		

	//	Formation_Data_Single armyData;
	//	Formation_Data_Single corpData;
	//	Formation_Data_Single divisionData;
	//	Formation_Data_Single brigadeData;
	//	Formation_Data_Single battalionData;
	//	Formation_Data_Single companyData;
	//	Formation_Data_Single platoonData;
	//	Formation_Data_Single squadData;


	//	auto army_view = zone.view<Test::Army, Test::Unit_Formation_Data>();

	//	for (auto armies : army_view) {
	//		auto& army = army_view.get<Test::Unit_Formation_Data>(armies);
	//		armyData = { army.sCollide_Box, army.subformationData };
	//		if (Utilities::bFRect_Intersect(army.sCollide_Box, map.sCollide_Box)) {
	//			corpData = Get_Subformation(armyData);
	//			for (int b = 0; b < corpData.size(); b++) {
	//				for (int i = 0; i < Map::size; i++) {
	//					for (int j = 0; j < Map::size; j++) {
	//						if (Utilities::bFRect_Intersect(company.sCollide_Box, map.nodes[i].nodes[j].sCollide_Box)) {
	//							auto& platoon = zone.get<Platoon>(company.iSub_Units[b]);
	//							for (int c = 0; c < platoon.iSub_Units.size(); c++) {
	//								for (int k = 0; k < Map::size; k++) {

	//									if (Utilities::bFRect_Intersect(platoon.sCollide_Box, map.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
	//										auto& squad = zone.get<Squad>(platoon.iSub_Units[c]);
	//										for (int l = 0; l < Map::size; l++) {

	//											if (Utilities::bFRect_Intersect(squad.sCollide_Box, map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
	//												for (int d = 0; d < squad.iSub_Units.size(); d++) {
	//													if (squad.bAlive.at(d) != false) {
	//														for (int m = 0; m < map.nodes[i].nodes[j].nodes[k].cells[l].entities.size(); m++) {
	//															auto& radius = zone.get<Radius>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
	//															auto& mass = zone.get<Mass>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
	//															auto& x = zone.get<Component::Position>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
	//															auto& y = zone.get<Component::Position>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
	//															for (int e = 0; e < squad.iSub_Units.size(); e++) {
	//																float fx = squad.fPX.at(e) - x.x;
	//																float fy = squad.fPY.at(e) - y.y;
	//																float fDistance = (fx * fx) + (fy * fy);
	//																if (fDistance <= ((squad.fRadius.at(e) + radius.fRadius) * (squad.fRadius.at(e) + radius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
	//																	fDistance = sqrtf(fDistance);
	//																	float fOverlap = fDistance - (squad.fRadius.at(e) + radius.fRadius);
	//																	DataTypes::f2d resolver = {};
	//																	resolver.fX = fOverlap * (x.x - squad.fPX.at(e)) / fDistance;
	//																	resolver.fY = fOverlap * (y.y - squad.fPY.at(e)) / fDistance;
	//																	float fTotalmass = squad.fMass.at(e) + mass.fKilos;
	//																	float fNomalizedMassA = (squad.fMass.at(e) / fTotalmass);
	//																	float fNomalizedMassB = (mass.fKilos / fTotalmass);
	//																	squad.fPX.at(e) += (resolver.fX * fNomalizedMassB); // * normalized mass
	//																	x.x -= (resolver.fX * fNomalizedMassA);
	//																	squad.fPY.at(e) += (resolver.fY * fNomalizedMassB);
	//																	y.y -= (resolver.fY * fNomalizedMassA);
	//																}
	//															}
	//														}
	//													}
	//												}
	//											}
	//										}
	//									}
	//								}
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}


}