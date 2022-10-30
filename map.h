#pragma once
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <algorithm>
#include <iostream>
#include "entt.hpp"
#include "components.h"
#include "entity_loader.h"
#include "collision.h"
#include "sprite_sheets.h"

//cell 100x100 pixels (change pixels to a meters??)
//map 100 cells x 100 cells

//hardcode the default size of the cell and the size of the map (eventually add the map as a cell for the "world" array)
//each cell needs an ID

//create the cell bounding boxes (the top left will be 0,0 and the map will grow right and down)
//parse all the trees and place them into their cell vector

namespace Maps {
    tmx::Map map;
    SDL_Texture* pTexture = NULL;
    int template_ID = 0;
    std::map<std::string, int> savedTemplate_IDs;

    // read name of the template object (it is already unique)
    // assign it an auto incrementented int
    // check if the object has already been assigned an int ID
    int Check_For_Template_ID (std::string &name) {
        if (savedTemplate_IDs.count(name) == 0){
            savedTemplate_IDs[name] = template_ID;
            int unit_ID = template_ID;
            template_ID++;
          //  std::cout << "created and assigned template key: " << unit_ID << std::endl;
            return unit_ID;
        }
        else {
           // std::cout << "assigned existing template key: " << savedTemplate_IDs[name] << std::endl;
            return savedTemplate_IDs[name];
        }
        std::cout << "failed to assign template key for: " << name << std::endl;
    }

    int Get_Existing_Template_ID (std::string &name, std::string &entity_class) {
        if (savedTemplate_IDs.count(name) == 0) {
            //if it is not found save the class as the name (the class will always have the name of the base unit of the class and thus will default to the base unit if the random unit is not found)
            name = entity_class;
            if (savedTemplate_IDs.count(name) == 0) {
                savedTemplate_IDs[name] = template_ID;
                int unit_ID = template_ID;
                template_ID++;
                //  std::cout << "created and assigned template key: " << unit_ID << std::endl;
                return unit_ID;
            }
            return savedTemplate_IDs[name];
        }
        else {
            // std::cout << "assigned existing template key: " << savedTemplate_IDs[name] << std::endl;
            return savedTemplate_IDs[name];
        }
        std::cout << "failed to assign template key for: " << name << std::endl;
    }

