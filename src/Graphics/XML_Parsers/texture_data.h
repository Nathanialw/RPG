#pragma once
#include "rendering_components.h"
#include "string"
#include "unordered_map"

namespace Texture_Data {
  std::unordered_map<std::string, Rendering_Components::Sheet_Data> Packer_Textures;
  std::unordered_map<std::string, Rendering_Components::Image_Data> Image_Textures;

  struct Type_Data {
    std::string type;
    std::string xml_path;
    std::string img_path;
  };

  struct Sheet_Data {
    std::unordered_map<std::string, Rendering_Components::Sheet_Data> *packerData;
    std::unordered_map<std::string, Rendering_Components::Image_Data> *imageData;
  };
}// namespace Texture_Data