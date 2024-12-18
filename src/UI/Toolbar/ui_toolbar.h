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


namespace  UI_toolbar {

    enum class Buttons {
        inventory,
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
                    {"generalskills", Skills::General::Toggle},
                    {"mageskills", Skills::Mage::Toggle},
                    {"rogueskills", Skills::Rogue::Toggle},
                    {"warriorskills", Skills::Warrior::Toggle},
                    {"perks", PLACEHOLDER},
                    {"stats", PLACEHOLDER},
                    {"events", PLACEHOLDER},
                    {"map", Minimap::Toggle},
                    {"religion", PLACEHOLDER}
            }},
            "buttonon",
            "buttonoff",
            32.0f
    );

    void Toggle(Buttons i) {
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
        toolBar.Hover_Highlight();
    }

    void Render() {
        toolBar.Display();
    }

    bool Close_All() {
        return toolBar.Close_All();
    }
}