#include "close.h"
#include "game_loop.h"
#include "init.h"
#include "SDL2/SDL.h"

int main(int argc, char* argv[]) {
    srand((unsigned int) time(NULL));

    Init::Loaded loaded;

    Init::Init_Client();
    while (loaded.instance != 0) {
        if (!World::world[loaded.instance].loaded) {
            Init::Start_Game(World::world[loaded.instance].zone, World::world[loaded.instance].tilesetName, loaded.instance);
        } else {
            Init::Reload_Zone(World::world[loaded.instance].zone, loaded.instance);
        }
        // Clear and reset data
        SDL_RenderSetScale(Graphics::renderer, 1.0f, 1.0f);
        World::Mouse_Hover_Entities.clear();

        if (Game_Loop::Game_Running(World::world[loaded.instance].zone, loaded.instance) == 0) {
            break;
        }
        if (loaded.instance == 1) {
            loaded.instance = 2;
            Init::Restart_Game();
        }
    }
    Close_Game::Close_Game();

    return 0;
}