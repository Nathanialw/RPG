#pragma once

#include <iostream>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include "SQLite_dialogue.h"
#include "utilities.h"
#include "world.h"
#include "create_entities.h"

namespace Maps {

  tmx::Map map;
  SDL_Texture *pTexture = NULL;

  void Create_Map() {
    World::Generate_Region();

    if (map.load("assets/maps/aaaa.tmx")) {
      for (auto &spriteSheet: map.getTilesets()) {
        std::string name = spriteSheet.getName();
      }

      std::cout << "Loaded Map version: " << map.getVersion().upper << ", " << map.getVersion().lower << std::endl;

      const auto &layers = map.getLayers();
      std::cout << "Map has " << layers.size() << " layers" << std::endl;

      for (const auto &layer: layers) {
        if (layer->getName() == "ground") {
          if (layer->getType() == tmx::Layer::Type::Tile) {
            for (auto &tiles: map.getTilesets()) {
              std::string name = tiles.getName();
              std::string imgpath = tiles.getImagePath();
              Graphics::Create_Tileset(name, imgpath.c_str());
            }
          }
        } else if (layer->getName() == "widgets") {
          if (layer->getType() == tmx::Layer::Type::Tile) {
            for (auto &tiles: map.getTilesets()) {
              std::string name = tiles.getName();
              std::string imgpath = tiles.getImagePath();
              Graphics::Create_Tileset(name, imgpath.c_str());
            }
          }
        } else if (layer->getName() == "taller_widgets1") {
          if (layer->getType() == tmx::Layer::Type::Tile) {
            for (auto &tiles: map.getTilesets()) {
              std::string name = tiles.getName();
              std::string imgpath = tiles.getImagePath();
              Graphics::Create_Tileset(name, imgpath.c_str());
            }
          }
        } else if (layer->getName() == "unit_spawns") {
          for (auto &object: layer->getLayerAs<tmx::ObjectGroup>().getObjects()) {
            auto &position = object.getPosition();
            std::string tilePath = object.getTilesetName();
            std::string entity_class = object.getClass();

            // translates isometric position to world position
//	    float tileWidth = 128;
//	    float tileHeight = 64;
//	    float numTilesX = position.x / tileWidth;
//	    float numTilesY = position.y / tileHeight;
//	    float x = 64.0f + position.x - (numTilesY * tileWidth / 2.0f);
//	    float y = (numTilesX * tileHeight) + (position.y / 2.0f);

            float tileWidth = 512;
            float tileHeight = 512;
            float numTilesX = position.x / tileWidth;
            float numTilesY = position.y / tileHeight;
            float x = position.x;
            float y = position.y;

            //if it is random it needs to grab a name from a unit that was already loaded into graphics or default to a default unit name
            //get an array of all the potential names, check each on against teh std::map of graphics, keep all the ones already there and pick a random one

            // gets the tilset vector index and the image index within the vector
            auto id = object.getTileID();

            auto &tilesets = Maps::map.getTilesets();
            int p = Maps::map.getTilesets().size() - 1;

            int tilesetCount = p;
            for (tilesetCount = p; tilesetCount >= 0; --tilesetCount) {
              const tmx::Tileset *tileset = &tilesets[tilesetCount];
              if (tileset->getFirstGID() - 1 <= id) {
                id -= tilesets[tilesetCount].getFirstGID() - 1;
                std::string name = tileset->getName();
                break;
              }
            }
            id -= 1;

            auto klk = map.getTilesets().at(tilesetCount);
            auto ggg = klk.getTiles();
            auto aaaa = ggg.at(id);
            auto imgPath = aaaa.imagePath;

            //gets the collision box/boxes for objects
            std::vector<tmx::Object> collision_boxes;
            Collision::aabb aabb;
            tmx::Vector2<float> offset = {0.0f, 0.0f};
            std::vector<std::vector<tmx::Vector2<float>>> pointVecs;
            Component::Line_Segment line;

            std::string tilesetName = object.getTilesetName();
            auto &templateTilesets = map.getTemplateTilesets();

            std::string templateName = "";
            if (tilesetName != "") {
              auto ffs = templateTilesets.at(tilesetName);
              std::string templateName = ffs.getName();
              u_int32_t tilesetIndex = 0;
              auto fas = ffs.getTiles();

              if (fas.size() > 1) {
                auto asa = fas.at(object.getTileID() - 1);
                tilesetIndex = fas.at(object.getTileID() - 1).ID;
                std::string templateFile = asa.imagePath.substr(asa.imagePath.find_last_of("/\\") + 1);
                std::string::size_type const p(templateFile.find_last_of('.'));
                templateName = templateFile.substr(0, p);
              }

              if (ffs.getTiles().size() > tilesetIndex) {
                auto s = ffs.getTiles().at(tilesetIndex);

                collision_boxes = s.objectGroup.getObjects();
                float sizeX = s.imageSize.x;
                float sizeY = s.imageSize.y;

                int j = 0;
                for (auto rects: collision_boxes) {
                  aabb.hx = rects.getAABB().width / 2.0f;
                  aabb.hy = rects.getAABB().height / 2.0f;
                  float aabbx = rects.getAABB().left;
                  float aabby = rects.getAABB().top;

                  if (rects.getShape() == tmx::Object::Shape::Polygon) {

                  }

                  if (rects.getShape() == tmx::Object::Shape::Point) {
                    line.p[j].x = x - ((sizeX / 2.0f) - aabbx);
                    line.p[j].y = y - (sizeY - aabby);
                    j++;
                  } else if (rects.getPoints().size() == 1) {
                    std::vector<tmx::Vector2<float>> pointVec = rects.getPoints();
                    for (int i = 0; i < pointVec.size(); i++) {
                      offset = pointVec[i];
                    }
                  } else if (rects.getPoints().size() > 1) {
                    std::vector<tmx::Vector2<float>> pointVec = rects.getPoints();
                    for (int i = 0; i < pointVec.size(); i++) {
                      pointVec[i].x = pointVec[i].x - ((sizeX / 2.0f) - aabbx);
                      pointVec[i].y = pointVec[i].y - (sizeY - aabby);
                    }
                    pointVecs.emplace_back(pointVec);
                  }
                }

                bool player = false;
                bool is_random = false;

                for (auto property: object.getProperties()) {
                  if (property.getName() == "player") {
                    player = property.getBoolValue();
                  }
                  if (property.getName() == "is_random") {
                    is_random = property.getBoolValue();
                  }
                }

                std::string texture = templateTilesets.at(tilesetName).getImagePath();
                std::string temptexture = "assets/" + texture;
                db::Unit_Data data = {templateName, imgPath, "", ""};

                // Utilities::Log(texture);

                if (!Create_Entities::PVG_Building(World::zone, x, y, x, y, templateName, tilesetIndex, aabb, pointVecs, line)) {
                  if (!Create_Entities::Polygon_Building(World::zone, x, y, templateName, entity_class, texture, aabb, pointVecs, line, offset)) {
                    Create_Entities::Create_Entity(World::zone, x, y, entity_class, is_random, data, player);
                  }
                }
              }
            } else {
              std::string templateFile = imgPath.substr(imgPath.find_last_of("/\\") + 1);
              std::string::size_type const p(templateFile.find_last_of('.'));
              templateName = templateFile.substr(0, p);

              auto s = aaaa;

              collision_boxes = s.objectGroup.getObjects();
              float sizeX = s.imageSize.x;
              float sizeY = s.imageSize.y;

              int j = 0;
              for (auto rects: collision_boxes) {
                aabb.hx = rects.getAABB().width / 2.0f;
                aabb.hy = rects.getAABB().height / 2.0f;
                float aabbx = rects.getAABB().left;
                float aabby = rects.getAABB().top;

                if (rects.getShape() == tmx::Object::Shape::Point) {
                  offset = rects.getPosition();
                }

                if (rects.getShape() == tmx::Object::Shape::Point && rects.getPoints().size() > 1) {
                  line.p[j].x = x - ((sizeX / 2.0f) - aabbx);
                  line.p[j].y = y - (sizeY - aabby);
                  j++;
                } else if (rects.getPoints().size() > 0) {
                  std::vector<tmx::Vector2<float>> pointVec = rects.getPoints();
                  for (int i = 0; i < pointVec.size(); i++) {
                    pointVec[i].x = pointVec[i].x - ((sizeX / 2.0f) - aabbx);
                    pointVec[i].y = pointVec[i].y - (sizeY - aabby);
                  }
                  pointVecs.emplace_back(pointVec);
                }
              }
              Utilities::Log(templateName);
              bool player = false;
              bool is_random = false;
              db::Unit_Data data = {templateName, imgPath, "", ""};

              // imgPath is the location of the image used in Tiled, not useful for collections of images, the correct image path is derived later
              if (!Create_Entities::PVG_Building(World::zone, x, y, x, y, templateName, id, aabb, pointVecs, line)) {
                if (!Create_Entities::Polygon_Building(World::zone, x, y, templateName, entity_class, imgPath, aabb, pointVecs, line, offset)) {
                  Create_Entities::Create_Entity(World::zone, x, y, entity_class, is_random, data, player);
                }
              }
            }
          }
        } else {
          std::cout << "Failed loading map" << std::endl;
        }
      }
    }
  }
}

