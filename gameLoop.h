#pragma once
#include "graphics.h"

using namespace Graphics;


namespace GameLoop {

	void gameLoop() {
		while (1) {
			Graphics::createTexture();
		}
	}

}