#pragma once
#include "death_control.h"
#include "rendering_components.h"
#include "rendering_functions.h"

void Frame_Increment(entt::entity &entity, Component::Scale &scale, Rendering_Components::Sprite_Sheet_Info &sheetData, Action_Component::Action &action, Component::Direction &direction) {

  action.frameTime += Timer::timeStep;

  // no need to increment frame index of a static sprite, exit now
  if (action.state == Action_Component::isStatic) {
    return;
  }

  if (action.frameState == Action_Component::last) {
    if (!sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].reverses && (action.state != Action_Component::dying || action.state != Action_Component::dead)) {
      if (action.frameTime >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed) {

        action.state = Action_Component::idle;
        action.frameTime -= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed;
        action.frame = 0;
        action.frameState = Action_Component::start;
      }
    } else {
      action.frameState = Action_Component::start;
    }
  }

  if (action.frameTime >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed) {
    action.frameTime -= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].frameSpeed;
    //            sheetData.frameTime = 0;
    if (action.frameState == Action_Component::start) {
      if (action.state == Action_Component::dying || action.state == Action_Component::dead) {

      }
      //without this line teh frames break and I don't know why
//      else if (action.state != Action_Component::walk && action.state != Action_Component::struck && action.state != Action_Component::attack && action.state != Action_Component::attack2 && action.state != Action_Component::cast && action.state != Action_Component::casting && action.state != Action_Component::talk && action.state != Action_Component::idle) {
//        Action_Component::Set_State(action, Action_Component::idle);
//      }
      /* else if (action.state == Action_Component::struck || action.state == Action_Component::attack || action.state == Action_Component::cast) { */
      //Action_Component::Set_State(action, Action_Component::idle);
      /* } */
      action.frameState = Action_Component::mid;
    }

    ///reset frame count if over
    sheetData.frameIndex = sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].startFrame + (sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames * PVG_Direction_Enum(direction)) + action.frame;

    if (Death_Control::Death_Sequence(direction, entity, scale, sheetData, action, sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames)) {
      return;
    }

    ///calculate reversing
    if (!sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].reverses) {
      sheetData.reversing = 0;
    }
    // increment if it is reversing
    if (sheetData.reversing) {
      action.frame--;
      if (action.frame < 1) {
        sheetData.reversing = 0;
      }
    } else if (!sheetData.reversing) {
      action.frame++;
    }
    /// -1 because of the zero index
    if (sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].reverses) {
      if (action.frame >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames - 1) {
        sheetData.reversing = 1;
      }
    } else if (action.frame >= sheetData.sheetData->at(sheetData.sheet_name).actionFrameData[action.state].NumFrames) {
      if (action.state != Action_Component::walk && action.state != Action_Component::run) {
        action.frameState = Action_Component::last;
      }
      action.frame = 0;
    }
  }
}

void Update_Item_Frame(SDL_Rect &clipRect, uint8_t row, uint16_t column) {
  clipRect = {column * clipRect.w, row * clipRect.h, clipRect.w, clipRect.h};
}

void Update_Equipment(Rendering_Components::Equip_Slot_Data &item, Action_Component::Action &action, Component::Direction &direction) {
  item.FrameIndex = item.ItemSheetData->at(item.name).actionFrameData[action.state].startFrame + (item.ItemSheetData->at(item.name).actionFrameData[action.state].NumFrames * PVG_Direction_Enum(direction)) + action.frame;
}
