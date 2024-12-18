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

    bool ACTION_PLACEHOLDER() {
        return false;
    }

    typedef bool (*Action)();

    Button::Bar<(int)Buttons::SIZE, Action> toolBar = Button::Bar<(int)Buttons::SIZE, Action>({
            {
                    {"inventory", Bag_UI::Toggle},
                    {"character", UI_Stats::Toggle},
                    {"lowpower", UI_Spellbook::Toggle},
                    {"midpower", ACTION_PLACEHOLDER},
                    {"highpower", ACTION_PLACEHOLDER},
                    {"generalskills", Skills::General::Toggle},
                    {"mageskills", Skills::Mage::Toggle},
                    {"rogueskills", Skills::Rogue::Toggle},
                    {"warriorskills", Skills::Warrior::Toggle},
                    {"perks", ACTION_PLACEHOLDER},
                    {"stats", ACTION_PLACEHOLDER},
                    {"events", ACTION_PLACEHOLDER},
                    {"map", Minimap::Toggle},
                    {"religion", ACTION_PLACEHOLDER}
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

    void Close_All() {
        toolBar.Close_All();
    }
}