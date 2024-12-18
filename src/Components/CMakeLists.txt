target_sources(RPG
        PUBLIC
        body_components.h
        classes.h
        components.h
        player_components.h
        movement_components.h
        building_components.h
        collision_components.h
        skills_Components.h
)

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})
