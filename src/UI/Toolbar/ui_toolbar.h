//
// Created by nathanial on 12/16/24.
//
#pragma  once
#include "array"

#include "ui_minimap.h"
#include "Button_Bar/bar.h"
#include "UI/ui_spellbook.h"
#include "Skills/general.h"
#include "Skills/mage.h"
#include "Skills/rogue.h"
#include "Skills/warrior.h"
#include "Button_Bar/button.h"
#include "ui_stats.h"
#include "menu.h"


namespace  UI_toolbar {

    enum class Buttons {
        inventory = 0,
        character,
        lowpower,
        midpower,
        highpower,
        generalskills,
        mageskills,
        rogueskills,
        warriorskills,
        perks,
        stats,
        events,
        map,
        religion,
        menu,
        SIZE
    };

    typedef bool (*Action)(Toggle_Type);

    bool PLACEHOLDER(Toggle_Type toggleType) {
        if (toggleType == Toggle_Type::get)
            return false;
        if (toggleType == Toggle_Type::on)
            return true;
        if (toggleType == Toggle_Type::off)
            return false;
        return false;
    }

    Button::Bar<(int)Buttons::SIZE, Action> toolBar = Button::Bar<(int)Buttons::SIZE, Action>({
        {
            {"inventory", Bag_UI::Toggle},
            {"character", UI_Stats::Toggle},
            {"lowpower", UI_Spellbook::Toggle},
            {"midpower", PLACEHOLDER},
            {"highpower", PLACEHOLDER},
            {"generalskills", Skill::General::Toggle},
            {"mageskills", Skill::Mage::Toggle},
            {"rogueskills", Skill::Rogue::Toggle},
            {"warriorskills", Skill::Warrior::Toggle},
            {"perks", PLACEHOLDER},
            {"stats", PLACEHOLDER},
            {"events", PLACEHOLDER},
            {"map", Minimap::Toggle},
            {"religion", PLACEHOLDER},
            {"options", Menu::Toggle}
        }},
        "buttonon",
        "buttonoff",
        32.0f
    );

    void Toggle(const Buttons i) {
        toolBar.Toggle((int)i);
    }

    bool Mouse_inside() {
        return toolBar.Mouse_Inside();
    };

    void Update(f2 scale) {
        toolBar.Update(scale);
    }

    bool Click_Button() {
        return toolBar.Click_Button();
    }

    void Hover_Highlight() {
        if (!UI_toolbar::Mouse_inside())
            return;

        toolBar.Hover_Highlight();
    }

    void Render() {
        toolBar.Display();
    }

    bool Close_All() {
        return toolBar.Close_All();
    }

    Buttons Get_Open() {
        auto i = toolBar.Get_Open();
        if (i == -1)
            return Buttons::SIZE;
        return (Buttons)i;
    }
}