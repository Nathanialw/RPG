#pragma once
#include <stdint.h>
#include "../tinyxml2.h"
#include "base_structs.h"
#include "components.h"
#include "graphics.h"
#include "spritesheet_structs.h"
#include <string>
#include <vector>
#include "SQLite_unit_data.h"

// need the type of a unit
//
// 
// use it to fund a random entity of that type

//teh container array is the size of the number of Entity_Types, the inner vector is the number of entities in the db for each unit type saved as a string

namespace Texture_Packer {
  ///on init we need to do it parse the SQLite db for all sprite sheets names "texture_packer" and use the result to preallocate all the nodes of the std::unordered_map

  std::unordered_map<std::string, Rendering_Components::Sheet_Data> Packer_Textures;

  struct Type_Data{
    std::string type;
    std::string xml_path;
    std::string img_path;
  };

  Type_Data Get_Sprite_Sheet(std::string sheetname) {// needs to search for  a specific row that I can input in the arguments
    Type_Data typeData;
    ///check if the name exists??
    std::string sheet_name = db::Append_Quotes(sheetname);

    const unsigned char* sheet;
    const unsigned char* type;
    const unsigned char* image;
    sqlite3_stmt* stmt;
    char buf[400];
    const char* jj = "SELECT xml_path, equip_type, image_path FROM unit_data WHERE name = ";
    strcpy(buf, jj);
    strcat(buf, sheet_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      sheet = sqlite3_column_text(stmt, 0);
      const char * s = (const char *)sheet;
      typeData.xml_path = std::string(reinterpret_cast< const char *> (s));

      type = sqlite3_column_text(stmt, 1);
      s = (const char *)type;
      typeData.type = std::string(reinterpret_cast< const char *> (s));

      image = sqlite3_column_text(stmt, 2);
      if (image) {
	s = (const char *) image;
	typeData.img_path = std::string(reinterpret_cast< const char *> (s));
      }
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

  std::unordered_map<std::string, Rendering_Components::Sheet_Data>* TexturePacker_Import(std::string &templateName, std::string &xml_path, SDL_Texture* texture) {
    ///check if the sheet data already exists    
    if ( Packer_Textures[templateName].frameList.size() > 1) {
      return &Packer_Textures;
    }

    ///get path from db
    Type_Data typeData = Get_Sprite_Sheet(templateName);
    if (typeData.xml_path.c_str() == NULL || typeData.xml_path == ""){
      Utilities::Log("TexturePacker_Import() failed, empty xml_path");
      return NULL;
    }

    const char* imgPath;
    //    if (typeData.img_path.c_str() != NULL || typeData.img_path.c_str() == "") {
    std::string imgPathStr = "assets/" + typeData.img_path;
    imgPath = imgPathStr.c_str();
    //    }

    const char* xmlPath;
    std::string xmlPathStr = "assets/" + typeData.xml_path;
    xmlPath = xmlPathStr.c_str();

    tinyxml2::XMLDocument spriteSheetData;
    spriteSheetData.LoadFile(xmlPath);
    tinyxml2::XMLElement *pSpriteElement;
    pSpriteElement = spriteSheetData.RootElement()->FirstChildElement("sprite");

    Rendering_Components::Sprite_Sheet_Data frame = {};
    Rendering_Components::Sheet_Data spritesheet;
    spritesheet.frameList.reserve(200);

    if (texture) {
      spritesheet.texture = texture;
    }
    else {// assigns texture from img path in db unit_data
      texture = Graphics::createTexture(imgPath);
      spritesheet.texture = texture;
      std::cout << "texture found from image path " << imgPath << " texture: " << texture << std::endl;
      if (spritesheet.texture == NULL) {
	std::cout << "Not texture found from image path " << imgPath << " texture: " << texture << std::endl;
	return NULL;
      }
    }

    int frameIndex = 0;   
    while (pSpriteElement != NULL) {
      ///get frame data for each state
      std::string n = pSpriteElement->Attribute("n");
      Spritesheet_Structs::Get_Frame_Action_Data(typeData.type, templateName, n, spritesheet.actionFrameData, frameIndex);
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
    Packer_Textures[templateName] = spritesheet;
    //        spritesheet.color = Graphics::Set_Random_Color();
    return &Packer_Textures;
  }

  std::unordered_map<std::string, Rendering_Components::Sheet_Data>* TexturePacker_Import_Tileset(std::string &templateName, Entity_Loader::Building_Data data, SDL_Texture* texture, int &buildingIndex, std::string &tilesetName) {
    //loads xml and image from db, only do once per xml

    ///get path from db
    //    Type_Data typeData = Get_Sprite_Sheet(templateName);
    if (data.xml.c_str() == NULL || data.xml == ""){
      Utilities::Log("TexturePacker_Import() failed, empty xml_path");
      return NULL;
    }

    const char* imgPath;
    std::string imgPathStr = "assets/" + data.img;
    imgPath = imgPathStr.c_str();

    std::string base_filename = imgPathStr.substr(imgPathStr.find_last_of("/\\") + 1);
    std::string::size_type const p(base_filename.find_last_of('.'));
    tilesetName = base_filename.substr(0, p);

    const char* xmlPath;
    std::string xmlPathStr = "assets/" + data.xml;
    xmlPath = xmlPathStr.c_str();


    tinyxml2::XMLDocument spriteSheetData;
    spriteSheetData.LoadFile(xmlPath);
    tinyxml2::XMLElement *pSpriteElement;
    pSpriteElement = spriteSheetData.RootElement()->FirstChildElement("sprite");
        
    Rendering_Components::Sprite_Sheet_Data frame = {};
    Rendering_Components::Sheet_Data spritesheet;
    spritesheet.frameList.reserve(200);
    
    if (texture) {
      spritesheet.texture = texture;
    }
    else {// assigns texture from img path in db unit_data
      texture = Graphics::createTexture(imgPath);
      spritesheet.texture = texture;
      // std::cout << "texture found from image path " << imgPath << " texture: " << texture << std::endl;
      if (spritesheet.texture == NULL) {
	std::cout << "Not texture found from image path " << imgPath << " texture: " << texture << std::endl;
	return NULL;
      }
    }    

    if (Packer_Textures[tilesetName].frameList.size() < 1) {
      bool indexFound = false;
      int frameIndex = 0;   
      while (pSpriteElement != NULL) {
	///get frame data for each state
	std::string n = pSpriteElement->Attribute("n");
	Spritesheet_Structs::Get_Frame_Action_Data("tileset", templateName, n, spritesheet.actionFrameData, frameIndex);
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
    }

    pSpriteElement = spriteSheetData.RootElement()->FirstChildElement("sprite");
    bool indexFound = false;
    int frameIndex = 0;   
    while (pSpriteElement != NULL) {
      ///get frame data for each state
      std::string n = pSpriteElement->Attribute("n");
      if (Spritesheet_Structs::Get_Tileset_Frame_Data(templateName, n, spritesheet.actionFrameData)) {
	buildingIndex = frameIndex;
	break;
      }
      frameIndex++;
      pSpriteElement = pSpriteElement->NextSiblingElement("sprite");
    }
    
    if ( Packer_Textures[tilesetName].frameList.size() > 1) {
      return &Packer_Textures;
    }
    else {    
      spritesheet.frameList.shrink_to_fit();
      Packer_Textures[tilesetName] = spritesheet;
      //spritesheet.color = Graphics::Set_Random_Color();
    }
    return &Packer_Textures;
  }
}

