#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "graphics.h"
#include "timer.h"
#include "interface.h"
#include "utilities.h"
#include "items.h"
#include "ui.h"
#include "camera.h"
#include "mouse_control.h"
#include "components.h"
#include "character_stats.h"
#include "tooltips.h"
#include "damage_text.h"
#include "ui_resources.h"
#include "map.h"
#include "world_grid.h"
#include "dynamic_quad_tree.h"
#include "player_control.h"

namespace Rendering {
	namespace {
		bool showSpriteBox = false;
		bool renderType = true;
		bool debug = false;
		float fRenderable = 0.0f;
	}

	struct tileData {
		SDL_Texture* texture;
		SDL_Rect tileSpriteRect;
	};

	void sort_Positions(entt::registry &zone) {
		zone.sort<Component::Renderable>([](const auto& lhs, const auto& rhs) { return lhs.y < rhs.y; }); //sorts position least to
	}

    int PVG_Direction_Enum(Component::Direction &direction) {
        switch (direction) {
            case Component::Direction::N: return 3;
            case Component::Direction::S: return 0;
            case Component::Direction::E: return 2;
            case Component::Direction::W: return 1;
            case Component::Direction::NW: return 5;
            case Component::Direction::NE: return 7;
            case Component::Direction::SW: return 4;
            case Component::Direction::SE: return 6;
        }
    }

    SDL_Rect Update_Frame_PVG(SDL_Rect &frame, Component::Frame_Data &frameData, Component::Direction &direction, Component::spriteframes &sheet) {
        int spritesPerLine = sheet.sheetWidth/sheet.clip.w;
            ///the x index in absolute terms
        int x_index = sheet.frameStart + ((PVG_Direction_Enum(direction) * frameData.NumFrames) + frameData.currentFrame);
            ///the x index in relative terms
        int x_rel_index = x_index % spritesPerLine;
            ///the x index in pixel terms
        frame.x = x_rel_index * sheet.clip.w;
            ///after dividing away the x index, add the remainder
        int y_index = x_index / spritesPerLine;
            ///the y index in pixel terms
        frame.y = y_index * sheet.clip.w;
        return frame;
    }

    int Original_Direction_Enum(Component::Direction &direction) {
        switch (direction) {
            case Component::Direction::N: return 2;
            case Component::Direction::S: return 6;
            case Component::Direction::E: return 4;
            case Component::Direction::W: return 0;
            case Component::Direction::NW: return 1;
            case Component::Direction::NE: return 3;
            case Component::Direction::SW: return 7;
            case Component::Direction::SE: return 5;
        }
    }

    SDL_Rect Update_Frame_Flare (SDL_Rect &frame, Component::Frame_Data &frameData, Component::Direction& direction, Component::spriteframes& spritesheet) {
        frame.x = (spritesheet.frameStart + frameData.currentFrame) * frame.w;
        frame.y = frame.h * Original_Direction_Enum(direction);
        return frame;
    }

    SDL_Rect Get_Spritesheet_Type(SDL_Rect &frame, Component::Sprite_Sheet_Info &animation, Component::Direction& direction, Component::Actions& act) {
        if (animation.type == "flare"){
            frame = Update_Frame_Flare(frame, act.frameCount[act.action], direction, animation.sheet[act.action]);
        }
        else if (animation.type == "pvg"){
            frame = Update_Frame_PVG(frame, act.frameCount[act.action], direction, animation.sheet[act.action]);
        }
//        else if (animation.type == "RPG_Tools") {
////            Frame_Increment(sheetData, action, direction);
//        }
        else {
            //Utilities::Log("Error! Frame_Update() could not find animation type!");
                ///maybe have a special return value to make this more obvious
            frame = Update_Frame_Flare(frame, act.frameCount[act.action], direction, animation.sheet[act.action]);
            return frame;
        }
        return frame;
    }

    bool Death_Sequence (Component::Actions& act) {
        if (act.action == Component::dead) {
            if (act.frameCount[act.action].currentFrame < act.frameCount[act.action].NumFrames - 1) {
                act.frameCount[act.action].currentFrame++;
            }
            return true;
        }
        return false;
    }

