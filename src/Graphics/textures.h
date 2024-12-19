//
// Created by desktop on 12/13/24.
//
#pragma once
#include "components.h"
#include "graphics.h"

namespace Texture {

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
    SDL_Texture *cox_icons;
    SDL_Texture *cox_skills;
    SDL_Texture *cox_spells;
    SDL_Texture *cox_map_tiles;
    SDL_Texture *cox_monsters;
    SDL_Texture *cox_objects;
    SDL_Texture *cox_traps;
    SDL_Texture *vikingBuildingIcons;
    SDL_Texture *medievelBuildingIcons;

    SDL_Texture *bagSlotBorder;
    SDL_Texture *itemBorderCommon;
    SDL_Texture *itemBorderMagic;
    SDL_Texture *itemBorderRare;
    SDL_Texture *itemBorderEite;
    SDL_Texture *armorSpriteSheet;
    SDL_Texture *spellbook;
    SDL_Texture *debug_window;

    SDL_Texture *tooltipBackground;
    SDL_Texture *checkmark;
    SDL_Texture *fogOfWar;

    static void Load_Textures() {
       Graphics:: Create_Font();
        fireball_explosion_0 = Graphics::createTexture("assets/sprites/spells/fire/explosions/fireball_explosion_0.png");
        fireball_icon = Graphics::createTexture("assets/sprites/UI/icons/spell_icons/fire/Fantasy Game Skill Icon 1 - Magic - Fire Ball level 1.jpg");
        death1 = Graphics::createTexture("assets/sprites/UI/icons/Dark Magic/01.png");
        death2 = Graphics::createTexture("assets/sprites/UI/icons/Dark Magic/02.png");
        death3 = Graphics::createTexture("assets/sprites/UI/icons/Dark Magic/03.png");
        death4 = Graphics::createTexture("assets/sprites/UI/icons/Dark Magic/04.png");
        death5 = Graphics::createTexture("assets/sprites/UI/icons/Dark Magic/05.png");
        death6 = Graphics::createTexture("assets/sprites/UI/icons/Dark Magic/06.png");
        death7 = Graphics::createTexture("assets/sprites/UI/icons/Dark Magic/07.png");
        death8 = Graphics::createTexture("assets/sprites/UI/icons/Dark Magic/08.png");

        sinisterstrike = Graphics::createTexture("assets/sprites/UI/icons/misc_icons/old Ancient Beast icons/blood hack.jpg");
        healself = Graphics::createTexture("assets/sprites/UI/icons/spells/leaf-jade-2.png");
        healother = Graphics::createTexture("assets/sprites/UI/icons/spells/leaf-orange-2.png");
        lightningstrike = Graphics::createTexture("assets/sprites/spells/lightning/fx1_blue_topEffect/spell_bluetop_1_1.png");
        lightningball = Graphics::createTexture("assets/sprites/spells/lightning/fx8_lighteningBall/lighteningball_1_20_1.png");
        chargedbolt = Graphics::createTexture("assets/sprites/spells/lightning/chargedbolt/chargedbolt-0.png");
        leafspell = Graphics::createTexture("assets/sprites/spells/nature/leafspell/0026.png");
        swirlingsmoke = Graphics::createTexture("assets/sprites/spells/air/swirlingsmoke/0000.png");
        icebolt = Graphics::createTexture("assets/sprites/UI/icons/spell_icons/ice/Fantasy Game Skill Icon 1 - Magic - Ice Lance level 1.jpg");
        fire_1b_40 = Graphics::createTexture("assets/sprites/spells/fire/fire/fire_1b_40_1.png");
        warp_effect = Graphics::createTexture("assets/sprites/spells/arcane/warp_effect-sheet/warp_effect-sheet-3.png");
        barrier000 = Graphics::createTexture("assets/sprites/spells/arcane/warp_effect-sheet-4.png");
        BBLBubbleEllipseYellow = Graphics::createTexture("assets/sprites/spells/holy/BBLBubbleEllipseYellow.png");
        magic_flame = Graphics::createTexture("assets/sprites/spells/holy/BBLBubbleEllipseYellow.png");
        Snowflake1 = Graphics::createTexture("assets/sprites/spells/frost/hjm_frost_bolt/frames/hjm_frost_bolt01.png");
        Snowflake2 = Graphics::createTexture("assets/sprites/spells/frost/hjm-snow_flake/frames/hjm-snow_flake_003.png");

        arrow = Graphics::createTexture("assets/sprites/spells/archery/arrow.png");

        bow = Graphics::createTexture("assets/sprites/UI/icons/misc_icons/Icons/10.png");
        arrowsFlying = Graphics::createTexture("assets/sprites/UI/icons/misc_icons/Icons/85.png");
        redArrow = Graphics::createTexture("assets/sprites/UI/icons/misc_icons/Icons/95.png");
        blueArrow = Graphics::createTexture("assets/sprites/UI/icons/misc_icons/Icons/96.png");
        greenArrow = Graphics::createTexture("assets/sprites/UI/icons/misc_icons/Icons/97.png");

        rainofFire = Graphics::createTexture("assets/sprites/UI/icons/misc_icons/modif/firestream.png");
        thunderstorm = Graphics::createTexture("assets/sprites/UI/icons/misc_icons/attack_icons/thunder-1.png");
        blizzard = Graphics::createTexture("assets/sprites/UI/icons/misc_icons/attack_icons/blizzard-3.png");
        summonGhost = Graphics::createTexture("assets/sprites/UI/icons/misc_icons/painterly-spell-icons-4/wind-grasp-eerie-3.png");
        summonDemon = Graphics::createTexture("assets/sprites/UI/icons/misc_icons/painterly-spell-icons-3/horror-red-3.png");
        reanimate = Graphics::createTexture("assets/sprites/UI/icons/misc_icons/painterly-spell-icons-3/horror-red-1.png");
        raiseZombie = Graphics::createTexture("assets/sprites/UI/icons/misc_icons/old Ancient Beast icons/monster.png");
        raiseSkeleton = Graphics::createTexture("assets/sprites/UI/icons/Dark Magic/08.png");
        firebolt = Graphics::createTexture("assets/sprites/UI/icons/spell_icons/fire/Fantasy Game Skill Icon 1 - Magic - Fire Ball level 3.jpg");

        charCreateBackground = Graphics::createTexture("assets/sprites/UI/backgrounds/Background_47.png");
        window_glow = Graphics::createTexture("assets/sprites/UI/window_glow.png");
        fog = Graphics::createTexture("assets/sprites/UI/mist/fog_0.png");

        cox_icons = Graphics::createTexture("assets/sprites/icons/cox_icons.png");
        cox_skills = Graphics::createTexture("assets/sprites/icons/cox_skills.png");
        cox_spells = Graphics::createTexture("assets/sprites/icons/cox_spells.png");
        cox_map_tiles = Graphics::createTexture("assets/sprites/icons/cox_map_tiles.png");
        cox_monsters = Graphics::createTexture("assets/sprites/icons/cox_monsters.png");
        cox_objects = Graphics::createTexture("assets/sprites/icons/cox_objects.png");
        cox_traps = Graphics::createTexture("assets/sprites/icons/cox_traps.png");

        icons = Graphics::createTexture("assets/sprites/icons/icons.png");
        vikingBuildingIcons = Graphics::createTexture("assets/sprites/buildings/viking/icons.png");
        medievelBuildingIcons = Graphics::createTexture("assets/sprites/buildings/medieval/buildings.png");

        weapons_icons = Graphics::createTexture("assets/sprites/items/weaponIcons32x32_png_Transparent/weapon_icons_32_transparent.png");
        itsmars_Inventory = Graphics::createTexture("assets/sprites/UI/itsmars_Inventory.png");
        cursor_0 = Graphics::createTexture("assets/sprites/UI/cursors/cursor.png");
        //    cursor_1 = createTexture("assets/sprites/UI/cursors/cursor_outline_blue.png");
        //    cursor_1 = createTexture("assets/sprites/UI/cursors/cursor_outline_red.png");
        cursor_1 = Graphics::createTexture("assets/sprites/UI/cursors/cursor_shiny.png");
        selector32 = Graphics::createTexture("assets/sprites/UI/selector/selector32.png");

        emptyBagIcon = Graphics::createTexture("assets/sprites/UI/tooltips/tooltipBackground2.png");// currently NULL
        bagSlotBorder = Graphics::createTexture("assets/sprites/UI/icon_borders/frame-0-grey.png");

        itemBorderCommon = Graphics::createTexture("assets/sprites/UI/icon_borders/frame-0-grey.png");
        itemBorderMagic = Graphics::createTexture("assets/sprites/UI/icon_borders/frame-0-blue.png");
        itemBorderRare = Graphics::createTexture("assets/sprites/UI/icon_borders/frame-0-orange.png");
        itemBorderEite = Graphics::createTexture("assets/sprites/UI/icon_borders/frame-0-red.png");

        armorSpriteSheet = Graphics::createTexture("assets/sprites/items/flare_armor_transparent.png");

        //    fogOfWar = createTexture("assets/sprites/fog_of_war/blackFog.png");
        fogOfWar = Graphics::createTexture("assets/sprites/UI/tooltips/fogOfWar.png");
        tooltipBackground = Graphics::createTexture("assets/sprites/UI/tooltips/tooltipBackground2.png");
        checkmark = Graphics::createTexture("assets/sprites/UI/Inventory/29.png");
        spellbook = Graphics::createTexture("assets/sprites/UI/spellbook/spellbook.png");
        debug_window = Graphics::createTexture("assets/sprites/UI/debug/debug_frame.png");

        default_icon = Graphics::createTexture("assets/sprites/default.jpg");
    }
}