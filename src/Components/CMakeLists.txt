target_sources(RPG_game
        PUBLIC
        body_components.h
        classes.h
        components.h
        player_components.h
)

target_include_directories(RPG_game PUBLIC ${CMAKE_CURRENT_LIST_DIR})
