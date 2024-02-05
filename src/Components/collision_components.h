#pragma once
#include "tmxlite/Object.hpp"

namespace Collision_Component {

  struct Dynamic_Collider {
    bool awake = true;
  };

  struct Dynamic_Collision {
    float x = 0;
    float y = 0;
  };

  struct aabb {
    int hx = 0;
    int hy = 0;
  };

  struct Collider_Data {
    std::string name;
    Rendering_Components::Sprite_Offset offset = {};
    float radius = 0.0f;
    Component::Position position = {};
    std::string colliderType;
    float placementOffset = 0.0f;
    std::vector<std::vector<tmx::Vector2<float>>> pointVecs = {};
    Collision_Component::aabb aabb = {};
    Component::Line_Segment line = {};
  };

  struct Circle {
    float r = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
  };

  struct Placement_Box {
    u_int8_t groupIndex = 1;
    bool isSensor = false;
    std::vector<tmx::Vector2<float>> pointVecs = {};
  };

  struct Colliders {
    b2BodyType bodyType = b2_staticBody;
    std::vector<bool> isSensor;
    std::vector<std::vector<tmx::Vector2<float>>> pointVecs = {};
    std::vector<Circle> circlesVecs = {};
  };

  struct Building_Colliders {
    Colliders colliders;
    ///these will eventuall get vectors probably
    Placement_Box placementBox;
    std::vector<Component::Line_Segment> lineSegment{};
  };

  std::unordered_map<std::string, Building_Colliders> houseColliders;
}// namespace Collision_Component
