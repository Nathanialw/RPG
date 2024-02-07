#pragma once
#include "components.h"

namespace Movement_Functions {

  //takes in the x,y of the unit and checks if it is within the destination x,y within an accuracy of the set variable
  bool Check_If_Arrived(const float &unitX, const float &unitY, const float &destinationX, const float &destinationY) {
    float accuracy = 5.0f;
    if (unitX + accuracy > destinationX &&
        unitX - accuracy < destinationX &&
        unitY - accuracy < destinationY &&
        unitY + accuracy > destinationY) {
      return true;
    } else {
      return false;
    }
  }

  bool Check_If_Arrived(const Component::Position &position, const Component::Position &targetPosition) {
    float accuracy = 5.0f;
    if (position.x + accuracy > targetPosition.x &&
        position.x - accuracy < targetPosition.x &&
        position.y - accuracy < targetPosition.y &&
        position.y + accuracy > targetPosition.y) {
      return true;
    } else {
      return false;
    }
  }
}// namespace Movement_Functions