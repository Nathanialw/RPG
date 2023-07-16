#pragma once
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include "SQLite_dialogue.h"
#include "utilities.h"
#include "world.h"

namespace Maps {

  tmx::Map map;
  SDL_Texture* pTexture = NULL;

  void Create_Map()
  {
    World::Generate_Region();

    if (map.load("assets/maps/forest.tmx"))
      {
	for (auto &spriteSheet: map.getTilesets())
	  {
	    std::string name = spriteSheet.getName();
	  }

	std::cout << "Loaded Map version: " << map.getVersion().upper << ", " << map.getVersion().lower << std::endl;

	const auto &layers = map.getLayers();
	std::cout << "Map has " << layers.size() << " layers" << std::endl;

	for (const auto &layer: layers)
	  {
	    if (layer->getName() == "ground")
	      {
		if (layer->getType() == tmx::Layer::Type::Tile) {
		  for (auto &tiles : map.getTilesets()) {
		    std::string name = tiles.getName();
		    std::string imgpath = tiles.getImagePath();
		    Graphics::Create_Tileset(name, imgpath.c_str());
		  }
		}
	      }

	    else if (layer->getName() == "widgets")
	      {
		if (layer->getType() == tmx::Layer::Type::Tile) {
		  for (auto &tiles : map.getTilesets()) {
		    std::string name = tiles.getName();
		    std::string imgpath = tiles.getImagePath();
		    Graphics::Create_Tileset(name, imgpath.c_str());
		  }
		}
	      }

	    else if (layer->getName() == "taller_widgets1")
	      {
		if (layer->getType() == tmx::Layer::Type::Tile) {
		  for (auto &tiles : map.getTilesets()) {
		    std::string name = tiles.getName();
		    std::string imgpath = tiles.getImagePath();
		    Graphics::Create_Tileset(name, imgpath.c_str());
		  }
		}
	      }

	    else if (layer->getName() == "unit_spawns")
	      {
		for (auto &object : layer->getLayerAs<tmx::ObjectGroup>().getObjects())
		  {
		    auto &position = object.getPosition();
		    std::string tilePath = object.getTilesetName();

		    std::string entity_class = object.getClass();

		    // translates isometric position to world position
		    float tileWidth = 128;
		    float tileHeight = 64;
		    float numTilesX = position.x / tileWidth;
		    float numTilesY = position.y / tileHeight;
		    float x = 64.0f + position.x - (numTilesY * tileWidth / 2.0f);
		    float y = (numTilesX * tileHeight) + (position.y / 2.0f);
		    //if it is random it needs to grab a name from a unit that was already loaded into graphics or default to a default unit name
		    //get an array of all the potential names, check each on against teh std::map of graphics, keep all the ones already there and pick a random one
		    //if (is_random == false ) {}
		    std::string tilesetName = object.getTilesetName();
		    auto &templateTilesets = map.getTemplateTilesets();
		    //gets the collision box/boxes for a building
		    std::vector<tmx::Object> collision_boxes;
		    Collision::aabb aabb;
		    std::vector<std::vector<tmx::Vector2<float>>> pointVecs;
		    Component::Line_Segment line;

		    auto ffs = templateTilesets.at(tilesetName);
		    std::string templateName = ffs.getName();

		    auto fas = ffs.getTiles();
		    if (fas.size() > 1) {
		      auto asa = fas.at(object.getTileID() - 1);
		      std::string templateFile = asa.imagePath.substr(asa.imagePath.find_last_of("/\\") + 1);
		      std::string::size_type const p(templateFile.find_last_of('.'));
		      std::string qwe = templateFile.substr(0, p);
		      // Utilities::Log(qwe);
		      templateName = qwe;
		    }

		    for (auto s: ffs.getTiles())
		      {
			collision_boxes = s.objectGroup.getObjects();
			float sizeX = s.imageSize.x;
			float sizeY = s.imageSize.y;

			int j = 0;
			for (auto rects: collision_boxes)
			  {
			    aabb.hx = rects.getAABB().width / 2.0f;
			    aabb.hy = rects.getAABB().height / 2.0f;
			    float aabbx = rects.getAABB().left;
			    float aabby = rects.getAABB().top;

			    if (rects.getShape() == tmx::Object::Shape::Point)
			      {
				line.p[j].x = x - ((sizeX / 2.0f) - aabbx);
				line.p[j].y = y - (sizeY - aabby);
				j++;
			      }

			    else if (rects.getPoints().size() > 0)
			      {
				std::vector<tmx::Vector2<float>> pointVec = rects.getPoints();
				for (int i = 0; i < pointVec.size(); i++)
				  {
				    pointVec[i].x = pointVec[i].x - ((sizeX / 2.0f) - aabbx);
				    pointVec[i].y = pointVec[i].y - (sizeY - aabby);
				  }
				pointVecs.emplace_back(pointVec);
			      }
			  }
		      }

		    bool player = false;
		    bool is_random = false;

		    for (auto property: object.getProperties())
		      {
			if (property.getName() == "player")
			  {
			    player = property.getBoolValue();
			  }
			if (property.getName() == "is_random")
			  {
			    is_random = property.getBoolValue();
			  }
		      }

		    std::string texture = templateTilesets.at(tilesetName).getImagePath();
		    std::string temptexture = "assets/" + texture;
		    // Utilities::Log(texture);

		    if (!Create_Entities::PVG_Building(World::zone, x, y, templateName, entity_class, texture, aabb, pointVecs, line)) {
		      if (!Create_Entities::Polygon_Building(World::zone, x, y, templateName, entity_class, texture, aabb, pointVecs, line))
			{
			  Create_Entities::Create_Entity(World::zone, x, y, templateName, entity_class, is_random, texture, player);
			}
		    }
		  };
	      }
	  }
      }
    else
      {
	std::cout << "Failed loading map" << std::endl;
      }
  };
}
