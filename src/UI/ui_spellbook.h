#pragma once

#include "Input/mouse_control.h"
#include "Input/mouse_data.h"
#include "graphics.h"
#include "textures.h"
#include "Spells/Warrior/sinister_strike.h"
#include "spell_data.h"
#include "ui_elements.h"

#include "Archery/archery.h"
#include "Bard/auras.h"
#include "Fire/fire.h"
#include "Holy/holy.h"
#include "Ice/ice.h"
#include "Necromancy/necromancy.h"

#include "Air/smoke.h"
#include "Arcane/arcane.h"
#include "Life/heal_self.h"
#include "Life/life.h"
#include "Nature/nature.h"
#include "lightning/lightning.h"
#include "lightning/lightningball.h"
#include "Warrior/auto_attack.h"

namespace UI_Spellbook {

    struct Spellbook {
        std::array<std::array<Spell_Data::Spell, 12>, (int) Spell_Data::SIZE> Skill_Trees;
        bool b_isOpen = false;
        SDL_FRect panelRect;
        SDL_Rect backgroundFrame = {0, 0, 1137, 700};
        SDL_Texture *background = NULL;
        SDL_Rect leftPageFrame = {0, 0, 1137, 700};
        SDL_Texture *leftPage = NULL;
        SDL_Rect rightPageFrame = {0, 0, 1137, 700};
        SDL_Texture *rightPage = NULL;
        int currentTab = 0;
        entt::entity spell;
    };
    Spellbook spellbook;

    void Update_Position(SDL_FRect &frameRect) {
        SDL_Rect rect = Utilities::SDL_FRect_To_SDL_Rect(frameRect);
        frameRect = UI::Center_Rect(rect);
        int w;
        int h;
        SDL_GetRendererOutputSize(Graphics::renderer, &w, &h);
        frameRect.y = (float) h - frameRect.h;
    }

    int PLACEHOLDER(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
        return 0;
    }