    void Create_Entity(entt::registry& zone, float x, float y, std::string &name, std::string entity_class, bool is_random, std::string &filepath) {
        auto entity = zone.create();
        Entity_Loader::Data data;
        int unit_ID = 0;

        //get the entity_class from the template in the tiled map
        //get the is_random from the template in the tiled map

        //if it is random, grab a random entry of the same class from the DB table, including the key name
        //
        //scaling only applies to trees
        if (is_random == 1) {
            name = Entity_Loader::Get_All_Of_Class(entity_class);

            //check if the random name has a tamplate ID, if it doesn't revert to default name
            unit_ID = Get_Existing_Template_ID(name, entity_class);
            Graphics::Create_Game_Object(unit_ID, filepath.c_str());
            data = Entity_Loader::parse_data(name);
//
            ////randomEntity must be converted into a std::string
//
            //auto& scale = zone.get<Component::Scale>(entity);
            //auto& radius = zone.get<Component::Radius>(entity);
//
            ////random number between 0.25 and 2
            //int rand_scale = 0.5;
            //scale.scale = rand_scale;
        }
        else {
            unit_ID = Check_For_Template_ID(name);
            Graphics::Create_Game_Object(unit_ID, filepath.c_str());
            data = Entity_Loader::parse_data(name);
        }

        // translates isometric position to world position
        float tileWidth = 128;
        float tileHeight = 64;
        float numTilesX =  x / tileWidth;
        float numTilesY =  y / tileHeight;
        x = 64.0f + x - (numTilesY * tileWidth / 2.0f);
        y = -37.0f + (numTilesX * tileHeight) + (y / 2.0f);

        //Add shared components
        auto& position = zone.emplace<Component::Position>(entity, x, y);
        auto& scale = zone.emplace<Component::Scale>(entity, data.scale);

        zone.emplace<Component::Radius>(entity, data.radius);
        zone.emplace<Component::Direction>(entity, Component::Direction::N);
        zone.emplace<Component::handle>(entity, name);
        zone.emplace<Component::Mass>(entity, data.mass);
        zone.emplace<Component::Alive>(entity, true);
        zone.emplace<Component::Sprite_Offset>(entity, data.x_offset_sprite, data.y_offset_sprite);
        zone.emplace<Component::animation>(entity, Graphics::unitTextures[unit_ID]); /// need to load the texture only once and pass the pointer into that function

        //dynamic entities
        if (data.body_type == 1) {
            bool yes = true;

            Collision::Create_Dynamic_Body(zone, entity, position.x, position.y, data.radius, data.mass, yes);
            zone.emplace<Component::Actions>(entity, Component::idle);
            auto &frame = zone.get<Component::Actions>(entity).frameCount = { {0, 0}, { 4, 0}, {7, 0}, {4, 0}, {4,0}, {2,0}, {5,0}, {4,0} };

            int width0 = frame[0].NumFrames * data.sprite_width;
            int width1 = frame[1].NumFrames * data.sprite_width;
            int width2 = (frame[2].NumFrames + 1) * data.sprite_width;
            int width3 = frame[3].NumFrames * data.sprite_width;
            int width4 = frame[4].NumFrames * data.sprite_width;
            int width5 = frame[5].NumFrames * data.sprite_width;
            int width6 = (frame[6].NumFrames + 1) * data.sprite_width;
            int width7 = frame[7].NumFrames * data.sprite_width;

            int start0 = width0;
            int start1 = start0 + width1;
            int start2 = start1 + width2;
            int start3 = start2 + width3;
            int start4 = start3 + width4;
            int start5 = start4 + width5;
            int start6 = start5 + width6;
            zone.get<Component::animation>(entity).sheet = { //populate the vector
                { NULL },
                { {start0, 0, data.sprite_width, data.sprite_height}, start0, width1,  1, 0, 75.0f, 0.0f},//idle array[numframes] = { 2ms, 4ms, 2ms}
                { {start1, 0, data.sprite_width, data.sprite_height}, start1, width2,  0, 0, 75.0f, 0.0f},//walk
                { {start2, 0, data.sprite_width, data.sprite_height}, start2, width3,  0, 0, 75.0f, 0.0f},//atack
                { {start3, 0, data.sprite_width, data.sprite_height}, start3, width4,  0, 0, 75.0f, 0.0f},//cast
                { {start4, 0, data.sprite_width, data.sprite_height}, start4, width5,  0, 0, 75.0f, 0.0f},//block
                { {start5, 0, data.sprite_width, data.sprite_height}, start5, width6,  0, 0, 75.0f, 0.0f}, //reverse to summon
                { {start6, 0, data.sprite_width, data.sprite_height}, start6, width7,  1, 0, 75.0f, 0.0f},//ranged
            };

            zone.emplace<Component::Melee_Damage>(entity, data.damage_min, data.damage_max);
            zone.emplace<Component::Attack_Speed>(entity, data.attack_speed, 0);
            zone.emplace<Component::Velocity>(entity, 0.f, 0.0f, 0.f, 0.0f, data.speed);
            zone.emplace<Component::Health>(entity, data.health);
            zone.emplace<Component::Melee_Range>(entity, (data.radius + data.melee_range));
            zone.emplace<Component::Soldier>(entity);
            zone.emplace<Component::Commandable>(entity);
            zone.emplace<Component::Spellbook>(entity);

            if (name == "'player'") {
                zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::player);
                zone.emplace<Input>(entity);
                zone.emplace<Camera>(entity, 0.0f, 0.0f, Graphics::resolution.w, Graphics::resolution.h, 2.0f, 2.0f);

            }
            else {
                zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::unit);
                zone.emplace<Component::Sight_Range>(entity, data.sight_radius, position.x - (data.sight_radius / 2.0f), position.y - (data.sight_radius / 2.0f), data.sight_radius, data.sight_radius);
            }
        }

        //static entities
        else if (data.body_type == 0) {
            Collision::Create_Static_Body(zone, entity, position.x, position.y, data.radius);
            zone.get<Component::animation>(entity).sheet = {  //populate the vector
                {{ 0, 0, data.sprite_width, data.sprite_height}, 0, data.sprite_width, 0, 0, 16.0f, 0.0f }
            };
            zone.emplace<Actions>(entity, isStatic);
            zone.get<Actions>(entity).frameCount = { { 0, 0} };

            zone.emplace<Component::Entity_Type>(entity, Entity_Type::foliage);
        }
    }

    // read the map data and import it into the data structures
    void Create_Map() {
        if (map.load("maps/untitled3.tmx")){
            for (auto &spriteSheet : map.getTilesets()) {

                std::string name = spriteSheet.getName();
                std::string filePathString = spriteSheet.getImagePath();
                filePathString.erase(0, 5);
                const char* filePathChar = filePathString.c_str();
                Graphics::pTexture[name] = Graphics::createTexture(filePathChar);

            }
            std::cout << "Loaded Map version: " << map.getVersion().upper << ", " << map.getVersion().lower << std::endl;
            if (map.isInfinite())
            {
                std::cout << "Map is infinite.\n";
            }

            const auto& layers = map.getLayers();
            std::cout << "Map has " << layers.size() << " layers" << std::endl;

            for (const auto& layer : layers) {
                if (layer->getType() == tmx::Layer::Type::Tile) {
                    const auto& tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();
                    if (tiles.empty()) {
                        std::cout << "Layer has missing tile data\n";
                    }
                    else {
                        std::cout << "Layer has " << tiles.size() << " tiles.\n";
                    }
                }
                if (layer->getType() == tmx::Layer::Type::Object) {
                    //layer->getLayerAs<tmx::ObjectLayer>();
                    //for (auto object : ) {

                    //layer->getLayerAs<tmx::ObjectGroup>().
                    for (auto &object : layer->getLayerAs<tmx::ObjectGroup>().getObjects()) {
                        auto &position = object.getPosition();

                        std::string name = object.getName();

                        std::string entity_class = object.getClass();

                        bool is_random = false;
                        for (auto i : object.getProperties()) {
                            is_random = i.getBoolValue();
                        }
                        //if it is random it needs to grab a name from a unit that was already loaded into graphics or default to a default unit name
                        //get an array of all the potential names, check each on against teh std::map of graphics, keep all the ones already there and pick a random one
                        //if (is_random == false ) {}
                        std::string texture = object.getTilesetName();
                        auto &ff = map.getTemplateTilesets();
                        texture = ff.at(texture).getImagePath();
                        texture.erase(0, 5);

                        Create_Entity(World::zone, position.x, position.y, name, entity_class, is_random, texture);
                    };
                    //}
                }
            }

        }
        else
        {
            std::cout << "Failed loading map" << std::endl;
        }
    }
}
