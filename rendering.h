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
		zone.sort<Renderable>([](const auto& lhs, const auto& rhs) { return lhs.y < rhs.y; }); //sorts position least to
	}

	int Original_Direction_Enum(Direction &direction) {
		switch (direction) {
		case Direction::N: return 2;
		case Direction::S: return 6;
		case Direction::E: return 4;
		case Direction::W: return 0;
		case Direction::NW: return 1;
		case Direction::NE: return 3;
		case Direction::SW: return 7;
		case Direction::SE: return 5;
		}
	}

	SDL_Rect Frame_Update(spriteframes& spritesheet, Direction& direction, Actions& act) {

		SDL_Rect frame = spritesheet.clip;

		int direcitonIndex = Original_Direction_Enum(direction);
		if (act.action != isStatic) {




			frame.y = frame.h * direcitonIndex; //check which directioon is facing then change clip.y to sprite height x direction enum

			if (frame.x > spritesheet.frameStart + spritesheet.sheetWidth - frame.w) {
				frame.x = spritesheet.frameStart;
			}
			if (act.action != dead) {
				if (act.action == struck) {
					if (act.frameCount[act.action].currentFrame > act.frameCount[act.action].NumFrames) {
						act.frameCount[act.action].currentFrame = 0;
						act.action = idle;
						return frame;
					}
					else {
						act.frameCount[act.action].currentFrame++;
						return frame;
					}
				}

				if (spritesheet.bReversable) {
					if (spritesheet.bReversing == true) {
						frame.x += (frame.w * act.frameCount[act.action].currentFrame);
						//std::cout << "forward :" << x.x << std::endl;
						if (frame.x >= spritesheet.frameStart + spritesheet.sheetWidth - frame.w) { spritesheet.bReversing = false; };
							act.frameCount[act.action].currentFrame++;
							return frame;
						}
					else if (spritesheet.bReversing == false) {
					frame.x = (frame.x + (frame.w * act.frameCount[act.action].currentFrame)) - frame.w;
					//std::cout << "reversing :" << x.x << std::endl;
					if (frame.x <= spritesheet.frameStart) { spritesheet.bReversing = true; };
					act.frameCount[act.action].currentFrame--;
					return frame;
					}
				}

				if (!spritesheet.bReversable) {
					frame.x += (frame.w * act.frameCount[act.action].currentFrame);
					//std::cout << "forward :" << x.x << std::endl;
					if (frame.x > spritesheet.frameStart + spritesheet.sheetWidth - frame.w) {
						frame.x = spritesheet.frameStart;
					}
					if (act.frameCount[act.action].currentFrame >= act.frameCount[act.action].NumFrames) {
						act.frameCount[act.action].currentFrame = 0;
						return frame;
					}
					else {
						act.frameCount[act.action].currentFrame++;
						return frame;
					}
				}
			}
		}

		if (act.action == dead) {
			if (act.frameCount[act.action].currentFrame < act.frameCount[act.action].NumFrames) {
				act.frameCount[act.action].currentFrame++;
			}
			frame.x += (frame.w * act.frameCount[act.action].currentFrame);
		}
		return frame;
	}


	SDL_FRect Scale_Sprite_for_Render(SDL_Rect& clippedSprite, const float& scale) {
		SDL_FRect fScaledImage = Utilities::SDL_Rect_To_SDL_FRect(clippedSprite);
		fScaledImage = {
			fScaledImage.x - (fScaledImage.w * scale),
			fScaledImage.y - (fScaledImage.h * scale),
			fScaledImage.w * scale,
			fScaledImage.h * scale
		};

		return fScaledImage;
	}

	void Animation_Frame(entt::registry& zone, Component::Camera &camera) { //state

		SDL_Rect xClipPos;
		float sx;
		float sy;
		auto view1 = zone.view<Renderable, Position, animation, Actions, Direction, Sprite_Offset, Scale, Entity_Type>();

		for (auto entity : view1) {
			auto& alpha = view1.get<Renderable>(entity).alpha;
			auto& d = view1.get<Direction>(entity);
			auto& scale = view1.get<Scale>(entity).scale;
			auto& anim = view1.get<animation>(entity);
			auto& x = view1.get<Position>(entity);
			auto& y = view1.get<Position>(entity);
			auto& act = view1.get<Actions>(entity);
			auto& type = view1.get<Entity_Type>(entity);
			auto& spriteOffset = view1.get<Sprite_Offset>(entity).offset;
			//only fire this at 60 frames/sec
			anim.sheet[act.action].currentFrameTime += Timer::timeStep;
			if (anim.sheet[act.action].currentFrameTime >= anim.sheet[act.action].timeBetweenFrames) {
				anim.sheet[act.action].currentFrameTime = 0;
				xClipPos = Frame_Update(anim.sheet[act.action], d, act);//get action and direction state sprite draw from
				anim.renderPosition = Scale_Sprite_for_Render(xClipPos, scale);										//save sprite for vector
				anim.clipSprite = xClipPos;											//save position for renderer
			}
			sx = x.x - camera.screen.x;
			sy = y.y - camera.screen.y;

			anim.renderPosition.x = (sx - spriteOffset.x);
			anim.renderPosition.y = (sy - spriteOffset.y);
			if (type == Entity_Type::item){
				anim.renderPosition.w = (spriteOffset.x * 2.0f);
				anim.renderPosition.h = (spriteOffset.y * 2.0f);
			}
			//fade rendering objects at bottom of screen
			SDL_SetTextureAlphaMod(anim.pTexture, alpha);

			Graphics::Render_FRect(anim.pTexture, &anim.clipSprite, &anim.renderPosition);
			if (showSpriteBox) {
				//SDL_RenderDrawRect(Graphics::renderer, &anim.renderPosition);
			}

		}
	}

	SDL_Rect Explosion_Frame_Update(Sprite_Frames &frame, SDL_Rect frameToUpdateClipOf) {
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
		auto view = zone.view<Explosion, Position, Frame_Delay, Texture, Sprite_Frames>();

		for (auto spell : view) {
			auto& anim = view.get<Explosion>(spell);
			auto& x = view.get<Position>(spell);
			auto& y = view.get<Position>(spell);
			auto& texture = view.get<Texture>(spell);
			auto& frames = view.get<Sprite_Frames>(spell);
			auto& delay = view.get<Frame_Delay>(spell);

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

	void Texture_Packer_Render(entt::registry &zone, Camera& camera) {
		auto view = zone.view<Position, Texture, Sprite_Vector, Direction, Action_State>();
		for (auto lion : view) {
			auto &position = view.get<Position>(lion);
			SDL_Texture *texture = view.get<Texture>(lion).pTexture;
			const Action_State &actionState = view.get<Action_State>(lion);
			const Direction &direction = view.get<Direction>(lion);

			Sprite_Sheet_Data* clippedSprite = view.get<Sprite_Vector>(lion).sheet;
			Frame *frameDataArray = view.get<Sprite_Vector>(lion).states;
			int &currentFrame = view.get<Sprite_Vector>(lion).currentFrame;
			const int& frameTime= view.get<Sprite_Vector>(lion).frameTime;
			int& frameTimeCount= view.get<Sprite_Vector>(lion).FrameTimeCount;
			SDL_Rect& clip = view.get<Sprite_Vector>(lion).clip;
			SDL_FRect& render = view.get<Sprite_Vector>(lion).render;

			frameTimeCount += Timer::timeStep;
			if (frameTimeCount >= frameTime) {
				frameTimeCount = 0;

				Frame frameData = frameDataArray[actionState];

				int startFrame = frameData.startFrame + (frameData.numFrames * (int)direction);
				int endFrame = startFrame + frameData.numFrames;


				if (currentFrame < startFrame) {
					currentFrame = startFrame;
				}
				if (actionState != Action_State::dead) {
					if (currentFrame >= endFrame) {
						currentFrame = startFrame;
					}
				}
				if (actionState != Action_State::dead && currentFrame != endFrame) {
					currentFrame++;
				}
			}
			auto& sprite = clippedSprite[currentFrame - 1];

			clip = sprite.clip;
			SDL_FRect offset = Utilities::SDL_Rect_To_SDL_FRect(sprite.offset);
			render = { position.x + offset.x, position.y + offset.y, (float)clip.w, (float)clip.h };

			render.x -= camera.screen.x;
			render.y -= camera.screen.y;

			SDL_RenderCopyF(Graphics::renderer, texture, &clip, &render);


			SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 0, 255);
			SDL_RenderDrawRectF(Graphics::renderer, &render);
			SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, 255);
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

	void Render_Map(entt::registry &zone, tmx::Map& map, Camera& camera) {
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
						//check the rendering order
						//auto text = Graphics::Load_Text_Texture(std::to_string(k), {255, 155, 155, 255});
						//Graphics::Render_FRect(text.pTexture, &text.k, &renderPosition);
						//SDL_DestroyTexture(text.pTexture);
					}
					g++;
				}
				if (i == 1) {
					Animation_Frame(zone, camera);
					Texture_Packer_Render(zone, camera);
					Render_Explosions(zone, camera);
				}
			}
		}
		int ks = h + g;
		//h = h / g;
		//system("CLS");
		//Utilities::Log("x: ");
		//Utilities::Log(X);
		//Utilities::Log(g);
		//Utilities::Log(" ");
		//Utilities::Log("y: ");
		//Utilities::Log(Y);
		//Utilities::Log(h);
		//Utilities::Log(" ");
		//Utilities::Log(h);
		//Utilities::Log(o);
		//Utilities::Log(" ");
		//Utilities::Log(" ");

	}


	void Render_Mouse_Item(entt::registry& zone, Component::Camera &camera) {

		SDL_FRect DisplayRect = {};

		auto view = zone.view<Position, Icon, On_Mouse>();

		for (auto item : view) {
			const auto& icon = view.get<Icon>(item);
			const auto& x = view.get<Position>(item).x;
			const auto& y = view.get<Position>(item).y;

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
			zone.clear<Renderable>();
			}
		else {
			renderType = true;
			zone.clear<Renderable>();
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

		auto objectsView = zone.view<Position>(entt::exclude<Inventory>);
		float bottomOfScreenEdge = camera.screen.y + camera.screen.h;
		float bottomOfRenderRect = renderRect.y + renderRect.h;

		for (auto entity : objectsView) {
			auto& x = objectsView.get<Position>(entity).x;
			auto& y = objectsView.get<Position>(entity).y;
			SDL_FPoint point = {x, y};

			if (Utilities::bFPoint_FRectIntersect(point, renderRect)) {
				int alpha = Set_Render_Position_Alpha(bottomOfScreenEdge, bottomOfRenderRect, y);
				zone.emplace_or_replace<Renderable>(entity, y, alpha);
				j++;
			}
			else {
				zone.remove<Renderable>(entity);
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
		auto focus_view = zone.view<Camera, Position>();

		for (auto player : focus_view) {
			auto& camera = focus_view.get<Camera>(player);
			auto& position = focus_view.get<Position>(player);

			Camera_Control::Update_Camera_Follow(camera, position, Graphics::resolution);
			Update_Cursor(camera);
		}
	}

	void Remove_Entities_From_Registry(entt::registry& zone) {
		auto view = zone.view<Destroyed>(entt::exclude<Component::In_Object_Tree>);
		for (auto entity : view) {
			zone.destroy(entity);
		}
	}

	void Rendering(entt::registry& zone) {
		Update_Camera_And_Mouse(zone);
		SDL_FPoint mouse = { Mouse::iXMouse, Mouse::iYMouse };
		Character_Stats::Update_Unit_Stats(zone);
		UI::Move_To_Item_Routine(zone, Mouse::itemCurrentlyHeld);
		Player_Control::Move_To_Atack_Routine(zone);


		auto camera_view = zone.view<Camera>();
		for (auto entity : camera_view) {
			auto& camera = camera_view.get<Camera>(entity);

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
			Interface::Run_Interface(zone, camera);
			Items::Update_Mouse_Slot_Position(zone, Mouse::mouseItem, Mouse::itemCurrentlyHeld, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
			Tooltip::Show_Item_Tooltip(zone, mouse, camera);
			Damage_Text::Show_Damage(zone, camera);
			UI_Resources::Render_Health(zone, camera);
			Render_Mouse_Item(zone, camera);


			SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            if (Items::showGroundItems == true) {                //****//search quad tree instead
                auto view = zone.view<Ground_Item, Renderable>();
                for (auto item : view) {
                    auto &box =zone.get<Ground_Item>(item);
                    SDL_RenderDrawRectF(Graphics::renderer, &box.box);
                }
            }


            SDL_RenderPresent(Graphics::renderer);
		}
	}
}
