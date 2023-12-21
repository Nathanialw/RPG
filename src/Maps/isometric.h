#pragma once
//
//namespace isometric
//{
//    void tiled()
//    {
//
//    if (layer->getName() == "ground")
//    {
//        // get gid of each tile
//        std::vector<int> GIDs;
//        for (auto tile : layer->getLayerAs<tmx::TileLayer>().getTiles()) {
//        if (GIDs.empty())
//        {
//            GIDs.emplace_back(tile.ID);
//        }
//
//        bool found = false;
//
//        for (auto i : GIDs)
//        {
//            if (i == tile.ID)
//        {
//
//                found = true;
//        break;
//        }
//    }
//    if (found == false)
//    {
//        GIDs.emplace_back(tile.ID);
//    }
//
//
//    // get tilesets of mapx
//    for (auto i : GIDs) {
//        for (auto tileset: map.getTilesets()) {
//    //match tileGID to texture GID
//            if (tileset.getFirstGID() <= i) {
//                int num = i - tileset.getFirstGID();
//                if (num <= 0) {
//    //save texture
//                    std::string name = tileset.getName();
//                    if (!Graphics::pTexture[name]) {
//                        std::string filePathString = tileset.getImagePath();
//                        filePathString.erase(0, 5);
//                        const char *filePathChar = filePathString.c_str();
//                        Graphics::pTexture[name] = Graphics::createTexture(filePathChar);
//    //                                        std::cout << "loaded: " << name << std::endl;
//                    }
//                    break;
//                }
//            } else {
//                break;
//            }
//        }
//    }
//
//
//}