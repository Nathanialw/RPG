#pragma once
#include "SDL2/SDL.h"
#include "vector"
#include <map>

#include "components.h"
#include "mounts.h"

namespace Rendering_Components {

  struct Building {
    int frameIndex;
  };

  struct Background {
  };

  struct Foreground {
  };

  struct Sprite_Offset {
    float x;
    float y;
  };

  struct Sprite_Sheet_Data {
    SDL_Rect clip;
    float x_offset;
    float y_offset;
  };

  struct Frame_Data_Packer {
    int startFrame = 9999;
    int NumFrames = 0;
    int reverses = 0;
    int frameSpeed = 75;
  };

  struct Color {
    u_int8_t r;
    u_int8_t g;
    u_int8_t b;
  };

  struct Portrait {
    SDL_Texture *texture = NULL;
    SDL_Color color = {255, 255, 255};
  };

  struct Body {
    SDL_Texture *texture = NULL;
    SDL_Color color = {255, 255, 255};
  };

  struct Unit_Frame_Portrait {
    SDL_Texture *texture = NULL;
    std::unordered_map<int, Portrait> gear;
  };

  struct Body_Frame {
    SDL_Texture *texture = NULL;
    std::unordered_map<int, Portrait> gear;
  };

  /// one per item, but they can point to the same texture

  struct Sheet_Data {
    SDL_Texture *texture = NULL;
    std::vector<Sprite_Sheet_Data> frameList;
    /// start frame by state, number of frames per state.
    std::unordered_map<uint8_t, Frame_Data_Packer> actionFrameData;
    ///store the frame duration for each frame of each state, probably not worth it
    //        std::unordered_map<Component::Action_State, std::vector<int>> Frame_Speed_By_Action;
  };

  struct Sheet_Data_Flare {
    SDL_Texture *texture = NULL;//texture
    SDL_Color color = {255, 255, 255};
    int sheetWidth = 0;
    int spriteWidth = 0;
    int spriteHeight = 0;
    //        Frame_Data_Packer actionFrameDataArray[17];
    //        std::vector<Frame_Data_Packer> actionFrameDataVector[20];

    // find the frame data from the action
    std::unordered_map<uint8_t, Frame_Data_Packer> actionFrameData;

    // find the frame data from the index of the building
  };

  struct Mount_Sprite {
    std::unordered_map<std::string, Sheet_Data> *mount = NULL;
    std::string front_name = "default";
  };

  struct Equip_Slot_Data {
    std::unordered_map<std::string, Sheet_Data> *ItemSheetData = NULL;
    //name is the index into the map that stores the data
    std::string name = "empty";
    SDL_Color color = {255, 255, 255};
    int FrameIndex = 0;
    entt::entity itemID;
  };

  struct Equipment_Sprites {
    Equip_Slot_Data sheet[30];
  };

  ///component for the unit
  struct Sprite_Sheet_Info {
    std::string type = "default";

    //        a union can how different types of data in the same memory address
    //        union {
    std::unordered_map<std::string, Sheet_Data_Flare> *flareSpritesheet;
    std::unordered_map<std::string, Sheet_Data> *sheetData;
    //        };

    std::string sheet_name = "default";

    std::unordered_map<std::string, Sheet_Data> *mount = NULL;
    std::string mount_name = "unmounted";
    SDL_Color color = {255, 255, 255};

    //        Equip_Slot_Data equipmentSheets[30];

    ///possible replacement for a string map is to store the name of the sprite sheet in a map and match it to a unique index, save that index and make the sheetData and vector, access the vector with the index
    //        std::string* sheetData;
    //        int index = 0;
    uint16_t frameIndex = 0;
    uint8_t reversing = 0;
    //        int currentFrame = 0;
    //        Frame finalFrame = normalFrame;
  };
}// namespace Rendering_Components
