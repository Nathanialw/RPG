#pragma once
#include "SQLite_objects_retrieve.h"
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
    ///these will eventually get vectors probably
    Placement_Box placementBox;
    std::vector<Component::Line_Segment> lineSegment{};
  };

  std::unordered_map<std::string, Building_Colliders> polygonColliders;

  void Update_Offsets() {
    for (auto &colliders: polygonColliders) {
      //retrieve offsets from DB using name colliders.first
      auto offset = SQLite_Object_Retrieve::Get_Building_Data(colliders.first);

      //update collider offsets
      for (auto &polygons: colliders.second.colliders.pointVecs) {
        for (auto &point: polygons) {
          point.x += offset.x;
          point.y += offset.y;
        }
      };

      for (auto &circle: colliders.second.colliders.circlesVecs) {
        circle.x += offset.x;
        circle.y += offset.y;
      }

      for (auto &point: colliders.second.placementBox.pointVecs) {
        point.x += offset.x;
        point.y += offset.y;
      }

      for (auto &point: colliders.second.lineSegment) {
        point.start.x += offset.x;
        point.start.y += offset.y;
        point.end.x += offset.x;
        point.end.y += offset.y;
      };
    }
  }

  void Update_Offset(const std::string &name) {
    //retrieve offsets from DB using name colliders.first
    auto offset = SQLite_Object_Retrieve::Get_Building_Data(name);

    auto colliders = polygonColliders[name];
    //update collider offsets
    for (auto &polygons: colliders.colliders.pointVecs) {
      for (auto &point: polygons) {
        point.x += offset.x;
        point.y += offset.y;
      }
    };

    for (auto &circle: colliders.colliders.circlesVecs) {
      circle.x += offset.x;
      circle.y += offset.y;
    }

    for (auto &point: colliders.placementBox.pointVecs) {
      point.x += offset.x;
      point.y += offset.y;
    }

    for (auto &point: colliders.lineSegment) {
      point.start.x += offset.x;
      point.start.y += offset.y;
      point.end.x += offset.x;
      point.end.y += offset.y;
    };
  }

}// namespace Collision_Component
