# Game/Server project configuration

set(TARGET_NAME hldll)

# Compile definitions
set(COMPILE_DEFS_BASE
  WIN32
  _WINDOWS
  QUIVER
  VOXEL
  QUAKE2
  VALVE_DLL
  CLIENT_WEAPONS
  _CRT_SECURE_NO_WARNINGS
  _CRT_NONSTDC_NO_WARNINGS
)

set(COMPILE_DEFS_DEBUG
  _DEBUG
)

set(COMPILE_DEFS_RELEASE
  NDEBUG
)

# Compile options
set(COMPILE_OPTS_DEBUG
  /MTd # Runtime library
)

set(COMPILE_OPTS_RELEASE
  /GL # Whole program optimization
  /Gy # Function level linking
  /MT # Runtime library
  /Oi # Generate intrinsic functions
)

# Linker options
set(LINK_OPTS_BASE
  /DEBUG # Generate Debug info
  /DEF:${CMAKE_CURRENT_SOURCE_DIR}/hl.def
  /SUBSYSTEM:WINDOWS
)

set(LINK_OPTS_RELEASE
  /OPT:ICF # Enable COMDAT Folding
  /OPT:REF # Optimize references
)

# Include directories
set(INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/${ASHEEP_DLLS_FOLDER}
  ${ENGINE_DIR}
  ${COMMON_DIR}
  ${PM_SHARED_DIR}
  ${GAME_SHARED_DIR}
  ${PUBLIC_DIR}
)

# Header files
set(HEADER_FILES
  activity.h
  activitymap.h
  animation.h
  basemonster.h
  cbase.h
  cdll_dll.h
  client.h
  decals.h
  defaultai.h
  doors.h
  effects.h
  enginecallback.h
  explode.h
  extdll.h
  flyingmonster.h
  func_break.h
  gamerules.h
  hornet.h
  items.h
  monsterevent.h
  monsters.h
  nodes.h
  plane.h
  player.h
  saverestore.h
  schedule.h
  scripted.h
  scriptevent.h
  skill.h
  soundent.h
  spectator.h
  squadmonster.h
  talkmonster.h
  teamplay_gamerules.h
  trains.h
  util.h
  vector.h
  weapons.h
  ${ENGINE_DIR}/eiface.h
  ${PM_SHARED_DIR}/pm_debug.h
  ${PM_SHARED_DIR}/pm_defs.h
  ${PM_SHARED_DIR}/pm_info.h
  ${PM_SHARED_DIR}/pm_materials.h
  ${PM_SHARED_DIR}/pm_movevars.h
  ${PM_SHARED_DIR}/pm_shared.h
  ${MOD_CONFIG_FILE_PATH}
  ${ASHEEP_DLLS_FOLDER}/basemonsterrepel.h
  ${ASHEEP_DLLS_FOLDER}/basemonsterwithheadcrabai.h
  ${ASHEEP_DLLS_FOLDER}/basemonsterwithichthyosaurai.h
  ${ASHEEP_DLLS_FOLDER}/basemonsterwithichthyosaurai_constants.h
  ${ASHEEP_DLLS_FOLDER}/basemonsterwithsnarkai.h
  ${ASHEEP_DLLS_FOLDER}/basemonsterwithzombieai.h
  ${ASHEEP_DLLS_FOLDER}/basemonsterwithzombieai_util.h
  ${ASHEEP_DLLS_FOLDER}/baseweaponarwithgrenadelauncher.h
  ${ASHEEP_DLLS_FOLDER}/baseweapongrenade.h
  ${ASHEEP_DLLS_FOLDER}/baseweaponmelee.h
  ${ASHEEP_DLLS_FOLDER}/baseweaponpistol.h
  ${ASHEEP_DLLS_FOLDER}/baseweaponshotgun.h
  ${ASHEEP_DLLS_FOLDER}/baseweaponwiththrowablemonster.h
  ${ASHEEP_DLLS_FOLDER}/combat_tactics.h
  ${ASHEEP_DLLS_FOLDER}/dead_actor.h
  ${ASHEEP_DLLS_FOLDER}/hgrunt.h
  ${ASHEEP_DLLS_FOLDER}/hterror.h
  ${ASHEEP_DLLS_FOLDER}/monsters_classnames.h
  ${ASHEEP_DLLS_FOLDER}/player_utils.h
  ${ASHEEP_DLLS_FOLDER}/scientist.h
  ${ASHEEP_DLLS_FOLDER}/room_type.h
  ${ASHEEP_DLLS_FOLDER}/spawnereffect.h
  ${ASHEEP_DLLS_FOLDER}/spforce.h
  ${ASHEEP_DLLS_FOLDER}/squadmonsterwithhgruntai.h
  ${ASHEEP_DLLS_FOLDER}/squadmonsterwithhgruntai_util.h
  ${ASHEEP_DLLS_FOLDER}/squadmonsterwithislaveai.h
  ${ASHEEP_DLLS_FOLDER}/squadmonsterwithislaveai_util.h
  ${ASHEEP_DLLS_FOLDER}/talkmonsterwithbarneyai.h
  ${ASHEEP_DLLS_FOLDER}/talkmonsterwithbarneyai_util.h
  ${ASHEEP_DLLS_FOLDER}/talkmonsterwithscientistai.h
  ${ASHEEP_DLLS_FOLDER}/toadweaponmonster.h
  ${ASHEEP_DLLS_FOLDER}/weaponinterface.h
  ${ASHEEP_DLLS_FOLDER}/weapon_util.h
)

