#pragma once
#include "ui_XP.h"
#include "textures.h"
#include "ui_elements.h"
#include "tooltips.h"
#include "icons.h"
#include "mouse_control.h"
#include "Button_Bar/button.h"
#include "Text/text.h"

namespace UI_Stats {
    //disease status
    //poison status

    enum Placement {
        first,
        nextLine,
        adjacent
    };

    SDL_Texture *addAttribute = NULL;
    bool attributesToSpend = true;

    struct Tab_Info {
        bool b_isOpen = false;
        SDL_FRect panelRect{};
        SDL_Rect backgroundFrame = {0, 0, 1137, 700};
        SDL_Texture *background = NULL;
        SDL_Rect leftPageFrame = {0, 0, 1137, 700};
        SDL_Texture *leftPage = NULL;
        SDL_Rect rightPageFrame = {0, 0, 1137, 700};
        SDL_Texture *rightPage = NULL;
        int currentTab = 0;
        entt::entity spell{};
    };

    struct asdaa {
        std::string name;
        std::string value;
        std::string description;
    };

    Tab_Info tab;
    std::array<std::array<asdaa, 18>, 2> stats;
    std::array<SDL_FRect, 18> textBox;
    std::array<SDL_FRect, 18> valueBox;
    std::array<SDL_FRect, 4> icons;
    float lineHeight = 20.0f;

    void Update(entt::registry &zone) {
        auto view = zone.view<Component::Input, Component::Name, Component::Stats, Component::Religion, Component::Health, Component::Mana, Component::XP, Component::Gold, Component::Dungeon_Level, Component::Time_On_Map>();
        for (auto entity: view) {
            auto &name = view.get<Component::Name>(entity);
            auto &playerStats = view.get<Component::Stats>(entity);
            auto &religion = view.get<Component::Religion>(entity);
            auto &health = view.get<Component::Health>(entity);
            auto &mana = view.get<Component::Mana>(entity);
            auto &xp = view.get<Component::XP>(entity);
            auto &gold = view.get<Component::Gold>(entity);
            auto &dungeonLevel = view.get<Component::Dungeon_Level>(entity);
            auto &timeOnMap = view.get<Component::Time_On_Map>(entity);

            //left page unused right now

            stats[1][0] = {"", name.first + " " + name.last, "Your character's name"};
            stats[1][1] = {"Race:", name.race, "Your character's race"};
            stats[1][2] = {"Sex:", name.sex, "Your character's sex"};
            stats[1][3] = {"Class:", name.Class, "Your character's class"};
            stats[1][4] = {"Religion:", religion.religion, "Your character's religion"};

            if (playerStats.unspent > 0) {
                attributesToSpend = true;
                stats[1][5] = {"Attribute Points:", std::to_string(playerStats.unspent), "The number of attribute points you have to spend"};
            }
            else
                attributesToSpend = false;

            stats[1][6] = {"Strength:", std::to_string(playerStats.strength), "Your strength represents your character's raw physical power and musculature.  You may carry a maximum amount of gold pieces equal to your Strength.  Armour and Melee Weapons have minimum Strength requirement to use.  To learn Basic/Advanced/Expert Warrior skills you need a Strength of 30/60/90, respectively.  On a successful melee hit, you have a %% chance equal to your Strength/3 of doing bonus damage from 1 to your Strength/10.  Strength is also used when interacting with certain squares, like Heavy Door, Open Pits and Great Webs."};
            stats[1][7] = {"Dexterity:", std::to_string(playerStats.dexterity), "Dexterity is a measure of your character's agility and reflexes.  It is used to determine your character's chance to hit in combat, and to avoid being hit.  Dexterity is also used when interacting with certain squares, like Traps, Secret Doors and Locks."};
            stats[1][8] = {"Intelligence:", std::to_string(playerStats.intelligence), "Intelligence is a measure of your character's mental acuity and knowledge.  It is used to determine your character's chance to learn spells, and to avoid being hit.  Intelligence is also used when interacting with certain squares, like Traps, Secret Doors and Locks."};
            stats[1][9] = {"Health:", std::to_string(health.currentHealth) + "/" + std::to_string(health.maxHealth), "Your health score represents your character's endurance and pain tolerance.  The number to the left of the slash is your current Health;  if this reaches zero or less, you die.  The number to the right of the slash shows your maximum Health; your Health can never raise above this.  To learn Basic/Advanced/Expert General Skill you need a Health of 30/60/90, respectively."};
            stats[1][10] = {"Spell Points:", std::to_string(mana.current) + "/" + std::to_string(mana.max), "Your Spell Points represent your character's reserves of magical power.  The number to the left of the slash is your current Spell Points, while the number to the right shows your maximum Spell Points.  With each new map entered, your Spell Points are restored to full.  Your Max Points are determined by your an interaction between your Intelligence, Thaumaturgy Skill level and Dungeon Level.  You cannot cast a Spell with a greater cost than your current Spell Points, and the cost of each Spell cast is deducted from your current Spell Points."};
            stats[1][11] = {"Faith:", std::to_string(religion.faith), "Your character's faith"};
            stats[1][12] = {"Gold:", std::to_string(gold.gold) + "/" + std::to_string(playerStats.strength), "Your character's gold"};
            stats[1][13] = {"Gems:", std::to_string(gold.gems), "Your character's gems"};
            stats[1][14] = {"Experience:", std::to_string(xp.xp) + "/" + std::to_string(XP::Get_Level(xp.level)), "Your character's experience"};
            stats[1][15] = {"Character Level:", std::to_string(xp.level), "Your character's level"};
            stats[1][16] = {"Dungeon Level:", std::to_string(dungeonLevel.level), "Your character's dungeon level"};
            stats[1][17] = {"Time on Map:", std::to_string(timeOnMap.current), "Your character's time on the map"};
        }
    }

