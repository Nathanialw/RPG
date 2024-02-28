target_sources(RPG
        PUBLIC
        init.h
        main_menu.h
        Game_Start/character_create.h
        game_loop.h
        close.h
        load_object_list.h
        settings.h
        game_objects.h
        splash_screen.h
)

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})
