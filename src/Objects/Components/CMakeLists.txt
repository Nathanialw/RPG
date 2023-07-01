target_sources(RPG_game
        PUBLIC
        action_components.h
        item_components.h
)

target_include_directories(RPG_game PUBLIC ${CMAKE_CURRENT_LIST_DIR})
