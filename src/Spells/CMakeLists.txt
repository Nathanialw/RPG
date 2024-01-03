target_sources(RPG
        PUBLIC
        instant_attack.h
        sinister_strike.h
        death_spells.h
        spells.h
        skills.h
        Fire/fire.h
        Life/life.h
        Ice/ice.h
        lightning/lightning.h
        lightning/lightningball.h
        Nature/leafspell.h
        spell_data.h
        Necromancy/summon_demon.h
)

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})