    SDL_Rect Update_Frame(Component::Sprite_Sheet_Info &animation, Component::Direction& direction, Component::Actions& act) {
        SDL_Rect frame = animation.sheet[act.action].clip;
        if (act.action != Component::isStatic) {            ;
            Component::spriteframes &spritesheet = animation.sheet[act.action];
            auto &action = act.frameCount[act.action];
            if (act.action == Component::attack) {
                Utilities::Log(action.currentFrame);
            }
            frame = Get_Spritesheet_Type(frame, animation, direction, act);
            if (Death_Sequence(act)) {
                return frame;
            }
                ///render first frame before increment
            if (action.currentFrame == 0) {
                action.currentFrame++;
                spritesheet.bReversing = false;
                return frame;
            }
            if (spritesheet.bReversing == false) {
                act.frameCount[act.action].currentFrame++;
            }
            if (spritesheet.bReversable) {
                if (spritesheet.bReversing == true) {
                    action.currentFrame--;
                }
                    /// -1 becausee of the zero index
                if (action.currentFrame >= action.NumFrames - 1){
                    spritesheet.bReversing = true;
                }
            }
            else if (action.currentFrame >= action.NumFrames) {
                act.frameCount[act.action].currentFrame = 0;
                if (act.action != Component::walk) {
                    act.action = Component::idle;
                }
            }
        }
        return frame;
    }

	SDL_FRect Scale_Sprite_for_Render(SDL_Rect& clippedSprite, float& scale) {
		SDL_FRect fScaledImage = Utilities::SDL_Rect_To_SDL_FRect(clippedSprite);
		fScaledImage = {
			fScaledImage.x - (fScaledImage.w * scale),
			fScaledImage.y - (fScaledImage.h * scale),
			fScaledImage.w * scale,
			fScaledImage.h * scale
		};
		return fScaledImage;
	}


    SDL_Rect Frame_Increment(Component::Sprite_Sheet_Info &sheetData, Component::Actions &action, Component::Direction &direction) {
        sheetData.frameTime += Timer::timeStep;
        if (sheetData.frameTime >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.action].frameSpeed) {
            ///reset frame count if over
            sheetData.frameTime = 0;
            int &currentFrame = action.frameCount[action.action].currentFrame;
            int &startFrame = sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.action].startFrame;
            int &numFrames = sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.action].NumFrames;
            sheetData.frameIndex = startFrame + (numFrames * PVG_Direction_Enum(direction)) + currentFrame;
            ///calculate reversing
            if (sheetData.reversing) {
                if (currentFrame <= 1) {
                    sheetData.reversing = 0;
                }
                currentFrame--;
            }
            else {
                currentFrame++;
            }
            if (currentFrame >= numFrames) {
                currentFrame = 0;
                if (sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.action].reverses) {
                    sheetData.reversing = 1;
                }
                else {
                    action.frameCount[action.action].currentFrame = 0;
                    if (action.action != Component::walk) {
                        action.action = Component::idle;
                    }
                }
            }
        }
    }

    SDL_FRect Position_For_Render(Component::Sprite_Sheet_Info &sheetData, Component::Position &position, Component::Camera &camera, Component::Scale &scale, Component::Sprite_Offset &offset, SDL_Rect &clipRect, SDL_FRect &renderRect) {
        clipRect = sheetData.sheetData->at(sheetData.sheet_name).frameList[sheetData.frameIndex].clip;
        renderRect = Scale_Sprite_for_Render(clipRect, scale.scale);
        renderRect.x = sheetData.sheetData->at(sheetData.sheet_name).frameList[sheetData.frameIndex].x_offset - offset.x + position.x - camera.screen.x;
        renderRect.y = sheetData.sheetData->at(sheetData.sheet_name).frameList[sheetData.frameIndex].y_offset - offset.y + position.y - camera.screen.y;
        return renderRect;
    }