# Source files
set(SOURCE_FILES 
  aflock.cpp
  agrunt.cpp
  airtank.cpp
  animating.cpp
  animation.cpp
  apache.cpp
  barnacle.cpp
  barney.cpp
  bigmomma.cpp
  bloater.cpp
  bmodels.cpp
  bullsquid.cpp
  buttons.cpp
  cbase.cpp
  client.cpp
  combat.cpp
  controller.cpp
  crossbow.cpp
  crowbar.cpp
  defaultai.cpp
  doors.cpp
  effects.cpp
  egon.cpp
  explode.cpp
  flyingmonster.cpp
  func_break.cpp
  func_tank.cpp
  game.cpp
  gamerules.cpp
  gargantua.cpp
  gauss.cpp
  genericmonster.cpp
  ggrenade.cpp
  globals.cpp
  gman.cpp
  handgrenade.cpp
  hassassin.cpp
  headcrab.cpp
  healthkit.cpp
  hgrunt.cpp
  hornet.cpp
  hornetgun.cpp
  houndeye.cpp
  h_ai.cpp
  h_battery.cpp
  h_cine.cpp
  h_cycler.cpp
  h_export.cpp
  ichthyosaur.cpp
  islave.cpp
  items.cpp
  leech.cpp
  lights.cpp
  maprules.cpp
  monstermaker.cpp
  monsters.cpp
  monsterstate.cpp
  mortar.cpp
  mp5.cpp
  multiplay_gamerules.cpp
  nihilanth.cpp
  nodes.cpp
  observer.cpp
  osprey.cpp
  pathcorner.cpp
  plane.cpp
  plats.cpp
  player.cpp
  python.cpp
  rat.cpp
  roach.cpp
  rpg.cpp
  satchel.cpp
  schedule.cpp
  scientist.cpp
  scripted.cpp
  shotgun.cpp
  singleplay_gamerules.cpp
  skill.cpp
  sound.cpp
  soundent.cpp
  spectator.cpp
  squadmonster.cpp
  squeakgrenade.cpp
  subs.cpp
  talkmonster.cpp
  teamplay_gamerules.cpp
  tempmonster.cpp
  tentacle.cpp
  triggers.cpp
  tripmine.cpp
  turret.cpp
  util.cpp 
  weapons.cpp 
  world.cpp 
  wpn_shared/hl_wpn_glock.cpp
  xen.cpp
  zombie.cpp
  ${GAME_SHARED_DIR}/voice_gamemgr.cpp
  ${ASHEEP_DLLS_FOLDER}/9mmm41a.cpp
  ${ASHEEP_DLLS_FOLDER}/adrian.cpp
  ${ASHEEP_DLLS_FOLDER}/archer.cpp
  ${ASHEEP_DLLS_FOLDER}/armor.cpp
  ${ASHEEP_DLLS_FOLDER}/asheep_triggers.cpp
  ${ASHEEP_DLLS_FOLDER}/asheep_weapons.cpp
  ${ASHEEP_DLLS_FOLDER}/barney9mmar.cpp
  ${ASHEEP_DLLS_FOLDER}/barney9mmhg.cpp
  ${ASHEEP_DLLS_FOLDER}/barneyhandgrenade.cpp
  ${ASHEEP_DLLS_FOLDER}/barneyshotgun.cpp
  ${ASHEEP_DLLS_FOLDER}/barniel.cpp
  ${ASHEEP_DLLS_FOLDER}/basemonsterrepel.cpp
  ${ASHEEP_DLLS_FOLDER}/basemonsterwithheadcrabai.cpp
  ${ASHEEP_DLLS_FOLDER}/basemonsterwithichthyosaurai.cpp
  ${ASHEEP_DLLS_FOLDER}/basemonsterwithichthyosaurai_constants.cpp
  ${ASHEEP_DLLS_FOLDER}/basemonsterwithsnarkai.cpp
  ${ASHEEP_DLLS_FOLDER}/basemonsterwithzombieai.cpp
  ${ASHEEP_DLLS_FOLDER}/basemonsterwithzombieai_util.cpp
  ${ASHEEP_DLLS_FOLDER}/baseweaponarwithgrenadelauncher.cpp
  ${ASHEEP_DLLS_FOLDER}/baseweapongrenade.cpp
  ${ASHEEP_DLLS_FOLDER}/baseweaponmelee.cpp
  ${ASHEEP_DLLS_FOLDER}/baseweaponpistol.cpp
  ${ASHEEP_DLLS_FOLDER}/baseweaponshotgun.cpp
  ${ASHEEP_DLLS_FOLDER}/baseweaponwiththrowablemonster.cpp
  ${ASHEEP_DLLS_FOLDER}/beretta.cpp
  ${ASHEEP_DLLS_FOLDER}/bodypart.cpp
  ${ASHEEP_DLLS_FOLDER}/combat_tactics.cpp
  ${ASHEEP_DLLS_FOLDER}/dead_actor.cpp
  ${ASHEEP_DLLS_FOLDER}/dead_adrian.cpp
  ${ASHEEP_DLLS_FOLDER}/dead_barney.cpp
  ${ASHEEP_DLLS_FOLDER}/dead_barniel.cpp
  ${ASHEEP_DLLS_FOLDER}/dead_gordon.cpp
  ${ASHEEP_DLLS_FOLDER}/dead_hevbarney.cpp
  ${ASHEEP_DLLS_FOLDER}/dead_hgrunt.cpp
  ${ASHEEP_DLLS_FOLDER}/dead_kate.cpp
  ${ASHEEP_DLLS_FOLDER}/dead_myself.cpp
  ${ASHEEP_DLLS_FOLDER}/dead_scientist.cpp
  ${ASHEEP_DLLS_FOLDER}/dead_spforce.cpp
  ${ASHEEP_DLLS_FOLDER}/dead_terror.cpp
  ${ASHEEP_DLLS_FOLDER}/dead_worker.cpp
  ${ASHEEP_DLLS_FOLDER}/exp_alien_slave.cpp
  ${ASHEEP_DLLS_FOLDER}/garbage.cpp
  ${ASHEEP_DLLS_FOLDER}/gordon.cpp
  ${ASHEEP_DLLS_FOLDER}/hevbarney.cpp
  ${ASHEEP_DLLS_FOLDER}/hgruntrepel.cpp
  ${ASHEEP_DLLS_FOLDER}/hterror.cpp
  ${ASHEEP_DLLS_FOLDER}/hterrorrepel.cpp
  ${ASHEEP_DLLS_FOLDER}/kate.cpp
  ${ASHEEP_DLLS_FOLDER}/kmedkit.cpp
  ${ASHEEP_DLLS_FOLDER}/myself.cpp
  ${ASHEEP_DLLS_FOLDER}/panther.cpp
  ${ASHEEP_DLLS_FOLDER}/player_utils.cpp
  ${ASHEEP_DLLS_FOLDER}/poolstick.cpp
  ${ASHEEP_DLLS_FOLDER}/room_type.cpp
  ${ASHEEP_DLLS_FOLDER}/spawnereffect.cpp
  ${ASHEEP_DLLS_FOLDER}/spforce.cpp
  ${ASHEEP_DLLS_FOLDER}/spforcerepel.cpp
  ${ASHEEP_DLLS_FOLDER}/squadmonsterwithhgruntai.cpp
  ${ASHEEP_DLLS_FOLDER}/squadmonsterwithhgruntai_util.cpp
  ${ASHEEP_DLLS_FOLDER}/squadmonsterwithislaveai.cpp
  ${ASHEEP_DLLS_FOLDER}/squadmonsterwithislaveai_util.cpp
  ${ASHEEP_DLLS_FOLDER}/talkmonsterwithbarneyai.cpp
  ${ASHEEP_DLLS_FOLDER}/talkmonsterwithbarneyai_util.cpp
  ${ASHEEP_DLLS_FOLDER}/talkmonsterwithscientistai.cpp
  ${ASHEEP_DLLS_FOLDER}/toad.cpp
  ${ASHEEP_DLLS_FOLDER}/toadweapon.cpp
  ${ASHEEP_DLLS_FOLDER}/toadweaponmonster.cpp
  ${ASHEEP_DLLS_FOLDER}/weapon_util.cpp
  ${ASHEEP_DLLS_FOLDER}/worker.cpp
  ${ASHEEP_DLLS_FOLDER}/zombie_barney.cpp
)

