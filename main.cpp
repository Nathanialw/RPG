#include <SDL2/SDL.h>
#include "close.h"
#include "init.h"
#include "game_loop.h"

int main(int argc, char* argv[]) {

    srand((unsigned int)time(NULL));

	Init::init();
    Game_Loop::Game_Loop();
    Close_Game::Close_Game();

	return 0;
}
