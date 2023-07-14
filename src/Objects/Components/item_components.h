#pragma once
#include <SDL2/SDL.h>
#include <sstream>
#include "map"



namespace Item_Component {

//    item 0 renders first
	enum class Item_Type {
        amulet,
        legs,
        belt,
        wrist,
        boots,
        kilt,
        clothes,
        chest,
        hair,
        gloves,
        ring,
        face,
        helm,
        crown,
        hood,
        shins,
        back,
		shoulders,
        jewelry,
        dirt,
        ranged,
        mainhand,
        offhand
	};

    struct Item_Pickup {

    };

    struct Equip_Items_Type{
    };

    entt::entity emptyEquipSlot;

    struct Equipment {
        std::string type = "none";
        std::map<Item_Type, entt::entity>equippedItems{
            { Item_Type::amulet, emptyEquipSlot },
            { Item_Type::back, emptyEquipSlot },
            { Item_Type::face, emptyEquipSlot },
            { Item_Type::belt, emptyEquipSlot },
            { Item_Type::wrist, emptyEquipSlot },
            { Item_Type::boots, emptyEquipSlot },
            { Item_Type::crown, emptyEquipSlot },
            { Item_Type::dirt, emptyEquipSlot },
            { Item_Type::helm, emptyEquipSlot },
            { Item_Type::hood, emptyEquipSlot },
            { Item_Type::legs, emptyEquipSlot },
            { Item_Type::kilt, emptyEquipSlot },
            { Item_Type::gloves, emptyEquipSlot },
            { Item_Type::clothes, emptyEquipSlot },
            { Item_Type::chest, emptyEquipSlot },
            { Item_Type::hair, emptyEquipSlot },
            { Item_Type::ring, emptyEquipSlot },
            { Item_Type::shins, emptyEquipSlot },
            { Item_Type::shoulders, emptyEquipSlot },
            { Item_Type::jewelry, emptyEquipSlot },
            { Item_Type::ranged, emptyEquipSlot },
            { Item_Type::offhand, emptyEquipSlot },
            { Item_Type::mainhand, emptyEquipSlot },
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

    enum class Offhand_Type {
        dagger,
        kama,
        shield
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

    struct Update_Ground_Item {

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

    std::unordered_map<Rarity, SDL_Color>rarityColor;
    std::unordered_map<Rarity, SDL_Texture*>rarityBorder;
    std::unordered_map<Stat, int>baseStatData;
    std::unordered_map<Stat, int>statData;
    std::unordered_map<Stat, std::string>statName;
    std::unordered_map<Item_Type, int>itemTypes;
    std::unordered_map<Armor_Type, int>armorTypes;
    std::unordered_map<Weapon_Type, int>weaponTypes;
    std::unordered_map<Offhand_Type, int>offhandTypes;
    std::unordered_map<Weapon_Material, int>weaponMaterials;
    std::unordered_map<Weapon_Material, std::string>weaponMaterialName;
    std::unordered_map<Weapon_Type, std::string>weaponTypeName;
    std::unordered_map<Offhand_Type, std::string>offhandTypeName;
    std::unordered_map<Armor_Type, std::string>ArmorTypeName;
    std::unordered_map<Item_Type, std::string>ItemTypeName;
}

