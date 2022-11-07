#pragma once
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <algorithm>
#include <iostream>
#include "entt/entt.hpp"
#include "components.h"
#include "SQLite_unit_data.h"
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
            data = Entity_Loader::parse_data(name);//
            ////randomEntity must be converted into a std::string//
            //auto& scale = zone.get<Component::Scale>(entity);
            //auto& radius = zone.get<Component::Radius>(entity);//
            ////random number between 0.25 and 2
            //int rand_scale = 0.5;
            //scale.scale = rand_scale;
        }
        else {
            if (name == "player") {
                Utilities::Log("player");
            }
            unit_ID = Check_For_Template_ID(name);
            Graphics::Create_Game_Object(unit_ID, filepath.c_str());
            data = Entity_Loader::parse_data(name);
        }

        SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
        std::string sheetname = Entity_Loader::Get_Sprite_Sheet(name);
        std::unordered_map<std::string, Component::Sheet_Data_Flare>* flareSheetData = NULL;
        std::unordered_map<std::string, Component::Sheet_Data> *packerframeData = NULL;

        if (sheetname == "texture_packer") {
            ///run texture packer
            packerframeData = Texture_Packer::TexturePacker_Import(name, sheetname, Graphics::unitTextures[unit_ID]);
        }
        else {
                ///get sheet data for new pointer to map
            SQLite_Spritesheets::Get_Flare_From_DB(sheetname, sheetDataFlare);
            flareSheetData = Populate_Flare_SpriteSheet(name, sheetDataFlare, Graphics::unitTextures[unit_ID]);
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
        auto &radius = zone.emplace<Component::Radius>(entity, data.radius * data.scale);
        zone.emplace<Component::Direction>(entity, Component::Direction::N);
        zone.emplace<Component::handle>(entity, name);
        zone.emplace<Component::Mass>(entity, data.mass);
        zone.emplace<Component::Alive>(entity, true);
        if (packerframeData) {
           auto &sprite = zone.emplace<Component::Sprite_Sheet_Info>(entity);
           sprite.sheetData = packerframeData;
           sprite.sheet_name = name;
           sprite.type = "RPG_Tools";
           zone.emplace<Component::Sprite_Offset>(entity, 75.0f * data.scale, 100.0f * data.scale);
        }
        else {
            auto &sprite = zone.emplace<Component::Sprite_Sheet_Info>(entity);
            sprite.flareSpritesheet = flareSheetData;
            sprite.sheet_name = name;
            sprite.type = sheetDataFlare.sheet_type;
            zone.emplace<Component::Sprite_Offset>(entity, sheetDataFlare.x_offset * data.scale, sheetDataFlare.y_offset * data.scale);
        }

        //dynamic entities
        if (data.body_type == 1) {
            bool yes = true;
            Collision::Create_Dynamic_Body(zone, entity, position.x, position.y, radius.fRadius, data.mass, yes);
            zone.emplace<Component::Action>(entity, Component::idle);
            zone.emplace<Component::Melee_Damage>(entity, data.damage_min, data.damage_max);
            zone.emplace<Component::Attack_Speed>(entity, data.attack_speed, 0);
            zone.emplace<Component::Velocity>(entity, 0.f, 0.0f, 0.f, 0.0f, data.speed);
            zone.emplace<Component::Health>(entity, data.health);
            zone.emplace<Component::Melee_Range>(entity, (data.radius + data.melee_range));
            zone.emplace<Component::Soldier>(entity);
            zone.emplace<Component::Commandable>(entity);
            zone.emplace<Component::Spellbook>(entity);
            if (name == "player") {
                zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::player);
                zone.emplace<Component::Input>(entity);
                zone.emplace<Component::Camera>(entity, 0.0f, 0.0f, Graphics::resolution.w, Graphics::resolution.h, 2.0f, 2.0f);
            }
            else {
                zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::unit);
                zone.emplace<Component::Sight_Range>(entity, data.sight_radius, position.x - (data.sight_radius / 2.0f), position.y - (data.sight_radius / 2.0f), data.sight_radius, data.sight_radius);
            }
        }
        //static entities
        else if (data.body_type == 0) {
            Collision::Create_Static_Body(zone, entity, position.x, position.y, data.radius);
            zone.emplace<Component::Action>(entity, Component::isStatic);
            zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::foliage);
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
            if (map.isInfinite()) {
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
                        if (texture == "") {
                            Utilities::Log("asds");
                        }
                        Create_Entity(World::zone, position.x, position.y, name, entity_class, is_random, texture);
                    };
                }
            }

        }
        else {
            std::cout << "Failed loading map" << std::endl;
        }
    };
}
