#pragma once
#include <SDL2/SDL.h>
#include "sstream"


namespace Item_Component {

	enum class Item_Type {
		neck,
		shoulders,
		gloves,
		belt,
		boots,
		legs,
		chest,
		helm,
		weapon,
		shield
	};

    entt::entity emptyEquipSlot;

    struct Equipment {
        std::map<Item_Type, entt::entity>equippedItems{
                { Item_Type::helm, emptyEquipSlot },
                { Item_Type::neck, emptyEquipSlot },
                { Item_Type::shoulders, emptyEquipSlot },
                { Item_Type::chest, emptyEquipSlot },
                { Item_Type::weapon, emptyEquipSlot },
                { Item_Type::gloves, emptyEquipSlot },
                { Item_Type::belt, emptyEquipSlot },
                { Item_Type::legs, emptyEquipSlot },
                { Item_Type::boots, emptyEquipSlot },
                { Item_Type::shield, emptyEquipSlot }
        };
    };


    enum class Armor_Type {
		cloth,
		padded,
		leather,
		mail,
		plate
	};

	enum class Weapon_Type {
		sword,
        axe,
		mace,
        spear
	};

    enum class Weapon_Material {
        copper,
        bronze,
        iron,
    };

	struct Ground_Item {
		SDL_FRect box = {};
        SDL_FRect ground_name = {};
	};

	struct Name {
		std::string name;
	};

	enum class Rarity {
		common,		//white
		magic,		//blue
		rare,		//yellow
		unique		//gold
	};

	struct Item_Equip {

	};

	struct Weapon_Damage {
		int min;
		int max;
	};


	struct statRange {
		int min;
		int max;
	};

	//This is the order that it will show the stats on the item
	enum class Stat {
		armor,
		damage,
		spellDamage,
		health,
		piety,
		attackSpeed
	};

	struct statValue {
		Stat name;
		int value;
	};

	struct Item_Stats {;
		std::map<Stat, int>stats = {

		};
	};
}

