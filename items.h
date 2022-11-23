#pragma once
#include <vector>
#include "world.h"
#include "components.h"
#include "item_components.h"
#include "graphics.h"
#include "utilities.h"
#include "camera.h"
#include "mouse_control.h"
#include "texture_packer_item.h"
#include "SQLite_item_data.h"

#include "SQLite_spritesheets.h"
using namespace Item_Component;

namespace Items {
	namespace {
		std::unordered_map<Rarity, SDL_Color>rarityColor;
		std::unordered_map<Rarity, SDL_Texture*>rarityBorder;
		std::unordered_map<Item_Component::Stat, int>baseStatData;
		std::unordered_map<Item_Component::Stat, int>statData;
		std::unordered_map<Item_Component::Stat, std::string>statName;
		std::unordered_map<Item_Type, int>itemTypes;
		std::unordered_map<Armor_Type, int>armorTypes;
		std::unordered_map<Weapon_Type, int>weaponTypes;
        std::unordered_map<Weapon_Material, int>weaponMaterials;
        std::unordered_map<Item_Component::Weapon_Material, std::string>weaponMaterialName;
        std::unordered_map<Item_Component::Weapon_Type, std::string>weaponTypeName;
        std::unordered_map<Item_Component::Armor_Type, std::string>ArmorTypeName;
		std::unordered_map<Item_Component::Item_Type, std::string>ItemTypeName;
		bool showGroundItems = false;
	}

	int i = 0;

	Item_Component::Weapon_Type Generate_Weapon_Type() {
		int randType = rand() % 4 + 1;
		switch (randType) {
            case 1: return Item_Component::Weapon_Type::sword;

            case 2: return Item_Component::Weapon_Type::mace;

            case 3: return Item_Component::Weapon_Type::axe;

            case 4: return Item_Component::Weapon_Type::spear;
		}
		Utilities::Log("Generate_Weapon_Type() fallthrough error");
        //defult return if it finds nothing
		return Weapon_Type::sword;
	}

    Item_Component::Item_Type Generate_Item_Type() {
		int randType = rand() % 5 + 1;

		switch (randType) {
            case 1: return Item_Type::helm;

            case 2: return Item_Type::chest;

            case 3: return Item_Type::gloves;

            case 4: return Item_Type::legs;

            case 5: return Item_Type::boots;
		}
		Utilities::Log("Generate_Item_Type() fallthrough error");
        //defult return if it finds nothing
		return Item_Type::helm;
	}

    Item_Component::Weapon_Material Generate_Weapon_Material() {
        int randType = rand() % 3 + 1;

        switch (randType) {
            case 1: return Weapon_Material::copper;
            case 2: return Weapon_Material::bronze;
            case 3: return Weapon_Material::iron;
        }
        Utilities::Log("Generate_Weapon_Material() fallthrough error");
        //defult return if it finds nothing
        return Weapon_Material::copper;
    }

    Item_Component::Armor_Type Generate_Armor_Type() {
		int randType = rand() % 5 + 1;

		switch (randType) {
            case 1: return Armor_Type::cloth;
            case 2: return Armor_Type::padded;
            case 3: return Armor_Type::leather;
            case 4: return Armor_Type::mail;
            case 5: return Armor_Type::plate;
		}
		Utilities::Log("Generate_Armor_Type() fallthrough error");
        //defult return if it finds nothing
		return Armor_Type::cloth;
	}

	std::string Create_Weapon(entt::entity& item, Rarity &rarity) {
		auto material = Generate_Weapon_Material();
		auto weaponType = Generate_Weapon_Type();

        std::string materialType = weaponMaterialName[material];
        std::string weapon = weaponTypeName[weaponType];
        std::string weaponName = materialType  + " " + weapon;

		World::zone.emplace<Item_Type>(item, Item_Type::weapon);
		World::zone.emplace<Rarity>(item, rarity);

            ///create the weapon from the db
            ///add to std::map
            ///save the string of the name
            ///provide lookup string when the player picks it up

       auto equippedSheetData = Texture_Packer_Item::TexturePacker_Import_Item(weapon);

        if (equippedSheetData.itemData == NULL) {
            return "none";
        }

        //get from spritesheet
		int column = weaponMaterials[material];
		int row = weaponTypes[weaponType];
		int size = 32;

		SDL_Rect sprite = { column * size , row * size  ,size  ,size };


		auto &sheetData = World::zone.emplace<Component::Sprite_Sheet_Info>(item);
        sheetData.sheetDataWeapon = equippedSheetData.itemData;
        sheetData.sheet_name = equippedSheetData.index;
        sheetData.type = "RPG_Tools";
        sheetData.weapon_name = equippedSheetData.index;
        sheetData.sheetData = equippedSheetData.itemData;

		auto& icon = World::zone.emplace<Component::Icon>(item, Graphics::emptyBagIcon, Graphics::weapons_icons, rarityBorder[rarity], Graphics::bagSlotBorder);
		icon.clipSprite = sprite;
		icon.clipIcon = {0,0,256,256};
		icon.renderRectSize = { 64.0f, 64.0f };
		icon.renderPositionOffset = { icon.renderRectSize.x / 2, icon.renderRectSize.y / 2 };

		World::zone.emplace<Weapon_Damage>(item, 1, 10);
		return equippedSheetData.index;
	}

