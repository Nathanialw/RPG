


namespace Fog {



    SDL_FRect fog = {0.0f, 0.0f, 1507.0f, 353.0f};

    void Fog(Component::Camera &camera) {
        SDL_FRect rect;
        rect.w = fog.w;
        rect.h = fog.h;

        fog.x += (0.05f * Timer::timeStep);
        rect.x = fog.x;
        fog.y += (0.05f * Timer::timeStep);
        rect.y = fog.y;

        if (rect.x >= camera.screen.w) {
            fog.x -= camera.screen.w;
        }

        if (rect.y >= camera.screen.h) {
            fog.y -= camera.screen.h;
        }

        //    int i = 0;
        SDL_FRect renderRect = Camera_Control::Convert_Rect_To_Screen_Coods(rect, camera);
        SDL_FRect rect2 = Camera_Control::Convert_Rect_To_Screen_Coods(rect, camera);

        while (renderRect.x < camera.screen.w) {
            while (renderRect.y < camera.screen.h) {
                if (renderRect.x > -camera.screen.w && renderRect.y > -camera.screen.h) {
                    SDL_RenderCopyF(Graphics::renderer, Graphics::fog, nullptr, &renderRect);
                    //          i++;
                }
                renderRect.y += renderRect.h;
            }
            renderRect.x += renderRect.w;
            renderRect.y = rect2.y;
        }
        //    Utilities::Log(i);
    }

    void Update_Fog(Component::Camera &camera) {
        if (1) {
//      Fog(camera);
        }
    }
}// namespace Fog