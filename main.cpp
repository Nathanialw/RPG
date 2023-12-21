
#include "close.h"
#include "game_loop.h"
#include "init.h"
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {

  srand((unsigned int) time(NULL));

  Init::Init_Client();
  while (Game_Loop::gamestate != Game_Loop::GameState::exit) {
    Init::Start_Game();
    //clear and reset data
    Init::Game game;
    if (Game_Loop::Game_Loop(game) == Game_Loop::GameState::exit) {
      break;
    }
    Init::Restart_Game();
  };
  Close_Game::Close_Game();

  return 0;
}