    void armor_texture(Item_Type &itemType, Component::Sprite_Sheet_Info &sheetData, std::unordered_map<std::string, Component::Sheet_Data>* equippedSheetData, std::string &itemName) {

        switch (itemType) {
            case Item_Component::Item_Type::legs:
                sheetData.sheetDataLegs = equippedSheetData;
                sheetData.legs_name = itemName;
                break;
            case Item_Component::Item_Type::chest:
                sheetData.sheetDataChestpiece = equippedSheetData;
                sheetData.chest_name = itemName;
                break;
            case Item_Component::Item_Type::helm:
                sheetData.sheetDataHelm = equippedSheetData;
                sheetData.helm_name = itemName;
                break;
            case Item_Component::Item_Type::boots:
                sheetData.sheetDataHelm = equippedSheetData;
                sheetData.helm_name = itemName;
                break;
            case Item_Component::Item_Type::gloves:
                sheetData.sheetDataHelm = equippedSheetData;
                sheetData.helm_name = itemName;
                break;
            case Item_Component::Item_Type::belt:
                sheetData.sheetDataHelm = equippedSheetData;
                sheetData.helm_name = itemName;
                break;
            case Item_Component::Item_Type::neck:
                sheetData.sheetDataHelm = equippedSheetData;
                sheetData.helm_name = itemName;
                break;
            case Item_Component::Item_Type::shield:
                sheetData.sheetDataHelm = equippedSheetData;
                sheetData.helm_name = itemName;
                break;
            case Item_Component::Item_Type::weapon:
                sheetData.sheetDataHelm = equippedSheetData;
                sheetData.helm_name = itemName;
                break;
            case Item_Component::Item_Type::shoulders:
                sheetData.sheetDataHelm = equippedSheetData;
                sheetData.helm_name = itemName;
                break;
        }
    }

	std::string Create_Armor(entt::entity& item, Rarity& rarity) {
		Item_Type itemType = Generate_Item_Type();
		Armor_Type armorType = Generate_Armor_Type();

		std::string type = ItemTypeName[itemType];
		std::string armor = ArmorTypeName[armorType];
		std::string itemName = armor + " " + type;
		World::zone.emplace<Item_Type>(item, itemType);
		World::zone.emplace<Rarity>(item, rarity);
		int column = itemTypes[itemType];
		int row = armorTypes[armorType];
		int size = 64;
		SDL_Rect sprite = { column * size, row * size, size, size };

        auto &sheetData = World::zone.emplace<Component::Sprite_Sheet_Info>(item);

        Texture_Packer_Item::Item_Data_And_Index equippedSheetData;
        if (itemType == Item_Component::Item_Type::legs || itemType == Item_Component::Item_Type::chest || itemType == Item_Component::Item_Type::helm) {
            equippedSheetData = Texture_Packer_Item::TexturePacker_Import_Item(type);
            if (equippedSheetData.itemData == NULL) {
                return "none";
            }
            armor_texture(itemType, sheetData, equippedSheetData.itemData, equippedSheetData.index);
        }
        else {
            return "none";
        }

        sheetData.sheet_name = equippedSheetData.index;
        sheetData.type = "RPG_Tools";
        sheetData.sheetData = equippedSheetData.itemData;

		auto& icon = World::zone.emplace<Component::Icon>(item, Graphics::emptyBagIcon, Graphics::armorSpriteSheet, rarityBorder[rarity], Graphics::bagSlotBorder);
		icon.clipSprite = sprite;
		icon.clipIcon = {0,0,256,256};
		icon.renderRectSize = { 64.0f, 64.0f };
		icon.renderPositionOffset = { icon.renderRectSize.x / 2, icon.renderRectSize.y / 2 };
		return equippedSheetData.index;
	}

