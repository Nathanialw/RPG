#pragma once
#include "ui_XP.h"
#include "textures.h"
#include "ui_elements.h"
#include "UI/Tooltips/tooltips.h"
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

    constexpr  int maxLines = 8;
    struct asdaa {
        std::string name;
        std::string value;
        std::array<std::string, maxLines> description;
    };

    Tab_Info tab;

    std::array<std::array<asdaa, 18>, maxLines> stats;
    std::array<SDL_FRect, 18> textBox;
    std::array<SDL_FRect, 18> valueBox;
    std::array<SDL_FRect, 4> icons;
    float lineHeight = 20.0f;

    void Update(entt::registry &zone) {
        auto view = zone.view<Component::Input, Component::Name, Component::Stats, Component::Religion, Component::Health, Component::Mana, Component::Level, Component::Gold, Component::Dungeon_Level, Component::Time_On_Map>();
        for (auto entity: view) {
            auto &name = view.get<Component::Name>(entity);
            auto &playerStats = view.get<Component::Stats>(entity);
            auto &religion = view.get<Component::Religion>(entity);
            auto &health = view.get<Component::Health>(entity);
            auto &mana = view.get<Component::Mana>(entity);
            auto &xp = view.get<Component::Level>(entity);
            auto &gold = view.get<Component::Gold>(entity);
            auto &dungeonLevel = view.get<Component::Dungeon_Level>(entity);
            auto &timeOnMap = view.get<Component::Time_On_Map>(entity);

            //left page unused right now

            stats[1][0] = {"", name.first + " " + name.last, {"Name", "Your character's name is chosen at the start of the game.", "It may be recorded in the high score list at the end of the game."}};
            stats[1][1] = {"Race:", name.race, {"Race", "Your character's Race helps determine starting Attributes, Skills. Abilities and Deficiencies, and what Perks you receive.", "Humans start with:", "Strength: 30, Dexterity 30, Intelligence 30, Max Health 30.", "They also start with 1 bonus Skill Point.", "They gain 0.2 bonus Skill Points with each Character Level."}};
            stats[1][2] = {"Sex:", name.sex, {"Sex", "Your character's Sex helps determine starting Attributes, and has a minor impact on gameplay.", "Human females tend to be weaker and slightly smarter than their male counterparts.  They start with Strength -5, Intelligence +5."}};
            stats[1][3] = {"Class:", name.Class, {"Class", "Your character's class helps determine starting Attributes, Skills, Abilities and Deficiencies, and what Perks you receive.", "Scholar's start with Intelligence +20, Dexterity -10, Strength -10 and 2 extra levels of History.", "Mage and General Skills cost 1 Skill Point.", "Rogue Skills cost 2 Skill Points.", "Warrior Skills cost 3 Skill Points.", "They may never gain Advanced Armour Use.", "They gain 10 Intelligence each time they read A Tome."}};
            stats[1][4] = {"Religion:", religion.religion, {"Religion", "Your character's Religion impacts access to Miracles and sets in Place strictures by which the player can gain or lose Faith Points.  Your standing can also improve or weaken with each deity according to the actions you take, regardless of what Religion you belong to.  For details on Miracles and your personal deity, open the Religion tab.  For details on your relationship with all gods, and the actions of which they approve and disapprove, open the Stats tab."}};

            if (playerStats.unspent > 0) {
                attributesToSpend = true;
                stats[1][5] = {"Attribute Points:", std::to_string(playerStats.unspent), {"Attribute Points", "Attribute Points are typically gained on achieving a new Character Level, though there are other means.", "Each Attribute Point may be spent to raise your Strength, Dexterity, Intelligence or Maximum Health by 1 Point, by clicking the red and white increase symbol that appears next to each stat when you have more than zero Attribute Points."}};
            }
            else
                attributesToSpend = false;

            stats[1][6] = {"Strength:", std::to_string(playerStats.strength), {"Strength", "Your Strength represents your character's raw physical power and musculature.",  "You may carry a maximum amount of gold pieces equal to your Strength.", "Armour and Melee Weapons have minimum Strength requirement to use.", "To learn Basic/Advanced/Expert Warrior skills you need a Strength of 30/60/90, respectively.", "On a successful melee hit, you have a %% chance equal to your Strength/3 of doing bonus damage from 1 to your Strength/10.", "Strength is also used when interacting with certain squares, like Heavy Door, Open Pits and Great Webs."}};
            stats[1][7] = {"Dexterity:", std::to_string(playerStats.dexterity), {"Dexterity", "Your Dexterity represents your character's nimbleness and speed.", "Your base percent chance to hit in combat is equal to your Dexterity, though this is modified by other factors like your Weapon and the monster's Defence rating.", "Missile Weapons have minimum Dexterity ratings to use.", "To learn Basic/Advanced/Expert Rogue Skills you need a Dexterity of 30/60/90, respectively.", "Dexterity is also used when interacting with certain squares, like Pendulum Blades and Mine Carts."}};
            stats[1][8] = {"Intelligence:", std::to_string(playerStats.intelligence), {"Intelligence", "Your Intelligence score  represents your character's education and cunning.", "Each Spell has a minimum Intelligence required to use it, regardless of the spell's cost and your Spell Points.", "Spell Points are determined based on your Intelligence, Thaumaturgy Skill and Dungeon Level.", "With each Character Level achieved you gain bonus Skill Points equal to your Intelligence/300 *with fractions held for future levels.)", "To learn Basic/Advanced/Expert Mage Skills you need an Intelligence of 30/60/90, respectively.","Intelligence is also used when interacting with certain squares, like Riddles, Secret Doors and certain monsters, like Sphinxes."}};
            stats[1][9] = {"Health:", std::to_string(health.currentHealth) + "/" + std::to_string(health.maxHealth), {"Health", "Your Health score represents your character's endurance and pain tolerance." , "The number to the left of the slash is your current Health; if this reaches zero or less, you die.  The number to the right of the slash shows your maximum Health; your Health can never raise above this.", "To learn Basic/Advanced/Expert General Skill you need a Health of 30/60/90, respectively."}};
            stats[1][10] = {"Spell Points:", std::to_string(mana.current) + "/" + std::to_string(mana.max), {"Spell Points", "Your Spell Points represent your character's reserves of magical power.", "The number to the left of the slash is your current Spell Points, while the number to the right shows your maximum Spell Points.", "With each new map entered, your Spell Points are restored to full.", "Your Max Points are determined by your an interaction between your Intelligence, Thaumaturgy Skill level and Dungeon Level.", "You cannot cast a Spell with a greater cost than your current Spell Points, and the cost of each Spell cast is deducted from your current Spell Points."}};
            stats[1][11] = {"Faith:", std::to_string(religion.faith), {"Faith", "Your Faith score represents how much favour you've earned from your god.", "Fath Points are earned by performing actions your god prefers, and are lost either through expenditure or by performing actions of which your god disapproves.", "You cannot cast a Miracle with a greater cost than your current Faith Points, and the cost of each Miracle cast is deducted from your current Faith Points."}};
            stats[1][12] = {"Gold:", std::to_string(gold.gold) + "/" + std::to_string(playerStats.strength), {"Gold", "Gold in conjuction with Gems (collectively known as Treasure) represents the monetary resources at your disposal.", "The number to the left of the slash is the amount of Gold you're carrying, while the number to the right is the maximum amount of Gold you can carry.", "You may typically carry no more Gold than your Strength, though there are certain circumstances that can modify this.", "Treasure is traded at Temples for Experience and is sometimes used to purchase Items, Spells or to pay for services."}};
            stats[1][13] = {"Gems:", std::to_string(gold.gems), {"Gems", "Gems, in conjunction with Gold (collectively known as Treasure) represents the monetary resources at your disposal.", "Unlike Gold there is no maximum to the value of Gems you may carry.", "Treasure is traded at Temples for Experience and is sometimes used to purchase Items, Spells or to pay for services."}};
            stats[1][14] = {"Experience:", std::to_string(xp.xp) + "/" + std::to_string(XP::Get_Level(xp.level)), {"Experience", "Your character's Experience determines when he/she advances to the next Character Level", "Gaining a Character Level grants you points to put toward your Strength, Dexterity, Intelligence or Maximum Health.", "You also gain at least 2 more points of Maximum Health and your Health is then restored to Maximum", "You also gain at least 1 Skill Point and a Perk.", "Each Character Level gained raises the Experience required to reach the next level by 10%%.", "Experience is gained by killing monsters, bringing Treasure to Temples and other, less common ways."}};
            stats[1][15] = {"Character Level:", std::to_string(xp.level), {"Character Level", "Your character's level is a rough indication of how powerful he/she is.", "Gaining a Character Level grants you points to put toward your Strength, Dexterity, Intelligence or Maximum Health.", "You additionally gain at least 2 more Points to your Maximum Health and your Health is then restored to Maximum.", "You also gain at least 1 Skill Point and a Perk.", "Each Character Level gained raises the Experience required for the next one by 10%%."}};
            stats[1][16] = {"Dungeon Level:", std::to_string(dungeonLevel.level), {"Dungeon Level", "This indicates how close you are to your goal; Xaskazien resides on Dungeon Level 30.", "If you occupy a Lair, Legendary Land or other special map type, its name will also be displayed on the Dungeon Level line, and you may query it for more details about the map you occupy."}};
            stats[1][17] = {"Time on Map:", std::to_string(timeOnMap.current), {"Turns On This Map", "This shows you how many turns you've spent on the current map.  The warning to  leave the map usually appears somewhere between turn 900 and turn 1200.  After the warning appears, each turn you remain on the map will leave a 1%% chance of a decidedly negative Event occurring.  If you've managed to uncover the turn on which the warning will occur, you will also see that listed, to the right of the slash."}};
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
        if (!attributesToSpend)
            return;

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

    void Click_Icons(entt::registry &zone, entt::entity &player) {
        if (!attributesToSpend)
            return;

        for (size_t i = 0; i < icons.size(); i++) {
            if (Mouse::bRect_inside_Cursor(icons[i])) {
                auto &unitStats = zone.get<Component::Stats>(player);
                auto &health = zone.get<Component::Health>(player);

                if (unitStats.unspent > 0) {
                    unitStats.unspent--;
                    switch (i) {
                        case 0:
                            unitStats.strength++;
                            unitStats.strengthBase++;
                            return;
                        case 1:
                            unitStats.dexterity++;
                            unitStats.dexterityBase++;
                            return;
                        case 2:
                            unitStats.intelligence++;
                            unitStats.intelligenceBase++;
                            return;
                        case 3:
                            health.maxHealth += 1;
                            health.base += 1;
                            health.currentHealth += 1;
                            return;
                        default:
                            return;
                    }
                }
            }
        }
    }

    void Draw_Text_Line(Component::Camera &camera, SDL_FRect &statBox, int currentTab, int j, SDL_FRect &defaultStatBox, Placement placement) {
        if (placement == Placement::nextLine)
            Next_Line(camera, statBox, defaultStatBox);
        if (placement == Placement::adjacent)
            Set_Adjacent(camera, statBox);

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

		std::array<std::string, maxLines> formattedDesc = {
			stats[currentTab][i].description[0],
			stats[currentTab][i].description[1],
			stats[currentTab][i].description[2],
			stats[currentTab][i].description[3],
			stats[currentTab][i].description[4],
			stats[currentTab][i].description[5],
			stats[currentTab][i].description[6],
			stats[currentTab][i].description[7],
		};

		float lineHeight = FC_GetHeight(Graphics::fcFont, "%s", "A");
		std::array<float, maxLines> spacing = {
			lineHeight * 0.50f,
			lineHeight * 0.25f,
			lineHeight * 0.25f,
			lineHeight * 0.25f,
			lineHeight * 0.25f,
			lineHeight * 0.25f,
			lineHeight * 0.0f,
		};

		Tooltips::Properties<maxLines> tooltipProperties = {
			formattedDesc,
			spacing,
			550.0f,
			1.0f,
			1.0f,
			Tooltips::MOUSE_TOP_RIGHT
		};

		Tooltips::Create_Tooltip(tooltipProperties);

//                Text::Create_Multiline_Texture(camera.scale, stats[currentTab][i].description, Mouse::iXMouse, Mouse::iYMouse);
                return;
            }
    }

    void Draw_Attibutes(Component::Camera &camera, SDL_FRect &statBox, int currentTab, int j, SDL_FRect &defaultStatBox, Placement placement) {
        if (!attributesToSpend) {
	    textBox[j] = {};
	    valueBox[j] = {};
            return;
	}

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