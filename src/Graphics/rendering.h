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
#include "character_stats.h"
#include "tooltips.h"
#include "damage_text.h"
#include "ui_resources.h"
#include "pause.h"
//#include "map.h"
#include "world_grid.h"
#include "dynamic_quad_tree.h"
#include "player_control.h"
#include "death_control.h"

#include "rendering_components.h"
namespace Rendering {

    using namespace Rendering_Components;

    namespace {
        bool showSpriteBox = false;
        bool debug = false;
    }

//	struct tileData {
//		SDL_Texture* texture;
//		SDL_Rect tileSpriteRect;
//	};

    void sort_Positions(entt::registry &zone) {
        // test whether a point lays on a line segment, positive is above and negative is below
        auto line_segments = zone.view<Component::Line_Segment, Component::Renderable>();
        auto points = zone.view<Component::Position, Component::Renderable>();

        for (auto building : line_segments) {
            //test each renderable entity, if it is above the line
            auto [line, renderable] = line_segments.get(building);
            for (auto entity : points) {
                auto [position, rend] = points.get(entity);

                //f(x) = k * x + b
                //slope is k
                float slope = (line.p[0].y - line.p[1].y) / (line.p[0].x - line.p[1].x);
                //y_offset is b
                int y_offset = line.p[1].y / (slope * line.p[1].x);
                //check whether the y position is above or below the line
                float y = (slope * position.x) + y_offset;
                if (position.y + y > position.y) {
                    //the entity is above the line

                    //check the collsion box, if they collide place the low y position entity, behind the higher y position, else do nothing
                    renderable.y = position.y - 1;
                }
                else {
                    //the entity is below the line
                    renderable.y = position.y + 1;
                }

                //set renderable.y to tested renderable.y + 1 for above and renderable.y - 1 for below then let the sort algo do its job
            }
        }
//        // sort the component to render before entities below and after entities above
//
//        //sorts point positions least to great
        int d = points.size_hint();
        Utilities::Log("sorting");
        Utilities::Log(d);
        zone.sort<Component::Renderable>([](const auto &lhs, const auto &rhs) { return lhs.y < rhs.y; });
    }

//    void RenderLine (entt::registry &zone, Component::Camera &camera) {
//        auto line_segments = zone.view<Component::Line_Segment, Component::Renderable>();
//        for (auto building : line_segments) {
//            auto [line, renderable] = line_segments.get(building);
//            SDL_SetRenderDrawBlendMode(Graphics::renderer, SDL_BLENDMODE_NONE);
//            SDL_SetRenderDrawColor(Graphics::renderer, 255,0,0,255);
//            float x1 = line.p[0].x - (camera.screen.x);
//            float y1 = line.p[0].y - (camera.screen.y);
//            float x2 = line.p[1].x - (camera.screen.x);
//            float y2 = line.p[1].y - (camera.screen.y);
//            SDL_RenderDrawLineF(Graphics::renderer, x1, y1, x2, y2);
//        }
//    }

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
        Utilities::Log("PVG_Direction_Enum passthrough error");
        return 0;
    }

    void Update_Frame_PVG(SDL_Rect &clipRect, Rendering_Components::Sprite_Sheet_Info &sheetData, Action_Component::Action &action, Component::Direction &direction) {
        int spritesPerLine = sheetData.flareSpritesheet->at(sheetData.sheet_name).sheetWidth / sheetData.flareSpritesheet->at(sheetData.sheet_name).spriteWidth;
        ///the x index in absolute terms
        int x_index = sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].startFrame + ((PVG_Direction_Enum(direction) * sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames) + action.frame);
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

        Utilities::Log("Original_Direction_Enum() fallthough error");
        return 2;
    }

    void Frame_Increment(entt::entity &entity, Component::Scale &scale, Sprite_Sheet_Info &sheetData, Action_Component::Action &action, Component::Direction &direction) {

        action.frameTime += Timer::timeStep;

        if (action.frameState == Action_Component::last) {
            if (!sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].reverses && (action.state != Action_Component::dying || action.state != Action_Component::dead)) {
                if (action.frameTime >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed) {
                    action.state = Action_Component::idle2;
                    action.frameTime -= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed;
                    action.frameState = Action_Component::start;
                }
            }
            else {
                action.frameState = Action_Component::start;
            }
        }
        if (action.frameTime >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed) {
            action.frameTime -= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed;
//            sheetData.frameTime = 0;

            if (action.frameState == Action_Component::start) {
                if (action.state == Action_Component::dying || action.state == Action_Component::dead) {

                }
                else if (action.state != Action_Component::walk && action.state != Action_Component::struck && action.state != Action_Component::attack && action.state != Action_Component::attack2 && action.state != Action_Component::cast && action.state != Action_Component::casting) {
                    action.state = Action_Component::idle;
                }
                else if (action.state == Action_Component::struck || action.state == Action_Component::attack || action.state == Action_Component::cast) {
                    action.state = Action_Component::idle;
                }
                action.frameState = Action_Component::mid;
            }

            ///reset frame count if over
            sheetData.frameIndex = sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].startFrame + (sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames * PVG_Direction_Enum(direction)) + action.frame;

            if (Death_Control::Death_Sequence(direction, entity, scale, sheetData, action, sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames)) {
                return;
            }

            ///calculate reversing
            if (!sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].reverses) {
                sheetData.reversing = 0;
            }
            if (sheetData.reversing) {
                action.frame--;
                if (action.frame < 1) {
                    sheetData.reversing = 0;
                }
            }
            else if (!sheetData.reversing) {
                action.frame++;
            }
            /// -1 because of the zero index
            if (sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].reverses) {
                if (action.frame >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames - 1) {
                    sheetData.reversing = 1;
                }
            }
            else if (action.frame >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames) {
                if (action.state != Action_Component::walk && action.state != Action_Component::run) {
                    action.frameState = Action_Component::last;
                }
                action.frame = 0;

            }
        }
    }

    void Update_Item_Frame(SDL_Rect &clipRect, uint8_t row, uint16_t column) {
        clipRect = { column * clipRect.w, row * clipRect.h, clipRect.w, clipRect.h };
    }

    void Update_Frame_Flare(SDL_Rect &clipRect, uint8_t currentFrame, int &startFrame, Component::Direction& direction) {
        clipRect.x = (startFrame + currentFrame) * clipRect.w;
        clipRect.y = clipRect.h * Original_Direction_Enum(direction);
    }

    void Get_Spritesheet_Type(SDL_Rect &clipRect, Sprite_Sheet_Info &sheetData, Component::Direction& direction, Action_Component::Action& action) {
        /// get the height and width
        clipRect = {0, 0, sheetData.flareSpritesheet->at(sheetData.sheet_name).spriteWidth, sheetData.flareSpritesheet->at(sheetData.sheet_name).spriteHeight};

        if (sheetData.type == "flare"){
            Update_Frame_Flare(clipRect, action.frame, sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].startFrame, direction);
        }
        else if (sheetData.type == "pvg"){
            Update_Frame_PVG(clipRect, sheetData, action, direction);
        }
        else if (sheetData.type == "item") {
            /// assigned unused variables to store spritesheet indexes, currentFrame == row, frameIndex == column
            Update_Item_Frame(clipRect, action.frame, sheetData.frameIndex);
        }
    }

    void Update_Frame(entt::entity entity, Component::Scale scale, Sprite_Sheet_Info &sheetData, Component::Direction& direction, Action_Component::Action& action) {

        action.frameTime += Timer::timeStep;
        if (action.frameState == Action_Component::last) {
            action.frameState = Action_Component::start;
        }

        if (action.frameTime >= sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed) {
            action.frameTime -= sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed;
//            sheetData.frameTime = 0;

            if (action.state != Action_Component::isStatic) {
                /// reset at the start so it had a chance loop through the logic once to trigger end of state actions
                if (action.frameState == Action_Component::start) {

                    if (action.state != Action_Component::walk && action.state != Action_Component::struck && action.state != Action_Component::attack && action.state != Action_Component::attack2 && action.state != Action_Component::dying && action.state != Action_Component::dead) {
                        action.state = Action_Component::idle;
                    } else if (action.state == Action_Component::struck || action.state == Action_Component::attack) {
                        action.state = Action_Component::idle;
                    }
                    action.frameState = Action_Component::mid;
                }

                if (Death_Control::Death_Sequence(direction, entity, scale, sheetData, action, sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames)) {
                    return;
                }
                ///render first frame before increment
                if (action.frame == 0) {
                    action.frame++;
                    sheetData.reversing = false;
                    return;
                }

                if (sheetData.reversing == false) {
                    action.frame++;
                }

                else if (sheetData.reversing == true) {
                    action.frame--;
                }

                if (sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].reverses) {
                    /// -1 because of the zero index
                    if (action.frame >= sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames - 1) {
                        sheetData.reversing = true;
                    }
                }

                else if (action.frame >= sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames) {

                    if (action.state != Action_Component::walk && action.state != Action_Component::run) {
                        action.frameState = Action_Component::last;
                    }
                    if (action.state == Action_Component::attack2) {
                        action.state = Action_Component::idle;
                    }
                    action.frame = 0;
                }
            }
        }
    }

    SDL_FRect Position_For_Render(std::unordered_map<std::string, Sheet_Data>*sheetData, std::string &name, uint16_t frameIndex, Component::Position &position, Component::Camera &camera, Component::Scale &scale, Sprite_Offset &offset, SDL_Rect &clipRect, SDL_FRect &renderRect, Component::Interaction_Rect &interactionRect) {
        clipRect = sheetData->at(name).frameList[frameIndex].clip;
        renderRect = Utilities::Scale_Rect(clipRect, scale.scale);
        renderRect.x = sheetData->at(name).frameList[frameIndex].x_offset * scale.scale - offset.x + position.x;
        renderRect.y = sheetData->at(name).frameList[frameIndex].y_offset * scale.scale - offset.y + position.y;
        interactionRect.rect = renderRect;
        renderRect.x -= camera.screen.x;
        renderRect.y -= camera.screen.y;
        return renderRect;
    }

    void Update_Equipment (Rendering_Components::Equip_Slot_Data &item, Action_Component::Action &action, Component::Direction &direction) {
        item.FrameIndex = item.ItemSheetData->at(item.name).actionFrameData[action.state].startFrame + (item.ItemSheetData->at(item.name).actionFrameData[action.state].NumFrames * PVG_Direction_Enum(direction)) + action.frame;
    }

    void Render_Equipment(Rendering_Components::Equipment_Sprites &equipment, Component::Scale &scale, Sprite_Sheet_Info &sheetData, Component::Camera &camera, Component::Position &position, Component::Renderable &renderable, Sprite_Offset &spriteOffset) {
        SDL_Rect clipRect;
        for (auto item : equipment.sheet) {
            if (item.ItemSheetData) {
                Component::Interaction_Rect itemInteractionRect = {};
                SDL_FRect renderRect = Position_For_Render(item.ItemSheetData, item.name, sheetData.frameIndex, position, camera, scale, spriteOffset, clipRect, renderRect, itemInteractionRect);
                SDL_Texture* texture = item.ItemSheetData->at(item.name).texture;
                Color color = item.ItemSheetData->at(item.name).color;
                SDL_SetTextureAlphaMod(texture, renderable.alpha);
                Graphics::Render_FRect(texture, color, &clipRect, &renderRect);
            }
        }
    }

    void Render_Sprite(entt::registry &zone, entt::entity &entity, Component::Camera &camera, Component::Scale &scale, Component::Renderable &renderable, Component::Position &position, Rendering_Components::Sprite_Offset &spriteOffset, Sprite_Sheet_Info &sheetData, Action_Component::Action &action, Component::Direction &direction) {
        SDL_Rect clipRect;
        SDL_FRect renderRect;

        if (zone.any_of<Component::Interaction_Rect>(entity)) {
            auto &interactionRect = zone.get<Component::Interaction_Rect>(entity);
            renderRect = Position_For_Render(sheetData.sheetData, sheetData.sheet_name, sheetData.frameIndex, position, camera, scale, spriteOffset, clipRect, renderRect, interactionRect);
        }
        else {
            Component::Interaction_Rect interactionRect = {};
            renderRect = Position_For_Render(sheetData.sheetData, sheetData.sheet_name, sheetData.frameIndex, position, camera, scale, spriteOffset, clipRect, renderRect, interactionRect);
        }

        SDL_Texture* texture = sheetData.sheetData->at(sheetData.sheet_name).texture;
        Color color = sheetData.sheetData->at(sheetData.sheet_name).color;
        SDL_SetTextureAlphaMod(texture, renderable.alpha);
        Graphics::Render_FRect(texture, color, &clipRect, &renderRect);
    }

    void Animation_Frame(entt::registry& zone, Component::Camera &camera) { //state

        auto view1 = zone.view<Component::Renderable, Action_Component::Action, Component::Position, Sprite_Sheet_Info, Component::Direction, Sprite_Offset, Component::Scale, Component::Entity_Type>();
        auto view = zone.view<Component::Renderable, Rendering_Components::Equipment_Sprites>();
        auto mounts = zone.view<Component::Renderable, Mount_Sprite>();

//        std::cout << "----------------------" << std::endl;
        int d = view1.size_hint();
        Utilities::Log("rendering");
        Utilities::Log(d);
        for (auto entity : view1) {
            auto [renderable, action, position, sheetData, direction, spriteOffset, scale, type] = view1.get(entity);
//            std::cout << renderable.y << " ";
            SDL_Rect clipRect;
            SDL_FRect renderRect;
            SDL_Texture* texture;
            Color color;

            if (sheetData.flareSpritesheet) {
                /// get the next frame
                Update_Frame(entity, scale, sheetData, direction, action);
                /// get/update the clip rect
                Get_Spritesheet_Type(clipRect, sheetData, direction, action);
                /// set the render rect size and position
                renderRect = Utilities::Scale_Rect(clipRect, scale.scale);
                renderRect.x = (position.x - camera.screen.x - spriteOffset.x);
                renderRect.y = (position.y - camera.screen.y - spriteOffset.y);

//                render icons
                if (type == Component::Entity_Type::item){
                    renderRect.w = (spriteOffset.x * 2.0f);
                    renderRect.h = (spriteOffset.y * 2.0f);
                }

                texture = sheetData.flareSpritesheet->at(sheetData.sheet_name).texture;
                color = sheetData.flareSpritesheet->at(sheetData.sheet_name).color;
                SDL_SetTextureAlphaMod(texture, renderable.alpha);
                Graphics::Render_FRect(texture, color, &clipRect, &renderRect);
            }

            else if (sheetData.sheetData) {
                if (mounts.contains(entity)) {
//                render horse half behind unit

                }

//                render unit
                Frame_Increment(entity, scale, sheetData, action, direction);
                Render_Sprite(zone, entity, camera, scale, renderable, position, spriteOffset, sheetData, action, direction);

//                render equipment
                if (view.contains(entity)) {
                    auto equipment = view.get<Rendering_Components::Equipment_Sprites>(entity);
                    Render_Equipment(equipment, scale, sheetData, camera, position, renderable, spriteOffset);
                }

                if (mounts.contains(entity)) {
//                render horse half in front unit

                }
            }

            else {
                Utilities::Log("Animation_Frame() fallthrough error: all pointers NULL");
                return;
            }
        }
//        std::cout << "\n";
    }

    void Render_Dead(entt::registry& zone, Component::Camera &camera) { //state

        auto view1 = zone.view<Component::Renderable, Action_Component::Action, Component::Position, Sprite_Sheet_Info, Component::Direction, Sprite_Offset, Component::Scale, Component::Entity_Type, Component::Dead>();
        auto view = zone.view<Component::Renderable, Rendering_Components::Equipment_Sprites>();
        auto mounts = zone.view<Component::Renderable, Mount_Sprite>();

//        std::cout << "----------------------" << std::endl;

        for (auto entity : view1) {
            auto [renderable, action, position, sheetData, direction, spriteOffset, scale, type, dead] = view1.get(entity);
//            std::cout << renderable.y << " ";
            SDL_Rect clipRect;
            SDL_FRect renderRect;
            SDL_Texture* texture;
            Color color;

            if (sheetData.flareSpritesheet) {
                /// get the next frame
                Update_Frame(entity, scale, sheetData, direction, action);
                /// get/update the clip rect
                Get_Spritesheet_Type(clipRect, sheetData, direction, action);
                /// set the render rect size and position
                renderRect = Utilities::Scale_Rect(clipRect, scale.scale);
                renderRect.x = (position.x - camera.screen.x - spriteOffset.x);
                renderRect.y = (position.y - camera.screen.y - spriteOffset.y);

//                render icons
                if (type == Component::Entity_Type::item){
                    renderRect.w = (spriteOffset.x * 2.0f);
                    renderRect.h = (spriteOffset.y * 2.0f);
                }

                texture = sheetData.flareSpritesheet->at(sheetData.sheet_name).texture;
                color = sheetData.flareSpritesheet->at(sheetData.sheet_name).color;
                SDL_SetTextureAlphaMod(texture, renderable.alpha);
                Graphics::Render_FRect(texture, color, &clipRect, &renderRect);
            }

            else if (sheetData.sheetData) {
                if (mounts.contains(entity)) {
//                render horse half behind unit

                }

//                render unit
                Frame_Increment(entity, scale, sheetData, action, direction);
                Render_Sprite(zone, entity, camera, scale, renderable, position, spriteOffset, sheetData, action, direction);

//                render equipment
                if (view.contains(entity)) {
                    auto equipment = view.get<Rendering_Components::Equipment_Sprites>(entity);
                    Render_Equipment(equipment, scale, sheetData, camera, position, renderable, spriteOffset);
                }

                if (mounts.contains(entity)) {
//                render horse half in front unit

                }
            }

            else {
                Utilities::Log("Animation_Frame() fallthrough error: all pointers NULL");
                return;
            }
        }
//        std::cout << "\n";
    }


    SDL_Rect Explosion_Frame_Update(Component::Sprite_Frames &frame) {
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
                    xClipPos = Explosion_Frame_Update(frames);		//get state and direction state sprite draw from
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

    void Render_Iso_Tiles(entt::registry &zone, tmx::Map& map, Component::Camera& camera)
    {
        float originX = 0.0f;
        float originY = 0.0f;
        SDL_Rect tileSpriteRect = {0, 0, (int) map.getTileSize().x, (int) map.getTileSize().y};
        SDL_FRect renderPosition = {0.0f, 0.0f, (float) map.getTileSize().x, (float) map.getTileSize().y};
        auto &numOfTiles = map.getTileCount();
        auto &layers = map.getLayers();
        const auto tileWidth = (float) map.getTileSize().x;
        const auto tileHeight = (float) map.getTileSize().y;
        int newX = (int) camera.screen.x;
        int newY = (int) camera.screen.y;
        int X = (newX / tileWidth) + (newY / tileHeight) - (camera.screen.w / 2 / (tileWidth / 2));
        int Y = (newY / tileHeight) - (newX / tileWidth) - (camera.screen.h / 2 / (tileHeight / 2));

        int width = X + (camera.screen.w / tileWidth * 4);
        int height = Y + (camera.screen.h / tileHeight * 3);

        int g = 0;
        //int o = 0;
        auto &tilesets = Maps::map.getTilesets();
        int p = Maps::map.getTilesets().size() - 1;
        for (int i = 0; i < layers.size(); i++) {
            if (layers[i]->getType() == tmx::Layer::Type::Tile) {
                const auto &tiles = layers[i]->getLayerAs<tmx::TileLayer>().getTiles();
                if (i == 2) {
                    /// renders all objects just above the 2nd layer but below the 3rd in tiled
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
                                    int tileCount = tileset->getColumnCount();
                                    SDL_Rect data = getTexture(id, tileCount, tileSpriteRect);
                                    if (Graphics::pTexture[name] != NULL) {
                                        Graphics::Render_FRect(Graphics::pTexture[name], {255, 255, 255}, &data, &renderPosition);
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
    }

    void Render_Ortho_Tiles(entt::registry &zone, tmx::Map& map, Component::Camera& camera)
    {
        float originX = 0.0f;
        float originY = 0.0f;
        SDL_Rect tileSpriteRect = {0, 0, (int) map.getTileSize().x, (int) map.getTileSize().y};
        SDL_FRect renderPosition = {0.0f, 0.0f, (float) map.getTileSize().x, (float) map.getTileSize().y};
        auto &numOfTiles = map.getTileCount();
        auto &layers = map.getLayers();
        const auto tileWidth = (float) map.getTileSize().x;
        const auto tileHeight = (float) map.getTileSize().y;
        int newX = (int) camera.screen.x;
        int newY = (int) camera.screen.y;

        int X = (newX / tileWidth);
        int Y = (newY / tileHeight);

        int width = X + (camera.screen.w / tileWidth);
        int height = Y + (camera.screen.h / tileHeight);

        int g = 0;
        //int o = 0;
        auto &tilesets = Maps::map.getTilesets();
        int p = Maps::map.getTilesets().size() - 1;
        for (int i = 0; i < layers.size(); i++) {
            if (layers[i]->getType() == tmx::Layer::Type::Tile) {
                const auto &tiles = layers[i]->getLayerAs<tmx::TileLayer>().getTiles();
                if (i == 2) {
                    /// renders all objects just above the 2nd layer but below the 3rd in tiled
                }
                for (int x = X; x < width; x++) {
                    if (x < 0) { x = 0; }
                    for (int y = Y; y < height; y++) {
                        //h++;
                        if (y < 0) { y = 0; }
                        if (x >= numOfTiles.x) { break; }
                        if (y >= numOfTiles.y) { break; }
                        renderPosition.x = originX + (x * tileWidth);
                        renderPosition.y = originY + (y * tileHeight);
                        renderPosition.x -= camera.screen.x;;
                        renderPosition.y -= camera.screen.y;;

                        int k = (numOfTiles.x * y) + x;
                        auto id = tiles[k].ID;

                        for (int tilesetCount = p; tilesetCount >= 0; --tilesetCount) {
                            const tmx::Tileset *tileset = &tilesets[tilesetCount];
                            if (tileset->getFirstGID() - 1 <= id) {
                                id -= tilesets[tilesetCount].getFirstGID() - 1;
                                std::string name = tileset->getName();
                                int tileCount = tileset->getColumnCount();
                                SDL_Rect data = getTexture(id, tileCount, tileSpriteRect);
                                if (Graphics::pTexture[name] != NULL) {
                                    Graphics::Render_FRect(Graphics::pTexture[name], {255, 255, 255}, &data, &renderPosition);
                                }
                                //h++;
                                break;
                            }
                        }
                    }
                    g++;
                }
                if (i == 1) {

                }
            }
        }
    }

    void Render_Map(entt::registry &zone, Component::Camera& camera) {
        SDL_RenderClear(Graphics::renderer);

        World::Render(camera);
//        Render_Iso_Tiles(zone, Maps::map, camera);
//        Render_Ortho_Tiles(zone, Maps::map, camera);

        //Render_Dead(zone, camera);
        Interface::Background();
        Animation_Frame(zone, camera);
        Render_Explosions(zone, camera);
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

        SDL_FRect renderRect = {
                camera.screen.x - (camera.screen.w / 4.0f),
                camera.screen.y - (camera.screen.h / 4.0f),
                camera.screen.w * 4.0f,
                camera.screen.h * 4.0f };
        auto objectsView = zone.view<Component::Position>(entt::exclude<Item_Component::Name>);
        //if you add Item_Component::Item_Type to this list it will not show ground items, instead I can give a graphic to a ground item
        float bottomOfScreenEdge = camera.screen.y + camera.screen.h;
        float bottomOfRenderRect = renderRect.y + renderRect.h;
        for (auto entity : objectsView) {
            auto& position = objectsView.get<Component::Position>(entity);
            SDL_FPoint point = {position.x, position.y};
            if (Utilities::bFPoint_FRectIntersect(point, renderRect)) {

                if(zone.any_of<Component::Renderable>(entity)) {
                    //  update renderable values
                    int alpha = Set_Render_Position_Alpha(bottomOfScreenEdge, bottomOfRenderRect, position.y);
                    auto &renderable = zone.get<Component::Renderable>(entity);
                    renderable.alpha = alpha;

                    if (zone.all_of<Component::Line_Segment>(entity) == false) {
                        renderable.y = position.y;
                    }
                }
                else {
//                    emplace and initialize renderable
                    int alpha = Set_Render_Position_Alpha(bottomOfScreenEdge, bottomOfRenderRect, position.y);
                    auto &renderable = zone.emplace_or_replace<Component::Renderable>(entity);
                    renderable.alpha = alpha;
                    if (zone.all_of<Component::Line_Segment>(entity) == false) {
                        renderable.y = position.y;
                    }
                }
            }
            else {
                zone.remove<Component::Renderable>(entity);
            }
        }
    }

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
            Camera_Control::Update_Camera_Follow(camera, position);
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

        auto camera_view = zone.view<Component::Camera>();
        for (auto entity : camera_view) {
            auto& camera = camera_view.get<Component::Camera>(entity);
//			SDL_RenderClear(Graphics::renderer);
            Add_Remove_Renderable_Component(zone, camera);
            sort_Positions(zone);
            Render_Map(zone, camera);
            Remove_Entities_From_Registry(zone); // cannot be done before clearing the entities from the quad tree

//            RenderLine(zone, camera);
            //
            Items::Show_Ground_Items(zone, camera);
            Items::Unit_Name_On_Mouseover(zone, camera);
            Social_Control::Show_Dialogue(zone, camera);
            Items::Name_On_Mouseover(zone, camera);
            UI::Render_UI(zone, Graphics::renderer, camera);
            Character_Stats::Render_Character_Stats(zone, camera);
            Items::Update_Mouse_Slot_Position(zone, Mouse::mouseItem, Mouse::itemCurrentlyHeld, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
            Damage_Text::Show_Damage(zone, camera);
            UI_Resources::Render_Resources(zone, camera);
            UI_Spellbook::Draw_Spellbook(camera);
            Pause::Pause_Control(camera);
            Menu::Render_Menu(zone, camera);
            //Mouse
            Interface::Foreground(zone, camera);
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
        }
    }

    void Present() {
        SDL_RenderPresent(Graphics::renderer);
    }
}
