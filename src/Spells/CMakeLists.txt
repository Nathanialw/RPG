target_sources(RPG
        PUBLIC
        instant_attack.h
        sinister_strike.h
        death_spells.h
        spells.h
        skills.h
        Fire/fire.h
        spell_data.h
)

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})
