#include "component.h"
#include "graphics.h"
#include "UI_elements.h"

namespace UI_Unit_Control {

  // the animation for the select ring

  void Loop_Sprite(SDL_Texture* texture, int &frameLength, int &numFrames, SDL_Rect &clip, SDL_FRect &frect, SDL_color &color) {
    // clip : size ont the frame in the spritesheet, will also use the size to render
    // rect : x,y is the targets rect around his feet, needs to be converted to screen coords probably



    SDL_FRect renderRect = Convert_Rect_To_Screen_Coods(frect, camera);

    Render_FRect(texture, color, const SDL_Rect* clip, SDL_FRect* renderRect);

  }

  // a UI for the commands you can issue a unit

  
  // build building UI, the first iteration will be jsut on the player but in the future a peasant will be used to build


  

  

}
