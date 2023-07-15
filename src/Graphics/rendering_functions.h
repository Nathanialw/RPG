#pragma once
#include "components.h"
#include "action_components.h"
#include "rendering_components.h"
#include "timer.h"

int PVG_Direction_Enum(Component::Direction &direction) {
  switch (direction) {
  case Component::Direction::N: return 3;
  case Component::Direction::S: return 0;
  case Component::Direction::E: return 2;
  case Component::Direction::W: return 1;
  case Component::Direction::NW: return 5;
  case Component::Direction::NE: return 7;
  case Component::Direction::SW: return 4;
  case Component::Direction::SE: return 6;
  }
  Utilities::Log("PVG_Direction_Enum passthrough error");
  return 0;
}

int Original_Direction_Enum(Component::Direction &direction) {
  switch (direction) {
  case Component::Direction::N: return 2;
  case Component::Direction::S: return 6;
  case Component::Direction::E: return 4;
  case Component::Direction::W: return 0;
  case Component::Direction::NW: return 1;
  case Component::Direction::NE: return 3;
  case Component::Direction::SW: return 7;
  case Component::Direction::SE: return 5;
  }

  Utilities::Log("Original_Direction_Enum() fallthough error");
  return 2;
}





    
  

