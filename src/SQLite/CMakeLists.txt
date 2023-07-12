target_sources(RPG
        PUBLIC
        db.h
        SQLite_dialogue.h
        SQLite_item_data.h
        SQLite_spell_data.h
        SQLite_spritesheets.h
        SQLite_unit_data.h
)

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})
