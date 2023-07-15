#pragma once
#include "rendering_functions.h"
#include "death_control.h"

  void Update_Frame_PVG(SDL_Rect &clipRect, Rendering_Components::Sprite_Sheet_Info &sheetData, Action_Component::Action &action, Component::Direction &direction) {
    int spritesPerLine = sheetData.flareSpritesheet->at(sheetData.sheet_name).sheetWidth / sheetData.flareSpritesheet->at(sheetData.sheet_name).spriteWidth;
    ///the x index in absolute terms
    int x_index = sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].startFrame + ((PVG_Direction_Enum(direction) * sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames) + action.frame);
    ///the x index in relative terms
    int x_rel_index = x_index % spritesPerLine;
    ///the x index in pixel terms
    clipRect.x = x_rel_index * sheetData.flareSpritesheet->at(sheetData.sheet_name).spriteWidth;
    ///after dividing away the x index, add the remainder
    int y_index = x_index / spritesPerLine;
    ///the y index in pixel terms
    clipRect.y = y_index * sheetData.flareSpritesheet->at(sheetData.sheet_name).spriteWidth;
  }

  void Update_Frame_Flare(SDL_Rect &clipRect, uint8_t currentFrame, int &startFrame, Component::Direction& direction) {
    clipRect.x = (startFrame + currentFrame) * clipRect.w;
    clipRect.y = clipRect.h * Original_Direction_Enum(direction);
  }


  void Update_Frame(entt::entity entity, Component::Scale scale, Rendering_Components::Sprite_Sheet_Info &sheetData, Component::Direction& direction, Action_Component::Action& action) {

    action.frameTime += Timer::timeStep;
    if (action.frameState == Action_Component::last) {
      action.frameState = Action_Component::start;
    }

    if (action.frameTime >= sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed) {
      action.frameTime -= sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed;
      //            sheetData.frameTime = 0;

      if (action.state != Action_Component::isStatic) {
	/// reset at the start so it had a chance loop through the logic once to trigger end of state actions
	if (action.frameState == Action_Component::start) {

	  if (action.state != Action_Component::walk && action.state != Action_Component::struck && action.state != Action_Component::attack && action.state != Action_Component::attack2 && action.state != Action_Component::dying && action.state != Action_Component::dead) {
	    action.state = Action_Component::idle;
	  } else if (action.state == Action_Component::struck || action.state == Action_Component::attack) {
	    action.state = Action_Component::idle;
	  }
	  action.frameState = Action_Component::mid;
	}

	if (Death_Control::Death_Sequence(direction, entity, scale, sheetData, action, sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames)) {
	  return;
	}
	///render first frame before increment
	if (action.frame == 0) {
	  action.frame++;
	  sheetData.reversing = false;
	  return;
	}

	if (sheetData.reversing == false) {
	  action.frame++;
	}

	else if (sheetData.reversing == true) {
	  action.frame--;
	}

	if (sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].reverses) {
	  /// -1 because of the zero index
	  if (action.frame >= sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames - 1) {
	    sheetData.reversing = true;
	  }
	}

	else if (action.frame >= sheetData.flareSpritesheet->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames) {

	  if (action.state != Action_Component::walk && action.state != Action_Component::run) {
	    action.frameState = Action_Component::last;
	  }
	  if (action.state == Action_Component::attack2) {
	    action.state = Action_Component::idle;
	  }
	  action.frame = 0;
	}
      }
    }
  }

  