//    void Texture_Packer_Render(entt::registry &zone, Component::Camera& camera) {
//        auto view = zone.view<Component::Renderable, Component::Position, Texture_Packer::Packer_Sheet_Data, Component::Direction, Component::Actions, Component::Sprite_Offset, Component::Scale>();
//        for (auto entity : view) {
//            auto [renderable, position, sheetData, direction, action, offset, scale] = view.get(entity);
//            SDL_Rect clipRect;
//            SDL_FRect renderRect;
//
//            Frame_Increment(sheetData, action, direction);
//            renderRect = Position_For_Render(sheetData, position, camera, scale, offset, clipRect, renderRect);
//            SDL_SetTextureAlphaMod(sheetData.sheetData->at(sheetData.sheet_name).texture, renderable.alpha);
//
//            Graphics::Render_FRect(sheetData.sheetData->at(sheetData.sheet_name).texture, &clipRect, &renderRect);
//        }
//    }


    void Animation_Frame(entt::registry& zone, Component::Camera &camera) { //state
		auto view1 = zone.view<Component::Renderable, Component::Position, Component::Sprite_Sheet_Info, Component::Actions, Component::Direction, Component::Sprite_Offset, Component::Scale, Component::Entity_Type>();
		for (auto entity : view1) {
            auto [renderable, position, sheetData, action, direction, spriteOffset, scale, type] = view1.get(entity);
            SDL_Rect clipRect;
            SDL_FRect renderRect;
            SDL_Texture* texture;
			    ///only fire this at 60 frames/sec
			if (sheetData.type != "RPG_Tools") {
                sheetData.sheet[action.action].currentFrameTime += Timer::timeStep;
                if (sheetData.sheet[action.action].currentFrameTime >= sheetData.sheet[action.action].timeBetweenFrames) {
                    sheetData.sheet[action.action].currentFrameTime = 0;
                    sheetData.clipSprite = Update_Frame(sheetData, direction, action);
                    sheetData.renderPosition = Scale_Sprite_for_Render(sheetData.clipSprite, scale.scale);
                }
                sheetData.renderPosition.x = (position.x - camera.screen.x - spriteOffset.x);
                sheetData.renderPosition.y = (position.y - camera.screen.y - spriteOffset.y);

                if (type == Component::Entity_Type::item){
                    sheetData.renderPosition.w = (spriteOffset.x * 2.0f);
                    sheetData.renderPosition.h = (spriteOffset.y * 2.0f);
                }
                    ///fade rendering objects at bottom of screen
                clipRect = sheetData.clipSprite;
                renderRect = sheetData.renderPosition;
                SDL_SetTextureAlphaMod(sheetData.texture, renderable.alpha);
                texture = sheetData.texture;
            }
            else {
                Frame_Increment(sheetData, action, direction);
                renderRect = Position_For_Render(sheetData, position, camera, scale, spriteOffset, clipRect, renderRect);
                texture = sheetData.sheetData->at(sheetData.sheet_name).texture;
            }
            SDL_SetTextureAlphaMod(texture, renderable.alpha);
			Graphics::Render_FRect(texture, &clipRect, &renderRect);
		}
	}

	SDL_Rect Explosion_Frame_Update(Component::Sprite_Frames &frame, SDL_Rect frameToUpdateClipOf) {
		//reset X to zero and increment Y
		if (frame.frameX >= 8) {
			frame.frameX = 0;
			frame.frameY++;
		}
		// set Output Rect values
		int row = frame.frameX * 128;
		int column = frame.frameY * 128;
		int width = 128;
		int height = 128;
		SDL_Rect rect = { row, column, width, height };
		//increment X
		frame.frameX++;
		return rect;
	}

	void Render_Explosions(entt::registry& zone, Component::Camera& camera) {
		SDL_Rect xClipPos;
		float sx;
		float sy;
		auto view = zone.view<Component::Explosion, Component::Position, Component::Frame_Delay, Component::Texture, Component::Sprite_Frames>();
		for (auto spell : view) {
			auto& anim = view.get<Component::Explosion>(spell);
			auto& x = view.get<Component::Position>(spell);
			auto& y = view.get<Component::Position>(spell);
			auto& texture = view.get<Component::Texture>(spell);
			auto& frames = view.get<Component::Sprite_Frames>(spell);
			auto& delay = view.get<Component::Frame_Delay>(spell);
			delay.currentFrameTime += Timer::timeStep;
			if (delay.currentFrameTime >= delay.timeBetweenFrames) {
				if (frames.currentFrame <= frames.maxFrames) { // if there are still frames remaining
				//only fire this at 60 frames/sec
					xClipPos = Explosion_Frame_Update(frames, texture.clippedSpriteFrame);		//get action and direction state sprite draw from
					anim.renderPosition = Utilities::SDL_Rect_To_SDL_FRect(xClipPos);		//save sprite for vector
					texture.clippedSpriteFrame = xClipPos;									//save position for renderer
					frames.currentFrame++;
				}
				else {
					//remove explosion from scene and free the entity
					zone.destroy(spell);
				}
				delay.currentFrameTime = 0;
			}
			sx = x.x - camera.screen.x - anim.offsetToAlignWithFireball.x;
			sy = y.y - camera.screen.y - anim.offsetToAlignWithFireball.y;
			anim.renderPosition.x = sx - anim.positionOffset.x;
			anim.renderPosition.y = sy - anim.positionOffset.y;
			SDL_RenderCopyF(Graphics::renderer, texture.pTexture, &texture.clippedSpriteFrame, &anim.renderPosition);
			if (showSpriteBox) {
				SDL_RenderDrawRectF(Graphics::renderer, &anim.renderPosition);
			}
		}
	}


	SDL_Rect getTexture(uint32_t x, int &sheetWidth, SDL_Rect &tileSpriteRect) {
		int y = 0;
		x -= 1;
		while (x >= sheetWidth) {
			x -= sheetWidth;
			y++;
		}
		tileSpriteRect.x = tileSpriteRect.w * x;
		tileSpriteRect.y = tileSpriteRect.h * y;
		return tileSpriteRect;
	}

	void Render_Map(entt::registry &zone, tmx::Map& map, Component::Camera& camera) {
		float originX = 0.0f;
		float originY = 0.0f;
		SDL_Rect tileSpriteRect = { 0, 0, (int)map.getTileSize().x , (int)map.getTileSize().y };
		SDL_FRect renderPosition = { 0.0f, 0.0f, (float)map.getTileSize().x, (float)map.getTileSize().y };
		auto& numOfTiles = map.getTileCount();
		auto& layers = map.getLayers();
		const auto tileWidth = (float)map.getTileSize().x;
		const auto tileHeight = (float)map.getTileSize().y;
		int newX = (int)camera.screen.x;
		int newY = (int)camera.screen.y;
		int X = (newX / tileWidth) + (newY / tileHeight) - (camera.screen.w / 2 / (tileWidth / 2));
		int Y = (newY / tileHeight) - (newX / tileWidth) - (camera.screen.h / 2 / (tileHeight / 2));
		//int X = (newX / tileWidth) + (newY / tileHeight) - (camera.screen.h / 2 / (tileHeight / 2));
		//int Y = (newY / tileHeight) - (newX / tileWidth) - (camera.screen.w / 2 / (tileWidth / 2));
		//int X = ((newX + newY) / tileWidth) - (camera.screen.w / 2 / (tileWidth / 2));
		//int Y = ((newY - newX) / tileHeight) - (camera.screen.h / 2 / (tileHeight / 2));
		//int X = (newY + newX) / tileWidth / 2 - camera.screen.w / 2 / (tileWidth /2);
		//int Y = (newY - newX) / tileWidth / 2 - camera.screen.h / 2 / (tileHeight/2);
		//of the sidways rect encompassing the screen
		int width = X + (camera.screen.w / tileWidth * 4);
		int height = Y + (camera.screen.h / tileHeight * 3);
		//int width = X + ((camera.screen.w + camera.screen.h) / tileWidth / 2);
		//int height = Y + ((camera.screen.h + camera.screen.w) / tileHeight / 2);
		int h = 0;
		int g = 0;
		//int o = 0;
		auto &tilesets = Maps::map.getTilesets();
		int p = Maps::map.getTilesets().size() - 1;
		for (int i = 0; i < layers.size(); i++) {
			if (layers[i]->getType() == tmx::Layer::Type::Tile) {
				const auto& tiles = layers[i]->getLayerAs<tmx::TileLayer>().getTiles();
				for (int x = X; x < width; x++) {
					if (x < 0) { x = 0; }
					for (int y = Y; y < height; y++) {
						//h++;
						if (y < 0) { y = 0; }
						if (x >= numOfTiles.x) { break; }
						if (y >= numOfTiles.y) { break; }
						renderPosition.x = originX + (x * tileWidth / 2.0f) - (y * tileWidth / 2.0f);
						renderPosition.y = originY + (x * tileHeight / 2.0f) + (y * tileHeight / 2.0f);
						renderPosition.x -= camera.screen.x;;
						renderPosition.y -= camera.screen.y;;
						int k = (numOfTiles.x * y) + x;
						if (tiles[k].ID != 0) {
							auto id = tiles[k].ID;
							for (int tilesetCount = p; tilesetCount >= 0; --tilesetCount) {
								const tmx::Tileset *tileset = &tilesets[tilesetCount];
								if (tileset->getFirstGID() - 1 <= id) {
									id -= tilesets[tilesetCount].getFirstGID() - 1;
									std::string name = tileset->getName();
									int width = tileset->getColumnCount();
									SDL_Rect data = getTexture(id, width, tileSpriteRect);
									if (Graphics::pTexture[name] != NULL) {
										Graphics::Render_FRect(Graphics::pTexture[name], &data, &renderPosition);
									}
									//h++;
									break;
								}
							}
						}
					}
					g++;
				}
				if (i == 1) {
					Animation_Frame(zone, camera);
//					Texture_Packer_Render(zone, camera);
					Render_Explosions(zone, camera);
				}
			}
		}
		int ks = h + g;
	}

	void Render_Mouse_Item(entt::registry& zone, Component::Camera &camera) {
		SDL_FRect DisplayRect = {};
		auto view = zone.view<Component::Position, Component::Icon, Component::On_Mouse>();
		for (auto item : view) {
			const auto& icon = view.get<Component::Icon>(item);
			const auto& x = view.get<Component::Position>(item).x;
			const auto& y = view.get<Component::Position>(item).y;
			DisplayRect.x = (x - camera.screen.x) - (icon.renderPositionOffset.x / camera.scale.x);
			DisplayRect.y = (y - camera.screen.y) - (icon.renderPositionOffset.y / camera.scale.y);
			DisplayRect.w = icon.renderRectSize.x / camera.scale.x;
			DisplayRect.h = icon.renderRectSize.y / camera.scale.y;
			//std::cout << "x: " << DisplayRect.x << " y: " << DisplayRect.y << " w: " << DisplayRect.w << " h: " << DisplayRect.h << std::endl;
			SDL_RenderCopyF(Graphics::renderer, icon.pTexture, &icon.clipSprite, &DisplayRect);
			SDL_RenderCopyF(Graphics::renderer, icon.pIconBorder, &icon.clipSprite, &DisplayRect);
			if (showSpriteBox) {
				//SDL_RenderDrawRect(Graphics::renderer, &DisplayRect);
			}
		}
	}

	void RenderCullMode(entt::registry& zone) {
		if (renderType == true) {
			renderType = false;
			zone.clear<Component::Renderable>();
			}
		else {
			renderType = true;
			zone.clear<Component::Renderable>();
		}
	}

	int Set_Render_Position_Alpha(float& screenEdge, float& renderEdge, float& yPosition) {
		if (yPosition <= screenEdge) {
			return 255;
		}
		else {
			float x = yPosition - screenEdge;
			float edgeBuffer = renderEdge - screenEdge;
			float y = edgeBuffer / x;
			int alpha = 255 - ( 255 / (int)y );
			return alpha;
		}
	}

	void Add_Remove_Renderable_Component(entt::registry &zone, Component::Camera &camera) {
		int j = 0;
		SDL_FRect renderRect = {
			camera.screen.x - (camera.screen.w / 2.0f),
			camera.screen.y - (camera.screen.h / 2.0f),
			camera.screen.w * 2.0f,
			camera.screen.h * 2.0f };
		auto objectsView = zone.view<Component::Position>(entt::exclude<Component::Inventory>);
		float bottomOfScreenEdge = camera.screen.y + camera.screen.h;
		float bottomOfRenderRect = renderRect.y + renderRect.h;
		for (auto entity : objectsView) {
			auto& position = objectsView.get<Component::Position>(entity);
			SDL_FPoint point = {position.x, position.y};
			if (Utilities::bFPoint_FRectIntersect(point, renderRect)) {
				int alpha = Set_Render_Position_Alpha(bottomOfScreenEdge, bottomOfRenderRect, position.y);
				zone.emplace_or_replace<Component::Renderable>(entity, position.y, alpha);
				j++;
			}
			else {
				zone.remove<Component::Renderable>(entity);
			}
		}
	}

	void Check_Renderable(entt::registry &zone) { //doesn't need to happen every frame
		fRenderable += Timer::timeStep;
		if (fRenderable >= 0) {
			fRenderable = 0;
			if (renderType) {
//				Add_Remove_Renderable_Component(zone, camera);
			}
			else {
			//	Vector_Renderable(Map::terrain);
			}
		}
	};

	void Update_Cursor(Component::Camera& camera) {
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		Mouse::iXMouse = (float)mx;
		Mouse::iYMouse = (float)my;
		Mouse::iXWorld_Mouse = (Mouse::iXMouse / camera.scale.x) + camera.screen.x;//getting mouse world Position corrected for scale
		Mouse::iYWorld_Mouse = (Mouse::iYMouse / camera.scale.y) + camera.screen.y;//getting mouse world Position corrected for scale
		Mouse::iXMouse = Mouse::iXMouse / camera.scale.x;  // getting the screen mouse position corrected for scale
		Mouse::iYMouse = Mouse::iYMouse / camera.scale.y;  // getting the screen mouse position corrected for scale
		Mouse::mousePoint = { (float)mx, (float)my };
		Mouse::screenMousePoint = Camera_Control::Convert_FPoint_To_Scale(Mouse::mousePoint, camera);
	}

	void Update_Camera_And_Mouse(entt::registry& zone) {
		auto focus_view = zone.view<Component::Camera, Component::Position>();
		for (auto player : focus_view) {
			auto& camera = focus_view.get<Component::Camera>(player);
			auto& position = focus_view.get<Component::Position>(player);
			Camera_Control::Update_Camera_Follow(camera, position, Graphics::resolution);
			Update_Cursor(camera);
		}
	}

	void Remove_Entities_From_Registry(entt::registry& zone) {
		auto view = zone.view<Component::Destroyed>(entt::exclude<Component::In_Object_Tree>);
		for (auto entity : view) {
			zone.destroy(entity);
		}
	}

	void Rendering(entt::registry& zone) {
		Update_Camera_And_Mouse(zone);
		SDL_FPoint mouse = { Mouse::iXMouse, Mouse::iYMouse };
		UI::Move_To_Item_Routine(zone, Mouse::itemCurrentlyHeld);
		Player_Control::Move_To_Atack_Routine(zone);
		Character_Stats::Update_Unit_Stats(zone);
		auto camera_view = zone.view<Component::Camera>();
		for (auto entity : camera_view) {
			auto& camera = camera_view.get<Component::Camera>(entity);
			SDL_RenderClear(Graphics::renderer);
			Add_Remove_Renderable_Component(zone, camera);
			sort_Positions(zone);
			Render_Map(zone, Maps::map, camera);
			Dynamic_Quad_Tree::Update_Quad_Tree_Positions(World::zone);
			Dynamic_Quad_Tree::Emplace_Objects_In_Quad_Tree(World::zone);
			Remove_Entities_From_Registry(zone); // cannot be done before clearing the entities from the quad tree
			Dynamic_Quad_Tree::Remove_From_Tree(zone);
			//Dynamic_Quad_Tree::Draw_Tree_Object_Rects(zone);
			Items::Show_Ground_Items(zone, camera);
            Items::Name_On_Mouseover(zone, camera);
			UI::Render_UI(zone, Graphics::renderer, camera);
			Character_Stats::Render_Character_Stats(zone, camera);
            Items::Update_Mouse_Slot_Position(zone, Mouse::mouseItem, Mouse::itemCurrentlyHeld, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
			Damage_Text::Show_Damage(zone, camera);
            UI_Resources::Render_Resources(zone, camera);
            UI_Spellbook::Draw_Spellbook();
            Menu::Render();
            //Mouse
            Interface::Run_Interface(zone, camera);
            //on top of mouse
            Tooltip::Show_Item_Tooltip(zone, mouse, camera);
            Render_Mouse_Item(zone, camera);
			SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            if (Items::showGroundItems == true) {                //****//search quad tree instead
                auto view = zone.view<Ground_Item, Component::Renderable>();
                for (auto item : view) {
                    auto &box =zone.get<Ground_Item>(item);
                    SDL_RenderDrawRectF(Graphics::renderer, &box.box);
                }
            }
            SDL_RenderPresent(Graphics::renderer);

		}
	}
}
