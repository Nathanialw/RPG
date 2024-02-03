#pragma once
#include "collision_components.h"
#include "components.h"
#include "tmxlite/Object.hpp"

namespace Building_Component {

  //aabb
  //polygon
  //circle
  struct Circle {
    SDL_FPoint point;
    float radius;
  };

  struct Polygon {
    std::vector<f2> polygon;
  };

  struct Placement {
    std::vector<Polygon> polygons;
    std::vector<Circle> circles;
    bool placeable = false;
  };
}// namespace Building_Component