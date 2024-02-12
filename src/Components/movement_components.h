#pragma once
#include "components.h"
#include "utilities.h"
#include <math.h>

namespace Movement_Component {


  //  Component::Direction Set_Direction16(const float angleInRadians) {
  //    if (angleInRadians > 1.37444 && angleInRadians <= 1.76714)
  //      return Component::Direction::N;
  //    if (angleInRadians > 0.98174 && angleInRadians <= 1.37444)
  //      return Component::Direction::NNE;
  //    if (angleInRadians > 0.58904 && angleInRadians <= 0.98174)
  //      return Component::Direction::NE;
  //    if (angleInRadians > 0.19634 && angleInRadians <= 0.58904)
  //      return Component::Direction::ENE;
  //    if (angleInRadians > 6.08683 && angleInRadians <= 0.19634)
  //      return Component::Direction::E;
  //    if (angleInRadians > 5.69413 && angleInRadians <= 6.08683)
  //      return Component::Direction::ESE;
  //    if (angleInRadians > 5.30143 && angleInRadians <= 5.69413)
  //      return Component::Direction::SE;
  //    if (angleInRadians > 4.90873 && angleInRadians <= 5.30143)
  //      return Component::Direction::SSE;
  //    if (angleInRadians > 4.51603 && angleInRadians <= 4.90873)
  //      return Component::Direction::S;
  //    if (angleInRadians > 4.12334 && angleInRadians <= 4.51603)
  //      return Component::Direction::SSW;
  //    if (angleInRadians > 3.73064 && angleInRadians <= 4.12334)
  //      return Component::Direction::SW;
  //    if (angleInRadians > 3.33794 && angleInRadians <= 3.73064)
  //      return Component::Direction::WSW;
  //    if (angleInRadians > 2.94524 && angleInRadians <= 3.33794)
  //      return Component::Direction::W;
  //    if (angleInRadians > 2.55254 && angleInRadians <= 2.94524)
  //      return Component::Direction::WNW;
  //    if (angleInRadians > 2.15984 && angleInRadians <= 2.55254)
  //      return Component::Direction::NW;
  //    if (angleInRadians > 1.76714 && angleInRadians <= 2.15984)
  //      return Component::Direction::NNW;
  //
  //
  //    Utilities::Log("Set_Direction() fallthrough error");
  //    return Component::Direction::N;
  //  }

  Component::Direction Set_Direction16(const float angleInRadians) {
    if (angleInRadians < -2.94524 || angleInRadians >= 2.94524)
      return Component::Direction::N;
    if (angleInRadians < 2.94524 && angleInRadians >= 2.55254)
      return Component::Direction::NNE;
    if (angleInRadians < 2.55254 && angleInRadians >= 2.15984)
      return Component::Direction::NE;
    if (angleInRadians < 2.15984 && angleInRadians >= 1.76714)
      return Component::Direction::ENE;
    if (angleInRadians < 1.76714 && angleInRadians >= 1.37444)
      return Component::Direction::E;
    if (angleInRadians < 1.37444 && angleInRadians >= 0.98174)
      return Component::Direction::ESE;
    if (angleInRadians < 0.98174 && angleInRadians >= 0.58904)
      return Component::Direction::SE;
    if (angleInRadians < 0.58904 && angleInRadians >= 0.19634)
      return Component::Direction::SSE;

    if (angleInRadians > -0.19634 && angleInRadians < 0.19634)
      return Component::Direction::S;
    if (angleInRadians > -0.58904 && angleInRadians <= -0.19634)
      return Component::Direction::SSW;
    if (angleInRadians > -0.98174 && angleInRadians <= -0.58904)
      return Component::Direction::SW;
    if (angleInRadians > -1.37444 && angleInRadians <= -0.98174)
      return Component::Direction::WSW;
    if (angleInRadians > -1.76714 && angleInRadians <= -1.37444)
      return Component::Direction::W;
    if (angleInRadians > -2.15984 && angleInRadians <= -1.76714)
      return Component::Direction::WNW;
    if (angleInRadians > -2.55254 && angleInRadians <= -2.15984)
      return Component::Direction::NW;
    if (angleInRadians > -2.94524 && angleInRadians <= -2.55254)
      return Component::Direction::NNW;


    Utilities::Log("Set_Direction16() fallthrough error");
    return Component::Direction::N;
  }

  Component::Direction Set_Direction(const float angleInRadians, const bool &hexDir) {
    if (!hexDir) {
      if (angleInRadians > 2.74889 || angleInRadians <= -2.74889)
        return Component::Direction::N;
      if (angleInRadians > 1.96349 && angleInRadians <= 2.74889)
        return Component::Direction::NE;
      if (angleInRadians > 1.17809 && angleInRadians <= 1.96349)
        return Component::Direction::E;
      if (angleInRadians > 0.39269 && angleInRadians <= 1.17809)
        return Component::Direction::SE;
      if (angleInRadians > -0.39269 && angleInRadians <= 0.39269)
        return Component::Direction::S;
      if (angleInRadians > -1.17811 && angleInRadians <= -0.39269)
        return Component::Direction::SW;
      if (angleInRadians > -1.96351 && angleInRadians <= -1.17811)
        return Component::Direction::W;
      if (angleInRadians > -2.74889 && angleInRadians <= -1.96351)
        return Component::Direction::NW;
    } else {
      return Set_Direction16(angleInRadians);
    }

    Utilities::Log("Set_Direction8() fallthrough error");
    return Component::Direction::N;
  }

  Component::Direction Look_At_Target(const float &positionX, const float &positionY, const float &targetX, const float &targetY, float &angleInRadians, const bool &hexDir) {
    angleInRadians = Utilities::Get_Direction_Point(positionX, positionY, targetX, targetY);
    return Set_Direction(angleInRadians, hexDir);
  }
};// namespace Movement_Component