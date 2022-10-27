#pragma once
#include <vector>
#include "world.h"
#include "components.h"
#include "item_components.h"
#include "graphics.h"
#include "utilities.h"
#include "camera.h"
#include "mouse_control.h"


// item

// item equip

//item bag

//item drop

using namespace Component;
using namespace Item_Component;

namespace Items {

	namespace {
		std::map<Rarity, SDL_Color>rarityColor;
		std::map<Rarity, SDL_Texture*>rarityBorder;

		std::map<Item_Component::Stat, int>baseStatData;
		std::map<Item_Component::Stat, int>statData;
		std::map<Item_Component::Stat, std::string>statName;

		std::map<Item_Type, int>itemTypes;
		std::map<Armor_Type, int>armorTypes;
		std::map<Weapon_Type, int>weaponTypes;

		std::map<Item_Component::Armor_Type, std::string>ArmorTypeName;
		std::map<Item_Component::Item_Type, std::string>ItemTypeName;


		std::vector<int>bag;
		int mousePickUp;
		bool showGroundItems = false;
		bool pick_up_debug = true;
	}

	int i = 0;

	Item_Component::Weapon_Type Generate_Weapon_Type() {
		int randType = rand() % 1 + 1;

		switch (randType) {
		case 1: return { Item_Component::Weapon_Type::sword };
		}
		Utilities::Log("Generate_Weapon_Type() fallthrough error");
		return Weapon_Type::sword;
	}

	Item_Type Generate_Item_Type() {
		int randType = rand() % 5 + 1;

		switch (randType) {
		case 1: return { Item_Type::helm };
		case 2: return { Item_Type::chest };
		case 3: return { Item_Type::gloves };
		case 4: return { Item_Type::legs };
		case 5: return { Item_Type::boots };
		}
		Utilities::Log("Generate_Item_Type() fallthrough error");
		return Item_Type::helm;
	}

	Armor_Type Generate_Armor_Type() {
		int randType = rand() % 5 + 1;

		switch (randType) {
		case 1: return { Armor_Type::cloth };
		case 2: return { Armor_Type::padded };
		case 3: return { Armor_Type::leather };
		case 4: return { Armor_Type::mail };
		case 5: return { Armor_Type::plate };
		}
		Utilities::Log("Generate_Armor_Type() fallthrough error");
		return Armor_Type::cloth;
	}

	//entt::entity Create_Weapon(entt::entity& item, Rarity &rarity) {
	//	auto itemType = Generate_Item_Type();
	//	auto weaponType = Generate_Weapon_Type();
	//
	//	World::zone.emplace<Item_Type>(item, weaponType);
	//	World::zone.emplace<Rarity>(item, rarity);
	//
	//	int column = itemTypes[itemType];
	//	int row = weaponTypes[weaponType];
	//	int size = 64;
	//
	//	SDL_Rect sprite = { row * size  ,column * size  ,size  ,size };
	//
	//	World::zone.emplace<animation>(item, Graphics::longsword_default); /// need to load hetexture	 only once and pass the pointer into this function
	//	World::zone.get<animation>(item).sheet = {
	//		{ sprite , 0, 64, 0, 0, 0.0f, 16.0f}
	//	};
	//
	//	auto& icon = World::zone.emplace<Icon>(item, Graphics::emptyBagIcon, Graphics::longsword_default_icon, rarityBorder[rarity], Graphics::bagSlotBorder);
	//	icon.clipSprite = sprite;
	//	icon.clipIcon = { 0, 0, 64, 64 };
	//	icon.renderRectSize = { 64, 64 };
	//	icon.renderPositionOffset = { icon.renderRectSize.x / 2, icon.renderRectSize.y / 2 };
	//	return item;
	//
	//
	//	World::zone.emplace<Weapon_Damage>(item, 1, 10);
	//	return item;
	//}

