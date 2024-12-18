target_sources(RPG
        PUBLIC
        damage_text.h
        interface.h
        menu.h
        ui_spellbook.h
        text_box.h
        tooltips.h
        ui.h
        ui_elements.h
        ui_resources.h
        ui_actionbar.h
        ui_unit_control.hpp
        unit_frames.h
        game_menu.h
        ui_target_info.h
        ui_minimap.h
        ui_XP.h
        Toolbar/ui_toolbar.h

        Button_Bar/highlight.h
        Button_Bar/bar.h
        Button_Bar/button.h

        Toolbar/Skills/skills.h
        Toolbar/Skills/general.h
        Toolbar/Skills/rogue.h
        Toolbar/Skills/warrior.h
        Toolbar/Skills/mage.h
        Toolbar/Spells/high_power.h
        Toolbar/Spells/low_power.h
        Toolbar/Spells/mid_power.h
        Toolbar/attitudes.h
        Toolbar/events.h
        Toolbar/religion.h
        Toolbar/perks.h
)

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})
