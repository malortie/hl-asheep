# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [asheep/2.0.0] - 2021-06-20

### Added

- CMake support
- Missing entity monster_adrian_dead
- Missing entity monster_kate_dead
- Missing entity monster_myself_dead
- Missing entity monster_terror_dead
- Missing entity monster_terror_repel

### Changed

- Custom cvars are now set by default to values from skill.cfg from Steampipe patch
- Tweaked alien slave exp beam color
- Tweaked reddish spawn effect beam color
- Reworked battery pickup deny code
- Reworked suit charger use deny code
- Reworked Kate medkit code
- Reworked CBaseMonsterRepel code
- Reworked armor pickup code
- Reworked suit pickup code
- Reworked monster_garbage code
- Reworked weapon holster code

### Removed

- Visual Studio 2017 projects files
- Hardcoded map fixes
- Experimental WON style viewmodel bob
- Entity env_funnel

### Fixed

- Missing fields in entities save/restore tables
- Wrong time base used for primary and secondary attack time
- Reload not canceling during weapon switching
- Missing bullet types
- Missing poolstick bullet type check
- Wrong macro used for 9mmM41a default give ammo
- Wrong macro used for barney9mmar default give ammo
- Wrong spread used in CBaseWeaponARWithGrenadeLauncher
- Grenades launched from 9mmM41a not using the correct skill value
- RPG playing wrong holster sequence when unloaded
- Beretta fast fire not playing fire empty sequence
- Kate medkit max carry ammo
- Grunt tossing grenade twice
- Grunt dropping weapon twice
- Grunt reload sound played twice
- Grunt death sound played twice
- Grunt voice attenuation
- Spforce occasionally playing the wrong reload sound
- Missing kick sound for grunt and spforce
- Human terror wrong bodygroup enum values
- Human terror speaking the wrong sentences
- Allies attacking Adrian
- Missing class names in friend array
- Panther trace attack effects not showing
- Panther collision box
- Panther damage absorbtion
- Players and monsters able to activate Kate only triggers
- monster_generic invincibility
- Alien slave exp not throwing the correct gib type
- Rat collision bounds
- Rat throwing alien gibs
- Grunt entity class relationship
- Rat entity class relationship
- Toad entity class relationship
- Finished implementation of trigger_sound
- Implemented missing monster_bodypart behavior
- Missing pistols weapon icons on pickup

## [asheep/1.0.1] - 2017-12-12

### Fixed

- Wrong worker dead poses
- Wrong grunt skin when using specific weapon
- Cards Barney not using the correct bodygroup

## [asheep/1.0.0] - 2017-07-29

### Added

- Azure sheep reimplementation source code
