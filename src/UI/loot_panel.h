#include "Input/mouse_control.h"
#include "graphics.h"
#include "tooltips.h"

namespace Loot_Panel {

	struct Loot_Panel {
		std::vector<entt::entity> *items = nullptr;
		float lootPanelSize = 64.0f;
		bool open = false;
		SDL_FRect interactPanel = {0, 0, 64.0f, 64.0f};
		SDL_FRect panel = {0, 0, 64.0f, 64.0f};
	} lootPanel;

	void Open() {
	}

	void Set_Loot(Component::Loot &lootDrops) {
		lootPanel.items = &lootDrops.items;
		lootPanel.open = false;
	}

	void Close() {
		lootPanel.items = nullptr;
		lootPanel.interactPanel = {0.0f, 0.0f, 0.0f, 0.0f};
	};

	bool Mouse_Inside_Panel(Component::Camera &camera, int &state) {
		if (lootPanel.items) {
			if (Mouse::bRect_inside_Cursor(lootPanel.interactPanel))
				return true;
		}
		return false;
	}

	bool loot_Item_To_Bag(entt::registry &zone, int &state, Component::Camera &camera, int &i) {
		auto view = zone.view<Component::Input>();
		for (auto entity: view) {
			auto &bag = zone.get<Component::Bag>(entity).bag;
			for (auto &slot: bag) {
				if (slot == Bag_UI::emptyBagSlot[state]) {
					slot = lootPanel.items->at(i);
					lootPanel.items->erase(lootPanel.items->begin() + i);
					zone.emplace_or_replace<Item_Component::Item_Pickup>(slot);
					//removes for main rendering loop
					zone.emplace_or_replace<Component::Inventory>(slot);
					return true;
				}
			}
			Utilities::Log("I am overburdened");
			return true;
		}
		return false;
	}

	bool Get_loot_Item(entt::registry &zone, int &state, Component::Camera &camera) {
		if (lootPanel.items) {
			if (!Mouse::mouseData.itemCurrentlyHeld) {
				SDL_FRect panel = lootPanel.panel;
				for (int i = 0; i < lootPanel.items->size(); ++i) {
					SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(panel, camera);
					if (Mouse::bRect_inside_Cursor(scaledSlot)) {
						//bag is open
						if (Bag_UI::bToggleCharacterUI) {
							Equipment_UI::Pick_Up_Item_To_Mouse(zone, lootPanel.items->at(i), Mouse::mouseData.itemCurrentlyHeld);
							//remove item from loot
							lootPanel.items->erase(lootPanel.items->begin() + i);
							return true;
						}
							//bag is closed
						else {
							if (loot_Item_To_Bag(zone, state, camera, i))
								return true;
						}
					}
					panel.y += lootPanel.lootPanelSize;
				}
			}
		}
		return false;
	}

	bool Get_loot_Item_To_Bag(entt::registry &zone, int &state, Component::Camera &camera) {
		if (lootPanel.items) {
			if (!Mouse::mouseData.itemCurrentlyHeld) {
				SDL_FRect panel = lootPanel.panel;
				for (int i = 0; i < lootPanel.items->size(); ++i) {
					SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(panel, camera);
					if (Mouse::bRect_inside_Cursor(scaledSlot)) {
						if (loot_Item_To_Bag(zone, state, camera, i))
							return true;
						else
							return false;
					}
					panel.y += lootPanel.lootPanelSize;
				}
			}
		}
		return false;
	}

	void Render_Loot(entt::registry &zone, int &state, Component::Camera &camera) {
		if (!lootPanel.open) {
			SDL_FPoint point = {Mouse::iXMouse, Mouse::iYMouse};
			point = Camera_Control::Revert_FPoint_To_Scale(point, camera);
			lootPanel.panel = {point.x - (lootPanel.lootPanelSize / 2.0f), point.y - (lootPanel.lootPanelSize / 2.0f), lootPanel.lootPanelSize, lootPanel.lootPanelSize};
			lootPanel.open = true;
		}
		//set panel to the top of the panel
		SDL_FRect panel = lootPanel.panel;

		if (lootPanel.items) {
			SDL_FRect scaledBackground = panel;
			scaledBackground.h = (float) lootPanel.items->size() * lootPanel.lootPanelSize;
			scaledBackground = Camera_Control::Convert_FRect_To_Scale(scaledBackground, camera);

			lootPanel.interactPanel = scaledBackground;
			SDL_RenderCopyF(Graphics::renderer, Graphics::tooltipBackground, nullptr, &scaledBackground);

			for (auto &item: *lootPanel.items) {
				auto &icon = zone.get<Component::Icon>(item);
				SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(panel, camera);

				SDL_SetTextureAlphaMod(icon.pBackground, 255);
				SDL_SetTextureAlphaMod(icon.pTexture, 255);
				SDL_SetTextureAlphaMod(icon.pIconRarityBorder, 255);
				SDL_RenderCopyF(Graphics::renderer, icon.pBackground, &icon.clipIcon, &scaledSlot);
				SDL_RenderCopyF(Graphics::renderer, icon.pTexture, &icon.clipSprite, &scaledSlot);
				SDL_RenderCopyF(Graphics::renderer, icon.pIconRarityBorder, &icon.clipIcon, &scaledSlot);

				//next slot down
				panel.y += lootPanel.lootPanelSize;
			}

			panel = lootPanel.panel;
			for (auto &item: *lootPanel.items) {
				SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(panel, camera);
				if (Mouse::bRect_inside_Cursor(scaledSlot)) {
					SDL_FPoint mouse = {Mouse::iXMouse, Mouse::iYMouse};
					Tooltip::Render_Tooltip(zone, item, state, mouse, camera);
				}
				panel.y += lootPanel.lootPanelSize;
			}
		}
	}
}// namespace Loot_Panel