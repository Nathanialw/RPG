#pragma once
#include <SDL2/SDL.h>
#include "sstream"
#include "graphics.h"
#include "items.h"
#include "ui.h"

namespace Character_Stats {

	SDL_FRect statsSheetOffsetRect = { 160.0f, 64.0f, 240.0f, 384.0f };

	SDL_FRect screenCharacterStats = {
		UI::defaultScreenPosition.x + statsSheetOffsetRect.x,
		UI::defaultScreenPosition.y + statsSheetOffsetRect.y,
		statsSheetOffsetRect.w,
		statsSheetOffsetRect.h
	};

	Graphics::Surface_Data Create_Text_Box(std::string& string, SDL_Color& rarity, SDL_Rect& textBox) {
		Graphics::Surface_Data text = Graphics::Load_Text_Texture(string, rarity);
		text.k = textBox;
		return text;
	}

	//update all stats
	//cycle through each equip slot and adds the stats to the base stats
	int Update_Stat(int characterStat, int itemStat) {
		return itemStat + characterStat;
	}

	//run when equipping or unequipping an item
	void Update_Equip_slots(entt::registry& zone) { //run funtion on item equip or unequip
		auto view = zone.view<Rendering_Components::Sprite_Sheet_Info, Item_Component::Item_Equip, Item_Component::Equipment, Rendering_Components::Equipment_Sprites>();
		for (auto entity : view) {
			auto& sheetData = view.get<Rendering_Components::Sprite_Sheet_Info>(entity);
			auto& equipment = view.get<Item_Component::Equipment>(entity);
			auto& equipmentSprites = view.get<Rendering_Components::Equipment_Sprites>(entity);

            //iterate through each equip slot
			for (auto &item : equipment.equippedItems) {
                if (item.second != Item_Component::emptyEquipSlot) {

                    //get the item at the item type index
                    auto &weaponSheet = zone.get<Rendering_Components::Sprite_Sheet_Info>(item.second);
                    if (weaponSheet.sheetData) {
                        equipmentSprites.sheet[(int) item.first].ItemSheetData = weaponSheet.sheetData;
                        equipmentSprites.sheet[(int) item.first].name = weaponSheet.sheet_name;
                        equipmentSprites.sheet[(int) item.first].itemID = item.second;
                    }
                }

                else if (item.second == Item_Component::emptyEquipSlot) {
                    equipmentSprites.sheet[(int)item.first].ItemSheetData = NULL;
                    equipmentSprites.sheet[(int)item.first].name = "empty";
                    equipmentSprites.sheet[(int)item.first].itemID = emptyEquipSlot;
                }
            }
		}
	}

    void Update_Unit_Offense(entt::registry &zone) {
        auto view = zone.view<Item_Component::Item_Equip, Component::Melee_Damage, Component::Attack_Speed, Item_Component::Equipment>();
        for (auto entity : view) {
            auto& damage = view.get<Component::Melee_Damage>(entity);
            auto& attackSpeed = view.get<Component::Attack_Speed>(entity);
            auto& equipment = view.get<Item_Component::Equipment>(entity);

            //set character stats to base values
            for (auto &stat : Items::statData) {
                if (stat.first == Item_Component::Stat::damage) {
                    stat.second = Items::baseStatData[stat.first];
                }
                if (stat.first == Item_Component::Stat::attackSpeed) {
                    stat.second = Items::baseStatData[stat.first];
                }
                if (stat.first == Item_Component::Stat::spellDamage) {
                    stat.second = Items::baseStatData[stat.first];
                }
            }

            //check if slot is occupied, add stats if it is
            for (auto &item : equipment.equippedItems) {
                if (item.second != Item_Component::emptyEquipSlot) {
                    auto &stats = zone.get<Item_Stats>(item.second).stats;
                    for (auto &stat: stats) {
                        Items::statData[stat.first] += stat.second;
                    }
                }
            }

            //update components from updated character stat data
            for (auto& stat : Items::statData) {
                switch (stat.first) {
                    case Item_Component::Stat::damage: damage.minDamage = stat.second; damage.maxDamage = stat.second; break;
                    case Item_Component::Stat::attackSpeed: attackSpeed.period = stat.second; break;
                }
            }
        }
    }

    void Update_Unit_Defense(entt::registry& zone) {
        auto view = zone.view<Item_Component::Item_Equip, Component::Health, Item_Component::Equipment>();
        for (auto entity : view) {
            auto& health = view.get<Component::Health>(entity);
            auto& equipment = view.get<Item_Component::Equipment>(entity);

            //set character stats to base values
            for (auto &stat : Items::statData) {
                if (stat.first == Item_Component::Stat::health) {
                    stat.second = Items::baseStatData[stat.first];
                }
                if (stat.first == Item_Component::Stat::armor) {
                    stat.second = Items::baseStatData[stat.first];
                }
                if (stat.first == Item_Component::Stat::piety) {
                    stat.second = Items::baseStatData[stat.first];
                }
            }

            //check if slot is occupied, add stats if it is
            for (auto &item : equipment.equippedItems) {
                if (item.second != Item_Component::emptyEquipSlot) {
                    auto &stats = zone.get<Item_Stats>(item.second).stats;
                    for (auto &stat: stats) {
                        Items::statData[stat.first] += stat.second;
                    }
                }
            }

            //update components from updated character stat data
            for (auto& stat : Items::statData) {
                switch (stat.first) {
                    case Item_Component::Stat::health: health.maxHealth = stat.second; break;
                }
            }
            zone.remove<Item_Component::Item_Equip>(entity);
        }
    }

