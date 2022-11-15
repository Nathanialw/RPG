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

    void Update_Frame_PVG(SDL_Rect &clipRect, Component::Sprite_Sheet_Info &sheetData, Component::Action &action, Component::Direction &direction) {
        int spritesPerLine = sheetData.flareSpritesheet->at(sheetData.sheet_name).sheetWidth / sheetData.flareSpritesheet->at(sheetData.sheet_name).spriteWidth;
            ///the x index in absolute terms
        int x_index = sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].startFrame + ((PVG_Direction_Enum(direction) * sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames) + sheetData.currentFrame);
            ///the x index in relative terms
        int x_rel_index = x_index % spritesPerLine;
            ///the x index in pixel terms
        clipRect.x = x_rel_index * sheetData.flareSpritesheet->at(sheetData.sheet_name).spriteWidth;
            ///after dividing away the x index, add the remainder
        int y_index = x_index / spritesPerLine;
            ///the y index in pixel terms
        clipRect.y = y_index * sheetData.flareSpritesheet->at(sheetData.sheet_name).spriteWidth;
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

    void Frame_Increment(Component::Sprite_Sheet_Info &sheetData, Component::Action &action, Component::Direction &direction) {
        sheetData.frameTime += Timer::timeStep;
        if (sheetData.finalFrame == Component::finalFrame) {
            sheetData.finalFrame = Component::firstFrame;
        }
        if (sheetData.frameTime >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed) {
            if (sheetData.finalFrame == Component::firstFrame) {
                if (action.state != Component::walk && action.state != Component::struck && action.state != Component::attack && action.state != Component::cast) {
                    action.state = Component::idle;
                }
                else if (action.state == Component::struck || action.state == Component::attack || action.state == Component::cast) {
                    action.state = Component::idle;
                }
                sheetData.finalFrame = Component::normalFrame;
            }
                ///reset frame count if over
            sheetData.frameTime = 0;
            int &currentFrame = sheetData.currentFrame;
            sheetData.frameIndex = sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].startFrame + (sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames * PVG_Direction_Enum(direction)) + currentFrame;
            if (sheetData.sheetDataWeapon) {
                sheetData.weaponFrameIndex = sheetData.sheetDataWeapon->at(sheetData.weapon_name).actionFrameData[action.state].startFrame + (sheetData.sheetDataWeapon->at(sheetData.weapon_name).actionFrameData[action.state].NumFrames * PVG_Direction_Enum(direction)) + currentFrame;
            }
            if (sheetData.sheetDataHelm) {
                sheetData.helmFrameIndex = sheetData.sheetDataHelm->at(sheetData.helm_name).actionFrameData[action.state].startFrame + (sheetData.sheetDataHelm->at(sheetData.helm_name).actionFrameData[action.state].NumFrames * PVG_Direction_Enum(direction)) + currentFrame;
            }
            if (sheetData.sheetDataChestpiece) {
                sheetData.chestpieceFrameIndex = sheetData.sheetDataChestpiece->at(sheetData.chest_name).actionFrameData[action.state].startFrame + (sheetData.sheetDataChestpiece->at(sheetData.chest_name).actionFrameData[action.state].NumFrames * PVG_Direction_Enum(direction)) + currentFrame;
            }
            if (sheetData.sheetDataLegs) {
                sheetData.legplateFrameIndex = sheetData.sheetDataLegs->at(sheetData.legs_name).actionFrameData[action.state].startFrame + (sheetData.sheetDataLegs->at(sheetData.legs_name).actionFrameData[action.state].NumFrames * PVG_Direction_Enum(direction)) + currentFrame;
            }
                ///calculate reversing
            if (!sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].reverses) {
                sheetData.reversing = 0;
            }
            if (sheetData.reversing) {
                currentFrame--;
                if (currentFrame < 1) {
                    sheetData.reversing = 0;
                }
            }
            else if (!sheetData.reversing) {
                currentFrame++;
            }
                /// -1 because of the zero index
            if (sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].reverses) {
                if (currentFrame >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames - 1) {
                    sheetData.reversing = 1;
                }
            }
            else if (currentFrame >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames) {
                if (action.state != Component::walk && action.state != Component::run) {
                    sheetData.finalFrame = Component::finalFrame;
                }
                sheetData.currentFrame = 0;

            }
        }
    }

    void Update_Item_Frame(SDL_Rect &clipRect, int &row, int &column) {
        clipRect = { column * clipRect.w, row * clipRect.h, clipRect.w, clipRect.h };
    }

    void Update_Frame_Flare(SDL_Rect &clipRect, int &currentFrame, int &startFrame, Component::Direction& direction) {
        clipRect.x = (startFrame + currentFrame) * clipRect.w;
        clipRect.y = clipRect.h * Original_Direction_Enum(direction);
    }

    void Get_Spritesheet_Type(SDL_Rect &clipRect, Component::Sprite_Sheet_Info &sheetData, Component::Direction& direction, Component::Action& action) {
            /// get the height and width
        clipRect = {0, 0, sheetData.flareSpritesheet->at(sheetData.sheet_name).spriteWidth, sheetData.flareSpritesheet->at(sheetData.sheet_name).spriteHeight};

        if (sheetData.type == "flare"){
            Update_Frame_Flare(clipRect, sheetData.currentFrame, sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].startFrame, direction);
        }
        else if (sheetData.type == "pvg"){
            Update_Frame_PVG(clipRect, sheetData, action, direction);
        }
        else if (sheetData.type == "item") {
                /// assigned unused variables to store spritesheet indexes, currentFrame == row, frameIndex == column
            Update_Item_Frame(clipRect, sheetData.currentFrame, sheetData.frameIndex);
        }
    }

    bool Death_Sequence (Component::Action& action, int &currentFrame, int &numFrames) {
        if (action.state == Component::dead) {
            if (currentFrame < numFrames - 1) {
                currentFrame++;
            }
            return true;
        }
        return false;
    }


    void Update_Frame(Component::Sprite_Sheet_Info &sheetData, Component::Direction& direction, Component::Action& action) {

        sheetData.frameTime += Timer::timeStep;
        if (sheetData.finalFrame == Component::finalFrame) {
            sheetData.finalFrame = Component::firstFrame;
        }

        if (sheetData.frameTime >= sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed) {
            sheetData.frameTime = 0;

            if (action.state != Component::isStatic) {
                /// reset at the start so it had a chance loop through the logic once to trigger end of state actions
                if (sheetData.finalFrame == Component::firstFrame) {

                    if (action.state != Component::walk && action.state != Component::struck && action.state != Component::attack && action.state != Component::dead) {
                        action.state = Component::idle;
                    } else if (action.state == Component::struck || action.state == Component::attack) {
                        action.state = Component::idle;
                    }

                    sheetData.finalFrame = Component::normalFrame;
                }

                if (Death_Sequence(action, sheetData.currentFrame, sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames)) {
                    return;
                }
                ///render first frame before increment
                if (sheetData.currentFrame == 0) {
                    sheetData.currentFrame++;
                    sheetData.reversing = false;
                    return;
                }

                if (sheetData.reversing == false) {
                    sheetData.currentFrame++;
                } else if (sheetData.reversing == true) {
                    sheetData.currentFrame--;
                }

                if (sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].reverses) {
                    /// -1 because of the zero index
                    if (sheetData.currentFrame >= sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames - 1) {
                        sheetData.reversing = true;
                    }
                } else if (sheetData.currentFrame >= sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames) {

                    if (action.state != Component::walk && action.state != Component::run) {
                        sheetData.finalFrame = Component::finalFrame;
                    }

                    sheetData.currentFrame = 0;
                }
            }
        }
    }

    SDL_FRect Position_For_Render(Component::Sprite_Sheet_Info &sheetData, Component::Position &position, Component::Camera &camera, Component::Scale &scale, Component::Sprite_Offset &offset, SDL_Rect &clipRect, SDL_FRect &renderRect) {
        clipRect = sheetData.sheetData->at(sheetData.sheet_name).frameList[sheetData.frameIndex].clip;
        renderRect = Scale_Sprite_for_Render(clipRect, scale.scale);
        renderRect.x = ((sheetData.sheetData->at(sheetData.sheet_name).frameList[sheetData.frameIndex].x_offset) * scale.scale) - offset.x + position.x - camera.screen.x + 20.0f;
        renderRect.y = ((sheetData.sheetData->at(sheetData.sheet_name).frameList[sheetData.frameIndex].y_offset) * scale.scale) - offset.y + position.y - camera.screen.y + 20.0f;
        return renderRect;
    }

    SDL_FRect Position_Weapon_For_Render(Component::Sprite_Sheet_Info &sheetData, Component::Position &position, Component::Camera &camera, Component::Scale &scale, Component::Sprite_Offset &offset, SDL_Rect &clipRect, SDL_FRect &renderRect) {
        clipRect = sheetData.sheetDataWeapon->at(sheetData.weapon_name).frameList[sheetData.weaponFrameIndex ].clip;
        renderRect = Scale_Sprite_for_Render(clipRect, scale.scale);
        renderRect.x = ((sheetData.sheetDataWeapon->at(sheetData.weapon_name).frameList[sheetData.weaponFrameIndex].x_offset) * scale.scale) - offset.x + position.x - camera.screen.x + 20.0f;
        renderRect.y = ((sheetData.sheetDataWeapon->at(sheetData.weapon_name).frameList[sheetData.weaponFrameIndex].y_offset) * scale.scale) - offset.y + position.y - camera.screen.y + 20.0f;
        return renderRect;
    }

    SDL_FRect Position_Helm_For_Render(Component::Sprite_Sheet_Info &sheetData, Component::Position &position, Component::Camera &camera, Component::Scale &scale, Component::Sprite_Offset &offset, SDL_Rect &clipRect, SDL_FRect &renderRect) {
        clipRect = sheetData.sheetDataHelm->at(sheetData.helm_name).frameList[sheetData.helmFrameIndex ].clip;
        renderRect = Scale_Sprite_for_Render(clipRect, scale.scale);
        renderRect.x = ((sheetData.sheetDataHelm->at(sheetData.helm_name).frameList[sheetData.helmFrameIndex].x_offset) * scale.scale) - offset.x + position.x - camera.screen.x + 20.0f;
        renderRect.y = ((sheetData.sheetDataHelm->at(sheetData.helm_name).frameList[sheetData.helmFrameIndex].y_offset) * scale.scale) - offset.y + position.y - camera.screen.y + 20.0f;
        return renderRect;
    }

    SDL_FRect Position_Chest_For_Render(Component::Sprite_Sheet_Info &sheetData, Component::Position &position, Component::Camera &camera, Component::Scale &scale, Component::Sprite_Offset &offset, SDL_Rect &clipRect, SDL_FRect &renderRect) {
        clipRect = sheetData.sheetDataChestpiece->at(sheetData.chest_name).frameList[sheetData.chestpieceFrameIndex ].clip;
        renderRect = Scale_Sprite_for_Render(clipRect, scale.scale);
        renderRect.x = ((sheetData.sheetDataChestpiece->at(sheetData.chest_name).frameList[sheetData.chestpieceFrameIndex].x_offset) * scale.scale) - offset.x + position.x - camera.screen.x + 20.0f;
        renderRect.y = ((sheetData.sheetDataChestpiece->at(sheetData.chest_name).frameList[sheetData.chestpieceFrameIndex].y_offset) * scale.scale) - offset.y + position.y - camera.screen.y + 20.0f;
        return renderRect;
    }

    SDL_FRect Position_Legs_For_Render(Component::Sprite_Sheet_Info &sheetData, Component::Position &position, Component::Camera &camera, Component::Scale &scale, Component::Sprite_Offset &offset, SDL_Rect &clipRect, SDL_FRect &renderRect) {
        clipRect = sheetData.sheetDataLegs->at(sheetData.legs_name).frameList[sheetData.legplateFrameIndex ].clip;
        renderRect = Scale_Sprite_for_Render(clipRect, scale.scale);
        renderRect.x = ((sheetData.sheetDataLegs->at(sheetData.legs_name).frameList[sheetData.legplateFrameIndex].x_offset) * scale.scale) - offset.x + position.x - camera.screen.x + 20.0f;
        renderRect.y = ((sheetData.sheetDataLegs->at(sheetData.legs_name).frameList[sheetData.legplateFrameIndex].y_offset) * scale.scale) - offset.y + position.y - camera.screen.y + 20.0f;
        return renderRect;
    }

    void Animation_Frame(entt::registry& zone, Component::Camera &camera) { //state

        auto view1 = zone.view<Component::Renderable, Component::Position, Component::Sprite_Sheet_Info, Component::Action, Component::Direction, Component::Sprite_Offset, Component::Scale, Component::Entity_Type>();

        for (auto entity : view1) {
            auto [renderable, position, sheetData, action, direction, spriteOffset, scale, type] = view1.get(entity);
            SDL_Rect clipRect;
            SDL_FRect renderRect;
            SDL_Texture* texture;

            if (sheetData.flareSpritesheet) {
                    /// get the next frame
                Update_Frame(sheetData, direction, action);
                    /// get/update the clip rect
                Get_Spritesheet_Type(clipRect, sheetData, direction, action);
                    /// set the render rect size and position
                renderRect = Scale_Sprite_for_Render(clipRect, scale.scale);
                renderRect.x = (position.x - camera.screen.x - spriteOffset.x);
                renderRect.y = (position.y - camera.screen.y - spriteOffset.y);

                if (type == Component::Entity_Type::item){
                    renderRect.w = (spriteOffset.x * 2.0f);
                    renderRect.h = (spriteOffset.y * 2.0f);
                }

                texture = sheetData.flareSpritesheet->at(sheetData.sheet_name).texture;
                SDL_SetTextureAlphaMod(texture, renderable.alpha);
                Graphics::Render_FRect(texture, &clipRect, &renderRect);
            }

            else if (sheetData.sheetData) {
                Frame_Increment(sheetData, action, direction);
                renderRect = Position_For_Render(sheetData, position, camera, scale, spriteOffset, clipRect, renderRect);
                texture = sheetData.sheetData->at(sheetData.sheet_name).texture;
                SDL_SetTextureAlphaMod(texture, renderable.alpha);
                Graphics::Render_FRect(texture, &clipRect, &renderRect);

//                Frame_Increment(sheetData, action, direction);
                if (sheetData.sheetDataLegs) {
                    renderRect = Position_Legs_For_Render(sheetData, position, camera, scale, spriteOffset, clipRect, renderRect);
                    texture = sheetData.sheetDataLegs->at(sheetData.legs_name).texture;
                    SDL_SetTextureAlphaMod(texture, renderable.alpha);
                    Graphics::Render_FRect(texture, &clipRect, &renderRect);
                }

                if (sheetData.sheetDataChestpiece) {
                    renderRect = Position_Chest_For_Render(sheetData, position, camera, scale, spriteOffset, clipRect, renderRect);
                    texture = sheetData.sheetDataChestpiece->at(sheetData.chest_name).texture;
                    SDL_SetTextureAlphaMod(texture, renderable.alpha);
                    Graphics::Render_FRect(texture, &clipRect, &renderRect);
                }

                if (sheetData.sheetDataHelm) {
                    renderRect = Position_Helm_For_Render(sheetData, position, camera, scale, spriteOffset, clipRect, renderRect);
                    texture = sheetData.sheetDataHelm->at(sheetData.helm_name).texture;
                    SDL_SetTextureAlphaMod(texture, renderable.alpha);
                    Graphics::Render_FRect(texture, &clipRect, &renderRect);
                }

                if (sheetData.sheetDataWeapon) {
                    renderRect = Position_Weapon_For_Render(sheetData, position, camera, scale, spriteOffset, clipRect, renderRect);
                    texture = sheetData.sheetDataWeapon->at(sheetData.weapon_name).texture;
                    SDL_SetTextureAlphaMod(texture, renderable.alpha);
                    Graphics::Render_FRect(texture, &clipRect, &renderRect);
                }
//                if (sheetData.sheetDataHair) {
//                    renderRect = Position_Weapon_For_Render(sheetData, position, camera, scale, spriteOffset, clipRect, renderRect);
//                    texture = sheetData.sheetDataHair->at(sheetData.hair_name).texture;
//                    SDL_SetTextureAlphaMod(texture, renderable.alpha);
//                    Graphics::Render_FRect(texture, &clipRect, &renderRect);
//                }
//
//                if (sheetData.sheetDataBack) {
//                    renderRect = Position_Weapon_For_Render(sheetData, position, camera, scale, spriteOffset, clipRect, renderRect);
//                    texture = sheetData.sheetDataBack->at(sheetData.back_name).texture;
//                    SDL_SetTextureAlphaMod(texture, renderable.alpha);
//                    Graphics::Render_FRect(texture, &clipRect, &renderRect);
//                }
//
//                if (sheetData.sheetDataBeard) {
//                    renderRect = Position_Weapon_For_Render(sheetData, position, camera, scale, spriteOffset, clipRect, renderRect);
//                    texture = sheetData.sheetDataBeard->at(sheetData.beard_name).texture;
//                    SDL_SetTextureAlphaMod(texture, renderable.alpha);
//                    Graphics::Render_FRect(texture, &clipRect, &renderRect);
//                }
//
//                if (sheetData.sheetDataBodysuit) {
//                    renderRect = Position_Weapon_For_Render(sheetData, position, camera, scale, spriteOffset, clipRect, renderRect);
//                    texture = sheetData.sheetDataBodysuit->at(sheetData.bodysuit_name).texture;
//                    SDL_SetTextureAlphaMod(texture, renderable.alpha);
//                    Graphics::Render_FRect(texture, &clipRect, &renderRect);
//                }
//
//                if (sheetData.sheetDataOffhand) {
//                    renderRect = Position_Weapon_For_Render(sheetData, position, camera, scale, spriteOffset, clipRect, renderRect);
//                    texture = sheetData.sheetDataOffhand->at(sheetData.offhand_name).texture;
//                    SDL_SetTextureAlphaMod(texture, renderable.alpha);
//                    Graphics::Render_FRect(texture, &clipRect, &renderRect);
//                }
            }

            else {
                Utilities::Log("Animation_Frame() fallthrough error: both pointers NULL");
                return;
            }
		}
	}

    void t_Get_Next_Frame_Index(Component::Sprite_Sheet_Info &sheetData, Component::Action &action, Component::Direction &direction) {
        sheetData.frameTime += Timer::timeStep;
        if (sheetData.frameTime >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed) {
            ///reset frame count if over
            sheetData.frameTime = 0;
            int &currentFrame = sheetData.currentFrame;
            sheetData.frameIndex = sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].startFrame + (sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames * PVG_Direction_Enum(direction)) + currentFrame;
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
            if (currentFrame >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames) {
                currentFrame = 0;
                if (sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].reverses) {
                    sheetData.reversing = 1;
                }
                else {
                    sheetData.currentFrame = 0;
                    if (action.state != Component::walk) {
                        action.state = Component::idle;
                    }
                }
            }
        }
    }

    /// Render without chasing pointers
    void t_Render(entt::registry& zone) {
        auto view1 = zone.view<Component::Renderable, Component::t_Texture, Component::t_Rendering_Data>();
        for (auto entity : view1) {
            auto [renderable, texture, renderingData] = view1.get(entity);
            SDL_SetTextureAlphaMod(texture.texture, renderable.alpha);
            Graphics::Render_FRect(texture.texture, &renderingData.clipRect, &renderingData.renderRect);
        }
    };

    void Render_Sequence (entt::registry& zone) {
//        t_Get_Next_Frame_Index();
//        t_Get_Next_Frame_Data();
        t_Render(zone);
    }

	SDL_Rect Explosion_Frame_Update(Component::Sprite_Frames &frame, SDL_Rect frameToUpdateClipOf) {
		    /// reset X to zero and increment Y
		if (frame.frameX >= 8) {
			frame.frameX = 0;
			frame.frameY++;
		}
		    /// set Output Rect values
		int row = frame.frameX * 128;
		int column = frame.frameY * 128;
		int width = 128;
		int height = 128;
		SDL_Rect rect = { row, column, width, height };
		    ///increment X
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
				    /// only fire this at 60 frames/sec
					xClipPos = Explosion_Frame_Update(frames, texture.clippedSpriteFrame);		//get state and direction state sprite draw from
					anim.renderPosition = Utilities::SDL_Rect_To_SDL_FRect(xClipPos);		//save sprite for vector
					texture.clippedSpriteFrame = xClipPos;									//save position for renderer
					frames.currentFrame++;
				}
				else {
					    /// remove explosion from scene and free the entity
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
                if (i == 2) {
                        /// renders all objects just above the 2nd layer but below the 3rd in tiled
                    Animation_Frame(zone, camera);
                    Render_Explosions(zone, camera);
                }
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
//			Dynamic_Quad_Tree::Draw_Tree_Object_Rects(zone);
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
