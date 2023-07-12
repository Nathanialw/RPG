#include <SDL2/SDL.h>
#include "components.h"
#include "graphics.h"

namespace UI_Unit_Control {

  // the animation for the select ring
    void Loop_Sprite(Component::Selected& frame, SDL_FRect &frect, Rendering_Components::Color &color, Component::Camera &camera) {
    // clip : size ont the frame in the spritesheet, will also use the size to render
   // rect : x,y is the targets rect around his feet, needs to be converted to screen coords probably

    // iterate through the frames, move to next once the frame length is reached
      SDL_Rect clipRect = frame.clip;

      frame.currentFrameTime += Timer::timeStep;
      
      if (frame.currentFrameTime >= frame.frameLength) {
	frame.currentFrameTime -= frame.frameLength;
	frame.currentFrame++;
	if (frame.currentFrame >= frame.numFrames) {
	  frame.currentFrame = 0;
	}
      }
      
      if (frame.texture) {
        clipRect.x = frame.clip.w * frame.currentFrame;
        SDL_FRect renderRect = Camera_Control::Convert_Rect_To_Screen_Coods(frect, camera);
        Graphics::Render_FRect(frame.texture, color, &clipRect, &renderRect);
      }
      else {
	std::cout << "Loop_Sprite() selector texture is NULL" << std::endl;
      }
    }

  // a UI for the commands you can issue a unit

  // build building UI, the first iteration will be jsut on the player but in the future a peasant will be used to build


  

  

}
