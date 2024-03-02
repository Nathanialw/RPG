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
  TTF_Font *font;
  FC_Font *fcFont = FC_CreateFont();

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

  SDL_Texture *fireball_explosion_0;
  SDL_Texture *fireball_icon;
  SDL_Texture *death1;
  SDL_Texture *death2;
  SDL_Texture *death3;
  SDL_Texture *death4;
  SDL_Texture *death5;
  SDL_Texture *death6;
  SDL_Texture *death7;
  SDL_Texture *death8;
  SDL_Texture *healself;
  SDL_Texture *healother;
  SDL_Texture *sinisterstrike;
  SDL_Texture *lightningstrike;
  SDL_Texture *lightningball;
  SDL_Texture *chargedbolt;
  SDL_Texture *leafspell;
  SDL_Texture *swirlingsmoke;
  SDL_Texture *icebolt;
  SDL_Texture *fire_1b_40;
  SDL_Texture *warp_effect;
  SDL_Texture *barrier000;
  SDL_Texture *BBLBubbleEllipseYellow;
  SDL_Texture *magic_flame;
  SDL_Texture *Snowflake1;
  SDL_Texture *Snowflake2;

  SDL_Texture *arrow;
  SDL_Texture *bow;
  SDL_Texture *arrowsFlying;
  SDL_Texture *redArrow;
  SDL_Texture *blueArrow;
  SDL_Texture *greenArrow;

  SDL_Texture *rainofFire;
  SDL_Texture *firebolt;
  SDL_Texture *thunderstorm;
  SDL_Texture *blizzard;
  SDL_Texture *summonGhost;
  SDL_Texture *summonDemon;
  SDL_Texture *reanimate;
  SDL_Texture *raiseZombie;
  SDL_Texture *raiseSkeleton;

  SDL_Texture *itsmars_Inventory;
  SDL_Texture *cursor_0;
  SDL_Texture *cursor_1;
  SDL_Texture *selector32;
  SDL_Texture *weapons_icons;
  SDL_Texture *default_icon;
  SDL_Texture *emptyBagIcon;

  SDL_Texture *charCreateBackground;
  SDL_Texture *window_glow;
  SDL_Texture *fog;

  SDL_Texture *icons;
  SDL_Texture *vikingBuildingIcons;
  SDL_Texture *medievelBuildingIcons;

  SDL_Texture *bagSlotBorder;
  SDL_Texture *itemBorderCommon;
  SDL_Texture *itemBorderMagic;
  SDL_Texture *itemBorderRare;
  SDL_Texture *itemBorderEite;
  SDL_Texture *armorSpriteSheet;
  SDL_Texture *spellbook;

  SDL_Texture *tooltipBackground;
  SDL_Texture *checkmark;
  SDL_Texture *fogOfWar;

  void Render_Rect(SDL_Texture *pTexture, SDL_Rect &clipSprite, SDL_Rect &scaledSlot) {
    SDL_RenderCopy(renderer, pTexture, &clipSprite, &scaledSlot);
  }

  void Render_FRectToScreen(SDL_Texture *texture, const SDL_Rect *sourceRect, SDL_FRect *targetRect) {
    SDL_RenderCopyF(renderer, texture, sourceRect, targetRect);
  }

  static SDL_Color Set_Random_Color() {
    SDL_Color color;
    color.r = rand() % 254 + 1;
    color.g = rand() % 254 + 1;
    color.b = rand() % 254 + 1;
    return color;
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

  static void Load_Textures() {
    Create_Font();
    fireball_explosion_0 = createTexture("assets/sprites/spells/fire/explosions/fireball_explosion_0.png");
    fireball_icon = createTexture("assets/sprites/UI/icons/spell_icons/fire/Fantasy Game Skills Icon 1 - Magic - Fire Ball level 1.jpg");
    death1 = createTexture("assets/sprites/UI/icons/Dark Magic/01.png");
    death2 = createTexture("assets/sprites/UI/icons/Dark Magic/02.png");
    death3 = createTexture("assets/sprites/UI/icons/Dark Magic/03.png");
    death4 = createTexture("assets/sprites/UI/icons/Dark Magic/04.png");
    death5 = createTexture("assets/sprites/UI/icons/Dark Magic/05.png");
    death6 = createTexture("assets/sprites/UI/icons/Dark Magic/06.png");
    death7 = createTexture("assets/sprites/UI/icons/Dark Magic/07.png");
    death8 = createTexture("assets/sprites/UI/icons/Dark Magic/08.png");

    sinisterstrike = createTexture("assets/sprites/UI/icons/misc_icons/old Ancient Beast icons/blood hack.jpg");
    healself = createTexture("assets/sprites/UI/icons/spells/leaf-jade-2.png");
    healother = createTexture("assets/sprites/UI/icons/spells/leaf-orange-2.png");
    lightningstrike = createTexture("assets/sprites/spells/lightning/fx1_blue_topEffect/spell_bluetop_1_1.png");
    lightningball = createTexture("assets/sprites/spells/lightning/fx8_lighteningBall/lighteningball_1_20_1.png");
    chargedbolt = createTexture("assets/sprites/spells/lightning/chargedbolt/chargedbolt-0.png");
    leafspell = createTexture("assets/sprites/spells/nature/leafspell/0026.png");
    swirlingsmoke = createTexture("assets/sprites/spells/air/swirlingsmoke/0000.png");
    icebolt = createTexture("assets/sprites/UI/icons/spell_icons/ice/Fantasy Game Skills Icon 1 - Magic - Ice Lance level 1.jpg");
    fire_1b_40 = createTexture("assets/sprites/spells/fire/fire/fire_1b_40_1.png");
    warp_effect = createTexture("assets/sprites/spells/arcane/warp_effect-sheet/warp_effect-sheet-3.png");
    barrier000 = createTexture("assets/sprites/spells/arcane/warp_effect-sheet-4.png");
    BBLBubbleEllipseYellow = createTexture("assets/sprites/spells/holy/BBLBubbleEllipseYellow.png");
    magic_flame = createTexture("assets/sprites/spells/holy/BBLBubbleEllipseYellow.png");
    Snowflake1 = createTexture("assets/sprites/spells/frost/hjm_frost_bolt/frames/hjm_frost_bolt01.png");
    Snowflake2 = createTexture("assets/sprites/spells/frost/hjm-snow_flake/frames/hjm-snow_flake_003.png");

    arrow = createTexture("assets/sprites/spells/archery/arrow.png");

    bow = createTexture("assets/sprites/UI/icons/misc_icons/Icons/10.png");
    arrowsFlying = createTexture("assets/sprites/UI/icons/misc_icons/Icons/85.png");
    redArrow = createTexture("assets/sprites/UI/icons/misc_icons/Icons/95.png");
    blueArrow = createTexture("assets/sprites/UI/icons/misc_icons/Icons/96.png");
    greenArrow = createTexture("assets/sprites/UI/icons/misc_icons/Icons/97.png");

    rainofFire = createTexture("assets/sprites/UI/icons/misc_icons/modif/firestream.png");
    thunderstorm = createTexture("assets/sprites/UI/icons/misc_icons/attack_icons/thunder-1.png");
    blizzard = createTexture("assets/sprites/UI/icons/misc_icons/attack_icons/blizzard-3.png");
    summonGhost = createTexture("assets/sprites/UI/icons/misc_icons/painterly-spell-icons-4/wind-grasp-eerie-3.png");
    summonDemon = createTexture("assets/sprites/UI/icons/misc_icons/painterly-spell-icons-3/horror-red-3.png");
    reanimate = createTexture("assets/sprites/UI/icons/misc_icons/painterly-spell-icons-3/horror-red-1.png");
    raiseZombie = createTexture("assets/sprites/UI/icons/misc_icons/old Ancient Beast icons/monster.png");
    raiseSkeleton = createTexture("assets/sprites/UI/icons/Dark Magic/08.png");
    firebolt = createTexture("assets/sprites/UI/icons/spell_icons/fire/Fantasy Game Skills Icon 1 - Magic - Fire Ball level 3.jpg");

    charCreateBackground = createTexture("assets/sprites/UI/backgrounds/Background_47.png");
    window_glow = createTexture("assets/sprites/UI/window_glow.png");
    fog = createTexture("assets/sprites/UI/mist/fog_0.png");

    icons = createTexture("assets/sprites/icons/icons.png");
    vikingBuildingIcons = createTexture("assets/sprites/buildings/viking/icons.png");
    medievelBuildingIcons = createTexture("assets/sprites/buildings/medieval/buildings.png");

    weapons_icons = createTexture("assets/sprites/items/weaponIcons32x32_png_Transparent/weapon_icons_32_transparent.png");
    itsmars_Inventory = createTexture("assets/sprites/UI/itsmars_Inventory.png");
    cursor_0 = createTexture("assets/sprites/UI/cursors/cursor.png");
    //    cursor_1 = createTexture("assets/sprites/UI/cursors/cursor_outline_blue.png");
    //    cursor_1 = createTexture("assets/sprites/UI/cursors/cursor_outline_red.png");
    cursor_1 = createTexture("assets/sprites/UI/cursors/cursor_shiny.png");
    selector32 = createTexture("assets/sprites/UI/selector/selector32.png");

    emptyBagIcon = createTexture("assets/sprites/UI/tooltips/tooltipBackground2.png");// currently NULL
    bagSlotBorder = createTexture("assets/sprites/UI/icon_borders/frame-0-grey.png");

    itemBorderCommon = createTexture("assets/sprites/UI/icon_borders/frame-0-grey.png");
    itemBorderMagic = createTexture("assets/sprites/UI/icon_borders/frame-0-blue.png");
    itemBorderRare = createTexture("assets/sprites/UI/icon_borders/frame-0-orange.png");
    itemBorderEite = createTexture("assets/sprites/UI/icon_borders/frame-0-red.png");

    armorSpriteSheet = createTexture("assets/sprites/items/flare_armor_transparent.png");

    //    fogOfWar = createTexture("assets/sprites/fog_of_war/blackFog.png");
    fogOfWar = createTexture("assets/sprites/UI/tooltips/fogOfWar.png");
    tooltipBackground = createTexture("assets/sprites/UI/tooltips/tooltipBackground2.png");
    checkmark = createTexture("assets/sprites/UI/Inventory/29.png");
    spellbook = createTexture("assets/sprites/UI/spellbook/spellbook.png");

    default_icon = createTexture("assets/sprites/default.jpg");
  }

  void createGraphicsContext() {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resolution.w, resolution.h, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    //locks to 60 fps
    //    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
  }

  void closeContext() {
    running = false;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
}// namespace Graphics
