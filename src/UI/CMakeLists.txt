target_sources(RPG_game
        PUBLIC
        damage_text.h
        interface.h
        menu.h
        spellbook.h
        text_box.h
        tooltips.h
        ui.h
        ui_elements.h
        ui_resources.h
        ui_actionbar.h
)

target_include_directories(RPG_game PUBLIC ${CMAKE_CURRENT_LIST_DIR})
