add_subdirectory(Components)
add_subdirectory(unit_control)
add_subdirectory(equip_unit)

include_directories(Components)
include_directories(unit_control)
include_directories(equip_unit)

target_sources (RPG
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

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})
