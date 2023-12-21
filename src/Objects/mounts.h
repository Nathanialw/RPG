#pragma once
#include "entt/entt.hpp"

namespace Mount_Components {

  struct Mount {


  };

  enum Mount_Type {
    horse
  };

  //    attach to an entity when it is mounted
  struct Mounted {
    entt::entity mount_ID;

  };

  //    for an entity that can be mounted
  struct Mountable {

  };


}

namespace Mount_Functions {

  void mount() {

    //        click on the mount

    //        set position as an offset to the mount

    //        set the state to mounted

  }

  void Create_Mountable() {

  }

  void Render_Mounted() {
    //split the horse in half and rend the character between the two halves so we can hide the riders leg
  }

}

namespace Mount_Routines {



}
