#pragma once
#include <stdint.h>
#include "db.h"
#include "action_components.h"
#include "rendering_components.h"

namespace Spritesheet_Structs {


  struct Frame_Data {
    //number of frames/end if sheet
    int numFrames = 0;
    //index of beginning frame
    int firstFrame = 0;
    //reversability
    bool reverses = false;
    //time between frames
    int64_t frameLength = 16;
  };

  struct Frame {
    int startFrame;
    int numFrames;
  };


  struct Frame_Data_Packer {
    int startFrame = 9999;
    int NumFrames = 0;
    int reverses = 0;
    int frameSpeed = 75;
  };

  struct spriteSheet {
    //size of sprites
    int w;
    int h;
    std::map<Action_Component::Action_State, Frame_Data>* frameData;
    float offset_x;
    float offset_y;
    SDL_Texture *texture;
  };

  spriteSheet* demon;
  spriteSheet* cow;
  spriteSheet* skeleton_00;

  std::map<Action_Component::Action_State, Frame_Data> frameData;

  struct Data_On_Unit {
    spriteSheet frameData;
    float timeOnFrame;
    int currentFrame;
  };

  struct sheetData {
    int w = 0;
    int h = 0;
    float x_offset = 0;
    float y_offset = 0;
    int sheet_width = 0;
    int64_t time_between_frames = 0;
    std::string sheet_type = "flare";
    Frame_Data isStatic = {0, 0, 0, 0};
    Frame_Data idle = {0, 0, 0, 0};
    Frame_Data walk = {0, 0, 0, 0};
    Frame_Data run = {0, 0, 0, 0};
    Frame_Data attack = {0, 0, 0, 0};
    Frame_Data attack2 = {0, 0, 0, 0};
    Frame_Data cast = {0, 0, 0, 0};
    Frame_Data struck = {0, 0, 0, 0};
    Frame_Data block = {0, 0, 0, 0};
    Frame_Data evade = {0, 0, 0, 0};
    Frame_Data stunned = {0, 0, 0, 0};
    Frame_Data dying = {0, 0, 0, 0};
    Frame_Data corpse = {0, 0, 0, 0};
    Frame_Data low_hp = {0, 0, 0, 0};
    Frame_Data resting = {0, 0, 0, 0};
    Frame_Data ranged = {0, 0, 0, 0};
    Frame_Data cheer = {0, 0, 0, 0};
    Frame_Data behavior = {0, 0, 0, 0};
    Frame_Data summoned = {0, 0, 0, 0};
  };

  std::string Get_Sprite_Sheet(std::string &name) {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    std::string unit_name = db::Append_Quotes(name);
    const unsigned char* sheet;
    sqlite3_stmt* stmt;
    char buf[300];
    const char* jj = "SELECT sprite_sheet FROM unit_data WHERE name = ";
    strcpy(buf, jj);
    strcat(buf, unit_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      sheet = sqlite3_column_text(stmt, 0);
      const char * s = (const char *)sheet;
      unit_name = std::string(reinterpret_cast< const char *> (s));
    }

    return unit_name;
  }

  void Calculate_Start_Frame (std::unordered_map<uint8_t, Rendering_Components::Frame_Data_Packer> &actionFrameData, Action_Component::Action_State &action, int &frameIndex) {
    if (actionFrameData[action].startFrame == 9999) {
      actionFrameData[action].startFrame = frameIndex;
    }
  }

  void Calculate_Num_Frames (std::string &frame, std::unordered_map<uint8_t,  Rendering_Components::Frame_Data_Packer> &actionFrameData, Action_Component::Action_State &action) {
    if (frame.back() != '1') {
      return;
    }
    else {
      actionFrameData[action].NumFrames++;
    }
  }

  bool Get_Tileset_Frame_Data (std::string &templateName, std::string &frame, std::unordered_map<uint8_t, Rendering_Components::Frame_Data_Packer> &actionFrameData) {
    // name should be the template name from tiled
    Action_Component::Action_State action;

    // load the whole xml sheet
    if (templateName == frame) {
      action = Action_Component::Action_State::isStatic;
      actionFrameData[action].frameSpeed = 75;
      std::cout << "Success! " << templateName << " found " << "frame" << frame << std::endl;
      return true;
    }    
    return false;
  }
  
