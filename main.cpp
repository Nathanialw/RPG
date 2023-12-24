
#include "close.h"
#include "game_loop.h"
#include "init.h"
#include <SDL2/SDL.h>


int main(int argc, char *argv[]) {

  srand((unsigned int) time(NULL));

  Init::Loaded loaded;

  Init::Init_Client();
  while (World::gamestate != World::GameState::exit) {
    if (World::gamestate == World::GameState::overworld || World::gamestate == World::GameState::restart) {
      //if overworld has not been created
      if (!loaded.overworld) {
        Init::Start_Game(World::zone, World::gamestate, loaded);
      }
      else {
        Init::Reload_Zone(World::zone, World::gamestate);
      }
    }
    if (World::gamestate == World::GameState::cave) {
      //if cave has not been created
      if (!loaded.cave) {
        Init::Start_Game(World::cave, World::gamestate, loaded);
      }
      else {
        Init::Reload_Zone(World::cave, World::gamestate);
      }
    }
    //clear and reset data
    if (World::gamestate == World::GameState::restart) {
      World::gamestate = World::GameState::overworld;
    }
    SDL_RenderSetScale(Graphics::renderer, 1.0f, 1.0f);
    World::Mouse_Hover_Entities.clear();

    if (World::gamestate == World::GameState::overworld) {
      if (Game_Loop::Game_Running(World::zone) == World::GameState::exit) {
          break;
        }
    }
    else if (World::gamestate == World::GameState::cave) {
      if (Game_Loop::Game_Running(World::cave) == World::GameState::exit) {
        break;
      }
    }

    if (World::gamestate == World::GameState::restart) {
      Init::Restart_Game();
    }
  };
  Close_Game::Close_Game();

  return 0;
}
