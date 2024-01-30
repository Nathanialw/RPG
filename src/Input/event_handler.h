#pragma once
//#include "action_components.h"
//#include "ai_control.h"
//#include "components.h"
//#include "death_spells.h"
//#include "events.h"
//#include "game_menu.h"
//#include "hotbar.h"
//#include "input_control.h"
//#include "interface.h"
//#include "items.h"
//#include "menu.h"
//#include "movement.h"
//#include "pause.h"
//#include "sinister_strike.h"
//#include "skills.h"
//#include "spells.h"
//#include "timer.h"
//#include "ui.h"
//#include "unit_control.h"
//#include "utilities.h"
//#include <SDL2/SDL.h>
#include "game_menu.h"
#include "hotbar.h"
#include "hotbar_structs.h"
#include "input_control.h"
#include "interface.h"

namespace Event_Handler {

  /*I will make a component that will be pased to this funtion tree so the functions can do work on the position value of an entity "<velocity> <player_controllable>" */
  const Uint8 *keyboardstate = SDL_GetKeyboardState(NULL);

  std::vector<SDL_Keycode> keys;

  void Movement_Input(entt::registry &zone, Action_Component::Action &action, entt::entity entity, Component::Input &input) {//can return bools for x and y dir, and 2 enums for direction and state
    if (Events::event.key.repeat == 0) {
      if (Events::event.type == SDL_KEYDOWN) {
        auto &velocity = zone.get<Component::Velocity>(entity);
        if (input.keyboardControl.contains(Events::event.key.keysym.sym)) {
          if (!input.keyboardControl[Events::event.key.keysym.sym].pressed) {
            input.keyboardControl[Events::event.key.keysym.sym].pressed = true;
            Player_Control::Update_Keyboard_Movement(zone, entity, velocity, input, Events::event.key.keysym.sym);
            Loot_Panel::Close();
          }
        }
      } else if (Events::event.type == SDL_KEYUP) {
        auto &vel = zone.get<Component::Velocity>(entity);
        if (input.keyboardControl.contains(Events::event.key.keysym.sym)) {
          if (input.keyboardControl[Events::event.key.keysym.sym].pressed) {
            input.keyboardControl[Events::event.key.keysym.sym].pressed = false;
            vel.magnitude.x = 0;
            vel.magnitude.y = 0;
          }
          //recheck to see which keys are pressed
          Player_Control::Check_Pressed_Keys(zone, entity);
        }
      }
    }
  };

  void Interface_Input(entt::registry &zone, Component::Camera &camera, Action_Component::Action &action, entt::entity entity, int &state) {//can return bools for x and y dir, and 2 enums for direction and state
    if (Events::event.key.repeat == 0) {
      if (Events::event.type == SDL_KEYDOWN) {
        auto &position = zone.get<Component::Position>(entity);

        if (Hotbar_Structs::keybinds.contains(Events::event.key.keysym.sym)) {
          Hotbar_Structs::keybinds[Events::event.key.keysym.sym](zone, entity, action, state, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, "");
        }
      } else if (Events::event.type == SDL_KEYUP) {
        if (Hotbar::keyupKeybinds.contains(Events::event.key.keysym.sym)) {
          Hotbar::keyupKeybinds[Events::event.key.keysym.sym](zone, entity, action, state, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, "");
        }
      }
    }
  };

