#pragma once
#include "tinyxml2.h"
#include "spritesheet_structs.h"
#include "components.h"
#include <string>

namespace Texture_Packer {
        ///on init we need to do it parse the SQLite db for all sprite sheets names "texture_packer" and use the result to preallocate all the nodes of the std::unordered_map

    std::unordered_map<std::string, Component::Sheet_Data> Packer_Textures;

    std::string Get_Sprite_Sheet(std::string sheetname) {// needs to search for  a specific row that I can input in the arguments
        std::string path;
            ///check if the name exists??
        std::string sheet_name = db::Append_Quotes(sheetname);

        const unsigned char* sheet;
        sqlite3_stmt* stmt;
        char buf[300];
        const char* jj = "SELECT xml_path FROM unit_data WHERE name = ";
        strcpy(buf, jj);
        strcat(buf, sheet_name.c_str());
        sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
        while (sqlite3_step(stmt) != SQLITE_DONE) {
            sheet = sqlite3_column_text(stmt, 0);
            const char * s = (const char *)sheet;
            path = std::string(reinterpret_cast< const char *> (s));
        }
        return path;
    }

    Component::Sprite_Offset Get_Sprite_Offets_From_db(std::string &sheet) {// needs to search for  a specific row that I can input in the arguments
        std::string sheet_name = db::Append_Quotes(sheet);
        Component::Sprite_Offset offset = {};
        sqlite3_stmt *stmt;
        const unsigned char *sheetType;
        char buf[200];
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
    int i = 75;

    void Get_Frame_Action_Data (std::string &frame, std::unordered_map<Component::Action_State,  Component::Frame_Data_Packer> &actionFrameData, int &frameIndex) {
            ///just grab the first 3 letters of the string
        std::string frameCopy = frame;
        std::string checkAction = frameCopy.erase(frameCopy.length()-6);
        Component::Action_State action;
            /// compare the string in the xml with the values, I should probably just read in from the db, just push the test strings back on a vector and iterate through comparing, I wonder if I can store the enum in the db too I would probably have to for it to be worth it.

       if (checkAction == "1-H Attack 1") {
           action = Component::Action_State::attack;
           actionFrameData[action].frameSpeed = i;
       } else if (checkAction == "Attack One Handed Overhead") {
           action = Component::Action_State::attack;
           actionFrameData[action].frameSpeed = i;
       } else if (checkAction == "1-H Idle") {
           action = Component::Action_State::idle;
           actionFrameData[action].frameSpeed = i;
       } else if (checkAction == "Idle1") {
           action = Component::Action_State::idle;
           actionFrameData[action].frameSpeed = i;
       } else if (checkAction == "1-H Walk") {
           action = Component::Action_State::walk;
           actionFrameData[action].frameSpeed = i;
       } else if (checkAction == "Walk") {
           action = Component::Action_State::walk;
           actionFrameData[action].frameSpeed = i;
       } else if (checkAction == "Get Hit 1") {
           action = Component::Action_State::struck;
           actionFrameData[action].frameSpeed = i;
       } else if (checkAction == "Get Hit") {
           action = Component::Action_State::struck;
           actionFrameData[action].frameSpeed = i;
       } else if (checkAction == "Down 1") {
           action = Component::Action_State::dead;
           actionFrameData[action].frameSpeed = i;
       } else if (checkAction == "Dead2") {
           action = Component::Action_State::dead;
           actionFrameData[action].frameSpeed = i;
       } else {
           return;
       }

       Calculate_Start_Frame(actionFrameData, action, frameIndex);
       Calculate_Num_Frames(frame, actionFrameData, action);
    }

    std::unordered_map<std::string, Component::Sheet_Data>* TexturePacker_Import(std::string &name, std::string &xml_path, SDL_Texture* texture) {
            ///check if the sheet data already exists

        if ( Packer_Textures[name].frameList.size() > 1) {
            return &Packer_Textures;
        }

            ///get path from db
        std::string str = Get_Sprite_Sheet(name);
		tinyxml2::XMLDocument spriteSheetData;
		const char* path = str.c_str();
        if (path == NULL){
            Utilities::Log("TexturePacker_Import() failed, empty xml_path");
            return NULL;
        }
		spriteSheetData.LoadFile(path);

		tinyxml2::XMLElement* pSpriteElement = spriteSheetData.RootElement()->FirstChildElement("sprite");

        Component::Sprite_Sheet_Data frame = {};
        Component::Sheet_Data spritesheet;
        spritesheet.frameList.reserve(200);
        spritesheet.texture = texture;

        int frameIndex = 0;

        if (pSpriteElement != NULL) {
                ///get frame data for each state
            std::string n = pSpriteElement->Attribute("n");
            Get_Frame_Action_Data(n, spritesheet.actionFrameData, frameIndex);
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
            Get_Frame_Action_Data(n, spritesheet.actionFrameData, frameIndex);
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
        return &Packer_Textures;
	}
}