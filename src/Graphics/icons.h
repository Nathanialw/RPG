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


namespace Icons {
  struct Offset {
    float x;
    float y;
  };

  struct Icon_Clip {
    SDL_Rect clipRect = {};
    Offset offset = {};
  };
  std::unordered_map<std::string, Icon_Clip> iconClipRects;


  void Load() {
    ///check if the sheet data already exists
    std::string xml_path = "assets/sprites/xml/icons/icons.xml";

    ///get path from db
    if (xml_path.c_str() == NULL || xml_path == "") {
      Utilities::Log("Load Icons() failed, empty xml_path");
      return;
    }

    const char *xmlPath = xml_path.c_str();

    tinyxml2::XMLDocument spriteSheetData;
    spriteSheetData.LoadFile(xmlPath);
    tinyxml2::XMLElement *pSpriteElement;

    pSpriteElement = spriteSheetData.RootElement()->FirstChildElement("sprite");

    Icon_Clip icon = {};

    while (pSpriteElement != NULL) {
      ///get frame data for each state
      std::string n = pSpriteElement->Attribute("n");
      ///get sprite data
      icon.clipRect.x = pSpriteElement->IntAttribute("x");
      icon.clipRect.y = pSpriteElement->IntAttribute("y");
      icon.clipRect.w = pSpriteElement->IntAttribute("w");
      icon.clipRect.h = pSpriteElement->IntAttribute("h");
      icon.offset.x = pSpriteElement->IntAttribute("oX");
      icon.offset.y = pSpriteElement->IntAttribute("oY");

      iconClipRects[n] = icon;
      ///this grabs the next line
      pSpriteElement = pSpriteElement->NextSiblingElement("sprite");
    }
  }
}
