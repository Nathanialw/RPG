#pragma once
#include "components.h"
#include "graphics.h"
#include "SQLite_unit_data.h"
#include "SQLite_spritesheets.h"
#include "character_data.h"
#include "action_components.h"
#include "collision.h"
#include "social_control.h"
#include "item_components.h"
#include "spellbook.h"
#include "ui_actionbar.h"


namespace Create_Entities {

    int template_ID = 0;
    std::map<std::string, int> savedTemplate_IDs;

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
            zone.emplace<Action_Component::Action>(entity, Action_Component::isStatic);
            zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::foliage);
            zone.emplace<Component::Line_Segment>(entity, line);
        }
        else if (entity_class == "rect_building") {
            Collision::Create_Static_Body_Rect(zone, entity, position.x, position.y, aabb);
            zone.emplace<Action_Component::Action>(entity, Action_Component::isStatic);
            zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::foliage);
        }
        else if (entity_class == "round_building") {
            float rad = 185.0f;
            Collision::Create_Static_Body(zone, entity, position.x, position.y, rad);
            zone.emplace<Action_Component::Action>(entity, Action_Component::isStatic);
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
            std::unordered_map<std::string, Rendering_Components::Sheet_Data_Flare>* flareSheetData = NULL;
            std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerframeData = NULL;

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

            auto &sprite = zone.emplace<Rendering_Components::Sprite_Sheet_Info>(entity);
            sprite.flareSpritesheet = flareSheetData;
            sprite.sheet_name = name;
            sprite.type = sheetDataFlare.sheet_type;
            zone.emplace<Rendering_Components::Sprite_Offset>(entity, sheetDataFlare.x_offset, sheetDataFlare.y_offset);

            Set_Collision_Box(zone, entity, entity_class, position, aabb, pointVecs, line);
            return true;
        }
        return false;
    }

    void Create_Entity(entt::registry& zone, float x, float y, std::string name, std::string entity_class, bool is_random, std::string imgpath, bool &player) {
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
            Graphics::Create_Game_Object(unit_ID, imgpath.c_str());
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
            Graphics::Create_Game_Object(unit_ID, imgpath.c_str());
            data = Entity_Loader::parse_data(name);
        }

        SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
        std::string sheetname = Entity_Loader::Get_Sprite_Sheet(name);
        std::unordered_map<std::string, Rendering_Components::Sheet_Data_Flare>* flareSheetData = NULL;
        std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerframeData = NULL;

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
        zone.emplace<Rendering_Components::Sprite_Offset>(entity, data.x_offset * data.scale, data.y_offset * data.scale);

        auto &full_name = zone.emplace<Component::Name>(entity);
        Character_Data::Get_Name(full_name);

        auto &sprite = zone.emplace<Rendering_Components::Sprite_Sheet_Info>(entity);

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
                zone.emplace<Rendering_Components::Equipment_Sprites>(entity);
            }

            bool yes = true;
            Collision::Create_Dynamic_Body(zone, entity, position.x, position.y, radius.fRadius * data.scale, data.mass * data.scale, yes);
            zone.emplace<Collision_Component::Dynamic_Collider>(entity);

            //do not attach to non combat
            if (data.temp_type_name != "non-combat") {
                zone.emplace<Component::Melee_Damage>(entity, data.damage_min, data.damage_max);
                zone.emplace<Component::Attack_Speed>(entity, data.attack_speed, 0);
            }

            zone.emplace<Component::Melee_Range>(entity, ((data.radius + data.melee_range) * data.scale));
            zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::unit);
            zone.emplace<Action_Component::Action>(entity, Action_Component::idle);
            zone.emplace<Component::Velocity>(entity, 0.0f, 0.0f, 0.0f, 0.0f, data.speed * data.scale);
            auto &health = zone.emplace<Component::Health>(entity, int(data.health * data.scale));
            zone.emplace<Component::Soldier>(entity);
            zone.emplace<Component::Commandable>(entity);
            zone.emplace<Component::Spellbook>(entity);

            if (player) {
                health.currentHealth += 200;
                zone.emplace<Component::Input>(entity);
                auto &camera = zone.emplace<Component::Camera>(entity, 0.0f, 0.0f, (Graphics::resolution.w), (Graphics::resolution.h), 2.0f, 2.0f);
                UI_Spellbook::Init_UI(camera);
                Action_Bar::Create_Action_Bar(zone, camera);
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
            zone.emplace<Action_Component::Action>(entity, Action_Component::isStatic);
            zone.emplace<Component::Entity_Type>(entity, Component::Entity_Type::foliage);
        }
    }
}