	void Create_Item(entt::entity &item, Component::Position& position, const std::string &name, Item_Stats &itemStats) {
		float scale = 1.0f;
        float rectSide = 64.0f;

		World::zone.emplace<Component::Scale>(item, scale);
		World::zone.emplace<Component::Action>(item, Component::dead);
		World::zone.emplace<Component::Direction>(item, Component::Direction::E);
		World::zone.emplace<Name>(item, name);
		World::zone.emplace<Component::Entity_Type>(item, Component::Entity_Type::item);
		auto &stats = World::zone.emplace<Item_Stats>(item);
		stats = itemStats;
		auto& offset = World::zone.emplace<Component::Sprite_Offset>(item, rectSide/2.0f * scale, rectSide/2.0f * scale);
		auto& position2 = World::zone.emplace<Component::Position>(item, position.x, position.y);
		World::zone.emplace<Component::Radius>(item, offset.x);
		World::zone.emplace<Component::Interaction_Rect>(item, offset.x, (offset.x * 2.0f));
		World::zone.emplace<Ground_Item>(item,
			((position2.x - offset.x) * scale), //half the width and height
			((position2.y - offset.y) * scale),	//half the width and height
			(rectSide * scale) * scale,
			(rectSide * scale) * scale,0.0f,0.0f,0.0f,0.0f);
	}

	statValue Get_Random_Stat() {

		int randStat = rand() % 6 + 1;

		int randStatValue = rand() % 5 + 1;

		switch (randStat) {
            case 1: return { Stat::health, randStatValue };

            case 2: return { Stat::damage, randStatValue };

            case 3: return { Stat::spellDamage, randStatValue };

            case 4: return { Stat::armor, randStatValue };

            case 5: return { Stat::piety, randStatValue };

            case 6: return { Stat::attackSpeed, randStatValue };
		}

		Utilities::Log("Get_Random_Stat() fallthrough error");

        return  {Stat::health, randStatValue};
	}

	Rarity Generate_Item_Rarity() {

		int randType = rand() % 100 + 1;

		if (randType >= 35) {
			return Rarity::common;
		}

        else if (randType >= 15) {
			return Rarity::magic;
		}

        else if (randType >= 5) {
			return Rarity::rare;
		}

        else if (randType >= 0) {
			return Rarity::unique;
		}

        Utilities::Log("Generate_Item_Rarity() fallthrough error");
		return  Rarity::common;
	}

	Item_Stats Generate_Item_Stats(Rarity& rarity) {
		Item_Stats itemStats;
		int numMods = 0;
		statValue randomStat;
		if (rarity == Rarity::common) {
			numMods = rand() % 2 + 1;
		}
		else if (rarity == Rarity::magic) {
			numMods = rand() % 4 + 2;
		}
		else if (rarity == Rarity::rare) {
			numMods = rand() % 6 + 3;
		}
		else if (rarity == Rarity::unique) {
			numMods = rand() % 8 + 4;
		}

		for (int index = 0; index < numMods; index++) {
			randomStat = Get_Random_Stat();
			if (randomStat.name == Stat::attackSpeed) { randomStat.value = randomStat.value * -10; };
			if (itemStats.stats.contains(randomStat.name)) {
				itemStats.stats[randomStat.name] += randomStat.value;
			}
			else {
				itemStats.stats.emplace(randomStat.name, randomStat.value);
			}
		}
		return itemStats;
	}

    std::string Choose_Item (entt::entity item, Rarity rarity) {
        int itemDrop = rand() % 10 + 1;
        if (itemDrop < 10) {
            return Create_Armor(item, rarity);
        }
        else {
            return Create_Weapon(item, rarity);
        }
//        std::cout << "Choose_Item() fallthrough error, failed to select and item type, returned default" << std::endl;
//        return Create_Armor(item, rarity);
    }

	void Create_And_Drop_Item(Component::Position& position) {
        Rarity rarity = Generate_Item_Rarity();
		Item_Stats itemStats = Generate_Item_Stats(rarity);
		auto item_uID = World::zone.create();
        std::string itemName = Choose_Item(item_uID, rarity);
        if (itemName == "none") {
            World::zone.destroy(item_uID);
            Utilities::Log("no drop");
            return;

        }
		//	int item = rand() % 100 + 1;
		//	if (item <= 10) { Create_Item(item, position, rarity, "sword", Item_Type::weapon, Weapon_Type::sword, Graphics::longsword_default, Graphics::longsword_default_icon, itemStats); }
		//	else if (item <= 12) { Create_Item(item, position, rarity, "padded armour", Item_Type::chest, Armor_Type::cloth, Graphics::armorSpriteSheet, Graphics::armorSpriteSheet, itemStats); }
		Create_Item(item_uID, position, itemName, itemStats);
	}

