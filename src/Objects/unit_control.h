#pragma once
#include "utilities.h"
#include "components.h"
#include "player_control.h"
#include "mouse_control.h"
#include "social_control.h"
#include "graphics.h"
#include "world.h"

namespace User_Mouse_Input {

  enum class Unit_Selection {
    soldiers,
    squads,
    platoons,
    companies,
    battalions
  };

  Unit_Selection eUnit_Selection;


  void Order_Move(entt::registry &zone, entt::entity &entity) {
    zone.emplace_or_replace<Component::Mouse_Move>(entity, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
    zone.emplace_or_replace<Component::Moving>(entity);
  }

  void Selection_Soldiers () {
    eUnit_Selection = Unit_Selection ::soldiers;
  }
	
  void Selection_Squads () {
    eUnit_Selection = Unit_Selection::squads;
  }
	
  void Selection_Platoons () {
    eUnit_Selection = Unit_Selection::platoons;
  }
		
  void Selection_Companies () {
    eUnit_Selection = Unit_Selection::companies;
  }
		
  void Selection_Battalions () {
    eUnit_Selection = Unit_Selection::battalions;
  }

  bool Select_Battalion(entt::registry &zone) {
    bool bSelected = false;
    auto platoon_view = zone.view<Component::Platoon>();
    for (auto platoons : platoon_view) {
      auto& platoon = platoon_view.get<Component::Platoon>(platoons);
      if (Mouse::Mouse_Selection_Box(platoon.sCollide_Box)) {
	zone.emplace_or_replace<Component::Selected>(platoons);
	bSelected = true;
      }
    }
    if (bSelected == false) {
      zone.clear<Component::Selected>();
    }
    return bSelected;
  }

  bool Select_Company(entt::registry& zone) {
    bool bSelected = false;
    auto platoon_view = zone.view<Component::Platoon>();
    for (auto platoons : platoon_view) {
      auto& platoon = platoon_view.get<Component::Platoon>(platoons);
      if (Mouse::Mouse_Selection_Box(platoon.sCollide_Box)) {
	zone.emplace_or_replace<Component::Selected>(platoons);
	bSelected = true;
      }
    }
    if (bSelected == false) {
      zone.clear<Component::Selected>();
    }
    return bSelected;
  }

  bool Select_Platoon(entt::registry& zone) {
    bool bSelected = false;
    auto platoon_view = zone.view<Component::Platoon>();
    for (auto platoons : platoon_view) {
      auto& platoon = platoon_view.get<Component::Platoon>(platoons);
      if (Mouse::Mouse_Selection_Box(platoon.sCollide_Box)) {
	zone.emplace_or_replace<Component::Selected>(platoons);
	bSelected = true;
      }
    }
    if (bSelected == false) {
      zone.clear<Component::Selected>();
    }
    return bSelected;
  }


  bool Select_Squad(entt::registry& zone) {
    bool bSelected = false;
    auto squad_view = zone.view<Component::Squad>();
    for (auto squads : squad_view) {
      auto& squad = squad_view.get<Component::Squad>(squads);
      if (Mouse::Mouse_Selection_Box(squad.sCollide_Box)) { 
	zone.emplace_or_replace<Component::Selected>(squads);
	bSelected = true;
      }
    }
    if (bSelected == false) {
      zone.clear<Component::Selected>();
    }
    return bSelected;
  }

  bool Tab_Target(entt::registry& zone) {
    bool bSelected = false;
    auto soldier_view = zone.view<Component::Soldier, Component::Commandable, Component::Interaction_Rect, Component::Renderable>();
    for (auto soldiers : soldier_view) {
      auto& interaction = soldier_view.get<Component::Interaction_Rect>(soldiers);
      if (Mouse::Mouse_Selection_Box(interaction.rect) || Mouse::bRect_inside_Cursor(interaction.rect)) {
	//                check for allies
	zone.emplace_or_replace<Component::Selected>(soldiers);
	//                if none, check for enemies, select only ONE enemy
	bSelected = true;
      }
    }
    if (bSelected == false) {
      zone.clear<Component::Selected>();
    }
    return bSelected;
  }

  bool Select_Soldier(entt::registry& zone, entt::entity player_ID) {
    bool bSelected = false;
    std::vector<entt::entity> selected;
    std::vector<entt::entity> selectedEnemy;

    auto soldier_view = zone.view<Component::Soldier, Component::Commandable, Component::Interaction_Rect, Component::Renderable>();
    for (auto soldier : soldier_view) {
      auto& interaction = soldier_view.get<Component::Interaction_Rect>(soldier);
      if (Mouse::Mouse_Selection_Box(interaction.rect) || Mouse::bRect_inside_Cursor(interaction.rect)) {
	if (soldier == player_ID) {
	  //                    prevent from selecting player
	  continue;
	}
	if (!Social_Control::Check_Relationship(zone, player_ID, soldier)) {
	  selected.emplace_back(soldier);
	}
	else {
	  selectedEnemy.emplace_back(soldier);
	}
	//                check for allies
	//                if none, check for enemies, select only ONE enemy
	bSelected = true;
      }
    }

    if (selected.size() > 0) {
      for (auto unit : selected) {
        auto &selectedComponent = zone.emplace_or_replace<Component::Selected>(unit);

        selectedComponent.texture = Graphics::selector32;
        selectedComponent.clip = {0,0,32,32};
        selectedComponent.numFrames = 8;
        selectedComponent.currentFrame = 0;
        selectedComponent.frameLength = 100;
        selectedComponent.currentFrameTime = 0;
      }
    }
    else if (selectedEnemy.size() > 0){
      auto &selectedComponent = zone.emplace_or_replace<Component::Selected>(selectedEnemy.at(0));

      selectedComponent.texture = Graphics::selector32;
      selectedComponent.clip = {0,0,32,32};
      selectedComponent.numFrames = 8;
      selectedComponent.currentFrame = 0;
      selectedComponent.frameLength = 100;
      selectedComponent.currentFrameTime = 0;      
    }

    if (bSelected == false) {
      zone.clear<Component::Selected>();
    }		
    return bSelected;
  }

  bool Tab_Target(entt::registry& zone, Component::Camera &camera, entt::entity player_ID) {
    bool bSelected = false;
    std::vector<entt::entity> selectedEnemy;

    auto soldier_view = zone.view<Component::Soldier, Component::Commandable, Component::Interaction_Rect, Component::Renderable>();
    auto &targetRange = zone.get<Component::Target_Range>(player_ID).rangeBox;
    SDL_FRect rect = {};

    //        if rangebox > screen rect cap to screen size
    if ((targetRange.w > camera.screen.w) || (targetRange.h > camera.screen.h)) {
      rect = camera.screen;
    }
    else {
      rect = targetRange;
    }

    for (auto soldier : soldier_view) {
      auto& interaction = soldier_view.get<Component::Interaction_Rect>(soldier);

      if (Utilities::bFRect_Intersect(rect, interaction.rect)) {
	if (soldier == player_ID) {
	  //                    prevent from selecting player
	  continue;
	}
	if (!Social_Control::Check_Relationship(zone, player_ID, soldier)) {
	  //                    prevent tab targeting allies
	  continue;
	}
	if (Social_Control::Check_Relationship(zone, player_ID, soldier)) {
	  if (zone.any_of<Component::Selected>(soldier)) {
	    continue;
	  }
	  else {
	    zone.clear<Component::Selected>();
	    selectedEnemy.emplace_back(soldier);
	    bSelected = true;
	    if (selectedEnemy.size() > 0) {
	      auto &selectedComponent = zone.emplace_or_replace<Component::Selected>(selectedEnemy.at(0));

	      selectedComponent.texture = Graphics::selector32;
	      selectedComponent.clip = {0,0,32,32};
	      selectedComponent.numFrames = 8;
	      selectedComponent.currentFrame = 0;
	      selectedComponent.frameLength = 100;
	      selectedComponent.currentFrameTime = 0;	      
	    }
	    return bSelected;
	  }
	}
      }
    }
    //
    //        if (bSelected == false) {
    //            zone.clear<Component::Selected>();
    //        }
    return bSelected;
  }


  void Select(entt::registry& zone) {
    Select_Platoon(zone);
    Select_Squad(zone);
    //		Select_Soldier(zone);
  }

  void Select_Units(entt::registry& zone, entt::entity player_ID) {
    //if unit.Soldier then select all in his squad
    if (zone.empty<Component::Selected>()) {
      switch (eUnit_Selection) {
      case Unit_Selection ::soldiers: Select_Soldier(zone, player_ID); break;
      case Unit_Selection ::squads: Select_Squad(zone); break;
      case Unit_Selection ::platoons: Select_Platoon(zone); break;
      case Unit_Selection ::companies: Select_Company(zone); break;
      case Unit_Selection ::battalions: Select_Battalion(zone); break;
      }
      Mouse::bLeft_Mouse_Pressed = false;
    }
    else {
      zone.clear<Component::Selected>();
      Mouse::bLeft_Mouse_Pressed = false;
    }
  }

  void Update_Move_Command_Box () {
    Mouse::bRight_Mouse_Pressed = true;
    Mouse::Mouse_Selection_Box_x = Mouse::iXWorld_Mouse;
    Mouse::Mouse_Selection_Box_y = Mouse::iYWorld_Mouse;
    Mouse::Mouse_Selection_Box_x_Display = Mouse::iXMouse;
    Mouse::Mouse_Selection_Box_y_Display = Mouse::iYMouse;
  }

  //take in an entity and set the units to save it and follow and attack it.
  bool Command_Unit_Attack(entt::registry& zone, entt::entity &player_ID, entt::entity &target_ID, Component::Radius &radius) {
    if (!zone.empty<Component::Selected>()) {
      auto view = zone.view<Component::Selected>();
      for (auto entity: view) {
	if (!Social_Control::Check_Relationship(zone, player_ID, entity)) {
	  Player_Control::Attack_Order(zone, entity, target_ID, radius);
	}
	else {
	  return false;
	}
      }
      Mouse::bRight_Mouse_Pressed = false;
      return true;
    }
    return false;
  }

  bool Command_Unit_Move(entt::registry& zone, entt::entity player_ID) {
    if (!zone.empty<Component::Selected>()) {
      float x = 0.0f;
      float y = Mouse::Mouse_Selection_Box_y;
      int i = 0;
      float spacing = 0;
      float z = abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse) / (50.0f));// z is the # of units that can fit along x
      auto view = zone.view<Component::Selected, Component::Soldier, Component::Commandable>();

      for (auto entity: view) {
	//            check if the selected unit is an aly
	if (!Social_Control::Check_Relationship(zone, player_ID, entity)) {
	  if (abs(Mouse::iXWorld_Mouse - Mouse::Mouse_Selection_Box_x) > 40.0f || abs(Mouse::iYWorld_Mouse - Mouse::Mouse_Selection_Box_y) > 40.0f) {
	    x = Mouse::Mouse_Selection_Box_x + spacing;
	    if (i == z) {
	      spacing = 0.0f;
	      x = Mouse::Mouse_Selection_Box_x + spacing;
	      y = y + 50.0f;
	      i = 0;
	    }
	    i++;
	    spacing = spacing + 50.0f; //spacing should be stored in "battalion" component

	    zone.emplace_or_replace<Component::Moving>(entity);
	    zone.emplace_or_replace<Component::Mouse_Move>(entity, x, y);

	  }
	  else { //moves all the units onto a single point, I want to have the spread out in some kind of formation
	    Order_Move(zone, entity);
	  }
	}
	else {
	  //                    if the selected is an enemy...
	  return false;
	}
      }
      Mouse::bRight_Mouse_Pressed = false;
      return true;
    }
    return false;
  }


  bool Command_Squad(entt::registry& zone) {
    if (!zone.empty<Component::Selected>()) {
      if (abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse)) > 50.0f) {
	auto view = zone.view<Component::Selected, Component::Squad>();
	float x = 0.0f;
	float y = Mouse::Mouse_Selection_Box_y;
	int i = 0;
	float spacing = 0.0f;
	float z = abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse) / (50.0f));// z is the # of units that can fit along x
	for (auto entity : view) {
	  auto& squad = view.get<Component::Squad>(entity);
	  for (int j = 0; j < squad.iSub_Units.size(); j++) {
	    x = Mouse::Mouse_Selection_Box_x + spacing;
	    if (i == z) {
	      spacing = 0.0f;
	      x = Mouse::Mouse_Selection_Box_x + spacing;
	      y = y + 50.0f;
	      i = 0;
	    }
	    i++;
	    spacing = spacing + 50; //spacing shoudl be stored in "battalion" component					

	    zone.emplace_or_replace<Component::Moving>(squad.iSub_Units[j]);
	    zone.emplace_or_replace<Component::Mouse_Move>(squad.iSub_Units[j], x, y);
	  }
	}
      }
      else { //moves all the units onto a single point, I want to have the spread out in some kind of formation
	auto squads_view = zone.view<Component::Selected, Component::Squad>();
	for (auto squads : squads_view) {
	  auto& squad = World::zone.get<Component::Squad>(squads);
	  for (int j = 0; j < squad.iSub_Units.size(); j++) {
	    zone.emplace_or_replace<Component::Moving>(squad.iSub_Units[j]);
	    zone.emplace_or_replace<Component::Mouse_Move>(squad.iSub_Units[j], Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
	  }
	}
      }
      Mouse::bRight_Mouse_Pressed = false;
      return true;
    }
    else {
      return false;
    }
  }

  void Command_Platoon(entt::registry& zone) {
    if (!zone.empty<Component::Selected>()) {
      if (abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse)) > 50.0f) {
	auto view = zone.view<Component::Selected, Component::Platoon>();
	float x = 0.0f;
	float y = Mouse::Mouse_Selection_Box_y;
	int i = 0;
	float spacing = 0.0f;
	float z = abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse) / (50.0f));// z is the # of units that can fit along x
	for (auto entity : view) {
	  auto& squad = view.get<Component::Platoon>(entity);
	  for (int j = 0; j < squad.iSub_Units.size(); j++) {
	    x = Mouse::Mouse_Selection_Box_x + spacing;
	    if (i == z) {
	      spacing = 0.0f;
	      x = Mouse::Mouse_Selection_Box_x + spacing;
	      y = y + 50.0f;
	      i = 0;
	    }
	    i++;
	    spacing = spacing + 50.0f; //spacing shoudl be stored in "battalion" component					

	    zone.emplace_or_replace<Component::Moving>(squad.iSub_Units[j]);
	    zone.emplace_or_replace<Component::Mouse_Move>(squad.iSub_Units[j], x, y);
	  }
	}
      }
      else { //moves all the units onto a single point, I want to have the spread out in some kind of formation
	auto squads_view = zone.view<Component::Selected, Component::Platoon>();
	for (auto squads : squads_view) {
	  auto& squad = World::zone.get<Component::Platoon>(squads);
	  for (int j = 0; j < squad.iSub_Units.size(); j++) {
	    zone.emplace_or_replace<Component::Moving>(squad.iSub_Units[j]);
	    zone.emplace_or_replace<Component::Mouse_Move>(squad.iSub_Units[j], Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
	  }
	}
      }
      Mouse::bRight_Mouse_Pressed = false;
    }
  }

  void Selection_Box(entt::registry& zone) {
    if (zone.empty<Component::Selected>()) {
      Mouse::bLeft_Mouse_Pressed = true;
      Mouse::Mouse_Selection_Box_x = Mouse::iXWorld_Mouse;
      Mouse::Mouse_Selection_Box_y = Mouse::iYWorld_Mouse;
      Mouse::Mouse_Selection_Box_x_Display = Mouse::iXMouse;
      Mouse::Mouse_Selection_Box_y_Display = Mouse::iYMouse;
    }
    else {
      zone.clear<Component::Selected>();
      Mouse::bLeft_Mouse_Pressed = true;
      Mouse::Mouse_Selection_Box_x = Mouse::iXWorld_Mouse;
      Mouse::Mouse_Selection_Box_y = Mouse::iYWorld_Mouse;
      Mouse::Mouse_Selection_Box_x_Display = Mouse::iXMouse;
      Mouse::Mouse_Selection_Box_y_Display = Mouse::iYMouse;
    }
  }


  void Select_Unit(entt::registry &zone) {
    if (!zone.empty<Component::Selected>()) {
      Mouse::bRight_Mouse_Pressed = true;
      Mouse::Mouse_Selection_Box_x = Mouse::iXWorld_Mouse;
      Mouse::Mouse_Selection_Box_y = Mouse::iYWorld_Mouse;
      Mouse::Mouse_Selection_Box_x_Display = Mouse::iXMouse;
      Mouse::Mouse_Selection_Box_y_Display = Mouse::iYMouse;
    }
  }

  //
  //
  //Unit Oraginzation
  //
	
  void Delete_Squad(entt::registry& zone) {
    auto view = zone.view<Component::Assigned_To_Formation, Component::Selected>();
    for (auto entity : view) {
      auto& sold = view.get<Component::Assigned_To_Formation>(entity);
			
      //auto &squad = scene.get<Squad>(sold.SquadAssgnedTo);
			
      //sold.SquadAssgnedTo
      //if (sold.SquadAssgnedTo.units[sold.index] == false) {
      //	scene.remove<Soldier>(entity);
      //};
      //for (int i = 0; i < Squads.size(); i++) {
      //if (sold.SquadAssgnedTo == Squads[i]) {
			
      //}
      //}
      //sold.SquadAssgnedTo->x.clear();
      //sold.SquadAssgnedTo->y.clear();		
    }
  };

  void Add_Soldiers_To_Squad(entt::registry& zone) {
    auto view = zone.view<Component::Position, Component::Selected, Component::Radius, Component::Mass, Component::Soldier>(entt::exclude<Component::Assigned_To_Formation>);
		
    int iUnit = 0;
		
    auto squad_ID = zone.create();
    auto& squad = zone.emplace<Component::Squad>(squad_ID);

    for (auto entity : view) {
      if (iUnit < squad.size) { // caps number of units per squad				
	auto& x = view.get<Component::Position>(entity);
	auto& y = view.get<Component::Position>(entity);
	auto& m = view.get<Component::Mass>(entity);
	auto& r = view.get<Component::Radius>(entity);
	auto& soldier = zone.emplace_or_replace<Component::Assigned_To_Formation>(entity, 0, squad_ID);
	const auto SOLDIER_ID = entt::to_entity(zone, soldier);
	const auto SQUAD_ID = entt::to_entity(zone, squad);
	squad.fPX.emplace_back(x.x);
	squad.fPY.emplace_back(y.y);
	squad.fMass.emplace_back(m.fKilos);
	squad.fRadius.emplace_back(r.fRadius);
	squad.iSub_Units.emplace_back(SOLDIER_ID);
	soldier.iIndex = (int)squad.iSub_Units.size() - 1;
	soldier.iUnit_Assigned_To = SQUAD_ID;
	iUnit++;
      }
    }
  };

  //assign squads to Platoons

  void Create_Companies(entt::registry& zone) {
    auto platoons_view = zone.view<Component::Platoon>(entt::exclude<Component::Assigned_To_Formation>);

    for (auto view : platoons_view) {
      int iUnit = 0;
      auto Company_ID = zone.create();
      auto& company = zone.emplace<Component::Company>(Company_ID);
      for (auto platoons : platoons_view) {
	if (iUnit < company.size) { // caps number of units per squad				
	  auto& platoon = platoons_view.get<Component::Platoon>(platoons);
	  const auto PLATOON_ID = entt::to_entity(zone, platoon);
	  const auto COMPANY_ID = entt::to_entity(zone, company);
	  auto& assigned = zone.emplace_or_replace<Component::Assigned_To_Formation>(platoons, 0, Company_ID);
	  //add squad ID to  to Platoon list
	  company.iSub_Units.emplace_back(PLATOON_ID);	//?? add Platoon ID to squad???
	  //add index to squad
	  assigned.iIndex = (int)company.iSub_Units.size() - 1;
	  //add squad collider to Platoon coords 
	  company.fPX.emplace_back(platoon.sCollide_Box.x);
	  company.fPY.emplace_back(platoon.sCollide_Box.y);
	  company.fPW.emplace_back(platoon.sCollide_Box.w);
	  company.fPH.emplace_back(platoon.sCollide_Box.h);
	  iUnit++;
	}
      }
    }
  };

  void Create_Platoons(entt::registry& zone) {
    auto squads_view = zone.view<Component::Squad>(entt::exclude<Component::Assigned_To_Formation>);

    for (auto entity2 : squads_view) {
      int iUnit = 0;
      auto Platoon_ID = zone.create();
      auto& platoon = zone.emplace<Component::Platoon>(Platoon_ID);
      for (auto squads : squads_view) {
	if (iUnit < platoon.size) { // caps number of units per squad				
	  auto& squad = squads_view.get<Component::Squad>(squads);
	  const auto SQUAD_ID = entt::to_entity(zone, squad);
	  const auto PLATOON_ID = entt::to_entity(zone, platoon);
	  auto& assigned = zone.emplace_or_replace<Component::Assigned_To_Formation>(squads, 0, PLATOON_ID);
	  //add squad ID to  to Platoon list
	  platoon.iSub_Units.emplace_back(SQUAD_ID);	//?? add Platoon ID to squad???
	  //add index to squad
	  assigned.iIndex = (int)platoon.iSub_Units.size() - 1;
	  //add squad collider to Platoon coords 
	  platoon.fPX.emplace_back(squad.sCollide_Box.x);
	  platoon.fPY.emplace_back(squad.sCollide_Box.y);
	  platoon.fPW.emplace_back(squad.sCollide_Box.w);
	  platoon.fPH.emplace_back(squad.sCollide_Box.h);
	  iUnit++;
	}
      }
    }
  };

  entt::entity Create_New_Squad(entt::registry &zone){		
    auto squad_ID = zone.create();
    auto& squad = zone.emplace<Component::Squad>(squad_ID);
    return squad_ID;
  }

  bool Replace_Umit_In_Squad (entt::registry& zone, entt::entity& squad_ID, entt::entity& entity, Component::Position& position, Component::Mass& mass, Component::Radius& radius) {
		
    Component::Squad& squad = zone.get<Component::Squad>(squad_ID);
    for (int i = 0; i < squad.bAlive.size(); i++) {
      if (squad.bAlive.at(i) == false) {
	auto& x = position.x;
	auto& y = position.y;
	auto& m = mass.fKilos;
	auto& r = radius.fRadius;

	auto& soldier = zone.emplace_or_replace<Component::Assigned_To_Formation>(entity, i, squad_ID);

	squad.fPX.at(i) = x;
	squad.fPY.at(i) = y;
	squad.fMass.at(i) = m;
	squad.fRadius.at(i) = r;
	squad.iSub_Units.at(i) = entity;
	squad.iStruck.at(i) = 0;
	squad.bAlive.at(i) = true;

	return true;
      }
    }
    return false;
  }

  bool Emplace_Umit_In_Squad (entt::registry& zone, entt::entity &squad_ID, entt::entity& entity, Component::Position &position, Component::Mass &mass, Component::Radius &radius) {
    Component::Squad& squad = zone.get<Component::Squad>(squad_ID);
		
    if (squad.bAlive.size() < squad.size) {
      auto& x = position.x;
      auto& y = position.y;
      auto& m = mass.fKilos;
      auto& r = radius.fRadius;
      auto& soldier = zone.emplace_or_replace<Component::Assigned_To_Formation>(entity, 0, squad_ID);

      squad.fPX.emplace_back(x);
      squad.fPY.emplace_back(y);
      squad.fMass.emplace_back(m);
      squad.fRadius.emplace_back(r);
      squad.iSub_Units.emplace_back(entity);
      squad.iStruck.emplace_back(0);
      squad.bAlive.emplace_back(true);

      soldier.iIndex = (int)squad.iSub_Units.size() - 1;
      return true;
    }
    return false;
  }

  bool Assign_Selected_Units_To_Squad(entt::registry& zone, entt::entity& entity, Component::Position& position, Component::Mass& mass, Component::Radius& radius) {
    //looks to see if any squads have empty slots and tries to fill it
    // if that finds notihng
    //will add a slot to a squad not yet full
    // if that fails
    ///creates a new  squad and adds the unit to it

    auto view = zone.view<Component::Squad>();
    for (auto squad_ID : view) {		
      if (Replace_Umit_In_Squad(zone, squad_ID, entity, position, mass, radius) == true) {
	return true;
      }
    }
    for (auto squad_ID : view) {	
      if (Emplace_Umit_In_Squad(zone, squad_ID, entity, position, mass, radius) == true) {
	return true;
      }
    }
    entt::entity new_squad_ID = Create_New_Squad(zone);
    Emplace_Umit_In_Squad(zone, new_squad_ID, entity, position, mass, radius);
    return true;
  }

  void Create_And_Fill_New_Squad(entt::registry& zone) {
		
    auto view = zone.view<Component::Position, Component::Radius, Component::Mass, Component::Soldier, Component::Selected>(entt::exclude<Component::Assigned_To_Formation>);
    for (auto unit_ID : view) {
      auto& position = view.get<Component::Position>(unit_ID);
      auto& mass = view.get<Component::Mass>(unit_ID);
      auto& radius = view.get<Component::Radius>(unit_ID);
      Assign_Selected_Units_To_Squad(zone, unit_ID, position, mass, radius);
    }
		
  };

  void Create_And_Fill_New_Platoon(entt::registry& zone) {

    auto view = zone.view<Component::Position, Component::Radius, Component::Mass, Component::Soldier>(entt::exclude<Component::Assigned_To_Formation>);
    for (auto unit_ID : view) {
      auto& position = view.get<Component::Position>(unit_ID);
      auto& mass = view.get<Component::Mass>(unit_ID);
      auto& radius = view.get<Component::Radius>(unit_ID);
      Assign_Selected_Units_To_Squad(zone, unit_ID, position, mass, radius);
    }

  };

  bool Assign_All_Units_To_Squad(entt::registry& zone, entt::entity& squad_ID) {
    Component::Squad& squad = zone.get<Component::Squad>(squad_ID);
    int iUnit = 0;

    auto view = zone.view<Component::Position, Component::Radius, Component::Mass, Component::Soldier>(entt::exclude<Component::Assigned_To_Formation>);
    for (auto entity : view) {

      auto& x = view.get<Component::Position>(entity).x;
      auto& y = view.get<Component::Position>(entity).y;
      auto& m = view.get<Component::Mass>(entity).fKilos;
      auto& r = view.get<Component::Radius>(entity).fRadius;
      auto& soldier = zone.emplace_or_replace<Component::Assigned_To_Formation>(entity, 0, squad_ID);

      squad.fPX.emplace_back(x);
      squad.fPY.emplace_back(y);
      squad.fMass.emplace_back(m);
      squad.fRadius.emplace_back(r);
      squad.iSub_Units.emplace_back(entity);
      squad.iStruck.emplace_back(0);
      squad.bAlive.emplace_back(true);

      soldier.iIndex = (int)squad.iSub_Units.size() - 1;

      iUnit++;
      if (iUnit >= squad.size) {
	return true;
      }
    }
    return false;
  }
  void Create_Squads_From_All_Unassigned(entt::registry &zone) {
    entt::entity squad_ID;
    bool units = true;

    while (units) {
		
      units = Assign_All_Units_To_Squad(zone, squad_ID);;
    }
  }

  void Assign_Soldiers_On_Spawn(entt::registry& zone) {
    Create_And_Fill_New_Platoon(zone);
    Create_Platoons(zone); 
    Create_Companies(zone);
  }

}
