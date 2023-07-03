#pragma once
#include "entt/entt.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "camera.h"
#include "components.h"
#include "../SDL_FontCache.h"
#include "rendering_components.h"

namespace Graphics_Component {

}

namespace Graphics {

        bool running;
        SDL_FRect resolution = { 0.0f, 0.0f, 1920.0f, 1200.0f };
        const char* title = "RPG";
        SDL_Window* window;
        SDL_FRect Screen;
        TTF_Font* font;
        FC_Font* fcFont = FC_CreateFont();

        entt::entity defaultIcon;

        struct Surface_Data
        {
            SDL_Texture* pTexture;
            SDL_Rect k;
        };

        struct Text_Box_Data
        {
            SDL_Rect textBoxBackground;
            SDL_FRect highlightBox;
            Surface_Data textdata;
        };

    SDL_Renderer* renderer;

    const int numberOfTextures = 100;
    int texture_ID = 0;
    std::array<SDL_Texture*, numberOfTextures> unitTextures;

    //where the actual texture is stored
    std::map<std::string, SDL_Texture*>pTexture;

    std::array<SDL_Texture*, numberOfTextures> spellTextures;
    std::array<SDL_Texture*, numberOfTextures> environmentTextures;
    std::array<SDL_Texture*, numberOfTextures> UITextures;
//    std::map<std::string, SDL_Texture*> itemTextures;

    SDL_Texture* fireball_explosion_0;
    SDL_Texture* itsmars_Inventory;
    SDL_Texture* cursor_0;
    SDL_Texture* weapons_icons;
    SDL_Texture* default_icon;
    SDL_Texture* emptyBagIcon;

    SDL_Texture* bagSlotBorder;
    SDL_Texture* itemBorderCommon;
    SDL_Texture* itemBorderMagic;
    SDL_Texture* itemBorderRare;
    SDL_Texture* itemBorderEite;
    SDL_Texture* armorSpriteSheet;
    SDL_Texture* spellbook;

    SDL_Texture* tooltipBackground;


    void Render_Rect(SDL_Texture* pTexture, SDL_Rect& clipSprite, SDL_Rect& scaledSlot)
    {
        SDL_RenderCopy(renderer, pTexture, &clipSprite, &scaledSlot);
    }

    void Render_FRectToScreen(SDL_Texture* texture, const SDL_Rect* sourceRect, SDL_FRect* targetRect)
    {
        SDL_RenderCopyF(renderer, texture, sourceRect, targetRect);
    }

    Rendering_Components::Color Set_Random_Color()
    {
        Rendering_Components::Color color;
        color.r = rand() % 254 + 1;
        color.g = rand() % 254 + 1;
        color.b = rand() % 254 + 1;
        return color;
    }


    void Render_FRect(SDL_Texture* texture, Rendering_Components::Color color, const SDL_Rect* sourceRect, SDL_FRect* targetRect)
    {
        SDL_SetTextureColorMod( texture, color.r, color.g, color.b );
        SDL_RenderCopyF(renderer, texture, sourceRect, targetRect);
    }

    Surface_Data Load_Text_Texture(std::string text, SDL_Color fontColor)
    {
        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), fontColor);   //convert font to Surface
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);	        //convert Surface to texture

        SDL_Rect k = surface->clip_rect;


        Surface_Data text_data = { texture, k };
        SDL_FreeSurface(surface);														//free surface memory
        return text_data;																//return SDL_Texture *texture
    };

    Text_Box_Data Create_Text_Background (Component::Camera &camera, SDL_Color textColor, std::string &text, Component::Position &position)
    {
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
            itemTextBox
        };
        return textBoxData;
    }

    void Create_Font()
    {
        FC_LoadFont(fcFont, Graphics::renderer, "assets/fonts/FreeSans.ttf", 20, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);
        font = TTF_OpenFont("assets/fonts/Chomsky.otf", 30);
    }

    SDL_Texture* createTexture(const char* spritesheet)
    {
        SDL_Surface* surface = IMG_Load(spritesheet);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        return texture;
    }

    entt::entity Create_Icon_Entity(entt::registry& zone, SDL_Texture *iconImage, SDL_Texture *iconBorder)
    {
        auto icon_entity = zone.create();
        auto& icon = zone.emplace<Component::Icon>(icon_entity, iconImage, iconBorder);
        icon.clipSprite = { 0, 0, 100, 100 };
        icon.renderRectSize = { 48, 48 };
        icon.renderPositionOffset = { icon.renderRectSize.x / 2, icon.renderRectSize.y / 2 };
        return icon_entity;
    }


    //both unitID and filepath are stored in tiled map object
    void Load_Texture(int &unitID, const char* filepath)
    {
        unitTextures[unitID] = createTexture(filepath);
    }

    //when creating the game objet
    void Create_Game_Object(int& unitID, const char* filepath)
    {
        if (unitTextures[unitID] == NULL)
        {
            Load_Texture(unitID, filepath);
            if (unitTextures[unitID] == NULL)
            {
                std::cout << "Create_Game_Object() failed to load  texture from file: " << filepath << std::endl;
            }
            else
            {
                //	std::cout << "loaded from file: " << filepath << std::endl;
            }
        }
        else
        {
            //std::cout << "already loaded: " << filepath << std::endl;
            //unitTextures[unitID];
        }
        //to render it jsut needs access to the texture array and the unitID
    }



    void Load_Textures(entt::registry& zone)
    {
        Create_Font();
        fireball_explosion_0 = createTexture("assets/sprites/spells/fireball_explosion_0.png");

        weapons_icons = createTexture("assets/sprites/items/weaponIcons32x32_png_Transparent/weapon_icons_32_transparent.png");
        itsmars_Inventory = createTexture("assets/sprites/UI/itsmars_Inventory.png");
        cursor_0 = createTexture("assets/sprites/UI/cursors/cursor.png");

        emptyBagIcon = createTexture("assets/sprites/UI/icon_borders/bagsSot.png"); // currently NULL
        bagSlotBorder = createTexture("assets/sprites/UI/icon_borders/32x32_BagSlotBorder.png");

        itemBorderCommon = createTexture("assets/sprites/UI/icon_borders/frame-0-grey.png");
        itemBorderMagic = createTexture("assets/sprites/UI/icon_borders/frame-0-blue.png");
        itemBorderRare = createTexture("assets/sprites/UI/icon_borders/frame-0-orange.png");
        itemBorderEite = createTexture("assets/sprites/UI/icon_borders/frame-0-red.png");

        armorSpriteSheet = createTexture("assets/sprites/items/flare_armor_transparent.png");

        tooltipBackground = createTexture("assets/sprites/UI/tooltips/tooltipBackground2.png");
        spellbook = createTexture("assets/sprites/UI/spellbook/spellbook.png");

        default_icon = createTexture("assets/sprites/default.jpg");
        defaultIcon = Create_Icon_Entity(zone, default_icon, Graphics::bagSlotBorder);
    }

    void createGraphicsContext(entt::registry& zone)
    {
        window = SDL_CreateWindow(title, resolution.x, resolution.y, resolution.w, resolution.h, SDL_WINDOW_RESIZABLE);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
        Load_Textures(zone);
    }

    void closeContext()
    {
        running = false;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}