target_sources(RPG
  PUBLIC
  character_data.h
  character_stats.h  
  equipment.h
  items.h
  recruitable_units.h        
  unit_status.h
    equip_player_start.h
  equip_unit.h
    action_components.h
  item_components.h
    ai_components.h
  ai_control.h
  collision.h
  combat_control.h
  create_entities.h
  death_control.h
  dialogue.h
  entity_control.h
  formation_collisions.h
  formation_positions.h
  mounts.h
  squad.h
  squad_control.h
  unit_control.h
  unit_formations.h
  unit_positions.h
  )


target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})
