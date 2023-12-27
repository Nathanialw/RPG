#pragma once
#include "components.h"
#include "utilities.h"


namespace Movement_Component {


  Component::Direction Set_Direction(float angleInRadians) {
    if (angleInRadians > 2.74889 || angleInRadians <= (-2.74889)) { return Component::Direction::N; }
    else if (angleInRadians > 1.96349 && angleInRadians <= (2.74889)) { return Component::Direction::NE; }
    else if (angleInRadians > 1.17809 && angleInRadians <= (1.96349)) { return Component::Direction::E; }
    else if (angleInRadians > 0.39269 && angleInRadians <= (1.17809)) { return Component::Direction::SE; }
    else if (angleInRadians > (-0.39269) && angleInRadians <= (0.39269)) { return Component::Direction::S; }
    else if (angleInRadians > (-1.17811) && angleInRadians <= (-0.39269)) { return Component::Direction::SW; }
    else if (angleInRadians > (-1.96351) && angleInRadians <= (-1.17811)) { return Component::Direction::W; }
    else if (angleInRadians > (-2.74889) && angleInRadians <= (-1.96351)) { return Component::Direction::NW; }

    Utilities::Log("Set_Direction() fallthrough error");
    return Component::Direction::N;
  }

  Component::Direction Look_At_Target(float &positionX, float &positionY, float &targetX, float &targetY, float &angleInRadians) {
    angleInRadians = Utilities::Get_Direction_Point(positionX, positionY, targetX, targetY);
    return Set_Direction(angleInRadians);
  }

};