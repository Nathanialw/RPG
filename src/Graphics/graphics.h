#pragma once

#include "Maps/World/world.h"
#include "SDL_FontCache/SDL_FontCache.h"
#include "components.h"
#include "entt/entt.hpp"
#include "rendering_components.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace Graphics_Component {

}

namespace Graphics {
    bool running;
    SDL_FRect resolution = {0.0f, 0.0f, 1920.0f, 1200.0f};
    const char *title = "RPG";
    SDL_Window *window;
    SDL_FRect Screen;
    f2 Scale;
    TTF_Font *font;
    FC_Font *fcFont = FC_CreateFont();
    SDL_Texture *default_icon;


    std::vector<entt::entity> defaultIcon(World_Data::numZones);

    struct Surface_Data {
        SDL_Texture *pTexture;
        SDL_Rect k;
    };

    struct Text_Box_Data {
        SDL_Rect textBoxBackground;
        SDL_FRect highlightBox;
        Surface_Data textdata;
    };

    SDL_Renderer *renderer;

    const int numberOfTextures = 200;
    int texture_ID = 0;
    std::array<SDL_Texture *, numberOfTextures> unitTextures;
    std::array<SDL_Texture *, numberOfTextures> unitPortaits;
    std::array<SDL_Texture *, numberOfTextures> unitBodies;


    struct Texture {
        SDL_Texture *texture = NULL;
        SDL_Texture *portrait = NULL;
        SDL_Texture *body = NULL;
    };

    //where the actual texture is stored
    std::map<std::string, SDL_Texture *> pTexture;

    std::array<SDL_Texture *, numberOfTextures> spellTextures;
    std::array<SDL_Texture *, numberOfTextures> environmentTextures;
    std::array<SDL_Texture *, numberOfTextures> UITextures;
    //    std::map<std::string, SDL_Texture*> itemTextures;



    void Render_Rect(SDL_Texture *pTexture, SDL_Rect &clipSprite, SDL_Rect &scaledSlot) {
        SDL_RenderCopy(renderer, pTexture, &clipSprite, &scaledSlot);
    }

    void Render_FRectToScreen(SDL_Texture *texture, const SDL_Rect *sourceRect, SDL_FRect *targetRect) {
        SDL_RenderCopyF(renderer, texture, sourceRect, targetRect);
    }

    void Render_FRect(SDL_Texture *texture, SDL_Color color, const SDL_Rect *sourceRect, SDL_FRect *targetRect) {
        SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
        SDL_RenderCopyF(renderer, texture, sourceRect, targetRect);
    }

    void Render_FRect(SDL_Texture *texture, const SDL_Rect *sourceRect, SDL_FRect *targetRect) {
        SDL_RenderCopyF(renderer, texture, sourceRect, targetRect);
    }

    void Render_FRect(SDL_Texture *texture, SDL_FRect *targetRect) {
        SDL_RenderCopyF(renderer, texture, nullptr, targetRect);
    }

    void Render_FRect(SDL_FRect targetRect) {
        SDL_RenderDrawRectF(renderer, &targetRect);
    }