    void Build_Spellbook() {
        spellbook.Skill_Trees[Spell_Data::fire][0] = Spell_Data::Spell(Texture::fireball_icon, Fire_Bolt::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::fire][1] = Spell_Data::Spell(Texture::firebolt, Fire_Ball::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::fire][2] = Spell_Data::Spell(Texture::rainofFire, Dragon_Breath::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::fire][3] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::fire][4] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::fire][5] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::fire][6] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::fire][7] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::fire][8] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::fire][9] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::fire][10] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::fire][11] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);

        spellbook.Skill_Trees[Spell_Data::cold][0] = Spell_Data::Spell(Texture::icebolt, Ice_Bolt::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::cold][1] = Spell_Data::Spell(Texture::Snowflake1, Snowflake1::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::cold][2] = Spell_Data::Spell(Texture::Snowflake2, Snowflake2::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::cold][3] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::cold][4] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::cold][5] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::cold][6] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::cold][7] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::cold][8] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::cold][9] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::cold][10] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::cold][11] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);

        spellbook.Skill_Trees[Spell_Data::arcane][0] = Spell_Data::Spell(Texture::warp_effect, Arcane::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::arcane][1] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::arcane][2] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::arcane][3] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::arcane][4] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::arcane][5] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::arcane][6] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::arcane][7] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::arcane][8] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::arcane][9] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::arcane][10] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::arcane][11] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);

        spellbook.Skill_Trees[Spell_Data::bard][0] = Spell_Data::Spell(Texture::arrowsFlying, Might::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::bard][1] = Spell_Data::Spell(Texture::greenArrow, Garlic::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::bard][2] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::bard][3] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::bard][4] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::bard][5] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::bard][6] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::bard][7] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::bard][8] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::bard][9] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::bard][10] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::bard][11] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);

        spellbook.Skill_Trees[Spell_Data::earth][0] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::earth][1] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::earth][2] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::earth][3] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::earth][4] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::earth][5] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::earth][6] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::earth][7] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::earth][8] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::earth][9] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::earth][10] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::earth][11] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);

        spellbook.Skill_Trees[Spell_Data::warrior][0] = Spell_Data::Spell(Texture::sinisterstrike, Sinister_Strike::Sinister_Strike);
        spellbook.Skill_Trees[Spell_Data::warrior][1] = Spell_Data::Spell(Texture::redArrow, Shoot_Arrow::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::warrior][2] = Spell_Data::Spell(Texture::sinisterstrike, Warrior::Auto_Attack);
        spellbook.Skill_Trees[Spell_Data::warrior][3] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::warrior][4] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::warrior][5] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::warrior][6] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::warrior][7] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::warrior][8] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::warrior][9] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::warrior][10] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::warrior][11] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);

        spellbook.Skill_Trees[Spell_Data::archery][0] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::archery][1] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::archery][2] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::archery][3] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::archery][4] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::archery][5] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::archery][6] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::archery][7] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::archery][8] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::archery][9] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::archery][10] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::archery][11] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);

        spellbook.Skill_Trees[Spell_Data::lightning][0] = Spell_Data::Spell(Texture::chargedbolt, Lightning::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::lightning][1] = Spell_Data::Spell(Texture::lightningball, LightningBall::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::lightning][2] = Spell_Data::Spell(Texture::thunderstorm, Lightning::Cast_Spell2);
        spellbook.Skill_Trees[Spell_Data::lightning][3] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::lightning][4] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::lightning][5] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::lightning][6] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::lightning][7] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::lightning][8] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::lightning][9] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::lightning][10] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::lightning][11] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);

        spellbook.Skill_Trees[Spell_Data::air][0] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::air][1] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::air][2] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::air][3] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::air][4] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::air][5] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::air][6] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::air][7] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::air][8] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::air][9] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::air][10] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::air][11] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);

        spellbook.Skill_Trees[Spell_Data::life][0] = Spell_Data::Spell(Texture::healself, Heal_Self::Heal_Self);
        spellbook.Skill_Trees[Spell_Data::life][1] = Spell_Data::Spell(Texture::healother, Life::Heal_Other);
        spellbook.Skill_Trees[Spell_Data::life][2] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::life][3] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::life][4] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::life][5] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::life][6] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::life][7] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::life][8] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::life][9] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::life][10] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::life][11] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);

        spellbook.Skill_Trees[Spell_Data::death][0] = Spell_Data::Spell(Texture::summonGhost, Arrogate_Ghost::Cast);
        spellbook.Skill_Trees[Spell_Data::death][1] = Spell_Data::Spell(Texture::summonDemon, Summon_Demon::Cast);
        spellbook.Skill_Trees[Spell_Data::death][2] = Spell_Data::Spell(Texture::raiseSkeleton, Raise_Skeleton::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::death][3] = Spell_Data::Spell(Texture::reanimate, Reanimate::Cast);
        spellbook.Skill_Trees[Spell_Data::death][4] = Spell_Data::Spell(Texture::raiseZombie, Raise_Zombie::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::death][5] = Spell_Data::Spell(Texture::death2, Metamorphosis::Cast_Spell);
        spellbook.Skill_Trees[Spell_Data::death][6] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::death][7] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::death][8] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::death][9] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::death][10] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::death][11] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);

        spellbook.Skill_Trees[Spell_Data::extra][0] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::extra][1] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::extra][2] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::extra][3] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::extra][4] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::extra][5] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::extra][6] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::extra][7] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::extra][8] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::extra][9] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::extra][10] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
        spellbook.Skill_Trees[Spell_Data::extra][11] = Spell_Data::Spell(Texture::default_icon, PLACEHOLDER);
    }

    //    run once the camera is attached to a unit
    void Update_Position() {
        spellbook.panelRect = UI::Center_Rect(spellbook.backgroundFrame);
    }

    void Init_UI() {
        Update_Position();
        spellbook.background = Texture::spellbook;
        Build_Spellbook();
    }

    void Close_Spellbook() {
        if (spellbook.b_isOpen)
            spellbook.b_isOpen = false;
    }

    bool Toggle(Toggle_Type toggleType = Toggle_Type::toggle) {
        if (toggleType == Toggle_Type::get)
            return spellbook.b_isOpen;

        if (toggleType == Toggle_Type::on) {
            spellbook.b_isOpen = true;
            return true;
        }
        if (toggleType == Toggle_Type::off) {
            spellbook.b_isOpen = false;
            return false;
        }

        if (spellbook.b_isOpen) {
            spellbook.b_isOpen = false;
            return false;
        }
        spellbook.b_isOpen = true;
        return true;
    }

    bool Get_SpellPage(entt::registry &zone, Component::Camera &camera, SDL_FRect &draw, Spell_Data::Skill_Tree tree) {
        float xSpacing = 32.0f;
        float ySpacing = 32.0f;
        float x = draw.x;
        float y = draw.y;
        float i = 1;
        float w = 0;
        for (int j = 0; j < spellbook.Skill_Trees[tree].size(); ++j) {
            SDL_FRect renderRect = {x + 192.0f + (spellbook.Skill_Trees[tree][j].icon.renderRectSize.x * w) + (xSpacing * w), y + 128.0f, spellbook.Skill_Trees[tree][j].icon.renderRectSize.x, spellbook.Skill_Trees[tree][j].icon.renderRectSize.x};
            w++;
            if ((j + 1) % 3 == 0) {
                x = draw.x;
                y = draw.y + ((ySpacing + spellbook.Skill_Trees[tree][j].icon.renderRectSize.x) * i);
                i++;
                w = 0.0f;
            }
            renderRect = UI::Update_Scale(camera.scale, renderRect);

            if (Mouse::bRect_inside_Cursor(renderRect)) {
                //        Utilities::Log("picked up spell at spellbook index " + std::to_string(j));
                Mouse::mouseData.tree = tree;
                Mouse::mouseData.index = j;
                Mouse::mouseData.type = Component::Icon_Type::spell;
                Mouse::mouseData.itemCurrentlyHeld = true;
                return true;
            }
        }
        return false;
    }

    void Next_Page() {
        spellbook.currentTab += 2;
        if (spellbook.currentTab + 1 > (Spell_Data::Skill_Tree::SIZE - 1))
            spellbook.currentTab = (Spell_Data::Skill_Tree::SIZE - 2);
    }

    void Previous_Page() {
        spellbook.currentTab -= 2;
        if (spellbook.currentTab < 0)
            spellbook.currentTab = 0;
    }

    bool Get_Spell(entt::registry &zone, Component::Camera &camera) {
        if (!Mouse::mouseData.itemCurrentlyHeld) {
            if (Get_SpellPage(zone, camera, spellbook.panelRect, Spell_Data::Skill_Tree(spellbook.currentTab)))
                return true;
            SDL_FRect page2 = spellbook.panelRect;
            page2.w /= 2;
            page2.x += page2.w - 64.0f;
            if (Get_SpellPage(zone, camera, page2, Spell_Data::Skill_Tree(spellbook.currentTab + 1)))
                return true;
        }
        return false;
    }

    void Draw_Spell_Page(Component::Camera &camera, SDL_FRect &draw, Spell_Data::Skill_Tree tree) {
        float xSpacing = 32.0f;
        float ySpacing = 32.0f;
        float x = draw.x;
        float y = draw.y;
        float i = 1;
        float w = 0;
        for (int j = 0; j < spellbook.Skill_Trees[tree].size(); ++j) {
            SDL_FRect renderRect = {x + 192.0f + (spellbook.Skill_Trees[tree][j].icon.renderRectSize.x * w) + (xSpacing * w), y + 128.0f, spellbook.Skill_Trees[tree][j].icon.renderRectSize.x, spellbook.Skill_Trees[tree][j].icon.renderRectSize.x};
            w++;
            if ((j + 1) % 3 == 0) {
                x = draw.x;
                y = draw.y + ((ySpacing + spellbook.Skill_Trees[tree][j].icon.renderRectSize.x) * i);
                i++;
                w = 0.0f;
            }
            renderRect = UI::Update_Scale(camera.scale, renderRect);

            SDL_RenderCopyF(Graphics::renderer, spellbook.Skill_Trees[tree][j].icon.pBackground, NULL, &renderRect);
            SDL_RenderCopyF(Graphics::renderer, spellbook.Skill_Trees[tree][j].icon.pTexture, NULL, &renderRect);
            SDL_RenderCopyF(Graphics::renderer, spellbook.Skill_Trees[tree][j].icon.pIconBorder, NULL, &renderRect);
            SDL_RenderCopyF(Graphics::renderer, spellbook.Skill_Trees[tree][j].icon.pIconRarityBorder, NULL, &renderRect);
        }
    }

    bool Check_Spellbook(Component::Camera &camera) {
        SDL_FRect draw = UI::Update_Scale(camera.scale, spellbook.panelRect);
        if (spellbook.b_isOpen && Mouse::bRect_inside_Cursor(draw))
            return true;
        return false;
    }

    void Draw_Pages(Component::Camera &camera, SDL_FRect &draw) {
        Draw_Spell_Page(camera, draw, Spell_Data::Skill_Tree(spellbook.currentTab));
        SDL_FRect page2 = spellbook.panelRect;
        page2.w /= 2;
        page2.x += page2.w - 64.0f;
        Draw_Spell_Page(camera, page2, Spell_Data::Skill_Tree(spellbook.currentTab + 1));
    }

    void Draw_Spellbook(Component::Camera &camera) {
        if (spellbook.b_isOpen) {
            SDL_FRect draw = UI::Update_Scale(camera.scale, spellbook.panelRect);
            SDL_RenderCopyF(Graphics::renderer, spellbook.background, &spellbook.backgroundFrame, &draw);
            Draw_Pages(camera, spellbook.panelRect);
        }
    }
}// namespace UI_Spellbook