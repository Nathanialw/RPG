#pragma once
//#include "world.h"

namespace Cave {

  bool Cave = false;
  bool Overworld = false;

  void Enter_Cave() {
    Cave = true;
  }

  void Exit_Cave() {
    Overworld = true;
  }

  void Load_Cave() {
    Cave = false;
  }

  void Load_Overworld() {
    Overworld = false;
  }

  //hide overworld move input character to new location?
  //set tile type to render
  //set unit types to create
}