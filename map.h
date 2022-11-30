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
#include "world.h"
#include "item_components.h"
#include "social_control.h"
#include "character_data.h"

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

    void Set_Collision_Box (entt::registry &zone, entt::entity &entity, std::string &entity_class, Component::Position &position, Collision::aabb &aabb, std::vector<std::vector<tmx::Vector2<float>>> &pointVecs, Component::Line_Segment &line) {
        if (entity_class == "polygon_building") {
            Collision::Create_Static_Body_Polygon(zone, entity, position.x, position.y, pointVecs);
            zone.emplace<Component::Action>(entity, Component::isStatic);
            zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::foliage);
            zone.emplace<Component::Line_Segment>(entity, line);
        }
        else if (entity_class == "rect_building") {
            Collision::Create_Static_Body_Rect(zone, entity, position.x, position.y, aabb);
            zone.emplace<Component::Action>(entity, Component::isStatic);
            zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::foliage);
        }
        else if (entity_class == "round_building") {
            float rad = 185.0f;
            Collision::Create_Static_Body(zone, entity, position.x, position.y, rad);
            zone.emplace<Component::Action>(entity, Component::isStatic);
            zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::foliage);
        }
    }

    bool Polygon_Building(entt::registry& zone, float x, float y, std::string &name, std::string entity_class, std::string &filepath, Collision::aabb &aabb, std::vector<std::vector<tmx::Vector2<float>>> &pointVecs, Component::Line_Segment &line) {
            /// if it is a building
        if (entity_class == "polygon_building" || entity_class == "rect_building" || entity_class == "round_building") {
            auto entity = zone.create();
            int unit_ID = Check_For_Template_ID(name);
            Graphics::Create_Game_Object(unit_ID, filepath.c_str());

            SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
            std::string layout = Entity_Loader::Get_Building_Sprite_layout(name);
            std::unordered_map<std::string, Component::Sheet_Data_Flare>* flareSheetData = NULL;
            std::unordered_map<std::string, Component::Sheet_Data> *packerframeData = NULL;

            ///get sheet data for new pointer to map
            SQLite_Spritesheets::Get_Flare_Building_From_DB(name, layout, sheetDataFlare);
            flareSheetData = Populate_Flare_SpriteSheet(name, sheetDataFlare, Graphics::unitTextures[unit_ID]);

            //Add shared components
            auto& position = zone.emplace<Component::Position>(entity, x, y);
            auto& scale = zone.emplace<Component::Scale>(entity, 1.0f);
            auto &radius = zone.emplace<Component::Radius>(entity, 1.0f);

            /// static objects must be set to west as it is the 0 position in the enumeration, ugh yeah I know
            zone.emplace<Component::Direction>(entity, Component::Direction::W);
            zone.emplace<Component::Name>(entity, name);
            zone.emplace<Component::Mass>(entity, 100.0f);
            zone.emplace<Component::Alive>(entity, true);

            auto &sprite = zone.emplace<Component::Sprite_Sheet_Info>(entity);
            sprite.flareSpritesheet = flareSheetData;
            sprite.sheet_name = name;
            sprite.type = sheetDataFlare.sheet_type;
            zone.emplace<Component::Sprite_Offset>(entity, sheetDataFlare.x_offset, sheetDataFlare.y_offset);

            Set_Collision_Box(zone, entity, entity_class, position, aabb, pointVecs, line);
            return true;
        }
        return false;
    }

    void Create_Entity(entt::registry& zone, float x, float y, std::string &name, std::string entity_class, bool is_random, std::string &filepath, bool &player) {
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

        //Add shared components
        auto& position = zone.emplace<Component::Position>(entity, x, y);
        auto& scale = zone.emplace<Component::Scale>(entity, data.scale);
        auto &radius = zone.emplace<Component::Radius>(entity, (data.radius * data.scale));
        zone.emplace<Component::Interaction_Rect>(entity, (x - data.interact_r) * data.scale, (y - data.interact_h / 2.0f) * data.scale, (data.interact_r * 2.0f) * data.scale, data.interact_h * data.scale);

        zone.emplace<Component::Mass>(entity, data.mass * data.scale);
        zone.emplace<Component::Alive>(entity, true);
        zone.emplace<Component::Unit>(entity);
        zone.emplace<Component::Sprite_Offset>(entity, data.x_offset * data.scale, data.y_offset * data.scale);

        auto &full_name = zone.emplace<Component::Name>(entity);
        Character_Data::Get_Name(full_name);

        auto &sprite = zone.emplace<Component::Sprite_Sheet_Info>(entity);

        //if RTP_pieces type
        if (packerframeData) {
           sprite.sheetData = packerframeData;
           sprite.sheet_name = name;
           sprite.type = "RPG_Tools";
        }
        else {
            sprite.flareSpritesheet = flareSheetData;
            sprite.sheet_name = name;
            sprite.type = sheetDataFlare.sheet_type;
        }

        //dynamic entities
        if (data.body_type == 1) {
            zone.emplace<Component::Direction>(entity, Component::Direction::S);

            auto raceData = Entity_Loader::Get_Race_Relationsips(data.race);
            zone.emplace<Social_Component::Race>(entity, raceData[0]);
            auto &relationships = zone.emplace<Social_Component::Relationships>(entity);
            for (int i = 0; i < raceData.size(); i++) {
                relationships.races[i] = raceData[i+1];
            }

            if (data.equip_type != "none") {
                zone.emplace<Item_Component::Equipment>(entity, data.equip_type);
            }

            bool yes = true;
            Collision::Create_Dynamic_Body(zone, entity, position.x, position.y, radius.fRadius * data.scale, data.mass * data.scale, yes);
            zone.emplace<Collision_Component::Dynamic_Collider>(entity);

            zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::unit);
            zone.emplace<Component::Action>(entity, Component::idle);
            zone.emplace<Component::Melee_Damage>(entity, data.damage_min, data.damage_max);
            zone.emplace<Component::Attack_Speed>(entity, data.attack_speed, 0);
            zone.emplace<Component::Velocity>(entity, 0.0f, 0.0f, 0.0f, 0.0f, data.speed * data.scale);
            auto &health = zone.emplace<Component::Health>(entity, data.health * data.scale);
            zone.emplace<Component::Melee_Range>(entity, ((data.radius + data.melee_range) * data.scale));
            zone.emplace<Component::Soldier>(entity);
            zone.emplace<Component::Commandable>(entity);
            zone.emplace<Component::Spellbook>(entity);

            if (player) {
                health.currentHealth += 200;
                zone.emplace<Component::Input>(entity);
                zone.emplace<Component::Camera>(entity, 0.0f, 0.0f, (Graphics::resolution.w), (Graphics::resolution.h), 2.0f, 2.0f);
            }
            else {
                zone.emplace<Component::Sight_Range>(entity, data.sight_radius * data.scale, position.x - (data.sight_radius / 2.0f * data.scale), position.y - (data.sight_radius / 2.0f * data.sight_radius), data.sight_radius * data.scale, data.sight_radius * data.scale);
            }
        }
        //static entities
        //if it is an aabb building
        else if (data.body_type == 0) {
            /// static objects must be set to west as it is the 0 position in the enumeration, ugh yeah I know
            zone.emplace<Component::Direction>(entity, Component::Direction::W);
            Collision::Create_Static_Body(zone, entity, position.x, position.y, (data.radius * data.scale));
            zone.emplace<Component::Action>(entity, Component::isStatic);
            zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::foliage);
        }
    }

    // read the map data and import it into the data structures
    void Create_Map() {
        if (map.load("maps/untitled3.tmx")){
            for (auto &spriteSheet : map.getTilesets()) {
                std::string name = spriteSheet.getName();
            }
            std::cout << "Loaded Map version: " << map.getVersion().upper << ", " << map.getVersion().lower << std::endl;

            const auto& layers = map.getLayers();
            std::cout << "Map has " << layers.size() << " layers" << std::endl;

            for (const auto& layer : layers) {
                if (layer->getName() == "ground") {
                    // get gid of each tile
                    std::vector<int> GIDs;
                    for (auto tile : layer->getLayerAs<tmx::TileLayer>().getTiles()) {
                        if (GIDs.empty()) {
                            GIDs.emplace_back(tile.ID);
                        }
                        bool found = false;
                        for (auto i : GIDs) {
                            if (i == tile.ID) {
                                found = true;
                                break;
                            }
                        }
                        if (found == false) {
                            GIDs.emplace_back(tile.ID);
                        }
                    }

                    // get tilesets of mapx
                    for (auto i : GIDs) {
                        for (auto tileset: map.getTilesets()) {
                            //match tileGID to texture GID
                            if (tileset.getFirstGID() <= i) {
                                int num = i - tileset.getFirstGID();
                                if (num <= 0) {
                                    //save texture
                                    std::string name = tileset.getName();
                                    if (!Graphics::pTexture[name]) {
                                        std::string filePathString = tileset.getImagePath();
                                        filePathString.erase(0, 5);
                                        const char *filePathChar = filePathString.c_str();
                                        Graphics::pTexture[name] = Graphics::createTexture(filePathChar);
//                                        std::cout << "loaded: " << name << std::endl;
                                    }
                                    break;
                                }
                            } else {
                                break;
                            }
                        }
                    }
                }
                else if (layer->getName() == "widgets") {
                }
                else if (layer->getName() == "taller_widgets1") {

                }
                else if (layer->getName() == "unit_spawns") {
                    //layer->getLayerAs<tmx::ObjectLayer>();
                    //for (auto object : ) {
                    //layer->getLayerAs<tmx::ObjectGroup>().
                    for (auto &object : layer->getLayerAs<tmx::ObjectGroup>().getObjects()) {
                        auto &position = object.getPosition();
                        std::string name = object.getName();
                        std::string entity_class = object.getClass();


                        // translates isometric position to world position
                        float tileWidth = 128;
                        float tileHeight = 64;
                        float numTilesX =  position.x / tileWidth;
                        float numTilesY =  position.y / tileHeight;
                        float x = 64.0f + position.x - (numTilesY * tileWidth / 2.0f);
                        float y = (numTilesX * tileHeight) + (position.y / 2.0f);
                        //if it is random it needs to grab a name from a unit that was already loaded into graphics or default to a default unit name
                        //get an array of all the potential names, check each on against teh std::map of graphics, keep all the ones already there and pick a random one
                        //if (is_random == false ) {}
                        std::string tilesetName = object.getTilesetName();
                        auto &templateTilesets = map.getTemplateTilesets();

                        //gets the collision box/boxes for a building
                        std::vector<tmx::Object> collision_boxes;
                        Collision::aabb aabb;
                        std::vector<std::vector<tmx::Vector2<float>>> pointVecs;
                        Component::Line_Segment line;

                        for (auto s :templateTilesets.at(tilesetName).getTiles()){
                            collision_boxes = s.objectGroup.getObjects();
                            float sizeX = s.imageSize.x;
                            float sizeY = s.imageSize.y;

                            int j = 0;
                            for (auto rects : collision_boxes) {
                                aabb.hx = rects.getAABB().width / 2.0f;
                                aabb.hy = rects.getAABB().height / 2.0f;
                                float aabbx = rects.getAABB().left;
                                float aabby = rects.getAABB().top;
                                if (rects.getShape() == tmx::Object::Shape::Point) {
                                    line.p[j].x = x - ((sizeX / 2.0f) - aabbx);
                                    line.p[j].y = y - (sizeY - aabby);
                                    j++;
                                }
                                else if (rects.getPoints().size() > 0) {
                                    std::vector<tmx::Vector2<float>> pointVec = rects.getPoints();
                                    for (int i = 0; i < pointVec.size(); i++) {
                                        pointVec[i].x = pointVec[i].x - ((sizeX / 2.0f) - aabbx);
                                        pointVec[i].y = pointVec[i].y - (sizeY - aabby);
                                    }
                                    pointVecs.emplace_back(pointVec);
                                }
                            }
                        }

                        bool player = false;
                        bool is_random = false;
                        for (auto property : object.getProperties()) {
                            if (property.getName() == "player"){
                                player = property.getBoolValue();
                            }
                            if (property.getName() == "is_random"){
                                is_random = property.getBoolValue();
                            }
                        }

                        std::string texture = templateTilesets.at(tilesetName).getImagePath();
                        texture.erase(0, 5);



                        if (!Polygon_Building(World::zone, x, y, name, entity_class, texture, aabb, pointVecs, line)) {
                            Create_Entity(World::zone, x, y, name, entity_class, is_random, texture, player);
                        }
                    };
                }
            }
        }
        else {
            std::cout << "Failed loading map" << std::endl;
        }
    };
}