  int i = 120;

  bool Get_Frame_Action_Data (std::string unitType, std::string &templateName, std::string &frame, std::unordered_map<uint8_t, Rendering_Components::Frame_Data_Packer> &actionFrameData, int &frameIndex) {
    // name should be the template name from tiled
    Action_Component::Action_State action;

    // load the whole xml sheet
    if (unitType == "tileset") {
      action = Action_Component::Action_State::isStatic;
      actionFrameData[action].frameSpeed = 75;
      // std::cout << "Success! " << templateName << " found " << "frame" << frame << std::endl;
      return true;
      
    }
    else {
      /// get the
      std::string keyCheck = templateName;
      std::string frameCopy = frame;

      if (frameCopy.substr(0, keyCheck.size()) != keyCheck) {
	//std::cout << "xml file name foes not match texture atlas sprite name - NAME: " << templateName << " FRAME: " << frame << std::endl;
	//            should assign a dummy value to prevent a crash nad to make it an obviouos bug
	return false;
      }

      auto index = frameCopy.find(templateName); // Find the starting position of substring in the string
      if  (index != std::string::npos) {
	frameCopy.erase(index, keyCheck.length() + 1); // erase function takes two parameter, the starting index in the string from where you want to erase characters and total no of characters you want to erase.
      }

      ///just grab the first 3 letters of the string
      std::string checkAction = frameCopy.erase(frameCopy.length()-6);

    
      /// compare the string in the xml with the values, I should probably just read in from the db, just push the test strings back on a vector and iterate through comparing, I wonder if I can store the enum in the db too I would probably have to for it to be worth it.

      if (unitType == "RTP_female" || unitType == "RTP_male") {
	if (checkAction == "1-H Attack 3") {
	  action = Action_Component::Action_State::attack;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Idle 2") {
	  action = Action_Component::Action_State::idle;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Running") {
	  action = Action_Component::Action_State::walk;
	  actionFrameData[action].frameSpeed = 75;
	} else if (checkAction == "Get Hit 2") {
	  action = Action_Component::Action_State::struck;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Dead-Down Forward") {
	  action = Action_Component::Action_State::dying;
	  actionFrameData[action].frameSpeed = 100;
	} else if (checkAction == "Praying") {
	  action = Action_Component::Action_State::casting;
	  actionFrameData[action].frameSpeed = 150;
	  actionFrameData[action].reverses = 0;
	} else if (checkAction == "Casting") {
	  action = Action_Component::Action_State::cast;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Down 2") {
	  action = Action_Component::Action_State::dead;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Talking 2") {
	  action = Action_Component::Action_State::talk;
	  actionFrameData[action].frameSpeed = i;
	}
	else {
	  //std::cout << "Passthrough Error: action for frame " << checkAction << " not found" << std::endl;
	  return false;
	}
      }
      else if (unitType == "classes_male" || unitType == "classes_female") {
	if (checkAction == "Attack One Handed Side Slash") {
	  action = Action_Component::Action_State::attack;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Attack One Handed Overhead") {
	  action = Action_Component::Action_State::attack2;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Idle3") {
	  action = Action_Component::Action_State::idle;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Run") {
	  action = Action_Component::Action_State::walk;
	  actionFrameData[action].frameSpeed = 75;
	} else if (checkAction == "Get Hit") {
	  action = Action_Component::Action_State::struck;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Critical Health Idle 1") {
	  action = Action_Component::Action_State::dying;
	  actionFrameData[action].frameSpeed = 100;
	} else if (checkAction == "Casting Idle") {
	  action = Action_Component::Action_State::casting;
	  actionFrameData[action].frameSpeed = 150;
	  actionFrameData[action].reverses = 1;
	} else if (checkAction == "Casting") {
	  action = Action_Component::Action_State::cast;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Dead") {
	  action = Action_Component::Action_State::dead;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Hand Casual") {
	  action = Action_Component::Action_State::talk;
	  actionFrameData[action].frameSpeed = i;
	}
	else {
	  // std::cout << "Passthrough Error: action for frame " << checkAction << " not found" << std::endl;
	  return false;
	}
      }

      else if (unitType == "Medieval_Underdeep_Dwarves_Male" || unitType == "medieval_human_female" || unitType == "medieval_human_male") {
	if (checkAction == "attack1") {
	  action = Action_Component::Action_State::attack;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "idle1") {
	  action = Action_Component::Action_State::idle;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "idle2") {
	  action = Action_Component::Action_State::idle2;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "running") {
	  action = Action_Component::Action_State::run;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "walking") {
	  action = Action_Component::Action_State::walk;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "collapse") {
	  action = Action_Component::Action_State::struck;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "ko") {
	  action = Action_Component::Action_State::dying;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "dead") {
	  action = Action_Component::Action_State::dead;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "kneel") {
	  action = Action_Component::Action_State::kneel;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "sitting") {
	  action = Action_Component::Action_State::cast;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "sleeping") {
	  action = Action_Component::Action_State::resting;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "unique") {
	  action = Action_Component::Action_State::behavior;
	  actionFrameData[action].frameSpeed = i;
	}
	else {
	  // std::cout << "Passthrough Error: action for frame " << checkAction << " not found" << std::endl;
	  return false;
	}
      }
      else {
	if (checkAction == "1-H Attack 1") {
	  action = Action_Component::Action_State::attack;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Attack Two Hand Swing") {
	  action = Action_Component::Action_State::attack;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Attack 1") {
	  action = Action_Component::Action_State::attack;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "1-H Idle") {
	  action = Action_Component::Action_State::idle;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Idle") {
	  action = Action_Component::Action_State::idle;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "idle") {
	  action = Action_Component::Action_State::idle;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Idle1") {
	  action = Action_Component::Action_State::idle;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "idle1") {
	  action = Action_Component::Action_State::idle;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "1-H Walk") {
	  action = Action_Component::Action_State::walk;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "walking") {
	  action = Action_Component::Action_State::walk;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "behavior") {
	  action = Action_Component::Action_State::struck;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "behavior1") {
	  action = Action_Component::Action_State::behavior;
	  actionFrameData[action].frameSpeed = i;
	  //            } else if (checkAction == "Walking") {
	  //                action = Action_Component::Action_State::walk;
	  //                actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Run") {
	  action = Action_Component::Action_State::walk;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Running") {
	  action = Action_Component::Action_State::walk;
	  actionFrameData[action].frameSpeed = 75;
	} else if (checkAction == "running") {
	  action = Action_Component::Action_State::run;
	  actionFrameData[action].frameSpeed = 75;
	} else if (checkAction == "Get Hit 1") {
	  action = Action_Component::Action_State::struck;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Get Hit") {
	  action = Action_Component::Action_State::struck;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "dead") {
	  action = Action_Component::Action_State::struck;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "collapse") {
	  action = Action_Component::Action_State::dying;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "ko") {
	  action = Action_Component::Action_State::dead;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Dead") {
	  action = Action_Component::Action_State::dead;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Dead2") {
	  action = Action_Component::Action_State::dead;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Down_1") {
	  action = Action_Component::Action_State::dead;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Dead To Down") {
	  action = Action_Component::Action_State::dying;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Dead2") {
	  action = Action_Component::Action_State::dying;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Woozy") {
	  action = Action_Component::Action_State::dying;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Cast Idle") {
	  action = Action_Component::Action_State::casting;
	  actionFrameData[action].frameSpeed = i;
	} else if (checkAction == "Casting") {
	  action = Action_Component::Action_State::cast;
	  actionFrameData[action].frameSpeed = i;
	} else {
	  //std::cout << "Passthrough Error: action for frame " << checkAction << " not found" << std::endl;
	  return false;
	}
      }
      //only run when the number changes
      //std::cout << checkAction << " frame data successfully saved" << std::endl;
    }

    Calculate_Start_Frame(actionFrameData, action, frameIndex);
    //        Utilities::Log("----");
    //        Utilities::Log(frame);
    Calculate_Num_Frames(frame, actionFrameData, action);
 
    return true;
  }
}
