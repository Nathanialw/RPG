#pragma once

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
    float radius = 0.0f;
    Component::Position position = {};
    std::string colliderType = "";
    std::vector<std::vector<tmx::Vector2<float>>> pointVecs = {};
    Collision_Component::aabb aabb = {};
    Component::Line_Segment line = {};
  };
}// namespace Collision_Component
