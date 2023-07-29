#pragma once

#include "../tinyxml2.h"
#include "spritesheet_structs.h"
#include "components.h"
#include <string>
#include "graphics.h"
#include "SQLite_item_data.h"

namespace Texture_Packer_Item {
    ///on init we need to do it parse the SQLite db for all sprite sheets names "texture_packer" and use the result to preallocate all the nodes of the std::unordered_map

    std::unordered_map<std::string, Rendering_Components::Sheet_Data> Packer_Textures_Items;
    std::unordered_map<std::string, SDL_Texture *> Item_Textures;

    struct Data {
        std::string item_name = "";
        std::string sprite_data = "";
        std::string xml_path = "";
        std::string texture_path = "";
        std::string equip_type = "";
    };

    Data Get_Sprite_Sheet(std::string &slot, std::string &equip_type) {// needs to search for  a specific row that I can input in the arguments
        std::string item_name;
        //get a random entry from item vector
        //need to know the UNIT_TYPE to get and the SLOT and that returns a vector of strings of the items

        if (SQLite_Item_Data::Items.at(equip_type).contains(SQLite_Item_Data::Get_Item_Type(slot)) && SQLite_Item_Data::Items.at(equip_type).at(SQLite_Item_Data::Get_Item_Type(slot)).size() != 0) {
            item_name = SQLite_Item_Data::Items.at(equip_type).at(SQLite_Item_Data::Get_Item_Type(slot))[rand() % SQLite_Item_Data::Items.at(equip_type).at(SQLite_Item_Data::Get_Item_Type(slot)).size()];
        } else {
            Utilities::Log("Get_Sprite_Sheet(std::string &slot) item unit_Type: " + equip_type + " - slot: " + slot + " - not found for the given unit_type and slot");
            Data data;
            return data;
        }

        Data data;
        data.item_name = item_name;
        std::string path;

        std::string sheet_name = db::Append_Quotes(item_name);

        const unsigned char *xml_path;
        const unsigned char *tex_path;
        const unsigned char *equip_typ;
        std::string texture_path;
        sqlite3_stmt *stmt2;
        char buf2[200];
        const char *jj2 = "SELECT xml_path, texture_path, equip_type FROM weapon_types WHERE type = ";
        strcpy(buf2, jj2);
        strcat(buf2, sheet_name.c_str());
        sqlite3_prepare_v2(db::db, buf2, -1, &stmt2, 0);
        while (sqlite3_step(stmt2) != SQLITE_DONE) {
            xml_path = sqlite3_column_text(stmt2, 0);
            const char *s = (const char *) xml_path;
            path = std::string(reinterpret_cast< const char *> (s));
            tex_path = sqlite3_column_text(stmt2, 1);
            const char *d = (const char *) tex_path;
            texture_path = std::string(reinterpret_cast< const char *> (d));
            equip_typ = sqlite3_column_text(stmt2, 2);
            const char *e = (const char *) equip_typ;
            data.equip_type = std::string(reinterpret_cast< const char *> (e));
        }
        data.xml_path = path;
        data.texture_path = texture_path;

        return data;
    }

    static void Load_Texture(std::string &index, const char *filepath) {
        Item_Textures[index] = Graphics::createTexture(filepath);
    }

    void Get_Item_Texture(std::string &index, const char *filepath) {
        if (Item_Textures[index] == NULL) {
            Load_Texture(index, filepath);
            if (Item_Textures[index] == NULL) {
                std::cout << "Create_Game_Object() failed to load  texture from file: " << filepath << std::endl;
            } else {
                //	std::cout << "loaded from file: " << filepath << std::endl;
            }
        } else {
            //std::cout << "already loaded: " << filepath << std::endl;
            //unitTextures[unitID];
        }
        //to render it jsut needs access to the texture array and the unitID
    }

    struct Item_Data_And_Index {
        std::unordered_map<std::string, Rendering_Components::Sheet_Data> *itemData;
        std::string index;
    };

    Item_Data_And_Index TexturePacker_Import_Item(std::string &itemType, std::string &equip_type) {

        ///get path from db
        Data dbData = Get_Sprite_Sheet(itemType, equip_type);
        tinyxml2::XMLDocument spriteSheetData;

        std::string pathStr = "assets/" + dbData.xml_path;

        if (dbData.xml_path == "") {
            return {NULL, ""};
        }
        const char *path = pathStr.c_str();

        if (path == NULL) {
            Utilities::Log("TexturePacker_Import_Item() failed, empty xml_path");
            return {NULL, ""};
        }

        spriteSheetData.LoadFile(path);

        tinyxml2::XMLElement *pSpriteElement = spriteSheetData.RootElement()->FirstChildElement("sprite");

        Rendering_Components::Sprite_Sheet_Data frame = {};
        Rendering_Components::Sheet_Data spritesheet;
        spritesheet.frameList.reserve(200);

        std::string igmPathStr = "assets/" + dbData.texture_path;

        const char *tex = igmPathStr.c_str();
        Get_Item_Texture(dbData.item_name, tex);
        spritesheet.texture = Item_Textures[dbData.item_name];
        //        spritesheet.color = Graphics::Set_Random_Color();

        int frameIndex = 0;
        bool check = true;
        ///get the rest of the xml
        while (pSpriteElement != NULL) {
            ///get frame data for each state
            std::string n = pSpriteElement->Attribute("n");

            Spritesheet_Structs::Get_Frame_Action_Data(dbData.equip_type, dbData.item_name, n, spritesheet.actionFrameData, frameIndex);

            ///get sprite data
            frame.clip.x = pSpriteElement->IntAttribute("x");
            frame.clip.y = pSpriteElement->IntAttribute("y");
            frame.clip.w = pSpriteElement->IntAttribute("w");
            frame.clip.h = pSpriteElement->IntAttribute("h");
            frame.x_offset = pSpriteElement->IntAttribute("oX");
            frame.y_offset = pSpriteElement->IntAttribute("oY");
            spritesheet.frameList.emplace_back(frame);
            frameIndex++;
            ///this grabs the next line
            pSpriteElement = pSpriteElement->NextSiblingElement("sprite");
        }

        spritesheet.frameList.shrink_to_fit();

        Packer_Textures_Items[dbData.item_name] = spritesheet;

        //        for (auto aa : spritesheet.actionFrameData) {
        //            std::cout << "state: " << aa.first << ", start frame: " << aa.second.startFrame << ", number of frames: " << aa.second.NumFrames << std::endl;
        //        }

        Item_Data_And_Index values;
        values.itemData = &Packer_Textures_Items;
        values.index = dbData.item_name;
        return values;
    }
}
