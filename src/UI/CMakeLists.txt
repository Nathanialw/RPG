target_sources(RPG
  PUBLIC
  damage_text.h
  interface.h
  menu.h
  spellbook.h
  text_box.h
  tooltips.h
  ui.h
  ui_elements.h
  ui_resources.h
  ui_actionbar.h
  ui_unit_control.hpp
  unit_frames.h
  game_menu.h
  )

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})
