#pragma once
#include "rendering_components.h"
#include "map.h"


SDL_Rect getTexture(uint32_t x, int &sheetWidth, SDL_Rect &tileSpriteRect) {
  int y = 0;
  x -= 1;
  while (x >= sheetWidth) {
    x -= sheetWidth;
    y++;
  }
  tileSpriteRect.x = tileSpriteRect.w * x;
  tileSpriteRect.y = tileSpriteRect.h * y;
  return tileSpriteRect;
}
//
//  void asdsa () {      {
//          auto &numOfTiles = map.getTileCount();
//          auto &layers = map.getLayers();
//
//          int g = 0;
//          //int o = 0;
//          auto &tilesets = Maps::map.getTilesets();
//          int p = Maps::map.getTilesets().size() - 1;
//          for (int i = 0; i < layers.size(); i++) {
//              if (layers[i]->getType() == tmx::Layer::Type::Tile) {
//                  const auto &tiles = layers[i]->getLayerAs<tmx::TileLayer>().getTiles();
//
//                  int k = (numOfTiles.x * y) + x;
//
//                  if (tiles[k].ID != 0) {
//                      auto id = tiles[k].ID;
//                      for (int tilesetCount = p; tilesetCount >= 0; --tilesetCount) {
//                          const tmx::Tileset *tileset = &tilesets[tilesetCount];
//                          if (tileset->getFirstGID() - 1 <= id) {
//                              id -= tilesets[tilesetCount].getFirstGID() - 1;
//                              std::string name = tileset->getName();
//                              int tileCount = tileset->getColumnCount();
//                              SDL_Rect data = getTexture(id, tileCount, tileSpriteRect);
//                              break;
//                          }
//                      }
//                  }
//
//
//                  }
//                  if (i == 1) {
//
//                  }
//              }
//          }
//      }


//void Render_Iso_Tiles(entt::registry &zone, tmx::Map& map, Component::Camera& camera)
//{
//  float originX = 0.0f;
//  float originY = 0.0f;
//  SDL_Rect tileSpriteRect = {0, 0, (int) map.getTileSize().x, (int) map.getTileSize().y};
//  SDL_FRect renderPosition = {0.0f, 0.0f, (float) map.getTileSize().x, (float) map.getTileSize().y};
//  auto &numOfTiles = map.getTileCount();
//  auto &layers = map.getLayers();
//  const auto tileWidth = (float) map.getTileSize().x;
//  const auto tileHeight = (float) map.getTileSize().y;
//  int newX = (int) camera.screen.x;
//  int newY = (int) camera.screen.y;
//  int X = (newX / tileWidth) + (newY / tileHeight) - (camera.screen.w / 2 / (tileWidth / 2));
//  int Y = (newY / tileHeight) - (newX / tileWidth) - (camera.screen.h / 2 / (tileHeight / 2));
//
//  int width = X + (camera.screen.w / tileWidth * 4);
//  int height = Y + (camera.screen.h / tileHeight * 3);
//
//  int g = 0;
//  //int o = 0;
//  auto &tilesets = Maps::map.getTilesets();
//  int p = Maps::map.getTilesets().size() - 1;
//  for (int i = 0; i < layers.size(); i++) {
//    if (layers[i]->getType() == tmx::Layer::Type::Tile) {
//      const auto &tiles = layers[i]->getLayerAs<tmx::TileLayer>().getTiles();
//      if (i == 2) {
//	/// renders all objects just above the 2nd layer but below the 3rd in tiled
//      }
//      for (int x = X; x < width; x++) {
//	if (x < 0) { x = 0; }
//	for (int y = Y; y < height; y++) {
//	  //h++;
//	  if (y < 0) { y = 0; }
//	  if (x >= numOfTiles.x) { break; }
//	  if (y >= numOfTiles.y) { break; }
//	  renderPosition.x = originX + (x * tileWidth / 2.0f) - (y * tileWidth / 2.0f);
//	  renderPosition.y = originY + (x * tileHeight / 2.0f) + (y * tileHeight / 2.0f);
//	  renderPosition.x -= camera.screen.x;;
//	  renderPosition.y -= camera.screen.y;;
//	  int k = (numOfTiles.x * y) + x;
//	  if (tiles[k].ID != 0) {
//	    auto id = tiles[k].ID;
//	    for (int tilesetCount = p; tilesetCount >= 0; --tilesetCount) {
//	      const tmx::Tileset *tileset = &tilesets[tilesetCount];
//	      if (tileset->getFirstGID() - 1 <= id) {
//		id -= tilesets[tilesetCount].getFirstGID() - 1;
//		std::string name = tileset->getName();
//		int tileCount = tileset->getColumnCount();
//		SDL_Rect data = getTexture(id, tileCount, tileSpriteRect);
//		if (Graphics::pTexture[name] != NULL) {
//		  Graphics::Render_FRect(Graphics::pTexture[name], {255, 255, 255}, &data, &renderPosition);
//		}
//		//h++;
//		break;
//	      }
//	    }
//	  }
//	}
//	g++;
//      }
//      if (i == 1) {
//
//      }
//    }
//  }
//}