    Surface_Data Load_Text_Texture(std::string text, SDL_Color fontColor) {
        SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), fontColor);//convert font to Surface
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);      //convert Surface to texture

        SDL_Rect k = surface->clip_rect;

        Surface_Data text_data = {texture, k};
        SDL_FreeSurface(surface);//free surface memory
        return text_data;        //return SDL_Texture *texture
    };

    Text_Box_Data Create_Text_Background(Component::Camera &camera, SDL_Color textColor, std::string &text, Component::Position &position) {
        //        float yPosition = position.y - 20.0f;
        SDL_FRect textBox = {};
        textBox.w = text.length() * 5.0f;
        textBox.h = 10.0f;

        textBox.x = position.x - (textBox.w / 2.0f);
        textBox.y = position.y - 10.0f;

        textBox.x -= camera.screen.x;
        textBox.y -= camera.screen.y;
        SDL_Rect textBoxBackground = Utilities::SDL_FRect_To_SDL_Rect(textBox);
        textBoxBackground.x -= 5;
        textBoxBackground.w += 10;

        Graphics::Surface_Data itemTextBox = Graphics::Load_Text_Texture(text, textColor);

        Text_Box_Data textBoxData =
                {
                        textBoxBackground,
                        textBox,
                        itemTextBox};
        return textBoxData;
    }

    void Create_Font() {
        FC_LoadFont(fcFont, Graphics::renderer, "assets/fonts/dum1/Dumbledor_Nathan.ttf", 20, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);
        font = TTF_OpenFont("assets/fonts/dum1/Dumbledor_Nathan.ttf", 30);
    }

    SDL_Texture *createTexture(const char *filepath) {
        SDL_Surface *surface = IMG_Load(filepath);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        return texture;
    }

    entt::entity Create_Icon_Entity(entt::registry &zone, SDL_Texture *iconImage, SDL_Texture *iconBorder, Component::Icon_Type type) {
        auto icon_entity = zone.create();
        auto &icon = zone.emplace_or_replace<Component::Icon>(icon_entity, iconImage, iconBorder);
        icon.clipSprite = {0, 0, 100, 100};
        icon.renderRectSize = {48, 48};
        icon.renderPositionOffset = {icon.renderRectSize.x / 2, icon.renderRectSize.y / 2};
        return icon_entity;
    }

    //both unitID and filepath are stored in tiled map object
    SDL_Texture *Load_Texture(int &unitID, std::array<SDL_Texture *, numberOfTextures> &textures, const char *filepath) {
        textures[unitID] = createTexture(filepath);
        if (textures[unitID] == nullptr) {
            unitTextures[unitID] = Graphics::default_icon;
        }
        return textures[unitID];
    }

    SDL_Texture* Create_Alpha_Texture() {
        SDL_Texture* textCanvas = SDL_CreateTexture(Graphics::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 2, 2);

        Uint32 color = (0 << 24) | (0 << 16) | (0 << 8) | 0;
        std::vector<Uint32> canvasPixels(2 * 2, color);

        if (!textCanvas ) {
            std::cerr << "Invalid texture: texture is null" << std::endl;
            return nullptr;
        }

        void *pixels;
        int pitch;
        if (SDL_LockTexture(textCanvas , nullptr, &pixels, &pitch) != 0) {
            std::cerr << "Failed to lock texture: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        auto *dst = (Uint32 *) pixels;
        std::copy(canvasPixels.begin(), canvasPixels.end(), dst);

        SDL_UnlockTexture(textCanvas);

        return textCanvas;
        //creating a texture with full alpha
    }

    SDL_Texture* Create_Canvas(int w, int h) {
        SDL_Texture* textCanvas = SDL_CreateTexture(Graphics::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
        SDL_Texture* alpha = Create_Alpha_Texture();
        SDL_SetRenderTarget(Graphics::renderer, textCanvas);
        SDL_RenderCopyF(Graphics::renderer, alpha, nullptr, nullptr);
        SDL_DestroyTexture(alpha);
        SDL_SetRenderTarget(Graphics::renderer, nullptr);
        SDL_SetTextureBlendMode(textCanvas, SDL_BLENDMODE_BLEND);
        return textCanvas;
        //creating a texture with full alpha
    }


    //when creating the game objet
    SDL_Texture *Create_Game_Object(int &unitID, const char *filepath) {
        SDL_Texture *texture;
        if (unitTextures[unitID] == nullptr) {
            Load_Texture(unitID, unitTextures, filepath);
            texture = unitTextures[unitID];
            if (texture == nullptr) {
                std::cout << "Create_Game_Object() failed to load  texture from file: " << filepath << std::endl;
                return texture;
            } else {
                texture = unitTextures[unitID];
                return texture;
                //	std::cout << "loaded from file: " << filepath << std::endl;
            }
        } else {
            texture = unitTextures[unitID];
            return texture;
            //std::cout << "already loaded: " << filepath << std::endl;
            //unitTextures[unitID];
        }
        return texture;
        //to render it jsut needs access to the texture array and the unitID
    }

    SDL_Texture *Load_Portrait(int &unitID, const char *filepath) {
        SDL_Texture *texture;
        if (unitPortaits[unitID] == nullptr) {
            texture = Load_Texture(unitID, unitPortaits, filepath);
            if (texture == nullptr) {
                //        std::cout << "Load_Portrait() failed to load  texture from file: " << filepath << std::endl;
                return texture;
            } else {
                texture = unitPortaits[unitID];
                return texture;
                //	std::cout << "loaded from file: " << filepath << std::endl;
            }
        } else {
            texture = unitPortaits[unitID];
            return texture;
            //std::cout << "already loaded: " << filepath << std::endl;
            //unitTextures[unitID];
        }
        return texture;
        //to render it jsut needs access to the texture array and the unitID
    }

    SDL_Texture *Load_Body(int &unitID, const char *filepath) {
        SDL_Texture *texture;
        if (unitBodies[unitID] == nullptr) {
            texture = Load_Texture(unitID, unitBodies, filepath);
            if (texture == nullptr) {
                //        std::cout << "Load_Body() failed to load  texture from file: " << filepath << std::endl;
                return texture;
            } else {
                texture = unitBodies[unitID];
                return texture;
                //	std::cout << "loaded from file: " << filepath << std::endl;
            }
        } else {
            texture = unitBodies[unitID];
            return texture;
            //std::cout << "already loaded: " << filepath << std::endl;
            //unitTextures[unitID];
        }
        return texture;
        //to render it jsut needs access to the texture array and the unitID
    }

    //when creating the game objet
    void Create_Tileset(std::string &name, const char *filepath) {
        if (pTexture[name] == nullptr) {
            pTexture[name] = createTexture(filepath);
            if (pTexture[name] == nullptr) {
                std::cout << "Create_Tileset() failed to load  texture from file: " << filepath << std::endl;
            } else {
                //	std::cout << "loaded from file: " << filepath << std::endl;
            }
        } else {
            //std::cout << "already loaded: " << filepath << std::endl;
            //unitTextures[unitID];
        }
        //to render it jsut needs access to the texture array and the unitID
    }

    enum BlendMode {
	MIN = 0,
	SUBTRACT = 1,
	ADD = 2,
	MAX = 3,
	REVSUBTRACT = 4
    };

    void Set_BlendMode(SDL_Texture *Ltexture, BlendMode blendMode = BlendMode::MIN) {
	SDL_BlendMode LblendMode;

	if (blendMode == BlendMode::SUBTRACT) {
	    LblendMode = SDL_ComposeCustomBlendMode(
		    SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_SUBTRACT,
		    SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_SUBTRACT
	    );
	}
	if (blendMode == BlendMode::REVSUBTRACT) {
	    LblendMode = SDL_ComposeCustomBlendMode(
		    SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_REV_SUBTRACT,
		    SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_REV_SUBTRACT
	    );
	}
	if (blendMode == BlendMode::ADD) {
	    LblendMode = SDL_ComposeCustomBlendMode(
		    SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD,
		    SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD
	    );
	}
	if (blendMode == BlendMode::MIN) {
	    LblendMode = SDL_ComposeCustomBlendMode(
		    SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_MINIMUM,
		    SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_MINIMUM
	    );
	}
	if (blendMode == BlendMode::MAX) {
	    LblendMode = SDL_ComposeCustomBlendMode(
		    SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_MAXIMUM,
		    SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_MAXIMUM
	    );
	}

	if (SDL_SetTextureBlendMode(Ltexture, LblendMode) != 0) {
	    std::cerr << "Failed to set blend mode: " << SDL_GetError() << std::endl;
	    return;
	}
    }

    void createGraphicsContext() {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resolution.w, resolution.h, SDL_WINDOW_FULLSCREEN_DESKTOP);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
        //locks to 60 fps
        //    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
        SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
        default_icon = createTexture("assets/sprites/default.jpg");
    }

    void closeContext() {
        running = false;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}// namespace Graphics
