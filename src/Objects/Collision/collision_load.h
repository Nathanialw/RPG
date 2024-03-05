#pragma once
#include "Graphics/XML_Parsers/texture_packer.h"
#include "collision_components.h"

namespace Collision {


  void Update_Offsets() {
    for (auto &colliders: Collision_Component::polygonColliders) {
      //retrieve offsets from DB using name colliders.first
      auto offset = SQLite_Object_Retrieve::Get_Building_Data(colliders.first);

      //update collider offsets
      for (auto &polygons: colliders.second.colliders.pointVecs) {
        for (auto &point: polygons) {
          point.x -= offset.x;
          point.y += offset.y;
        }
      };

      for (auto &circle: colliders.second.colliders.circlesVecs) {
        circle.x -= offset.x;
        circle.y += offset.y;
      }

      for (auto &point: colliders.second.placementBox.pointVecs) {
        point.x -= offset.x;
        point.y += offset.y;
      }

      for (auto &point: colliders.second.lineSegment) {
        point.start.x -= offset.x;
        point.start.y += offset.y;
        point.end.x -= offset.x;
        point.end.y += offset.y;
      };
    }
  }

  void Update_Offset_X(const std::string &name, const float &increment) {
    //retrieve offsets from DB using name colliders.first
    Collision_Component::Building_Colliders colliders;
    auto text = SQLite_Object_Retrieve::Get_Interior(name);
    (!text.empty()) ? colliders = Collision_Component::polygonColliders[name] : colliders = Collision_Component::polygonColliders[text];

    //update collider offsets
    for (auto &polygons: colliders.colliders.pointVecs) {
      for (auto &point: polygons) {
        point.x -= increment;
      }
    };

    for (auto &circle: colliders.colliders.circlesVecs) {
      circle.x -= increment;
    }

    for (auto &point: colliders.placementBox.pointVecs) {
      point.x -= increment;
    }

    for (auto &point: colliders.lineSegment) {
      point.start.x -= increment;
      point.end.x -= increment;
    };
  }

  void Update_Offset_Y(const std::string &name, const float &increment) {
    //retrieve offsets from DB using name colliders.first
    Collision_Component::Building_Colliders colliders;
    auto text = SQLite_Object_Retrieve::Get_Interior(name);
    (!text.empty()) ? colliders = Collision_Component::polygonColliders[name] : colliders = Collision_Component::polygonColliders[text];

    //update collider offsets
    for (auto &polygons: colliders.colliders.pointVecs) {
      for (auto &point: polygons) {
        point.y += increment;
      }
    };

    for (auto &circle: colliders.colliders.circlesVecs) {
      circle.y += increment;
    }

    for (auto &point: colliders.placementBox.pointVecs) {
      point.y += increment;
    }

    for (auto &point: colliders.lineSegment) {
      point.start.y += increment;
      point.end.y += increment;
    };
  }

  void Load() {
    Texture_Packer::TexturePacker_Import_Collision("assets/sprites/buildings/medieval/collision/interiors.xml", Collision_Component::polygonColliders);
    Texture_Packer::TexturePacker_Import_Collision("assets/sprites/buildings/medieval/collision/cave_entrances.xml", Collision_Component::polygonColliders);
    Texture_Packer::TexturePacker_Import_Collision("assets/sprites/buildings/medieval/collision/labyrinth0.xml", Collision_Component::polygonColliders);
    Texture_Packer::TexturePacker_Import_Collision("assets/sprites/buildings/medieval/collision/labyrinth0_objects.xml", Collision_Component::polygonColliders);
    Update_Offsets();
  }

}// namespace Collision
