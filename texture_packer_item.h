#pragma once
#include "tinyxml2.h"
#include "spritesheet_structs.h"
#include "components.h"
#include <string>
#include "graphics.h"
#include "SQLite_item_data.h"

namespace Texture_Packer_Item {
    ///on init we need to do it parse the SQLite db for all sprite sheets names "texture_packer" and use the result to preallocate all the nodes of the std::unordered_map

    std::unordered_map<std::string, Component::Sheet_Data> Packer_Textures_Items;
    std::unordered_map<std::string, SDL_Texture*> Item_Textures;

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
        }
        else {
            Utilities::Log("Get_Sprite_Sheet(std::string &slot) item not found for the given unit_type and slot");
            Data data;
            return data;
        }

        Data data;
        data.item_name = item_name;
        std::string path;

        std::string sheet_name = db::Append_Quotes(item_name);

        const unsigned char* xml_path;
        const unsigned char* tex_path;
        const unsigned char* equip_typ;
        std::string texture_path;
        sqlite3_stmt* stmt2;
        char buf2[200];
        const char* jj2 = "SELECT xml_path, texture_path, equip_type FROM weapon_types WHERE type = ";
        strcpy(buf2, jj2);
        strcat(buf2, sheet_name.c_str());
        sqlite3_prepare_v2(db::db, buf2, -1, &stmt2, 0);
        while (sqlite3_step(stmt2) != SQLITE_DONE) {
            xml_path = sqlite3_column_text(stmt2, 0);
            const char * s = (const char *)xml_path;
            path = std::string(reinterpret_cast< const char *> (s));
            tex_path = sqlite3_column_text(stmt2, 1);
            const char * d = (const char *)tex_path;
            texture_path = std::string(reinterpret_cast< const char *> (d));
            equip_typ = sqlite3_column_text(stmt2, 2);
            const char * e = (const char *)equip_typ;
            data.equip_type = std::string(reinterpret_cast< const char *> (e));
        }
        data.xml_path = path;
        data.texture_path = texture_path;

        return data;
    }

    void Calculate_Start_Frame (std::unordered_map<Component::Action_State, Component::Frame_Data_Packer> &actionFrameData, Component::Action_State &action, int &frameIndex) {
        /// I know this is terrible but it does work.
        if (actionFrameData[action].startFrame == 9999) {
            actionFrameData[action].startFrame = frameIndex;
        }
    }

    void Calculate_Num_Frames (std::string &frame, std::unordered_map<Component::Action_State,  Component::Frame_Data_Packer> &actionFrameData, Component::Action_State &action) {
        if (frame.back() != '0') {
            return;
        }
        else {
            actionFrameData[action].NumFrames++;
        }
    }

    int i = 120;

    bool Get_Frame_Action_Data (std::string unitType, bool &check, std::string key, std::string &frame, std::unordered_map<Component::Action_State,  Component::Frame_Data_Packer> &actionFrameData, int &frameIndex) {
        std::string keyCheck = key;
        std::string frameCopy = frame;
        /// presupposes that the top of the list is "itemName_00_1-H Attack 1_01_ 0" with 19 ending chars

        if (frameCopy.substr(0, keyCheck.size()) != keyCheck) {
            return false;
        }

        frameCopy = frame;
        /// remove the prepended keyCheck string from frameCopy
        auto index = frameCopy.find(keyCheck); // Find the starting position of substring in the string
        if (index != std::string::npos) {
            frameCopy.erase(index, keyCheck.length() + 1); // erase function takes two parameter, the starting index in the string from where you want to erase characters and total no of characters you want to erase.
        }
        ///remove the last 6 chars
        std::string checkAction = frameCopy.erase(frameCopy.length() - 6);
//        checkAction = key + checkAction;

//        std::cout << checkAction << std::endl;
        Component::Action_State action;
        /// compare the string in the xml with the values, I should probably just read in from the db, just push the test strings back on a vector and iterate through comparing, I wonder if I can store the enum in the db too I would probably have to for it to be worth it.
        if (unitType == "RTP_female" || unitType == "RTP_male") {
            if (checkAction == "1-H Attack 3") {
                action = Component::Action_State::attack;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Idle 2") {
                action = Component::Action_State::idle;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Running") {
                action = Component::Action_State::walk;
                actionFrameData[action].frameSpeed = 75;
            } else if (checkAction == "Get Hit 2") {
                action = Component::Action_State::struck;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Dead-Down Forward") {
                action = Component::Action_State::dying;
                actionFrameData[action].frameSpeed = 100;
            } else if (checkAction == "Praying") {
                action = Component::Action_State::casting;
                actionFrameData[action].frameSpeed = 150;
                actionFrameData[action].reverses = 0;
            } else if (checkAction == "Casting") {
                action = Component::Action_State::cast;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Down 2") {
                action = Component::Action_State::dead;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Talking 2") {
                action = Component::Action_State::talk;
                actionFrameData[action].frameSpeed = i;
            } else {
                return false;
            }
        }

        else if (unitType == "classes_male" || unitType == "classes_female") {
            if (checkAction == "Attack Two Hand Swing") {
                action = Component::Action_State::attack;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Idle3") {
                action = Component::Action_State::idle;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Run") {
                action = Component::Action_State::walk;
                actionFrameData[action].frameSpeed = 75;
            } else if (checkAction == "Get Hit") {
                action = Component::Action_State::struck;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Critical Health Idle 1") {
                action = Component::Action_State::dying;
                actionFrameData[action].frameSpeed = 100;
            } else if (checkAction == "Casting Idle") {
                action = Component::Action_State::casting;
                actionFrameData[action].frameSpeed = 150;
                actionFrameData[action].reverses = 1;
            } else if (checkAction == "Casting") {
                action = Component::Action_State::cast;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Dead") {
                action = Component::Action_State::dead;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Hand Casual") {
                action = Component::Action_State::talk;
                actionFrameData[action].frameSpeed = i;
            } else {
                return false;
            }
        }

        else if (unitType == "Medieval_Underdeep_Dwarves_Male" || unitType == "medieval_human_female" || unitType == "medieval_human_male") {
            if (checkAction == "attack1") {
                action = Component::Action_State::attack;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "idle1") {
                action = Component::Action_State::idle;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "running") {
                action = Component::Action_State::walk;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "collapse") {
                action = Component::Action_State::struck;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "ko") {
                action = Component::Action_State::dying;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "dead") {
                action = Component::Action_State::dead;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "kneel") {
                action = Component::Action_State::casting;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "sitting") {
                action = Component::Action_State::cast;
                actionFrameData[action].frameSpeed = i;
            } else {
                return false;
            }
        }

        else {
            if (checkAction == "1-H Attack 1") {
                action = Component::Action_State::attack;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Attack 1") {
                action = Component::Action_State::attack;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "1-H Idle") {
                action = Component::Action_State::idle;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Idle") {
                action = Component::Action_State::idle;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Idle1") {
                action = Component::Action_State::idle;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "1-H Walk") {
                action = Component::Action_State::walk;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Run") {
                action = Component::Action_State::walk;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Running") {
                action = Component::Action_State::walk;
                actionFrameData[action].frameSpeed = 75;
            } else if (checkAction == "Get Hit 1") {
                action = Component::Action_State::struck;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Get Hit") {
                action = Component::Action_State::struck;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Down 1") {
                action = Component::Action_State::dying;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Down_1") {
                action = Component::Action_State::dead;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Dead To Down") {
                action = Component::Action_State::dying;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Dead2") {
                action = Component::Action_State::dying;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Casting") {
                action = Component::Action_State::cast;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Use Skill") {
                action = Component::Action_State::cast;
                actionFrameData[action].frameSpeed = i;
            } else {
                return false;
            }
        }
        Calculate_Start_Frame(actionFrameData, action, frameIndex);
        Calculate_Num_Frames(frame, actionFrameData, action);
        return true;
    }

    void Load_Texture(std::string &index, const char* filepath) {
        Item_Textures[index] = Graphics::createTexture(filepath);
    }

    void Get_Item_Texture(std::string &index, const char* filepath) {
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
        std::unordered_map<std::string, Component::Sheet_Data>* itemData;
        std::string index;
    };

        /// ie "Sword"
    Item_Data_And_Index TexturePacker_Import_Item(std::string &itemType, std::string &equip_type) {

            ///get path from db
        Data dbData = Get_Sprite_Sheet(itemType, equip_type);
        tinyxml2::XMLDocument spriteSheetData;
        if (dbData.xml_path == "") {
            return {NULL, ""};
        }
        const char* path = dbData.xml_path.c_str();
        if (path == NULL){
            Utilities::Log("TexturePacker_Import_Item() failed, empty xml_path");
            return {NULL, ""};
        }
        Utilities::Log(dbData.xml_path);
        spriteSheetData.LoadFile(path);

        tinyxml2::XMLElement* pSpriteElement = spriteSheetData.RootElement()->FirstChildElement("sprite");

        Component::Sprite_Sheet_Data frame = {};
        Component::Sheet_Data spritesheet;
        spritesheet.frameList.reserve(200);
        const char* tex = dbData.texture_path.c_str();
        Get_Item_Texture(dbData.item_name, tex);
        spritesheet.texture = Item_Textures[dbData.item_name];

        int frameIndex = 0;
        bool check = true;
            ///get the rest of the xml
        while (pSpriteElement != NULL) {
                ///get frame data for each state
            std::string n = pSpriteElement->Attribute("n");

            Get_Frame_Action_Data(dbData.equip_type, check, dbData.item_name, n, spritesheet.actionFrameData, frameIndex);

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

        for (auto aa : spritesheet.actionFrameData) {
//            std::cout << "state: " << aa.first << ", start frame: " << aa.second.startFrame << ", number of frames: " << aa.second.NumFrames << std::endl;
        }

        Item_Data_And_Index values;
        values.itemData = &Packer_Textures_Items;
        values.index = dbData.item_name;
        return values;
    }
}