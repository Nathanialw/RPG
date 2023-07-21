#include "entt/entt.hpp"
#include "SDL2/SDL.h"

// What is a squad in the game?

/*
 * They command individual entities in the context only of the squad
 *
 *
 */

namespace Squad_Components {

  //attaches to the leader
  struct Squad {
    //        Could be unit entities or formation entities
    entt::entity personnel[12];
    u_int8_t size;
    std::string name = "Default Formation Name";
    bool detach;
    SDL_FRect position;
  };

  //attaches to each member of the squad
  struct Squad_Member {
    std::string name = "Default Formation Name";
  };

}

namespace Squad_Functions {

  //    Keep the squad within a specific rect size

  //    searh for places within that rect to order the units to be

  //    Attack enemies within range

}

namespace Squad_Routines {



}

