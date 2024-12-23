//
// Created by desktop on 12/8/24.
//
#pragma once

#include "graphics.h"
#include "Button_Bar/button.h"

namespace Minimap {
    bool mapOpen = false;
    SDL_Texture *minimapTexture = nullptr;
    SDL_FRect minimapRect;
    int currentState;

    bool hovered;
    entt::entity selectedEntity;

    bool Toggle(Toggle_Type toggleType = Toggle_Type::toggle) {
	if (toggleType == Toggle_Type::get)
	    return mapOpen;

	if (toggleType == Toggle_Type::on) {
	    mapOpen = true;
	    return true;
	}
	if (toggleType == Toggle_Type::off) {
	    mapOpen = false;
	    return false;
	}

	if (mapOpen) {
	    mapOpen = false;
	    return false;
	}
	mapOpen = true;
	return true;
    }

    float Update_Minimap_Size(float cameraHeight) {
	auto minimapHeight = cameraHeight * 0.75f;
	minimapRect = {cameraHeight * 0.25f * 0.25f, cameraHeight * 0.25f * 0.25f, minimapHeight, minimapHeight};
	return minimapHeight;
    }

    void Draw_Map(int state, float cellSize) {
	if (currentState != state) {
	    if (minimapTexture) {
		SDL_DestroyTexture(minimapTexture);
		minimapTexture = nullptr;
	    }
	}

	if (minimapTexture) {
	    SDL_RenderCopyF(Graphics::renderer, minimapTexture, nullptr, &minimapRect);
	} else {
	    currentState = state;
	    //settarget texutre
	    minimapTexture = SDL_CreateTexture(Graphics::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int) (cellSize * World_Data::REGION_SIZE), (int) (cellSize * World_Data::REGION_SIZE));
	    SDL_SetRenderTarget(Graphics::renderer, minimapTexture);

	    for (int i = 0; i < World_Data::REGION_SIZE; i++) {
		for (int j = 0; j < World_Data::REGION_SIZE; j++) {
		    SDL_FRect renderRect;
		    renderRect.x = (float) i * cellSize;
		    renderRect.y = (float) j * cellSize;
		    renderRect.w = (float) cellSize;
		    renderRect.h = (float) cellSize;

		    SDL_Texture *texture = World::world[state].tileTextures[0];

		    if (Texture_Data::Packer_Textures.contains(World::world[state].tileset)) {
			auto clipRect = Texture_Data::Packer_Textures[World::world[state].tileset].frameList[Texture_Data::Packer_Textures[World::world[state].tileset].frameList.size() - 1].clip;
			SDL_RenderCopyF(Graphics::renderer, texture, &clipRect, &renderRect);
			clipRect = Texture_Data::Packer_Textures[World::world[state].tileset].frameList[World_Data::tilesEntities[0][i][j].tileTexture].clip;
			SDL_RenderCopyF(Graphics::renderer, texture, &clipRect, &renderRect);
		    } else {
			SDL_RenderCopyF(Graphics::renderer, texture, nullptr, &renderRect);
		    }
		}
	    }

	    SDL_SetRenderTarget(Graphics::renderer, nullptr);
	}
    }

    SDL_FRect Get_Render_Rect(float cellSize, Component::Position &position) {
	float tileSize = 512.0f;
	float objectSize = 32.0f;

	Component::Position miniMapPosition{};
	miniMapPosition.x = minimapRect.x + (position.x / tileSize) * cellSize;
	miniMapPosition.y = minimapRect.y + (position.y / tileSize) * cellSize;

	SDL_FRect rect = {miniMapPosition.x, miniMapPosition.y, objectSize, objectSize};
	auto scaledRect = Utilities::Maintain_Scale_Rect(rect, Graphics::Scale.x);

	return Utilities::Center_Rect(scaledRect);
    }

    void Draw_Object(entt::registry &zone, float cellSize) {
	auto view = zone.view<Component::Position, Component::Sprite_Icon>(entt::exclude<Component::Unit>);

	for (auto entity: view) {

	    auto spriteIcon = view.get<Component::Sprite_Icon>(entity);
	    auto &position = view.get<Component::Position>(entity);

	    SDL_FRect renderRect = Get_Render_Rect(cellSize, position);

	    if (!hovered && Mouse::FRect_inside_Screen_Cursor(renderRect)) {
		hovered = true;
		selectedEntity = entity;
	    }

	    SDL_RenderCopyF(Graphics::renderer, spriteIcon.texture, &spriteIcon.clipRect, &renderRect);
	}
    }

    void Draw_Units(entt::registry &zone, float cellSize) {
	auto view = zone.view<Component::Position, Component::Unit, Component::Sprite_Icon>();

	Component::Sprite_Icon player;
	SDL_FRect playerRect;

	for (auto entity: view) {
	    auto spriteIcon = view.get<Component::Sprite_Icon>(entity);
	    auto &position = view.get<Component::Position>(entity);

	    SDL_FRect renderRect = Get_Render_Rect(cellSize, position);

	    if (!hovered && Mouse::FRect_inside_Screen_Cursor(renderRect)) {
		hovered = true;
		selectedEntity = entity;
	    }

	    if (zone.any_of<Component::Input>(entity)) {
		player = spriteIcon;
		playerRect = renderRect;
		continue;
	    }

	    SDL_RenderCopyF(Graphics::renderer, spriteIcon.texture, &spriteIcon.clipRect, &renderRect);
	}

	//render player on top of all others
	SDL_RenderCopyF(Graphics::renderer, player.texture, &player.clipRect, &playerRect);
    }

    void Draw_Tooltip(entt::registry &zone) {
	if (hovered) {
	    auto name = zone.get<Component::Name>(selectedEntity).first;

	    std::array<std::string, 6> formattedDesc = {
		    name,
	    };

	    float lineHeight = FC_GetHeight(Graphics::fcFont, "%s", "A");
	    std::array<float, 6> spacing = {
		    lineHeight * 0.0f,
	    };

	    Tooltips::Properties<6> tooltipProperties = {
		    formattedDesc,
		    spacing,
		    550.0f,
		    20.0f,
		    10.0f,
		    Tooltips::MOUSE_TOP_RIGHT
	    };

	    Tooltips::Create_Tooltip(tooltipProperties);
	}
    }

    void Render(entt::registry &zone, int state, Component::Camera &camera) {
	if (mapOpen) {
	    auto cellSize = (Update_Minimap_Size(camera.screen.h) / World_Data::REGION_SIZE);

	    hovered = false;
	    Draw_Map(state, cellSize);
	    //Draw_Terrain_Objects
	    Draw_Object(zone, cellSize);
	    Draw_Units(zone, cellSize);
	    Draw_Tooltip(zone);
	}
    }
}