    bool Toggle(Toggle_Type toggleType = Toggle_Type::toggle) {
        if (toggleType == Toggle_Type::get)
            return tab.b_isOpen;

        if (toggleType == Toggle_Type::on) {
            tab.b_isOpen = true;
            return true;
        }
        if (toggleType == Toggle_Type::off) {
            tab.b_isOpen = false;
            return false;
        }

        if (tab.b_isOpen) {
            tab.b_isOpen = false;
            return false;
        }
        tab.b_isOpen = true;
        return true;
    }

    void Update_Position() {
        tab.panelRect = UI::Center_Rect(tab.backgroundFrame);
        tab.background = Texture::spellbook;
    }

    void Next_Line(Component::Camera &camera, SDL_FRect &statBox, SDL_FRect &defaultStatBox) {
        statBox.x = defaultStatBox.x;
        statBox.y += lineHeight / camera.scale.y;
    }

    void Set_Adjacent(Component::Camera &camera, SDL_FRect &statBox) {
        statBox.x += ((lineHeight * 8.0f) + (lineHeight * 0.25f)) / camera.scale.x;
    }

    void Draw_Icon(Component::Camera &camera, SDL_FRect &statBox, int currentTab, int j, SDL_FRect &defaultStatBox, Placement placement = Placement::adjacent) {
        if (!attributesToSpend)
            return;

        auto iconClipA = Icons::iconClipRects["increasebutton"];

        SDL_Rect iconClip = iconClipA.clipRect;
        Set_Adjacent(camera, statBox);
        SDL_FRect rect = statBox;
        rect.x -= ((lineHeight * 3.0f) / camera.scale.y);
        rect.w = (lineHeight / camera.scale.y);
        rect.h = (lineHeight / camera.scale.y);

        SDL_RenderCopyF(Graphics::renderer, Texture::cox_icons, &iconClip, &rect);
        icons[j  - 6] = rect;
    }

    void Draw_Icons() {
        for (auto &icon : icons) {
            if (Mouse::bRect_inside_Cursor(icon)) {
                SDL_Rect highLightClip = Icons::iconClipRects["goldindicator"].clipRect;
                SDL_SetTextureAlphaMod(Texture::cox_icons, 100);
                SDL_RenderCopyF(Graphics::renderer, Texture::cox_icons, &highLightClip, &icon);
                SDL_SetTextureAlphaMod(Texture::cox_icons, 255);
                return;
            }
        }
    }