	std::string Create_Armor(entt::entity& item, Rarity& rarity) {
		auto itemType = Generate_Item_Type();
		auto armorType = Generate_Armor_Type();

		std::string type = ItemTypeName[itemType];
		std::string armor = ArmorTypeName[armorType];
		std::string itemName = armor + " " + type;

		World::zone.emplace<Item_Type>(item, itemType);
		World::zone.emplace<Rarity>(item, rarity);

		int column = itemTypes[itemType];
		int row = armorTypes[armorType];
		int size = 64;

		SDL_Rect sprite = { column * size , row * size  ,size  ,size };

		World::zone.emplace<animation>(item, Graphics::armorSpriteSheet); /// need to load hetexture	 only once and pass the pointer into this function
		World::zone.get<animation>(item).sheet = {
			{ sprite , 0, 64, 0, 0, 0.0f, 16.0f}
		};

		auto& icon = World::zone.emplace<Icon>(item, Graphics::emptyBagIcon, Graphics::armorSpriteSheet, rarityBorder[rarity], Graphics::bagSlotBorder);
		icon.clipSprite = sprite;
		icon.clipIcon = { 0, 0, 64, 64 };
		icon.renderRectSize = { 64, 64 };
		icon.renderPositionOffset = { icon.renderRectSize.x / 2, icon.renderRectSize.y / 2 };


		return itemName;
	}


