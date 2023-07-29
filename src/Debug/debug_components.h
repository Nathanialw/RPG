#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>



namespace Debug {

  unsigned long int numEntities;
  bool showPosition = false;
  bool settings[4];

  void Load_Settings() {
      settings[0] = false;
      settings[1] = false;
      settings[2] = false;
      settings[3] = false;
  };

	struct DEBUG_Position {

	};


}