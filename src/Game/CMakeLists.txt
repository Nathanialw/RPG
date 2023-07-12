target_sources(RPG
        PUBLIC
        init.h
        main_menu.h
        char_create.h
        game_loop.h
        close.h
)

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})
