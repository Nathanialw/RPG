add_subdirectory(Components)
add_subdirectory(unit_control)

target_sources (RPG_game
        PUBLIC
        character_data.h
        character_stats.h
        death_spells.h
        equipment.h
        items.h
        recruitable_units.h
        skills.h
        spells.h
        unit_status.h
)

target_include_directories(RPG_game PUBLIC ${CMAKE_CURRENT_LIST_DIR})