    SDL_FRect Get_And_Scale_StatBox(entt::registry& zone, Component::Camera& camera) {
		SDL_FRect statsBox = { UI::defaultScreenPosition.x + statsSheetOffsetRect.x, UI::defaultScreenPosition.y + statsSheetOffsetRect.y, statsSheetOffsetRect.w, statsSheetOffsetRect.h };
		return Camera_Control::Convert_FRect_To_Scale(zone, statsBox, camera);
	}

	void Render_Character_Stats(entt::registry &zone, Component::Camera &camera) {
		if (UI::bToggleCharacterUI == true) {

			SDL_FRect statBox = Get_And_Scale_StatBox(zone, camera);
			SDL_Color black = { 0, 0, 0, 255 };

			float currentRow = 0.0f;
			float charHeight = (20.0f / camera.scale.y);
			float charWidth =  (10.0f / camera.scale.x);

			for (auto& stat : Items::statData) {
//				Graphics::Surface_Data statNameData = Graphics::Load_Text_Texture(Items::statName[stat.first], black);

				SDL_FRect statNameRect = statBox;
				statNameRect.y = statBox.y + currentRow;
//				statNameRect.h = charHeight;
//				statNameRect.w = Items::statName[stat.first].size() * charWidth;

                FC_Draw(Graphics::fcFont, Graphics::renderer, statNameRect.x, statNameRect.y, Items::statName[stat.first].c_str());
//				Graphics::Render_FRect(statNameData.pTexture, &statNameData.k, &statNameRect);
//				SDL_DestroyTexture(statNameData.pTexture);
//				//SDL_RenderDrawRect(Graphics::renderer, &rowRect);
//
				std::string statValue = std::to_string(stat.second);
//				Graphics::Surface_Data statValueData = Graphics::Load_Text_Texture(statValue, black);
//
				SDL_FRect statValueRect = (statBox);
				statValueRect.y = statBox.y + currentRow;
				statValueRect.x = statBox.x + statBox.w - statValueRect.w;
//				statValueRect.h = charHeight;
//				statValueRect.w = statValue.size() * charWidth;
//
//
//				Graphics::Render_FRect(statValueData.pTexture, &statValueData.k, &statValueRect);
//				SDL_DestroyTexture(statValueData.pTexture);
//				currentRow += charHeight;
                FC_Draw(Graphics::fcFont, Graphics::renderer, statValueRect.x, statValueRect.y, statValue.c_str());
			}
		}
	}

	void Init_Player_Stats(entt::registry& zone) { //run funtion on item equip or unequip
		auto view = zone.view<Component::Input, Component::Melee_Damage, Component::Health, Component::Attack_Speed, Item_Component::Equipment>();
		for (auto entity : view) {
			auto& damage = view.get<Component::Melee_Damage>(entity);
			auto& health = view.get<Component::Health>(entity);
			auto& attackSpeed = view.get<Component::Attack_Speed>(entity);
			auto& equipment = view.get<Item_Component::Equipment>(entity);

			//set charcter stats to base values
			for (auto& stat : Items::statData) {
				stat.second = Items::baseStatData[stat.first];
			}

			//add equipment stats to charcter stats
			for (auto& item : equipment.equippedItems) {
				if (item.second != Item_Component::emptyEquipSlot) {
					auto& stats = zone.get<Item_Stats>(item.second).stats;
					for (auto& stat : stats) {
						Items::statData[stat.first] += stat.second;
					}
				}
			}

			//update components from updated character stat data
			for (auto& stat : Items::statData) {
				switch (stat.first) {
				case Item_Component::Stat::damage: damage.minDamage = stat.second; damage.maxDamage = stat.second; break;
				case Item_Component::Stat::health: health.maxHealth = stat.second; break;
				case Item_Component::Stat::attackSpeed: attackSpeed.period = stat.second; break;
				}
			}

		}
	}

    void Equip_Units (entt::registry &zone) {
        auto view = zone.view<Item_Component::Equipment, Component::Position>();
        for (auto unit : view) {
            auto &equipment = view.get<Item_Component::Equipment>(unit);
            auto &position = view.get<Component::Position>(unit);

            //create a weapon Component::Position& position, Component::Direction &direction
            equipment.equippedItems[Item_Type::mainhand] = Items::Create_And_Equip_Weapon(position, equipment.type);
            equipment.equippedItems[Item_Type::offhand] = Items::Create_And_Equip_Offhand(position, equipment.type);
            //create a chest
            equipment.equippedItems[Item_Type::legs] = Items::Create_And_Equip_Armor(position, Item_Type::legs, equipment.type);
            //create a leggings
            equipment.equippedItems[Item_Type::chest] = Items::Create_And_Equip_Armor(position, Item_Type::chest, equipment.type);
            //create hair
            equipment.equippedItems[Item_Type::hair] = Items::Create_And_Equip_Armor(position, Item_Type::hair, equipment.type);
            //create kilt
//            equipment.equippedItems[Item_Type::clothes] = Items::Create_And_Equip_Armor(position, Item_Type::clothes, equipment.type);

            zone.emplace<Item_Component::Item_Equip>(unit);
        }
    }

    void Update_Items(entt::registry &zone) {
        Update_Equip_slots(zone);
        Update_Unit_Offense(zone);
        //defensive has to come after offensive as it removes the Item_Equip component
        Update_Unit_Defense(zone);
    }

	void Init_UI(entt::registry& zone) {
		Items::Init_Item_Data();
		UI::Bag_UI::Create_Bag_UI(zone);
		UI::Equipment_UI::Create_Equipment_UI(zone);
        Equip_Units(zone);
		Init_Player_Stats(zone);
	}
}