    void Draw_Text_Line(Component::Camera &camera, SDL_FRect &statBox, int currentTab, int j, SDL_FRect &defaultStatBox, Placement placement) {
        if (placement == Placement::nextLine)
            Next_Line(camera, statBox, defaultStatBox);
        if (placement == Placement::adjacent)
            Set_Adjacent(camera, statBox);

        float x, y;
        SDL_RenderGetScale(Graphics::renderer, &x, &y);
        std::cout << x << " " << y << std::endl;

        FC_Scale scale = {1.0f / camera.scale.x, 1.0f / camera.scale.y};
        SDL_Rect textRect = FC_DrawScaleRight(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, stats[currentTab][j].name.c_str());
        textBox[j] = Utilities::SDL_Rect_To_SDL_FRect(textRect);

        std::string text = " " + stats[currentTab][j].value;
        SDL_Rect valueRect = FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, Color::black, text.c_str());
        valueBox[j] = Utilities::SDL_Rect_To_SDL_FRect(valueRect);
    }

    void Draw_Tooltip(Component::Camera &camera, int currentTab) {
        for (size_t i = 0; i < textBox.size(); i++)
            if (Mouse::bRect_inside_Cursor(textBox[i]) || Mouse::bRect_inside_Cursor(valueBox[i])) {
                Text::Create_Multiline_Texture(camera.scale, stats[currentTab][i].description, Mouse::iXMouse, Mouse::iYMouse);
                return;
            }
    }

    void Draw_Attibutes(Component::Camera &camera, SDL_FRect &statBox, int currentTab, int j, SDL_FRect &defaultStatBox, Placement placement) {
        if (!attributesToSpend)
            return;

        Set_Adjacent(camera, statBox);
        SDL_FRect asd = statBox;
        asd.x -= (((lineHeight * 3.0f) + (lineHeight * 0.25f)) / camera.scale.y);
        asd.w = (lineHeight / camera.scale.y);
        asd.h = (lineHeight / camera.scale.y);
        Draw_Text_Line(camera, asd, currentTab, j, defaultStatBox, Placement::first);
    }

    void Draw_Stats_Page(Component::Camera &camera, SDL_FRect &draw, int currentTab) {
        SDL_FRect renderRect = {draw.x + 245.0f, draw.y + 128.0f, 0.0f, 0.0f};
        SDL_FRect statBox = Camera_Control::Convert_FRect_To_Scale(renderRect, camera);
        SDL_FRect defaultStatBox = statBox;

        Draw_Text_Line(camera, statBox, currentTab, 0, defaultStatBox, Placement::first);
        Next_Line(camera, statBox, defaultStatBox);
        Draw_Text_Line(camera, statBox, currentTab, 1, defaultStatBox, Placement::nextLine);
        Draw_Text_Line(camera, statBox, currentTab, 2, defaultStatBox, Placement::adjacent);
        Draw_Text_Line(camera, statBox, currentTab, 3, defaultStatBox, Placement::nextLine);
        Draw_Text_Line(camera, statBox, currentTab, 4, defaultStatBox, Placement::adjacent);
        Next_Line(camera, statBox, defaultStatBox);
        Next_Line(camera, statBox, defaultStatBox);

        if (attributesToSpend)
            Draw_Attibutes(camera, statBox, currentTab, 5, defaultStatBox, Placement::adjacent);

        Draw_Text_Line(camera, statBox, currentTab, 6, defaultStatBox, Placement::nextLine);
        Draw_Icon(camera, statBox, currentTab, 6, defaultStatBox, Placement::adjacent);
        Draw_Text_Line(camera, statBox, currentTab, 7, defaultStatBox, Placement::nextLine);
        Draw_Icon(camera, statBox, currentTab, 7, defaultStatBox, Placement::adjacent);
        Draw_Text_Line(camera, statBox, currentTab, 8, defaultStatBox, Placement::nextLine);
        Draw_Icon(camera, statBox, currentTab, 8, defaultStatBox, Placement::adjacent);
        Draw_Text_Line(camera, statBox, currentTab, 9, defaultStatBox, Placement::nextLine);
        Draw_Icon(camera, statBox, currentTab, 9, defaultStatBox, Placement::adjacent);
        //draw health bar
        Next_Line(camera, statBox, defaultStatBox);

        Draw_Text_Line(camera, statBox, currentTab, 10, defaultStatBox, Placement::nextLine);
        Draw_Text_Line(camera, statBox, currentTab, 11, defaultStatBox, Placement::adjacent);
        //draw Mana bar
        Next_Line(camera, statBox, defaultStatBox);

        Draw_Text_Line(camera, statBox, currentTab, 12, defaultStatBox, Placement::nextLine);
        Draw_Text_Line(camera, statBox, currentTab, 13, defaultStatBox, Placement::adjacent);
        //draw gold bar
        Next_Line(camera, statBox, defaultStatBox);

        Draw_Text_Line(camera, statBox, currentTab, 14, defaultStatBox, Placement::nextLine);
        //draw XP bar
        Next_Line(camera, statBox, defaultStatBox);

        Draw_Text_Line(camera, statBox, currentTab, 15, defaultStatBox, Placement::nextLine);
        Draw_Text_Line(camera, statBox, currentTab, 16, defaultStatBox, Placement::adjacent);
        Draw_Text_Line(camera, statBox, currentTab, 17, defaultStatBox, Placement::nextLine);

        Draw_Icons();
        Draw_Tooltip(camera, currentTab);
    }

    void Draw_Pages(Component::Camera &camera) {
        SDL_FRect draw = UI::Update_Scale(camera.scale, tab.panelRect);
        SDL_RenderCopyF(Graphics::renderer, tab.background, &tab.backgroundFrame, &draw);
//        Draw_Stats_Page(camera, draw, tab.currentTab);
        SDL_FRect page2 = tab.panelRect;
        page2.w /= 2;
        page2.x += page2.w - 64.0f;
        Draw_Stats_Page(camera, page2, tab.currentTab + 1);
    }

    void Draw_Attributes(entt::registry &zone, Component::Camera &camera) {
        if (tab.b_isOpen) {
            Update(zone);
            Draw_Pages(camera);
        }
    }

}// namespace UI_Info