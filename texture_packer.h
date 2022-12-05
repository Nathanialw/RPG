#pragma once
#include "tinyxml2.h"
#include "spritesheet_structs.h"
#include "components.h"
#include <string>

namespace Texture_Packer {
        ///on init we need to do it parse the SQLite db for all sprite sheets names "texture_packer" and use the result to preallocate all the nodes of the std::unordered_map

    std::unordered_map<std::string, Rendering_Components::Sheet_Data> Packer_Textures;

    struct Type_Data{
        std::string type;
        std::string path;
    };

    Type_Data Get_Sprite_Sheet(std::string sheetname) {// needs to search for  a specific row that I can input in the arguments
        Type_Data typeData;
            ///check if the name exists??
        std::string sheet_name = db::Append_Quotes(sheetname);

        const unsigned char* sheet;
        const unsigned char* type;
        sqlite3_stmt* stmt;
        char buf[400];
        const char* jj = "SELECT xml_path, equip_type FROM unit_data WHERE name = ";
        strcpy(buf, jj);
        strcat(buf, sheet_name.c_str());
        sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
        while (sqlite3_step(stmt) != SQLITE_DONE) {
            sheet = sqlite3_column_text(stmt, 0);
            const char * s = (const char *)sheet;
            typeData.path = std::string(reinterpret_cast< const char *> (s));
            type = sqlite3_column_text(stmt, 1);
            s = (const char *)type;
            typeData.type = std::string(reinterpret_cast< const char *> (s));
        }
        return typeData;
    }

    Rendering_Components::Sprite_Offset Get_Sprite_Offets_From_db(std::string &sheet) {// needs to search for  a specific row that I can input in the arguments
        std::string sheet_name = db::Append_Quotes(sheet);
        Rendering_Components::Sprite_Offset offset = {};
        sqlite3_stmt *stmt;
        const unsigned char *sheetType;
        char buf[300];
        const char *ii = "SELECT x_offset, y_offset FROM sprite_layout WHERE sprite_sheet = ";
        strcpy(buf, ii);
        strcat(buf, sheet_name.c_str());
        sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
        while (sqlite3_step(stmt) != SQLITE_DONE) {
            offset.x = (float) sqlite3_column_double(stmt, 0);
            offset.y = (float) sqlite3_column_double(stmt, 1);
        }
        return offset;
    }

    void Calculate_Start_Frame (std::unordered_map<Component::Action_State, Rendering_Components::Frame_Data_Packer> &actionFrameData, Component::Action_State &action, int &frameIndex) {
        if (actionFrameData[action].startFrame == 9999) {
            actionFrameData[action].startFrame = frameIndex;
        }
    }

    void Calculate_Num_Frames (std::string &frame, std::unordered_map<Component::Action_State,  Rendering_Components::Frame_Data_Packer> &actionFrameData, Component::Action_State &action) {
        if (frame.back() != '1') {
            return;
        }
        else {
            actionFrameData[action].NumFrames++;
        }
    }

    int i = 120;

    void Get_Frame_Action_Data (std::string unitType, std::string &name, std::string &frame, std::unordered_map<Component::Action_State,  Rendering_Components::Frame_Data_Packer> &actionFrameData, int &frameIndex) {

            /// get the
        std::string keyCheck = name;
        std::string frameCopy = frame;

        auto index = frameCopy.find(name); // Find the starting position of substring in the string
        if  (index != std::string::npos) {
            frameCopy.erase(index, keyCheck.length() + 1); // erase function takes two parameter, the starting index in the string from where you want to erase characters and total no of characters you want to erase.
        }

            ///just grab the first 3 letters of the string
        std::string checkAction = frameCopy.erase(frameCopy.length()-6);
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
        }
            else {
                return;
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
            }
            else {
                return;
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
                return;
            }
        }

        else {
            if (checkAction == "1-H Attack 1") {
                action = Component::Action_State::attack;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Attack Two Hand Swing") {
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
            } else if (checkAction == "idle1") {
                action = Component::Action_State::idle;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "1-H Walk") {
                action = Component::Action_State::walk;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "walking") {
                action = Component::Action_State::walk;
                actionFrameData[action].frameSpeed = i;
//            } else if (checkAction == "Walking") {
//                action = Component::Action_State::walk;
//                actionFrameData[action].frameSpeed = i;
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
            } else if (checkAction == "dead") {
                action = Component::Action_State::struck;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "collapse") {
                action = Component::Action_State::dying;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "ko") {
                action = Component::Action_State::dead;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Dead") {
                action = Component::Action_State::dead;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Dead2") {
                action = Component::Action_State::dead;
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
            } else if (checkAction == "Woozy") {
                action = Component::Action_State::dying;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "ko") {
                action = Component::Action_State::dying;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Cast Idle") {
                action = Component::Action_State::casting;
                actionFrameData[action].frameSpeed = i;
            } else if (checkAction == "Casting") {
                action = Component::Action_State::cast;
                actionFrameData[action].frameSpeed = i;
            } else {
                return;
            }
        }
        //only run when the number changes

        Calculate_Start_Frame(actionFrameData, action, frameIndex);

//        Utilities::Log("----");
//        Utilities::Log(frame);
        Calculate_Num_Frames(frame, actionFrameData, action);
    }

    std::unordered_map<std::string, Rendering_Components::Sheet_Data>* TexturePacker_Import(std::string &name, std::string &xml_path, SDL_Texture* texture) {
            ///check if the sheet data already exists


        if ( Packer_Textures[name].frameList.size() > 1) {
            return &Packer_Textures;
        }

            ///get path from db
        Type_Data typeData = Get_Sprite_Sheet(name);
		tinyxml2::XMLDocument spriteSheetData;
		const char* path = typeData.path.c_str();
        if (path == NULL){
            Utilities::Log("TexturePacker_Import() failed, empty xml_path");
            return NULL;
        }
        spriteSheetData.LoadFile(path);
        tinyxml2::XMLElement *pSpriteElement;
        pSpriteElement = spriteSheetData.RootElement()->FirstChildElement("sprite");

        Rendering_Components::Sprite_Sheet_Data frame = {};
        Rendering_Components::Sheet_Data spritesheet;
        spritesheet.frameList.reserve(200);
        spritesheet.texture = texture;

        int frameIndex = 0;

        if (pSpriteElement != NULL) {
                ///get frame data for each state
            std::string n = pSpriteElement->Attribute("n");
            Get_Frame_Action_Data(typeData.type, name, n, spritesheet.actionFrameData, frameIndex);
                ///get sprite data
            frame.clip.x = pSpriteElement->IntAttribute("x");
            frame.clip.y = pSpriteElement->IntAttribute("y");
            frame.clip.w = pSpriteElement->IntAttribute("w");
            frame.clip.h = pSpriteElement->IntAttribute("h");
            frame.x_offset = pSpriteElement->IntAttribute("oX");
            frame.y_offset = pSpriteElement->IntAttribute("oY");
            spritesheet.frameList.emplace_back(frame);
                ///so we only grab this once
            frameIndex++;
                ///this grabs the next line
            pSpriteElement = pSpriteElement->NextSiblingElement("sprite");
        }
            ///get the rest of the xml
        while (pSpriteElement != NULL) {
                ///get frame data for each state
            std::string n = pSpriteElement->Attribute("n");
            Get_Frame_Action_Data(typeData.type, name, n, spritesheet.actionFrameData, frameIndex);
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
        Packer_Textures[name] = spritesheet;
//        spritesheet.color = Graphics::Set_Random_Color();
        return &Packer_Textures;
	}
}