set(PM_SHARED_SOURCE_FILES
  ${PM_SHARED_DIR}/pm_debug.c
  ${PM_SHARED_DIR}/pm_math.c
  ${PM_SHARED_DIR}/pm_shared.c
)

set_source_files_properties(
  ${PM_SHARED_SOURCE_FILES}
  PROPERTIES
    LANGUAGE C
)

# Visual Studio filters
source_group(TREE ${PROJECT_SOURCE_DIR} 
  PREFIX "Header Files" 
  FILES ${HEADER_FILES}
)

source_group(TREE ${PROJECT_SOURCE_DIR} 
  PREFIX "Source Files" 
  FILES ${SOURCE_FILES} ${PM_SHARED_SOURCE_FILES}
)

# Game/Server target definition
add_library(${TARGET_NAME} MODULE 
  ${HEADER_FILES}
  ${SOURCE_FILES}
  ${PM_SHARED_SOURCE_FILES}
)

set_target_properties(${TARGET_NAME}
  PROPERTIES 
    OUTPUT_NAME hl
)

target_include_directories(${TARGET_NAME} PRIVATE ${INCLUDE_DIRS})

target_compile_definitions(${TARGET_NAME} PRIVATE
  ${COMPILE_DEFS_BASE}
  $<IF:$<CONFIG:Debug>,${COMPILE_DEFS_DEBUG},${COMPILE_DEFS_RELEASE}>
)