  void Mouse_Hover(entt::registry &zone, int &state) {
    //    std::vector<entt::entity>* Mouse_Hover_Entities;
    //    if (state == int::overworld) {Mouse_Hover_Entities = &World::Mouse_Hover_EntitiesOverworld;}
    //    if (state == int::cave) {Mouse_Hover_Entities = &World::Mouse_Hover_EntitiesCave;}

    for (auto &entity: World::Mouse_Hover_Entities) {
      if (zone.valid(entity)) {
        if (zone.any_of<Rendering_Components::Sprite_Sheet_Info>(entity)) {
          zone.get<Rendering_Components::Sprite_Sheet_Info>(entity).color = {200, 200, 200};
        } else {
          Utilities::Log("Mouse_Hover() entity may not exist");
          continue;
        }
      } else {
        Utilities::Log("Mouse_Hover() entity may not exist");
        continue;
      }
    }
    World::Mouse_Hover_Entities.clear();
    //target unit
    SDL_FRect mouseRect = Utilities::Get_FRect_From_Point_Radius(Mouse::cursorRadius, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
    Quad_Tree::Entity_Data targetData = Quad_Tree::Entity_vs_Mouse_Collision(zone, mouseRect, state);
    //change target SDL_Render slightly for render
    if (zone.any_of<Rendering_Components::Sprite_Sheet_Info>(targetData.entity_ID)) {
      auto &ff = zone.get<Rendering_Components::Sprite_Sheet_Info>(targetData.entity_ID);
      ff.color = {255, 255, 255};
      World::Mouse_Hover_Entities.emplace_back(targetData.entity_ID);
      //recolour pointer
      zone.get<Component::Icon>(Mouse::cursor_ID).pBackground = Graphics::cursor_1;
      return;
    }
    // add component to keep track maybe
    //recolour pointer
    zone.get<Component::Icon>(Mouse::cursor_ID).pBackground = Graphics::cursor_0;
    //revert color
  }

  void Mouse_Input(entt::registry &zone, int &state, entt::entity &player_ID, Component::Position &playerPosition, Component::Camera &camera) {
    if (Events::event.key.type == SDL_MOUSEBUTTONDOWN) {
      if (Events::event.button.button == SDL_BUTTON_LEFT) {
        if (Game_Menu_Control::Check_Menu_Button(zone)) {
          return;
        }
        //        loot
        else if (Loot_Panel::Mouse_Inside_Panel(camera, state)) {
          //activate autocast on spell
          return;
        }
        //        actionbar
        else if (Action_Bar::Mouse_Inside_Actionbar(camera, state)) {
          if (Mouse_Struct::mouseData.type == Component::Icon_Type::spell) {
            Action_Bar::Set_Mouse_Spell_On_Actionbar(zone, state, camera);
          } else if (Mouse_Struct::mouseData.type == Component::Icon_Type::none && !Mouse::itemCurrentlyHeld) {
            Action_Bar::Get_Mouse_Spell_From_Actionbar(zone, state, camera);
          }
          return;
        }
        //      spellbook
        else if (UI_Spellbook::Check_Spellbook(camera)) {
          if (Mouse_Struct::mouseData.type == Component::Icon_Type::spell) {
            Action_Bar::Clear_Spell_On_Mouse(zone);
          }
          if (!UI_Spellbook::Get_Spell(zone, camera)) {
            //check for button on spellbook
          }
          return;
        }
        //        items
        else if (UI::bToggleCharacterUI && Mouse::bRect_inside_Cursor(UI::Character_UI)) {
          if (Mouse_Struct::mouseData.type == Component::Icon_Type::item || Mouse_Struct::mouseData.type == Component::Icon_Type::none) {
            UI::Bag_UI::Interact_With_Bag(zone, player_ID, state, camera);
            if (UI::Equipment_UI::Interact_With_Equipment(zone, state, camera, player_ID)) {
              //updates character stats
              zone.emplace_or_replace<Item_Component::Item_Equip>(player_ID);
            }
          }
        } else {
          User_Mouse_Input::Selection_Box(zone);//if units are currently selected
                                                //          items
          if (Mouse_Struct::mouseData.type == Component::Icon_Type::building) {
            //set building
            if (Create_Entities::Create_Object(zone, state, Mouse::mouseItem)) {
              Create_Entities::Remove_From_Mouse(zone, Mouse::mouseItem);
              return;
            };
          }
          UI::Drop_Item_If_On_Mouse(zone, camera, Mouse::itemCurrentlyHeld);
          //          spells
          Action_Bar::Clear_Spell_On_Mouse(zone);
        }
      } else if (Events::event.button.button == SDL_BUTTON_RIGHT) {
        if (UI::bToggleCharacterUI) {
          if (Mouse_Struct::mouseData.type == Component::Icon_Type::item || Mouse_Struct::mouseData.type == Component::Icon_Type::none) {
            if (Mouse::bRect_inside_Cursor(UI::Character_UI)) {
              if (UI::Swap_Item_In_Bag_For_Equipped(zone, state, Mouse::screenMousePoint, camera, player_ID)) {
                zone.emplace_or_replace<Item_Component::Item_Equip>(player_ID);
                return;
              }
              return;
            }
          }
        } else if (Action_Bar::Mouse_Inside_Actionbar(camera, state)) {
          //activate autocast on spell
          return;
        } else if (UI_Spellbook::Check_Spellbook(camera)) {
          return;
        }
        //        loot
        if (Loot_Panel::Mouse_Inside_Panel(camera, state)) {
          //activate autocast on spell
          Loot_Panel::Get_loot_Item_To_Bag(zone, state, camera);
          return;
        }
        if (!Mouse::itemCurrentlyHeld) {
          if (Mouse_Struct::mouseData.type != Component::Icon_Type::spell) {
            Loot_Panel::Close();
            if (Input_Control::Check_For_Mouse_Target(zone, state, Items::showGroundItems, player_ID, playerPosition)) {
              Mouse::bRight_Mouse_Pressed = false;//otherwise mouse move will override attack move
            } else if (!Mouse::bRight_Mouse_Pressed) {
              User_Mouse_Input::Update_Move_Command_Box();
              // if not selection units
            }
          }
        } else {
          if (Mouse_Struct::mouseData.type == Component::Icon_Type::building) {
            //set building
            if (Create_Entities::Create_Object(zone, state, Mouse::mouseItem)) {
              Create_Entities::Remove_From_Mouse(zone, Mouse::mouseItem);
              return;
            };
          }
          UI::Drop_Item_If_On_Mouse(zone, camera, Mouse::itemCurrentlyHeld);
        }
      }
    }

    if (Events::event.key.type == SDL_MOUSEBUTTONUP) {
      if (Events::event.button.button == SDL_BUTTON_LEFT) {
        if (UI::bToggleCharacterUI && Mouse::bRect_inside_Cursor(UI::Character_UI)) {
          return;
        } else if (UI_Spellbook::Check_Spellbook(camera)) {
          return;
        } else if (Game_Menu_Control::Check_Menu_Frame()) {
          return;
        } else if (Action_Bar::Mouse_Inside_Actionbar(camera, state)) {
          return;
        } else if (Loot_Panel::Mouse_Inside_Panel(camera, state)) {
          Loot_Panel::Get_loot_Item(zone, state, camera);
          return;
        } else {
          User_Mouse_Input::Select_Units(zone, player_ID);
        }
      }
      if (Events::event.button.button == SDL_BUTTON_RIGHT) {
        //        if (!UI::bToggleCharacterUI && !Mouse::bRect_inside_Cursor(UI::Character_UI)) {
        User_Mouse_Input::Command_Unit_Move(zone, player_ID);
        //        }
        Mouse::bRight_Mouse_Pressed = false;
      }
    }
  }

  void Update_User_Input(entt::registry &zone, int &state) {
    //        keep function running to maintain input and perform actions during pause
    if (!Menu::toggleMenu) {
      while (SDL_PollEvent(&Events::event) != 0) {
        //	auto view = zone.view<Component::Velocity, Action_Component::Action, Component::Position, Component::Melee_Range, Component::Input, Component::Camera>();
        auto view = zone.view<Action_Component::Action, Component::Position, Component::Input, Component::Camera>();
        for (auto player_ID: view) {
          if (Events::event.window.event == SDL_WINDOWEVENT_RESIZED) {
            //            recenter camera on player
            UI_Spellbook::Update_Position();
            Action_Bar::Update_Position(Action_Bar::actionBar.actionBar.actionBarFrame);
            Menu::Build_Menu(Menu::menu);
            break;
          }

          auto &playerPosition = view.get<Component::Position>(player_ID);
          auto &playerAction = view.get<Action_Component::Action>(player_ID);
          auto &camera = view.get<Component::Camera>(player_ID);
          auto &input = view.get<Component::Input>(player_ID);

          //          if (Events::event.key.type == SDL_MOUSEMOTION) {
          Mouse_Hover(zone, state);
          if (Events::event.key.type == SDL_MOUSEWHEEL) {
            Interface::Update_Zoom(zone, Events::event);
          } else if (Events::event.key.type == SDL_MOUSEBUTTONDOWN || Events::event.key.type == SDL_MOUSEBUTTONUP) {
            Mouse_Input(zone, state, player_ID, playerPosition, camera);
          } else if (Events::event.key.type == SDL_KEYDOWN || Events::event.key.type == SDL_KEYUP) {
            if (zone.any_of<Component::Velocity>(player_ID)) {
              Movement_Input(zone, playerAction, player_ID, input);
            }
            Interface_Input(zone, camera, playerAction, player_ID, state);
          }
          //if (event.key.type == SDL_JOYAXISMOTION) { // it works!
          //	if (event.jaxis. == 0) {
          //Joystick::JoystickInput(event);
          //std::cout << Joystick::JoystickInput(event) << std::endl;
          //	if (event.jaxis.axis == 0) {
          //	std::cout << event.jaxis.value << std::endl;
          //	}
        }
      }
    }
  };
}// namespace Event_Handler
