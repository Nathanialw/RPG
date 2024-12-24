//
// Created by nathanial on 12/22/24.
//
#pragma once
#include "entt/entt.hpp"
#include "base_structs.h"
#include "Toolbar/Skills/skills.h"
#include "skills_Components.h"

namespace Skill {
    namespace Shop {

	//keep the enum on the unit as a component
	enum class Friend_Unit_Type {
	    mundane_item_merchant,
	    magic_item_merchant,
	    potion_merchant,
	    scroll_merchant,
	    jewellery_merchant,
	    wand_merchant,
	    spell_merchant,
	    weapon_merchant,
	    armour_merchant,
	    smuggler,
	    trainer,
	    taskmaster,
	    repair,
	    cleric,

	    knight,
	    damsel,
	    SIZE
	};

	constexpr int SIZE = 6;

	typedef bool (*Action)();

	bool PLACEHOLDER() {
	    return false;
	}

	std::array<std::pair<int, int>, SIZE> Get_Skills(entt::registry &zone) {
	    std::array<std::pair<int, int>, SIZE> currentSkills = {};

	    auto view = zone.view<Component::Input, Skill_Component::Skills_Points,
		    Skill_Component::Armour_Smithing,
		    Skill_Component::Armour_Use,
		    Skill_Component::Blocking,
		    Skill_Component::Bludgeoning,
		    Skill_Component::Chopping,
		    Skill_Component::Combat>();

	    for (auto entity: view) {
		auto &armourSmithing = view.get<Skill_Component::Armour_Smithing>(entity);
		auto &armourUse = view.get<Skill_Component::Armour_Use>(entity);
		auto &blocking = view.get<Skill_Component::Blocking>(entity);
		auto &bludgeoning = view.get<Skill_Component::Bludgeoning>(entity);
		auto &chopping = view.get<Skill_Component::Chopping>(entity);
		auto &combat = view.get<Skill_Component::Combat>(entity);
		currentSkills[0] = {armourSmithing.level, armourSmithing.maxLevel};
		currentSkills[1] = {armourUse.level, armourUse.maxLevel};
		currentSkills[2] = {blocking.level, blocking.maxLevel};
		currentSkills[3] = {bludgeoning.level, bludgeoning.maxLevel};
		currentSkills[4] = {chopping.level, chopping.maxLevel};
		currentSkills[5] = {combat.level, combat.maxLevel};
	    }
	    return currentSkills;
	}

	Skill_Values Get_Skill_Points(entt::registry &zone) {
	    auto view = zone.view<Component::Input, Skill_Component::Skills_Points, Component::Stats>();
	    for (auto entity: view) {
		auto skillPoints = view.get<Skill_Component::Skills_Points>(entity);
		auto stats = view.get<Component::Stats>(entity);
		return {skillPoints.unspent, skillPoints.warriorCost, skillPoints.warriorMax, stats.strength};
	    }
	}

	std::array<std::string, SIZE> names = {
		"Armour Smithing",
		"Armour Use",
		"Blocking",
		"Bludgeoning",
		"Chopping",
		"Combat",
	};

	std::array<std::string, SIZE> icons = {
		"armoursmithingicon",
		"armouruseicon",
		"blockingicon",
		"bludgeoningicon",
		"choppingicon",
		"combaticon",
	};

	std::array<std::array<std::string, 5>, SIZE> descriptions = {
		{{" has no effect.", " grants a 25%% chance to create a new non-magical Armour with each new Dungeon map you enter.", " grants a 50%% chance, instead.", " grants a 75%% chance, instead.", "The type of Armour created will be randomly chosen but will correspond in strength to the Dungeon Level you've entered."},
		 {" has no effect.", " allows you to equip Light Armour", " allows you to equip Medium Armour", " allows you to equip Heavy Armour", ""},
		 {" has no effect.", " increases the Defence bonus of Shields you use by 50%%.", " increases the bonus by 100%%, instead.", " increases the bonus by 150%%, instead.", ""},
		 {" has no effect.", " adds 3 to the damage you do while using any Blunt Weapon.", " also gives you +35 to hit while using any Blunt Weapon.", " also adds 25%% to the weapon damage done by any Blunt Weapon.", "Bludgeoning may also stand in for Weapon Use when equipping Blunt Weapons, so long as you Class can attain an equivalent Weapon Use Skill level."},
		 {" has no effect.", " adds 3 to the damage you do while using any Axe.", " also gives you +35 to hit while using any Axe.", " also adds 25%% to the weapon damage done by any Axe.", "Chopping may also stand in for Weapon Use when equipping Axes, so long as you Class can attain an equivalent Weapon Use Skill level."},
		 {" has no effect.", " gives you +15 to hit.", " gives you +30 to hit, instead.", " gives you +45 to hit, instead.", ""}}
	};

	Skill_Tree <SIZE, Action> shop = Skill_Tree<SIZE, Action>(names, icons, descriptions, "Shop");

	void Update(entt::registry &zone, f2 scale) {
	    shop.Update(scale, Get_Skills(zone), Get_Skill_Points(zone));
	}

	bool Mouse_Inside() {
	    return shop.Mouse_Inside();
	}

	void Hover_Highlight() {
	    shop.Mouse_Over();
	}

	bool Click(entt::registry &zone, entt::entity &entity) {
	    //in Event Manager check for click on skill up button
	    //get index of skill
	    int i = shop.Click();
	    if (i == -1)
		return false;

	    auto &skills = zone.get<Skill_Component::Skills_Points>(entity);
	    bool clicked = false;

	    if (skills.unspent < skills.warriorCost)
		return false;

	    switch (i) {
		case 0:
		    clicked = Increase_Skill(zone.get<Skill_Component::Armour_Smithing>(entity), zone.get<Component::Stats>(entity).strength);
		    break;
		case 1:
		    clicked = Increase_Skill(zone.get<Skill_Component::Armour_Use>(entity), zone.get<Component::Stats>(entity).strength);
		    break;
		case 2:
		    clicked = Increase_Skill(zone.get<Skill_Component::Blocking>(entity), zone.get<Component::Stats>(entity).strength);
		    break;
		case 3:
		    clicked = Increase_Skill(zone.get<Skill_Component::Bludgeoning>(entity), zone.get<Component::Stats>(entity).strength);
		    break;
		case 4:
		    clicked = Increase_Skill(zone.get<Skill_Component::Chopping>(entity), zone.get<Component::Stats>(entity).strength);
		    break;
		case 5:
		    clicked = Increase_Skill(zone.get<Skill_Component::Combat>(entity), zone.get<Component::Stats>(entity).strength);
		    break;
		default:
		    break;
	    }

	    if (clicked)
		skills.unspent -= skills.warriorCost;

	    return clicked;
	};

	void Render(const f2 &scale) {
	    shop.Draw(scale);
	}

	bool Toggle(const Toggle_Type toggleType = Toggle_Type::toggle) {
	    return shop.Toggle(toggleType);
	}

	void Close() {
	    return shop.Close();
	}
    }
}