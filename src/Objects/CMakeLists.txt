add_subdirectory(object_components)
add_subdirectory(unit_control)
add_subdirectory(equip_unit)

target_sources(RPG
  PUBLIC
  character_data.h
  character_stats.h  
  equipment.h
  items.h
  recruitable_units.h        
  unit_status.h
  )

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})
