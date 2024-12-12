#pragma once

#include "Input/camera.h"
#include "SDL2/SDL.h"
#include "array"
#include "components.h"
#include "entt/entt.hpp"
#include "line_of_sight.h"

namespace Lighting {

    float lightRadiusF = 64.0f;
    Uint8 maxDarkness = 250;
    Uint8 minDarkness = 100;
    SDL_Texture *lightRadiusTexture[35];
    SDL_Texture *texture;
    SDL_Texture *base;
    std::vector<Uint32> fogOfWarPixels;
    bool setFogOfWar = false;

    //value positive for increase, negative for decrease
    void Sight_Update(entt::registry &zone, int value) {
        std::for_each(zone.view<Component::Light_Radius, Component::Input>().begin(), zone.view<Component::Light_Radius, Component::Input>().end(), [&](const auto &player) {
            auto &lightRadius = zone.get<Component::Light_Radius>(player).lightRadius;
            lightRadius += value;
            if (lightRadius >= 35)
                lightRadius = 35;
            if (lightRadius <= 5)
                lightRadius = 5;
        });
    }

    void Create_Texture(std::vector<Uint32> &pixelArray, SDL_Texture *lightRadius) {
        if (!lightRadius) {
            std::cerr << "Invalid texture: texture is null" << std::endl;
            return;
        }

        void *pixels;
        int pitch;
        if (SDL_LockTexture(lightRadius, nullptr, &pixels, &pitch) != 0) {
            std::cerr << "Failed to lock texture: " << SDL_GetError() << std::endl;
            return;
        }

        auto *dst = (Uint32 *) pixels;
        std::copy(pixelArray.begin(), pixelArray.end(), dst);

        SDL_UnlockTexture(lightRadius);
    }

    void Set_BlendMode(SDL_Texture *Ltexture) {
        SDL_BlendMode blendMode = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_MINIMUM,
                SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_MINIMUM
        );
        if (SDL_SetTextureBlendMode(Ltexture, blendMode) != 0) {
            std::cerr << "Failed to set blend mode: " << SDL_GetError() << std::endl;
            return;
        }
    }

    void Create_Light_radius(int lightStr) {
        if (lightRadiusTexture[lightStr])
            return;

        Uint32 color = (0 << 24) | (0 << 16) | (0 << 8) | 0;
        int width = lightStr * (lightRadiusF * 0.75f);
        int height = lightStr * (lightRadiusF * 0.5f);
        std::vector<Uint32> lightRadiusPixels(width * height);

        lightRadiusTexture[lightStr] = SDL_CreateTexture(Graphics::renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        Set_BlendMode(lightRadiusTexture[lightStr]);

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int index = i * width + j;

                Uint8 darkness;
                float dx = (j - (width * 0.5f)) / (lightStr * 0.75f);
                float dy = (i - (height * 0.5f)) / (lightStr * 0.5f);
                float distance = (dx * dx) + (dy * dy);

                if (distance <= 1.0f)
                    lightRadiusPixels[index] = (minDarkness << 24);
                else if (distance > (lightRadiusF * 0.5f) * (lightRadiusF * 0.5f))
                    lightRadiusPixels[index] = (maxDarkness << 24);
                else {
                    darkness = (Uint8) (minDarkness + (sqrt(distance) * (maxDarkness - minDarkness) / (lightRadiusF * 0.5f)));
                    color = darkness << 24;
                    lightRadiusPixels[index] = color;
                }
            }
        }

        std::cout << "Creating texture for light radius: " << lightRadiusPixels.size() << " " << lightStr << std::endl;
        Create_Texture(lightRadiusPixels, lightRadiusTexture[lightStr]);
    }

    void Init(Component::Camera &camera) {
        if (!setFogOfWar) {
            if (base)
                SDL_DestroyTexture(base);

            base = SDL_CreateTexture(Graphics::renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, camera.screen.w, camera.screen.h);

            if (!base) {
                std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
                return;
            }

            Uint32 color = (maxDarkness << 24) | (0 << 16) | (0 << 8) | 0;
            fogOfWarPixels.resize(camera.screen.w * camera.screen.h, color);
            for (auto &pixel: fogOfWarPixels)
                pixel = color;
            fogOfWarPixels.shrink_to_fit();
            setFogOfWar = true;

            Create_Texture(fogOfWarPixels, base);
        }
    }

    void Draw_Light_Radius(float x, float y, float lightStr, SDL_Texture *lightRadius) {
        SDL_SetRenderTarget(Graphics::renderer, texture);
        SDL_FRect rect = {x - ((lightStr * (lightRadiusF * 0.75f)) * 0.5f), y - ((lightStr * (lightRadiusF * 0.5f)) * 0.5f), lightStr * (lightRadiusF * 0.75f), lightStr * (lightRadiusF * 0.5f)};
        SDL_RenderCopyF(Graphics::renderer, lightRadius, nullptr, &rect);
        SDL_SetRenderTarget(Graphics::renderer, nullptr);
    }

    void Display(Component::Camera &camera) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_FRect rect = {0, 0, camera.screen.w, camera.screen.h};
        SDL_RenderCopyF(Graphics::renderer, texture, nullptr, &rect);
    }

    void Get_Emitters(entt::registry &zone, Component::Camera &camera) {
        if (texture)
            SDL_DestroyTexture(texture);

        texture = SDL_CreateTexture(Graphics::renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, camera.screen.w, camera.screen.h);
        if (!texture) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            return;
        }
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(Graphics::renderer, texture);
        SDL_RenderCopyF(Graphics::renderer, base, nullptr, nullptr);
        SDL_SetRenderTarget(Graphics::renderer, nullptr);

        Set_BlendMode(texture);

        auto view = zone.view<Component::Position, Component::Light_Radius, Component::Renderable>();
        for (auto entity: view) {
            auto light = view.get<Component::Light_Radius>(entity);
            auto position = view.get<Component::Position>(entity);
            auto screenPosition = Camera_Control::Convert_Position_To_Screen_Coods(camera, position);
            Create_Light_radius(light.lightRadius);
            Draw_Light_Radius(screenPosition.x, screenPosition.y, light.lightRadius, lightRadiusTexture[light.lightRadius]);
        }
    }

    void Render(entt::registry &zone, Component::Camera &camera) {
        Init(camera);
        Get_Emitters(zone, camera);
        Line_Of_Sight::Line_Of_Sight(zone, camera, lightRadiusF, texture);
        Display(camera);
    }


}// namespace Fog_Of_War