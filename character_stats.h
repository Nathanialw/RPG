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
	void Update_Unit_Stats(entt::registry& zone) { //run funtion on item equip or unequip
		auto view = zone.view<Component::Sprite_Sheet_Info, Item_Component::Item_Equip, Component::Melee_Damage, Component::Health, Component::Attack_Speed>();
		for (auto entity : view) {
			auto& damage = view.get<Component::Melee_Damage>(entity);
			auto& health = view.get<Component::Health>(entity);
			auto& attackSpeed = view.get<Component::Attack_Speed>(entity);
			auto& sheetData = view.get<Component::Sprite_Sheet_Info>(entity);

			//set character stats to base values
			for (auto &stat : Items::statData) {
				stat.second = Items::baseStatData[stat.first];
			}

			//add equipment stats to character stats
			for (auto &item : UI::Equipment_UI::equippedItems) {
				if (item.second != UI::Equipment_UI::emptyEquipSlot) {
					auto& stats = zone.get<Item_Stats>(item.second).stats;
					for (auto& stat : stats) {
						Items::statData[stat.first] += stat.second;
					}
                    /// add weapon sprite data to character rendering component
                    /// remove if nothing equipped
                    if (item.first == Item_Component::Item_Type::weapon) {
                        auto &weaponSheet = zone.get<Component::Sprite_Sheet_Info>(item.second);
                        if (weaponSheet.sheetDataWeapon) {
                            sheetData.sheetDataWeapon = weaponSheet.sheetDataWeapon;
                            sheetData.weapon_name = weaponSheet.weapon_name;
                        }
                    }
                    if (item.first == Item_Component::Item_Type::legs) {
                        auto &legsSheet = zone.get<Component::Sprite_Sheet_Info>(item.second);
                        if (legsSheet.sheetDataLegs) {
                            sheetData.sheetDataLegs = legsSheet.sheetDataLegs;
                            sheetData.legs_name = legsSheet.legs_name;
                        }
                    }
                    if (item.first == Item_Component::Item_Type::chest) {
                        auto &chestSheet = zone.get<Component::Sprite_Sheet_Info>(item.second);
                        if (chestSheet.sheetDataChestpiece) {
                            sheetData.sheetDataChestpiece = chestSheet.sheetDataChestpiece;
                            sheetData.chest_name = chestSheet.chest_name;
                        }
                    }
                    if (item.first == Item_Component::Item_Type::helm) {
                        auto &helmSheet = zone.get<Component::Sprite_Sheet_Info>(item.second);
                        if (helmSheet.sheetDataHelm) {
                            sheetData.sheetDataHelm = helmSheet.sheetDataHelm;
                            sheetData.helm_name = helmSheet.helm_name;
                        }
                    }
				}
                if (item.first == Item_Component::Item_Type::weapon) {
                    if (item.second == UI::Equipment_UI::emptyEquipSlot) {
                        sheetData.sheetDataWeapon = NULL;
                        sheetData.weapon_name = "unarmed";
                    }
                }
                if (item.first == Item_Component::Item_Type::legs) {
                    if (item.second == UI::Equipment_UI::emptyEquipSlot) {
                        sheetData.sheetDataLegs = NULL;
                        sheetData.legs_name = "unarmed";
                    }
                }
                if (item.first == Item_Component::Item_Type::chest) {
                    if (item.second == UI::Equipment_UI::emptyEquipSlot) {
                        sheetData.sheetDataChestpiece = NULL;
                        sheetData.chest_name = "unarmed";
                    }
                }
                if (item.first == Item_Component::Item_Type::helm) {
                    if (item.second == UI::Equipment_UI::emptyEquipSlot) {
                        sheetData.sheetDataHelm = NULL;
                        sheetData.helm_name = "unarmed";
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
				Graphics::Surface_Data statNameData = Graphics::Load_Text_Texture(Items::statName[stat.first], black);

				SDL_FRect statNameRect = statBox;
				statNameRect.y = statBox.y + currentRow;
				statNameRect.h = charHeight;
				statNameRect.w = Items::statName[stat.first].size() * charWidth;

				Graphics::Render_FRect(statNameData.pTexture, &statNameData.k, &statNameRect);
				SDL_DestroyTexture(statNameData.pTexture);
				//SDL_RenderDrawRect(Graphics::renderer, &rowRect);

				std::string statValue = std::to_string(stat.second);
				Graphics::Surface_Data statValueData = Graphics::Load_Text_Texture(statValue, black);

				SDL_FRect statValueRect = (statBox);
				statValueRect.y = statBox.y + currentRow;
				statValueRect.h = charHeight;
				statValueRect.w = statValue.size() * charWidth;
				statValueRect.x = statBox.x + statBox.w - statValueRect.w;


				Graphics::Render_FRect(statValueData.pTexture, &statValueData.k, &statValueRect);
				SDL_DestroyTexture(statValueData.pTexture);
				currentRow += charHeight;
			}
		}
	}

	void Init_Player_Stats(entt::registry& zone) { //run funtion on item equip or unequip
		auto view = zone.view<Component::Input, Component::Melee_Damage, Component::Health, Component::Attack_Speed>();
		for (auto entity : view) {
			auto& damage = view.get<Component::Melee_Damage>(entity);
			auto& health = view.get<Component::Health>(entity);
			auto& attackSpeed = view.get<Component::Attack_Speed>(entity);

			//set charcter stats to base values
			for (auto& stat : Items::statData) {
				stat.second = Items::baseStatData[stat.first];
			}

			//add equipment stats to charcter stats
			for (auto& item : UI::Equipment_UI::equippedItems) {
				if (item.second != UI::Equipment_UI::emptyEquipSlot) {
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

	void Init_UI(entt::registry& zone) {
		Items::Init_Item_Data();
		UI::Bag_UI::Create_Bag_UI(zone);
		UI::Equipment_UI::Create_Equipment_UI(zone);
		Init_Player_Stats(zone);
	}
}
