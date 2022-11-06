#pragma once
#include "tinyxml2.h"
#include "spritesheet_structs.h"
#include "components.h"
#include <string>

namespace Texture_Packer {
        ///on init we need to do it parse the SQLite db for all sprite sheets names "texture_packer" and use the result to preallocate all the nodes of the std::unordered_map


    std::unordered_map<std::string, Component::Sheet_Data> Packer_Textures;

        ///component for the unit
//    struct Packer_Sheet_Data {
////        std::string* sheetData;
//        std::unordered_map<std::string, Sheet_Data>* sheetData;
//        std::string sheet_name = "default";
//            ///possible replacement for a string map is to store the name of the sprite sheet in a map and match it to a unique index, save that index and make the sheetData and vector, access the vector with the index
////        int index = 0;
//        uint64_t frameTime = 0;
//        int frameIndex = 0;
//        int reversing = 0;
//        int currentFrame = 0;
//    };

    std::string Get_Sprite_Sheet(std::string &sheetname) {// needs to search for  a specific row that I can input in the arguments
        std::string path;
        //check if the name exists??
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

    void Get_Frame_Action_Data (std::string &frame, std::unordered_map<Component::Action_State,  Component::Frame_Data_Packer> &actionFrameData, int &frameIndex) {
            ///just grab the first 3 letters of the string
        std::string checkAction = frame.substr(0, 3);
        Component::Action_State action;
            /// compare the string in the xml with the values I should probably just read in from the db, just push the test strings back on a vector and iterate through comparing, I wonder if I can store the enum in the db too I would probably have to for it to be worth it.
       if (checkAction == "Att"){
           action = Component::Action_State::attack;
       } else if (checkAction == "Blo") {
           action = Component::Action_State::block;
       } else if (checkAction == "Cas") {
           action = Component::Action_State::cast;
       } else if (checkAction == "Dea") {
           action = Component::Action_State::dead;
       } else if (checkAction == "Get") {
           action = Component::Action_State::struck;
       } else if (checkAction == "Idl") {
           action = Component::Action_State::idle;
       } else if (checkAction == "Run") {
           action = Component::Action_State::walk;
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
                ///get frame data for each action
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
                ///get frame data for each action
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