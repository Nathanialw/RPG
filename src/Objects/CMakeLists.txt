target_sources(RPG
        PUBLIC
        Stats/character_data.h
        Stats/character_stats.h
        Items/equipment.h
        Items/items.h
        recruitable_units.h
        Stats/unit_status.h
        Items/equip_player_start.h
        Items/equip_unit.h
        ../Components/action_components.h
        ../Components/item_components.h
        ../Components/ai_components.h
        ai_control.h
        Collision/collision.h
        Combat/combat_control.h
        create_entities.h
        Combat/death_control.h
        Social/dialogue.h
        entity_control.h
        Collision/formation_collisions.h
        Movement/formation_positions.h
        mounts.h
        squad.h
        squad_control.h
        unit_control.h
        Movement/unit_formations.h
        Movement/unit_positions.h
        ../Game/load_zone.h
)


target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})
