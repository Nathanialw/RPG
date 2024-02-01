#pragma once
#include "SQLite_unit_data.h"
#include "base_structs.h"
#include "components.h"
#include "game_objects.h"
#include "graphics.h"
#include "spritesheet_structs.h"
#include "tinyxml/tinyxml2.h"
#include "utilities.h"
#include <string>
#include <vector>

// need the type of a unit
//
//
// use it to fund a random entity of that type

//teh container array is the size of the number of Entity_Types, the inner vector is the number of entities in the db for each unit type saved as a string

namespace Texture_Packer {
  ///on init we need to do it parse the SQLite db for all sprite sheets names "texture_packer" and use the result to preallocate all the nodes of the std::unordered_map

  std::unordered_map<std::string, Rendering_Components::Sheet_Data> Packer_Textures;
  std::unordered_map<std::string, Rendering_Components::Image_Data> Image_Textures;

  struct Type_Data {
    std::string type;
    std::string xml_path;
    std::string img_path;
  };

  Type_Data Get_Sprite_Sheet(std::string sheetname) {// needs to search for  a specific row that I can input in the arguments
    Type_Data typeData;
    ///check if the name exists??
    std::string sheet_name = db::Append_Quotes(sheetname);

    const unsigned char *sheet;
    const unsigned char *type;
    const unsigned char *image;
    sqlite3_stmt *stmt;
    char buf[400];
    const char *jj = "SELECT xml_path, equip_type, image_path FROM unit_data WHERE name = ";
    strcpy(buf, jj);
    strcat(buf, sheet_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      sheet = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) sheet;
      typeData.xml_path = std::string(reinterpret_cast<const char *>(s));

      type = sqlite3_column_text(stmt, 1);
      s = (const char *) type;
      typeData.type = std::string(reinterpret_cast<const char *>(s));

      image = sqlite3_column_text(stmt, 2);
      if (image) {
        s = (const char *) image;
        typeData.img_path = std::string(reinterpret_cast<const char *>(s));
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

  std::unordered_map<std::string, Rendering_Components::Sheet_Data> *TexturePacker_Import(std::string &templateName, std::string &xml_path, SDL_Texture *texture) {
    ///check if the sheet data already exists
    if (Packer_Textures[templateName].frameList.size() > 1) {
      return &Packer_Textures;
    }

    ///get path from db
    Type_Data typeData = Get_Sprite_Sheet(templateName);
    if (typeData.xml_path.c_str() == NULL || typeData.xml_path == "") {
      Utilities::Log("TexturePacker_Import() failed, empty xml_path");
      return NULL;
    }

    const char *imgPath;
    //    if (typeData.img_path.c_str() != NULL || typeData.img_path.c_str() == "") {
    std::string imgPathStr = "assets/" + typeData.img_path;
    imgPath = imgPathStr.c_str();
    //    }

    const char *xmlPath;
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
    } else {// assigns texture from img path in db unit_data
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

  SDL_Texture *Import_Tileset_Texture(int textureIndex, SDL_Texture *texture, const char *imgPath) {
    texture = Graphics::Create_Game_Object(textureIndex, imgPath);

    if (texture == NULL) {
      std::cout << "No texture found from image path: " << imgPath << ", texture: " << texture << std::endl;
      return texture;
    }
    return texture;
  }

  std::unordered_map<std::string, Rendering_Components::Sheet_Data> *TexturePacker_Import_Linear(std::string &templateName, std::string &xml_path, SDL_Texture *texture) {
    ///check if the sheet data already exists
    if (Packer_Textures[templateName].frameList.size() > 1) {
      return &Packer_Textures;
    }

    ///get path from db
    if (xml_path.c_str() == NULL || xml_path == "") {
      Utilities::Log("TexturePacker_Import() failed, empty xml_path");
      return NULL;
    }

    const char *xmlPath = xml_path.c_str();

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

    int frameIndex = 0;
    while (pSpriteElement != NULL) {
      ///get frame data for each state
      std::string n = pSpriteElement->Attribute("n");
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

    spritesheet.actionFrameData[Action_Component::walk].NumFrames = frameIndex;
    spritesheet.actionFrameData[Action_Component::walk].startFrame = 0;
    spritesheet.actionFrameData[Action_Component::walk].frameSpeed = 75;
    spritesheet.frameList.shrink_to_fit();
    Packer_Textures[templateName] = spritesheet;
    //        spritesheet.color = Graphics::Set_Random_Color();
    return &Packer_Textures;
  }

  void TexturePacker_Import_Tileset(Rendering_Components::Sheet_Data &spritesheet, std::string &templateName, std::string &tilesetName, const char *xmlPath) {
    //loads xml and image from db, only do once per xml

    ///get path from db
    //    Type_Data typeData = Get_Sprite_Sheet(templateName);
    if (Packer_Textures[tilesetName].frameList.size() > 1) {
      return;
    } else {
      tinyxml2::XMLDocument spriteSheetData;
      spriteSheetData.LoadFile(xmlPath);
      tinyxml2::XMLElement *pSpriteElement;
      pSpriteElement = spriteSheetData.RootElement()->FirstChildElement("sprite");
      Utilities::Log(tilesetName);
      std::string imgPath = spriteSheetData.RootElement()->Attribute("imagePath");

      std::vector<std::string> tilesetVec;
      tilesetVec.reserve(200);

      Rendering_Components::Sprite_Sheet_Data frame = {};
      spritesheet.frameList.reserve(200);

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
        tilesetVec.emplace_back(n);
        spritesheet.frameList.emplace_back(frame);
        Game_Objects_Lists::objectIndexes[n] = frameIndex;
        frameIndex++;
        ///this grabs the next line
        pSpriteElement = pSpriteElement->NextSiblingElement("sprite");
      }
      tilesetVec.shrink_to_fit();
      Game_Objects_Lists::Get_tileset(imgPath, tilesetVec);
    }
  }


  void TexturePacker_Import_Collision(const char *xmlPath) {
    tinyxml2::XMLDocument spriteSheetData;
    spriteSheetData.LoadFile(xmlPath);
    tinyxml2::XMLElement *pSpriteElement;
    Collision_Component::Colliders colliders;

    pSpriteElement = spriteSheetData.RootElement()->FirstChildElement("bodies");
    //    pSpriteElement = pSpriteElement->FirstChildElement("bodies");
    int numBodies = pSpriteElement->IntAttribute("numBodies");

    tinyxml2::XMLElement *bodyElement = pSpriteElement->FirstChildElement("body");
    for (int k = 0; k < numBodies; ++k) {
      if ((std::string) bodyElement->Attribute("dynamic") == "dynamic") colliders.bodyType = b2_dynamicBody;
      int numFixtures = bodyElement->IntAttribute("numFixtures");
      tinyxml2::XMLElement *fixtureElement = bodyElement->FirstChildElement("fixture");
      if (fixtureElement) {
        colliders.isSensor.resize(numFixtures);
        for (int i = 0; i < numFixtures; ++i) {
          if ((std::string) fixtureElement->Attribute("isSensor") == "true") colliders.isSensor[i] = true;
          std::string type = fixtureElement->Attribute("type");
          if (type == "POLYGON") {
            tinyxml2::XMLElement *polygonElement = fixtureElement->FirstChildElement("polygon");
            int numVertexes = polygonElement->IntAttribute("numVertexes");
            std::vector<tmx::Vector2<float>> vertexes;
            colliders.pointVecs.emplace_back(vertexes);
            tinyxml2::XMLElement *vertexElement = polygonElement->FirstChildElement("vertex");
            for (int j = 0; j < numVertexes; ++j) {
              tmx::Vector2 vec = {vertexElement->FloatAttribute("x"), vertexElement->FloatAttribute("y")};
              colliders.pointVecs[i].emplace_back(vec);
              vertexElement = vertexElement->NextSiblingElement("vertex");
            }
            colliders.pointVecs[i].shrink_to_fit();
          } else if (type == "CIRCLE") {
            tinyxml2::XMLElement *circleElement = fixtureElement->FirstChildElement("circle");
            Collision_Component::Circle circle;
            circle.r = circleElement->FloatAttribute("radius");
            circle.x = circleElement->FloatAttribute("x");
            circle.y = circleElement->FloatAttribute("y");
            colliders.circlesVecs.emplace_back(circle);
          }
          fixtureElement = fixtureElement->NextSiblingElement("fixture");
        }
      }
      colliders.isSensor.shrink_to_fit();
      colliders.pointVecs.shrink_to_fit();
      colliders.circlesVecs.shrink_to_fit();
      Collision_Component::houseColliders[bodyElement->Attribute("name")] = colliders;
      bodyElement = bodyElement->NextSiblingElement("body");
    }
  }

  struct Sheet_Data {
    std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerData;
    std::unordered_map<std::string, Rendering_Components::Image_Data> *imageData;
  };

  Sheet_Data Get_Texture_Data(int textureIndex, std::string &templateName, std::string img, std::string xml, std::string &tilesetName) {
    // get tileset name from texture path
    const char *imgPath;
    std::string imgPathStr = "assets/" + img;
    std::string base_filename = imgPathStr.substr(imgPathStr.find_last_of("/\\") + 1);
    std::string::size_type const p(base_filename.find_last_of('.'));
    tilesetName = base_filename.substr(0, p);

    // return texture data if it is already loaded


    std::cout << "Loading: " << textureIndex << ", " << templateName << ", " << xml << ", " << img << ", " << tilesetName << std::endl;

    if (xml.c_str() == nullptr || xml == "") {
      Utilities::Log("TexturePacker_Import() failed, empty xml_path");
      Rendering_Components::Image_Data imageData = {};

      if (img.c_str() == nullptr || img == "") {
        Utilities::Log("TexturePacker_Import() failed, empty xml_path");
        return {nullptr, nullptr};
      } else {
        //get texture path
        imgPath = imgPathStr.c_str();
        imageData.texture = Import_Tileset_Texture(textureIndex, imageData.texture, imgPath);
        SDL_QueryTexture(imageData.texture, NULL, NULL, &imageData.w, &imageData.h);
      }
      Utilities::Log("SUCCESS!");

      Image_Textures[tilesetName] = imageData;
      return {nullptr, &Image_Textures};
    }

    //    import xml if the xml string is valid
    else {
      Rendering_Components::Sheet_Data spriteSheet = {};

      if (Packer_Textures[tilesetName].texture) {
        //std::cout << "Tileset already loaded: " << tilesetName << std::endl;
        return {&Packer_Textures, nullptr};
      }

      const char *xmlPath;
      std::string xmlPathStr = "assets/" + xml;
      xmlPath = xmlPathStr.c_str();

      TexturePacker_Import_Tileset(spriteSheet, templateName, tilesetName, xmlPath);

      //    import texture
      if (img.c_str() == nullptr || img == "") {
        Utilities::Log("TexturePacker_Import() failed, empty xml_path");
        return {nullptr, nullptr};
      } else {
        //get texture path
        imgPath = imgPathStr.c_str();
        spriteSheet.texture = Import_Tileset_Texture(textureIndex, spriteSheet.texture, imgPath);
      }
      Utilities::Log("SUCCESS!");

      spriteSheet.frameList.shrink_to_fit();
      Packer_Textures[tilesetName] = spriteSheet;
      return {&Packer_Textures, nullptr};
    }
  }
}// namespace Texture_Packer
