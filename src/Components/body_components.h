#pragma once
#include <stdint.h>
#include "entt/entt.hpp"

namespace Body_Components {

  //    parts get damaged or destroyed during combat
  struct Head {

    //        damage reduces intelligence and has a higher chance to kill
    uint8_t nose;
    uint8_t tongue;

    uint8_t rightEye;
    uint8_t leftEye;

    uint8_t rightEar;
    uint8_t leftEar;
  };

  //    contains hands
  struct Arms {

    //        damaged arms and hands reduce damage you deal and your attack speed and strength
    uint8_t right;
    uint8_t left;

    uint8_t rightHand;
    uint8_t leftHand;
  };

  //    contains organs
  struct Torso {

    //        damaged organs hhave a higher chance to kill
    uint8_t heart;
    uint8_t stomach;

    //        damaged lungs reduce stamina
    uint8_t rightLung;
    uint8_t leftLung;
  };

  //contains feet
  struct Legs {

    //        damaged legs and feet reduce  mobility
    uint8_t right;
    uint8_t left;

    uint8_t rightFoot;
    uint8_t leftFoot;
  };

  enum Tissue_Damage {
    bruise,
    laceration
    //        etc
  };

  enum Bone_Damage {
    fractured,
    broken
    //        etc
  };

  struct Body {
    Head head;
    Arms arms;
    Torso torso;
    Legs legs;
  };

  void Init_Body(entt::registry &zone, entt::entity &entity) {
    zone.emplace_or_replace<Body>(entity);
  }
}