	void Create_Item(entt::entity &item, Position& position, Rarity &rarity, const std::string &name, Item_Stats &itemStats) {
		float scale = 0.5f;
		World::zone.emplace<Scale>(item, scale);
		World::zone.emplace<Actions>(item, isStatic);
		World::zone.emplace<Direction>(item, Direction::W);
		World::zone.emplace<Name>(item, name);
		World::zone.emplace<Component::Entity_Type>(item, Entity_Type::item);
		auto &stats = World::zone.emplace<Item_Stats>(item);
		stats = itemStats;
		auto& offset = World::zone.emplace<Sprite_Offset>(item, 16.0f * scale, 16.0f * scale).offset;
		auto& position2 = World::zone.emplace<Position>(item, position.x, position.y);
		World::zone.emplace<Radius>(item, 16.0f * scale);
		World::zone.emplace<Ground_Item>(item,
			((position2.x - offset.x) / 4.0f), //half the width and height
			((position2.y - offset.y) / 4.0f),	//half the width and height
			(64.0f / 4.0f) * scale,
			(64.0f / 4.0f) * scale), 0,0,0,0;

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
		int i = rand() % 100 + 1;

		if (i >= 35) {
			return Rarity::common;
		}
		else if (i >= 15) {
			return Rarity::magic;
		}
		else if (i >= 5) {
			return Rarity::rare;
		}
		else if (i >= 0) {
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

		for (int i = 0; i < numMods; i++) {
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

	void Create_And_Drop_Item(Position& position) {

		Rarity rarity = Generate_Item_Rarity();

		Item_Stats itemStats = Generate_Item_Stats(rarity);

		auto item = World::zone.create();

		std::string itemName = Create_Armor(item, rarity);

		//	int item = rand() % 100 + 1;
		//	if (item <= 10) { Create_Item(item, position, rarity, "sword", Item_Type::weapon, Weapon_Type::sword, Graphics::longsword_default, Graphics::longsword_default_icon, itemStats); }
		//	else if (item <= 12) { Create_Item(item, position, rarity, "padded armour", Item_Type::chest, Armor_Type::cloth, Graphics::armorSpriteSheet, Graphics::armorSpriteSheet, itemStats); }


		Create_Item(item, position, rarity, itemName, itemStats);

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

		auto view = zone.view<Ground_Item, Position, Name, Renderable>();
		for (auto item1 : view) {
			auto& itemPosition1 = view.get<Position>(item1);
			auto& name1 = view.get<Name>(item1).name;
			auto& groundItem1 = view.get<Ground_Item>(item1).box;
			SDL_Rect textBox1 = {};
			textBox1.w = name1.length() * 5;
			textBox1.h = 10;

			textBox1.x = itemPosition1.x - (textBox1.w / 2.0f);
			textBox1.y = itemPosition1.y - 10;
			for (auto item2 : view) {
				if (item1 != item2) {
					auto& itemPosition2 = view.get<Position>(item2);
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
			Position& position = zone.get<Position>(item);
			position.x = mouseX;
			position.y = mouseY;
		}
		Item_Collision(zone);
	}

	void Show_Ground_Items(entt::registry& zone, Camera& camera) {
		if (showGroundItems == true) {

		 	//****//search quad tree instead
			auto view = zone.view<Ground_Item, Position, Rarity, Name, Renderable>();

			for (auto item : view) {
				///need position for render location
				/// need an offset based on rect size for the text box and item position
				///
				///need name for string
				/// need rarity for colour
				auto& itemPosition = view.get<Position>(item);
				auto& rarity = view.get<Rarity>(item);
				auto& name = view.get<Name>(item).name;

                auto& highlightBox = view.get<Ground_Item>(item).ground_name;
				SDL_FRect textBox = {};

				Graphics::Surface_Data itemTextBox = Graphics::Load_Text_Texture(name, rarityColor[rarity]);

				textBox.w = name.length() * 5.0f;
				textBox.h = 10.0f;

				textBox.x = itemPosition.x - (textBox.w / 2.0f);
				textBox.y = itemPosition.y - 10.0f;

				textBox.x -= camera.screen.x;
				textBox.y -= camera.screen.y;
				SDL_Rect textBoxBackground = Utilities::SDL_FRect_To_SDL_Rect(textBox);
				textBoxBackground.x -= 5;
				textBoxBackground.w += 10;

				highlightBox = Utilities::SDL_Rect_To_SDL_FRect(textBoxBackground);
				Hightlight_Item_Under_Cursor(highlightBox);
				SDL_RenderFillRect(Graphics::renderer, &textBoxBackground);
				SDL_RenderCopyF(Graphics::renderer, itemTextBox.pTexture, &itemTextBox.k, &textBox);
				SDL_DestroyTexture(itemTextBox.pTexture);
			}
		}

		//run rect collsion to break up the item names

	}

    void Name_On_Mouseover (entt::registry& zone, Camera& camera) {
        auto view = zone.view<Ground_Item, Position, Rarity, Name, Renderable>();

        for (auto item : view) {
            auto &box = view.get<Ground_Item>(item);
            if (Mouse::FRect_inside_Cursor(box.box)) {
                //****//search quad tree instead
                ///need position for render location
                /// need an offset based on rect size for the text box and item position
                ///
                ///need name for string
                /// need rarity for colour
                auto &itemPosition = view.get<Position>(item);
                auto &rarity = view.get<Rarity>(item);
                auto &name = view.get<Name>(item).name;

                SDL_FRect textBox = {};

                Graphics::Surface_Data itemTextBox = Graphics::Load_Text_Texture(name, rarityColor[rarity]);

                textBox.w = name.length() * 5.0f;
                textBox.h = 10.0f;

                textBox.x = itemPosition.x - (textBox.w / 2.0f);
                textBox.y = itemPosition.y - 10.0f;

                textBox.x -= camera.screen.x;
                textBox.y -= camera.screen.y;
                SDL_Rect textBoxBackground = Utilities::SDL_FRect_To_SDL_Rect(textBox);
                textBoxBackground.x -= 5;
                textBoxBackground.w += 10;

                box.ground_name = Utilities::SDL_Rect_To_SDL_FRect(textBoxBackground);
                SDL_RenderFillRect(Graphics::renderer, &textBoxBackground);
                SDL_RenderCopyF(Graphics::renderer, itemTextBox.pTexture, &itemTextBox.k, &textBox);
                SDL_DestroyTexture(itemTextBox.pTexture);
            }
        }
    }

	void Init_Item_Data() {
		rarityColor = {
			{Rarity::common, { 255, 255, 255, 255 }},
			{Rarity::magic, { 51, 153, 255, 255 }},
			{Rarity::rare, { 255, 255, 0, 255 }},
			{Rarity::unique, { 255, 128, 0, 255 }},
		};

		rarityBorder = {
			{Rarity::common, NULL},
			{Rarity::magic, Graphics::itemBorderMagic},
			{Rarity::rare, Graphics::itemBorderRare},
			{Rarity::unique, Graphics::itemBorderCommon},
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
			{Weapon_Type::sword, 0}
		};

		ArmorTypeName = {
			{Item_Component::Armor_Type::cloth, "cloth"},
			{Item_Component::Armor_Type::padded, "padded"},
			{Item_Component::Armor_Type::leather, "leather"},
			{Item_Component::Armor_Type::mail, "mail"},
			{Item_Component::Armor_Type::plate, "plate"}
		};

		ItemTypeName = {
			{Item_Component::Item_Type::helm, "helmet"},
			{Item_Component::Item_Type::chest, "chestpiece"},
			{Item_Component::Item_Type::gloves, "gloves"},
			{Item_Component::Item_Type::legs, "legs"},
			{Item_Component::Item_Type::boots, "boots"},
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
