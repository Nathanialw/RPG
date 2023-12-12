#include <SDL2/SDL.h>
#include "init.h"
#include "game_loop.h"
#include "close.h"

int main(int argc, char* argv[]) {

  srand((unsigned int)time(NULL));

  if (Init::Init_Client()) {
    Game_Loop::Game_Loop();
  };
  Close_Game::Close_Game();

  return 0;
}
