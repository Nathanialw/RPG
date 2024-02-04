#pragma once
#include "collision_components.h"
#include "components.h"
#include "tmxlite/Object.hpp"

namespace Building_Component {

  typedef std::vector<f2> Polygon;
  //aabb
  //polygon
  //circle
  struct Circle {
    SDL_FPoint point;
    float radius;
  };

  struct Placement {
    std::vector<Polygon> polygons;
    std::vector<Circle> circles;
    bool obstructed = false;
  };

  struct Set_Placement {
    std::vector<Polygon> polygons;
    std::vector<Circle> circles;
  };
}// namespace Building_Component