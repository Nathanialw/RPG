#pragma once
#include "action_components.h"
#include "db.h"
#include "rendering_components.h"
#include <stdint.h>

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
    std::map<Action_Component::Action_State, Frame_Data> *frameData;
    float offset_x;
    float offset_y;
    SDL_Texture *texture;
  };

  spriteSheet *demon;
  spriteSheet *cow;
  spriteSheet *skeleton_00;

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
    const unsigned char *sheet;
    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT sprite_sheet FROM unit_data WHERE name = ";
    strcpy(buf, jj);
    strcat(buf, unit_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      sheet = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) sheet;
      unit_name = std::string(reinterpret_cast<const char *>(s));
    }
    return unit_name;
  }

  void Calculate_Start_Frame(std::unordered_map<uint8_t, Rendering_Components::Frame_Data_Packer> &actionFrameData, Action_Component::Action_State &action, int &frameIndex) {
    if (actionFrameData[action].startFrame == 9999) {
      actionFrameData[action].startFrame = frameIndex;
    }
  }

  void Calculate_Num_Frames(std::string &frame, std::unordered_map<uint8_t, Rendering_Components::Frame_Data_Packer> &actionFrameData, Action_Component::Action_State &action) {
    if (frame.back() != '1') {
      return;
    } else {
      actionFrameData[action].NumFrames++;
    }
  }

  void Calculate_Num_Frames_Unity(std::string frame, std::unordered_map<uint8_t, Rendering_Components::Frame_Data_Packer> &actionFrameData, Action_Component::Action_State &action) {
    std::string E = frame.substr(frame.size() - 9, 3);
    if (E != "_E_") {
      return;
    } else {
      actionFrameData[action].NumFrames++;
    }
  }

  bool Get_Tileset_Frame_Data(std::string &templateName, std::string &frame, std::unordered_map<uint8_t, Rendering_Components::Frame_Data_Packer> &actionFrameData) {
    // name should be the template name from tiled
    Action_Component::Action_State action;

    // load the whole xml sheet
    if (templateName == frame) {
      action = Action_Component::Action_State::isStatic;
      actionFrameData[action].frameSpeed = 75;
      std::cout << "Success! " << templateName << " found "
                << "frame" << frame << std::endl;
      return true;
    }
    return false;
  }

  std::string ss(const std::string &templateName, const std::string &frame, const int &fromEnd) {
    std::string frameCopy = frame;

    if (frameCopy.substr(0, templateName.size()) != templateName) {
      Utilities::Log("Get_Frame_Action_Data() error: Sheet name: " + templateName + " != frame: " + frameCopy.substr(0, templateName.size()));
      return "";
    }

    auto index = frameCopy.find(templateName);// Find the starting position of substring in the string
    if (index != std::string::npos) {
      frameCopy.erase(index, templateName.length() + 1);// erase function takes two parameter, the starting index in the string from where you want to erase characters and total no of characters you want to erase.
    }

    ///just grab the first 3 letters of the string
    return frameCopy.erase(frameCopy.length() - fromEnd);
  }

  void Set_Action(std::unordered_map<uint8_t, Rendering_Components::Frame_Data_Packer> &actionFrameData, std::vector<Action_Component::Action_State> &actions, Action_Component::Action_State &action, Action_Component::Action_State state, int speed) {
    action = state;
    actionFrameData[action].frameSpeed = speed;
    actions.emplace_back(action);
  }

  bool Get_Frame_Action_Data(const std::string unitType, std::string &templateName, std::string &frame, std::unordered_map<uint8_t, Rendering_Components::Frame_Data_Packer> &actionFrameData, int &frameIndex) {
    // name should be the template name from tiled
    int i = 75;

    std::vector<Action_Component::Action_State> actions;
    Action_Component::Action_State action;
    // load the whole xml sheet
    if (unitType == "tileset") {
      action = Action_Component::Action_State::isStatic;
      actionFrameData[action].frameSpeed = 50;
      // std::cout << "Success! " << templateName << " found " << "frame" << frame << std::endl;
      return true;

    } else if (unitType == "Unity" || unitType == "dwarf_female" || unitType == "elf_female" || unitType == "human_female") {
      /// get the
      std::string checkAction = ss(templateName, frame, 9);
      if (checkAction.empty()) return false;

      /// compare the string in the xml with the values, I should probably just read in from the db, just push the test strings back on a vector and iterate through comparing, I wonder if I can store the enum in the db too I would probably have to for it to be worth it.
      // was 75
      int speed = 100;
      int fastSpeed = 50;
      if (checkAction == "singleStrike2") {
        Set_Action(actionFrameData, actions, action, Action_Component::Action_State::attack, fastSpeed);
      } else if (checkAction == "singleStrike1") {
        Set_Action(actionFrameData, actions, action, Action_Component::Action_State::attack2, fastSpeed);
      } else if (checkAction == "idle") {
        Set_Action(actionFrameData, actions, action, Action_Component::Action_State::idle, speed);
      } else if (checkAction == "combatIdle") {
        Set_Action(actionFrameData, actions, action, Action_Component::Action_State::combatIdle, speed);
      } else if (checkAction == "kneel") {
        Set_Action(actionFrameData, actions, action, Action_Component::Action_State::kneel, fastSpeed);
      } else if (checkAction == "summon") {
        Set_Action(actionFrameData, actions, action, Action_Component::Action_State::summon, fastSpeed);
      } else if (checkAction == "run") {
        Set_Action(actionFrameData, actions, action, Action_Component::Action_State::walk, speed);
      } else if (checkAction == "walk") {
        Set_Action(actionFrameData, actions, action, Action_Component::Action_State::run, speed);
      } else if (checkAction == "struck1") {
        Set_Action(actionFrameData, actions, action, Action_Component::Action_State::struck, fastSpeed);
      } else if (checkAction == "death") {
        Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dying, speed);
      } else if (checkAction == "castEnd") {
        Set_Action(actionFrameData, actions, action, Action_Component::Action_State::cast, fastSpeed);
      } else if (checkAction == "castStart") {
        Set_Action(actionFrameData, actions, action, Action_Component::Action_State::casting, fastSpeed);
      } else if (checkAction == "dead") {
        Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dead, speed);
      } else {
        std::cout << "Passthrough Error: action for frame " << checkAction << " not found" << std::endl;
        return false;
      }
    }

    else {
      ///just grab the first 3 letters of the string
      std::string checkAction = ss(templateName, frame, 6);
      if (checkAction.empty()) return false;

      /// compare the string in the xml with the values, I should probably just read in from the db, just push the test strings back on a vector and iterate through comparing, I wonder if I can store the enum in the db too I would probably have to for it to be worth it.
      if (unitType == "RTP_female" || unitType == "RTP_male") {
        if (checkAction == "1-H Attack 3") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::attack, i);
        } else if (checkAction == "Idle 2") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::idle, i);
        } else if (checkAction == "Running") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::walk, 75);
        } else if (checkAction == "Get Hit 2") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::struck, i);
        } else if (checkAction == "Dead-Down Forward") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dying, 100);
        } else if (checkAction == "Praying") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::casting, 150);
        } else if (checkAction == "Casting") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::cast, i);
        } else if (checkAction == "Down 2") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dead, i);
        } else if (checkAction == "Talking 2") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::talk, i);
        } else {
          //std::cout << "Passthrough Error: action for frame " << checkAction << " not found" << std::endl;
          return false;
        }
      }

      else if (unitType == "classes_male" || unitType == "classes_female") {
        if (checkAction == "Attack One Handed Stab") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::attack, i);
        } else if (checkAction == "Attack One Handed Overhead") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::attack2, i);
        } else if (checkAction == "Attack Polearm") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::attackPolearm, i);
        } else if (checkAction == "Attack Dual Wield") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::attackAOE, i);
        } else if (checkAction == "Idle3") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::idle, i);
        } else if (checkAction == "Run") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::walk, 75);
        } else if (checkAction == "Get Hit") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::struck, i);
        } else if (checkAction == "Critical Health Idle 1") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dying, 100);
        } else if (checkAction == "Casting Idle") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::casting, 150);
          actionFrameData[action].reverses = 1;
        } else if (checkAction == "Casting") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::cast, i);
        } else if (checkAction == "Attack Bow") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::ranged, i);
        } else if (checkAction == "Dead") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dead, i);
        } else if (checkAction == "Dead2") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dead, i);
        } else if (checkAction == "Hand Casual") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::talk, i);
        } else if (checkAction == "Interact") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::interact, i);
        } else if (checkAction == "Jump") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::jump, i);
          actionFrameData[action].reverses = 1;
        } else if (checkAction == "Kick") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::kick, i);
        } else if (checkAction == "Kneel") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::kneel, i);
        } else if (checkAction == "Pray Kneel") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::pray_kneeled, i);
        } else if (checkAction == "Pray Standing") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::pray_standing, i);
        } else if (checkAction == "Search") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::search, i);
        } else if (checkAction == "Cleaning Object") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::cleaning_object, i);
        } else {
          // std::cout << "Passthrough Error: action for frame " << checkAction << " not found" << std::endl;
          return false;
        }
      }

      else if (unitType == "Medieval_Underdeep_Dwarves_Male" || unitType == "medieval_human_female" || unitType == "medieval_human_male") {
        if (checkAction == "attack1") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::attack, i);
        } else if (checkAction == "idle1") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::idle, i);
        } else if (checkAction == "idle2") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::idle2, i);
        } else if (checkAction == "running") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::run, i);
        } else if (checkAction == "walking") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::struck, i);
        } else if (checkAction == "collapse") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::struck, i);
        } else if (checkAction == "ko") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dying, i);
        } else if (checkAction == "dead") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dead, i);
        } else if (checkAction == "kneel") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::kneel, i);
        } else if (checkAction == "sitting") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::cast, i);
        } else if (checkAction == "sleeping") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::resting, i);
        } else if (checkAction == "unique") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::behavior, i);
        } else {
          // std::cout << "Passthrough Error: action for frame " << checkAction << " not found" << std::endl;
          return false;
        }
      }

      else {
        if (checkAction == "1-H Attack 1") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::attack, i);
        } else if (checkAction == "Attack Two Hand Swing") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::attack, i);
        } else if (checkAction == "Attack 1") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::attack, i);
        } else if (checkAction == "Attack 2") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::attack2, i);
        } else if (checkAction == "attack") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::attack, i);
        } else if (checkAction == "1-H Idle") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::idle, i);
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::combatIdle, i);
        } else if (checkAction == "Idle") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::idle, i);
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::combatIdle, i);
        } else if (checkAction == "idle") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::idle, i);
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::combatIdle, i);
        } else if (checkAction == "Idle1") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::idle, i);
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::combatIdle, i);
        } else if (checkAction == "idle1") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::idle, i);
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::combatIdle, i);
        } else if (checkAction == "1-H Walk") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::walk, i);
        } else if (checkAction == "walking") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::walk, i);
        } else if (checkAction == "behavior") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::behavior, i);
        } else if (checkAction == "Behavior") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::behavior, i);
        } else if (checkAction == "behavior1") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::behavior, i);
        } else if (checkAction == "Run") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::walk, i);
        } else if (checkAction == "Running") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::walk, i);
        } else if (checkAction == "running") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::walk, i);
        } else if (checkAction == "Get Hit 1") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::struck, i);
        } else if (checkAction == "Get Hit") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::struck, i);
        } else if (checkAction == "dead") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::struck, i);
        } else if (checkAction == "gethit") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::struck, i);
        } else if (checkAction == "collapse") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dying, i);
        } else if (checkAction == "ko") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dead, i);
        } else if (checkAction == "sleep") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dead, i);
        } else if (checkAction == "Dead") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dead, i);
        } else if (checkAction == "Dead2") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dead, i);
        } else if (checkAction == "Down_1") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dead, i);
        } else if (checkAction == "Dead To Down") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dying, i);
        } else if (checkAction == "Dead2") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dying, i);
        } else if (checkAction == "Woozy") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::dying, i);
        } else if (checkAction == "Cast Idle") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::casting, i);
        } else if (checkAction == "Casting") {
          Set_Action(actionFrameData, actions, action, Action_Component::Action_State::cast, i);
        } else {
          //std::cout << "Passthrough Error: action for frame " << checkAction << " not found" << std::endl;
          return false;
        }
      }
      //only run when the number changes
      //std::cout << checkAction << " frame data successfully saved" << std::endl;
    }
    for (auto act: actions) {
      Calculate_Start_Frame(actionFrameData, act, frameIndex);
    }
    //        Utilities::Log("----");
    //        Utilities::Log(frame);
    if (unitType == "Unity" || unitType == "dwarf_female" || unitType == "elf_female" || unitType == "human_female") {
      for (auto act: actions) {
        Calculate_Num_Frames_Unity(frame, actionFrameData, act);
      }
    } else {
      for (auto act: actions) {
        Calculate_Num_Frames(frame, actionFrameData, act);
      }
    }

    return true;
  }

  bool Get_Frame_Action_Data2(std::string &templateName, std::string &frame, std::unordered_map<uint8_t, Rendering_Components::Frame_Data_Packer> &actionFrameData, int &frameIndex) {
    // name should be the template name from tiled
    int i = 75;
    Action_Component::Action_State action;

    // load the whole xml sheet

    /// get the
    std::string keyCheck = templateName;
    std::string frameCopy = frame;

    if (frameCopy.substr(0, keyCheck.size()) != keyCheck) {
      //        std::cout << "xml file name foes not match texture atlas sprite name - NAME: " << templateName << " FRAME: " << frame << std::endl;
      //            should assign a dummy value to prevent a crash and to make it an obvious bug
      return false;
    }

    auto index = frameCopy.find(templateName);// Find the starting position of substring in the string
    if (index != std::string::npos) {
      frameCopy.erase(index, keyCheck.length() + 1);// erase function takes two parameter, the starting index in the string from where you want to erase characters and total no of characters you want to erase.
    }

    ///just grab the first 3 letters of the string
    std::string checkAction = frameCopy.erase(frameCopy.length() - 3);

    /// compare the string in the xml with the values, I should probably just read in from the db, just push the test strings back on a vector and iterate through comparing, I wonder if I can store the enum in the db too I would probably have to for it to be worth it.

    //only run when the number changes
    //std::cout << checkAction << " frame data successfully saved" << std::endl;


    return true;
  }
}// namespace Spritesheet_Structs
