#pragma once
#include "Graphics/XML_Parsers/spritesheet_structs.h"
#include "Graphics/XML_Parsers/texture_packer.h"
#include "SQLite_unit_data.h"
#include "base_structs.h"
#include "components.h"
#include "game_objects.h"
#include "graphics.h"
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
  std::unordered_map<std::string, Icon_Clip> buildingIconClipRects;


  void Load_Icons(std::unordered_map<std::string, Icon_Clip> &icons, std::string xml_path) {
    ///check if the sheet data already exists

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

      icons[n] = icon;
      ///this grabs the next line
      pSpriteElement = pSpriteElement->NextSiblingElement("sprite");
    }
  }

  void Load() {
    Load_Icons(iconClipRects, "assets/sprites/xml/icons/icons.xml");
    Load_Icons(iconClipRects, "assets/sprites/xml/icons/cox_icons.xml");
    Load_Icons(iconClipRects, "assets/sprites/xml/icons/cox_skills.xml");
    Load_Icons(iconClipRects, "assets/sprites/xml/icons/cox_spells.xml");
    Load_Icons(iconClipRects, "assets/sprites/xml/icons/cox_map_tiles.xml");
    Load_Icons(iconClipRects, "assets/sprites/xml/icons/cox_units.xml");
    Load_Icons(iconClipRects, "assets/sprites/xml/icons/cox_objects.xml");
    Load_Icons(iconClipRects, "assets/sprites/xml/icons/cox_traps.xml");
    Load_Icons(iconClipRects, "assets/sprites/xml/icons/cox_void.xml");
    Load_Icons(iconClipRects, "assets/sprites/xml/icons/cox_level_up.xml");
    Load_Icons(buildingIconClipRects, "assets/sprites/buildings/xml/Viking Pack/icons.xml");
    Load_Icons(buildingIconClipRects, "assets/sprites/buildings/xml/medieval/buildings.xml");
  }
}// namespace Icons
