#pragma once
#include "entt.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "components.h"
#include "camera.h"


namespace Graphics {

	namespace {
		bool running;
		SDL_FRect resolution = { 0.0f, 0.0f, 1920.0f, 1200.0f };
		SDL_Window* window;
		SDL_FRect Screen;
		TTF_Font* font;

		entt::entity defaultIcon;

		struct Surface_Data {
			SDL_Texture* pTexture;
			SDL_Rect k;
		};
	}

	SDL_Renderer* renderer;

	const int numberOfTextures = 100;
	std::array<SDL_Texture*, numberOfTextures> unitTextures;

	//where the actual texture is stored
	std::map<std::string, SDL_Texture*>pTexture;

	std::array<SDL_Texture*, numberOfTextures> spellTextures;
	std::array<SDL_Texture*, numberOfTextures> environmentTextures;

	SDL_Texture* skeleton_0;
	SDL_Texture* skeleton_1;
	SDL_Texture* skeleton_mage_0;
	SDL_Texture* archer_0;
	SDL_Texture* house_0;
	SDL_Texture* grass_0;
	SDL_Texture* fireball_0;
	SDL_Texture* fireball_explosion_0;
	SDL_Texture* longsword_default;
	SDL_Texture* longsword_default_icon;
	SDL_Texture* itsmars_Inventory;
	SDL_Texture* cursor_0;
	SDL_Texture* icon_axe1;
    SDL_Texture* weapons_icons;
	SDL_Texture* default_icon;
	SDL_Texture* emptyBagIcon;

	SDL_Texture* bagSlotBorder;
	SDL_Texture* emptyEquippedIcon;
	SDL_Texture* equipSlotBorder;
	SDL_Texture* itemBorderCommon;
	SDL_Texture* itemBorderMagic;
	SDL_Texture* itemBorderRare;
    SDL_Texture* itemBorderEite;
	SDL_Texture* armorSpriteSheet;

	SDL_Texture* tooltipBackground;
	SDL_Texture* cow;


	void Render_Rect(SDL_Texture* pTexture, SDL_Rect& clipSprite, SDL_Rect& scaledSlot) {
		SDL_RenderCopy(renderer, pTexture, &clipSprite, &scaledSlot);
	}

	void Render_FRectToScreen(SDL_Texture* texture, const SDL_Rect* sourceRect, SDL_FRect* targetRect) {
		//targetRect->x -= Camera_Control::;

		SDL_RenderCopyF(renderer, texture, sourceRect, targetRect);
	}

	void Render_FRect(SDL_Texture* texture, const SDL_Rect* sourceRect, SDL_FRect* targetRect) {
		SDL_RenderCopyF(renderer, texture, sourceRect, targetRect);
	}

	Surface_Data Load_Text_Texture(std::string text, SDL_Color fontColor) {
		SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), fontColor);   //convert font to Surface
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);	        //convert Surface to texture

		SDL_Rect k = surface->clip_rect;


		Surface_Data text_data = { texture, k };
		SDL_FreeSurface(surface);														//free surface memory
		return text_data;																//return SDL_Texture *texture
	};

	void Create_Font() {
		font = TTF_OpenFont("fonts/Chomsky.otf", 30);
	}

	SDL_Texture* createTexture(const char* spritesheet) {
		SDL_Surface* surface = IMG_Load(spritesheet);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		return texture;
	}

	entt::entity Create_Icon_Entity(entt::registry& zone, SDL_Texture *iconImage, SDL_Texture *iconBorder) {
		auto icon_entity = zone.create();
		auto& icon = zone.emplace<Component::Icon>(icon_entity, iconImage, iconBorder);
		icon.clipSprite = { 0, 0, 100, 100 };
		icon.renderRectSize = { 48, 48 };
		icon.renderPositionOffset = { icon.renderRectSize.x / 2, icon.renderRectSize.y / 2 };
		return icon_entity;
	}


	//both unitID and filepath are stored in tiled map object
	void Load_Texture(int &unitID, const char* filepath) {
		unitTextures[unitID] = createTexture(filepath);
	}

	//when creating the game objet
	void Create_Game_Object(int& unitID, const char* filepath) {
		if (unitTextures[unitID] == NULL) {
			Load_Texture(unitID, filepath);
			if (unitTextures[unitID] == NULL) {
				std::cout << "failed to load from file: " << filepath << std::endl;
			}
			else {
			//	std::cout << "loaded from file: " << filepath << std::endl;
			}
		}
		else {
			//std::cout << "already loaded: " << filepath << std::endl;
            //unitTextures[unitID];
		}
		//to render it jsut needs access to the texture array and the unitID

	}

	void Load_Textures(entt::registry& zone) {
		Create_Font();

		skeleton_1 = createTexture("sprites/units/skeleton/skeleton_00.png");
		skeleton_0 = createTexture("sprites/units/skeleton/armoured_skeleton_00.png");

		archer_0 = createTexture("sprites/units/archer/archer_00.png");

		fireball_0 = createTexture("sprites/spells/fireball_0.png");
		fireball_explosion_0 = createTexture("sprites/spells/fireball_explosion_0.png");

        weapons_icons = createTexture("sprites/items/weaponIcons32x32_png_Transparent/weapon_icons_32_transparent.png");
		longsword_default = createTexture("sprites/items/long_sword/w_longsword.png");
		icon_axe1 = createTexture("sprites/items/weaponIcons32x32_png_Transparent/icon_axe1.png");
		//longsword_default_icon = createTexture("sprites/items/weaponIcons32x32_png_Transparent/icon_sword_long4.png");
		itsmars_Inventory = createTexture("sprites/UI/itsmars_Inventory.png");
		cursor_0 = createTexture("sprites/UI/cursor.png");


		emptyBagIcon = createTexture("sprites/UI/icon_borders/bagsSot.png"); // currently NULL
		bagSlotBorder = createTexture("sprites/UI/icon_borders/32x32_BagSlotBorder.png");

		emptyEquippedIcon = createTexture("sprites/UI/icon_borders/bagsSot.png");
		equipSlotBorder = createTexture("sprites/UI/icon_borders/81x81_EquipSlotBorder.png");

		itemBorderCommon = createTexture("sprites/UI/icon_borders/frame-0-grey.png");
		itemBorderMagic = createTexture("sprites/UI/icon_borders/frame-0-blue.png");
		itemBorderRare = createTexture("sprites/UI/icon_borders/frame-0-orange.png");
        itemBorderEite = createTexture("sprites/UI/icon_borders/frame-0-red.png");

		armorSpriteSheet = createTexture("sprites/items/flare_armor_transparent.png");

		tooltipBackground = createTexture("sprites/UI/tooltips/tooltipBackground2.png");
		cow = createTexture("sprites/units/animals/no_death_animation/cow/walk_sheet_192x192_8x8_alpha.png");


		default_icon = createTexture("sprites/default.jpg");
		defaultIcon = Create_Icon_Entity(zone, default_icon, Graphics::bagSlotBorder);
	}

	void createGraphicsContext(entt::registry& zone) {
		SDL_CreateWindowAndRenderer((int)resolution.w, (int)resolution.h, NULL, &window, &renderer);
		SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
		Load_Textures(zone);
	}

	void closeContext() {
		SDL_DestroyWindow(window);
		running = false;
	}



}