target_compile_options(${TARGET_NAME} PRIVATE
  $<IF:$<CONFIG:Debug>,${COMPILE_OPTS_DEBUG},${COMPILE_OPTS_RELEASE}>
)

target_link_options(${TARGET_NAME} PRIVATE
  ${LINK_OPTS_BASE}
  $<$<NOT:$<CONFIG:Debug>>:${LINK_OPTS_RELEASE}>
)

set(LAUNCH_OPT_DEBUG "-game ${MOD_FOLDER} -dev -console +sv_cheats 1")
set(LAUNCH_OPT_RELEASE "-game ${MOD_FOLDER} -console +sv_cheats 1")

# Configuration for launching from Visual Studio
set_target_properties(${TARGET_NAME}
  PROPERTIES 
    VS_DEBUGGER_COMMAND ${GAME_DIR}/hl.exe
    VS_DEBUGGER_COMMAND_ARGUMENTS $<IF:$<CONFIG:Debug>,${LAUNCH_OPT_DEBUG},${LAUNCH_OPT_RELEASE}>
    VS_DEBUGGER_WORKING_DIRECTORY ${GAME_DIR}
)

# Used to copy the DLL file to the dlls folder in mod directory.
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${TARGET_NAME}> ${MOD_DIR}/dlls/$<TARGET_FILE_NAME:${TARGET_NAME}>
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PDB_FILE:${TARGET_NAME}> ${MOD_DIR}/dlls/$<TARGET_PDB_FILE_NAME:${TARGET_NAME}>
  VERBATIM
)

# Installation
install(TARGETS ${TARGET_NAME} DESTINATION dlls)
