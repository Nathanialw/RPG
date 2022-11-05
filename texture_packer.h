#pragma once
#include "tinyxml2.h"
#include "spritesheet_structs.h"

namespace Texture_Packer {
        ///on init we need to do it parse the SQLite db for all sprite sheets names "texture_packer" and use the result to preallocate all the nodes of the std::unordered_map
    struct Sprite_Sheet_Data {
        std::string name;
        SDL_Rect clip;
        int x_offset;
        int y_offset;
    };

    struct Sheet_Data {
        int width = 0;
        int height = 0;
        std::vector<Sprite_Sheet_Data> frameList;
    };

    std::unordered_map<std::string, Sheet_Data> Packer_Textures;

        ///component for the unit
    struct Packer_Sheet_Data {
        std::unordered_map<std::string, Sheet_Data>* sheetData;
    };

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


    std::unordered_map<std::string, Sheet_Data>* TexturePacker_Import(std::string &name, std::string &xml_path) {
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

        Sprite_Sheet_Data frame = {};
        Sheet_Data spritesheet;
        spritesheet.frameList.reserve(200);

        if (pSpriteElement != NULL) {
            frame.name = pSpriteElement->Attribute("n");
            frame.clip.x = pSpriteElement->IntAttribute("x");
            frame.clip.y = pSpriteElement->IntAttribute("y");
            frame.clip.w = pSpriteElement->IntAttribute("w");
            frame.clip.h = pSpriteElement->IntAttribute("h");
            frame.x_offset = pSpriteElement->IntAttribute("oX");
            frame.y_offset = pSpriteElement->IntAttribute("oY");
            spritesheet.frameList.emplace_back(frame);
                ///so we only grab this once
            spritesheet.height = pSpriteElement->IntAttribute("oH");
            spritesheet.width = pSpriteElement->IntAttribute("oW");
                ///this grabs the next line
            pSpriteElement = pSpriteElement->NextSiblingElement("sprite");
        }

                ///get the rest of the xml
        while (pSpriteElement != NULL) {
			//assign value
			frame.name = pSpriteElement->Attribute("n");
			frame.clip.x = pSpriteElement->IntAttribute("x");
			frame.clip.y = pSpriteElement->IntAttribute("y");
			frame.clip.w = pSpriteElement->IntAttribute("w");
			frame.clip.h = pSpriteElement->IntAttribute("h");
			frame.x_offset = pSpriteElement->IntAttribute("oX");
			frame.y_offset = pSpriteElement->IntAttribute("oY");
            spritesheet.frameList.emplace_back(frame);
                ///this grabs the next line
			pSpriteElement = pSpriteElement->NextSiblingElement("sprite");			
		}

        spritesheet.frameList.shrink_to_fit();
        Packer_Textures[name]= spritesheet;
        return &Packer_Textures;
	}
}