	//mouse click
	//walk over to item
	//when arrived at item pick it up into mouse array
	void Hightlight_Item_Under_Cursor(SDL_FRect& itemRect) {
		if (Mouse::bRect_inside_Cursor(itemRect)) { //if cursor in inside item text box
			//then change the background color of that box to another shade
			SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 155, 155);
		}
		else {
			//set beackground of textbox to black
			SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, 255);
		}
	}

	void Item_Collision(entt::registry& zone) {
        ///What we actually need to do is create a quad tree with each cell the size of one item and only one item per cell
        /// when an item it dropped it falls it queries the tree into the nearest vacant cell
		auto view = zone.view<Ground_Item, Component::Position, Name, Component::Renderable>();
		for (auto item1 : view) {
			auto& itemPosition1 = view.get<Component::Position>(item1);
			auto& name1 = view.get<Name>(item1).name;
			auto& groundItem1 = view.get<Ground_Item>(item1).box;
			SDL_Rect textBox1 = {};
			textBox1.w = name1.length() * 5;
			textBox1.h = 10;
			textBox1.x = itemPosition1.x - (textBox1.w / 2.0f);
			textBox1.y = itemPosition1.y - 10;
			for (auto item2 : view) {
				if (item1 != item2) {
					auto& itemPosition2 = view.get<Component::Position>(item2);
					auto& name2 = view.get<Name>(item2).name;
					auto& groundItem2 = view.get<Ground_Item>(item2).box;
					SDL_Rect textBox2 = {};
					textBox2.w = name2.length() * 5;
					textBox2.h = 10;
					textBox2.x = itemPosition2.x - (textBox2.w / 2.0f);
					textBox2.y = itemPosition2.y - 10;
					SDL_Point rectPosition = Utilities::Check_Collision_Rects(textBox1, textBox2);
					itemPosition1.x += rectPosition.x;
					itemPosition1.y += rectPosition.y;
					groundItem1.x += rectPosition.x;
					groundItem1.y += rectPosition.y;
					itemPosition2.x -= rectPosition.x;
					itemPosition2.y -= rectPosition.y;
					groundItem2.x -= rectPosition.x;
					groundItem2.y -= rectPosition.y;
				}
			}
		}
	}

	void Update_Mouse_Slot_Position(entt::registry &zone, entt::entity &item, bool &isItemCurrentlyHeld, float &mouseX, float &mouseY) {
		//set item in mouse array position to mouse x, y every frame
		if (isItemCurrentlyHeld == true) {
            Component::Position& position = zone.get<Component::Position>(item);
			position.x = mouseX;
			position.y = mouseY;
		}
		Item_Collision(zone);
	}

	void Show_Ground_Items(entt::registry& zone, Component::Camera& camera) {
		if (showGroundItems == true) {
		 	//****//search quad tree instead
			auto view = zone.view<Ground_Item, Component::Position, Rarity, Name, Component::Renderable>();
			for (auto item : view) {
				///need position for render location
				/// need an offset based on rect size for the text box and item position
				///
				///need name for string
				/// need rarity for colour
				auto& itemPosition = view.get<Component::Position>(item);
				auto& rarity = view.get<Rarity>(item);
				auto& name = view.get<Name>(item).name;
                Graphics::Text_Box_Data itemTextBox = Graphics::Create_Text_Background(camera, rarityColor[rarity], name,  itemPosition);
                auto& highlightBox = view.get<Ground_Item>(item).ground_name;
                highlightBox = Utilities::SDL_Rect_To_SDL_FRect(itemTextBox.textBoxBackground);
				Hightlight_Item_Under_Cursor(highlightBox);
                SDL_RenderFillRect(Graphics::renderer, &itemTextBox.textBoxBackground);
				SDL_RenderCopyF(Graphics::renderer, itemTextBox.textdata.pTexture, &itemTextBox.textdata.k, &itemTextBox.highlightBox);
				SDL_DestroyTexture(itemTextBox.textdata.pTexture);
			}
		}
		//run rect collsion to break up the item names
	}

    void Name_On_Mouseover (entt::registry& zone, Component::Camera& camera) {
        auto view = zone.view<Ground_Item, Component::Position, Rarity, Name, Component::Renderable>();
        for (auto item : view) {
            auto &box = view.get<Ground_Item>(item);
            if (Mouse::FRect_inside_Cursor(box.box)) {
                //****//search quad tree instead
                ///need position for render location
                /// need an offset based on rect size for the text box and item position
                ///
                ///need name for string
                /// need rarity for colour
                auto &itemPosition = view.get<Component::Position>(item);
                auto &rarity = view.get<Rarity>(item);
                auto &name = view.get<Name>(item).name;
                Graphics::Text_Box_Data itemTextBox = Graphics::Create_Text_Background(camera, rarityColor[rarity], name,  itemPosition);
                auto& highlightBox = view.get<Ground_Item>(item).ground_name;
                highlightBox = Utilities::SDL_Rect_To_SDL_FRect(itemTextBox.textBoxBackground);
                SDL_RenderFillRect(Graphics::renderer, &itemTextBox.textBoxBackground);
                SDL_RenderCopyF(Graphics::renderer, itemTextBox.textdata.pTexture, &itemTextBox.textdata.k, &itemTextBox.highlightBox);
                SDL_DestroyTexture(itemTextBox.textdata.pTexture);
            }
        }
    }

	void Init_Item_Data() {

        SQLite_Item_Data::Load_Item_Names("classes_female");

		rarityColor = {
			{Rarity::common, { 255, 255, 255, 200 }},
			{Rarity::magic, { 51, 153, 255, 255 }},
			{Rarity::rare, { 255, 128, 0, 255 }},
			{Rarity::unique, { 255, 20, 20, 255 }},
		};

		rarityBorder = {
			{Rarity::common, Graphics::itemBorderCommon},
			{Rarity::magic, Graphics::itemBorderMagic},
			{Rarity::rare, Graphics::itemBorderRare},
			{Rarity::unique, Graphics::itemBorderEite},
		};

		itemTypes = {
			{Item_Type::helm, 0},
			{Item_Type::chest, 1},
			{Item_Type::gloves, 2},
			{Item_Type::legs, 3},
			{Item_Type::boots, 4},
			//{Item_Type::neck, 1},
			//{Item_Type::shoulders, 2},
			//{Item_Type::weapon, 4},
			//{Item_Type::belt, 6},
			//{Item_Type::shield, 9}
		};

		armorTypes = {
			{Armor_Type::cloth, 0},
			{Armor_Type::padded, 1},
			{Armor_Type::leather, 2},
			{Armor_Type::mail, 3},
			{Armor_Type::plate, 4}
		};

		weaponTypes = {
			{Weapon_Type::sword, 0},
            {Weapon_Type::axe, 1},
            {Weapon_Type::mace, 2},
            {Weapon_Type::spear, 3}
		};

        weaponMaterials = {
            {Weapon_Material::copper, 0},
            {Weapon_Material::bronze, 1},
            {Weapon_Material::iron, 2},
        };

		ArmorTypeName = {
			{Item_Component::Armor_Type::cloth, "cloth"},
			{Item_Component::Armor_Type::padded, "padded"},
			{Item_Component::Armor_Type::leather, "leather"},
			{Item_Component::Armor_Type::mail, "mail"},
			{Item_Component::Armor_Type::plate, "plate"}
		};

		ItemTypeName = {
			{Item_Component::Item_Type::helm, "helm"},
			{Item_Component::Item_Type::chest, "chest"},
			{Item_Component::Item_Type::gloves, "gloves"},
			{Item_Component::Item_Type::legs, "legs"},
			{Item_Component::Item_Type::boots, "boots"},
		};

        weaponMaterialName = {
            {Item_Component::Weapon_Material::copper, "copper"},
            {Item_Component::Weapon_Material::bronze, "bronze"},
            {Item_Component::Weapon_Material::iron, "iron"},
        };

        weaponTypeName = {
                {Item_Component::Weapon_Type::sword, "sword"},
                {Item_Component::Weapon_Type::axe, "axe"},
                {Item_Component::Weapon_Type::mace, "mace"},
                {Item_Component::Weapon_Type::spear, "polearm"}
        };

		baseStatData = {
			{Item_Component::Stat::health, 100},
			{Item_Component::Stat::damage, 2},
			{Item_Component::Stat::spellDamage, 1},
			{Item_Component::Stat::armor, 100},
			{Item_Component::Stat::piety, 10},
			{Item_Component::Stat::attackSpeed, 500},
		};

		statData = baseStatData;

		statName = {
			{Item_Component::Stat::health, "Health"},
			{Item_Component::Stat::damage, "Damage"},
			{Item_Component::Stat::spellDamage, "Spell Damage"},
			{Item_Component::Stat::armor, "Armour"},
			{Item_Component::Stat::piety, "Piety"},
			{Item_Component::Stat::attackSpeed, "Attack Speed"},
		};
	}
}
