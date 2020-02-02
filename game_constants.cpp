/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_constants.h"

#include <game_constants_loader.h>
#include <engine_strings.h>
#include <engine_data.h>

using namespace std;

/// BEGIN SCRIPT-GENERATED CONSTANT INITIALIZATIONS
int32_t Game_Constants::CHUNK_SIZE=0;
double Game_Constants::TILE_SIZE=0.0;
int32_t Game_Constants::LOCAL_WORLD_SIZE=0;
uint32_t Game_Constants::MAXIMUM_CHUNKS_IN_CACHE=0;
uint32_t Game_Constants::MAXIMUM_CHUNKS_IN_POOL=0;
int32_t Game_Constants::CHUNK_CACHE_RANGE=0;
double Game_Constants::SHIP_COLLISION_REDUCTION=0.0;
int32_t Game_Constants::SHIP_COLLISION_STEPS=0;
/// END SCRIPT-GENERATED CONSTANT INITIALIZATIONS

void Game_Constants_Loader::set_game_constant (string name, string value) {
    if (name == "zoom_rate") {
        Engine_Data::ZOOM_RATE = Strings::string_to_double(value);
    } else if (name == "zoom_min") {
        Engine_Data::ZOOM_MIN = Strings::string_to_double(value);
    } else if (name == "zoom_max") {
        Engine_Data::ZOOM_MAX = Strings::string_to_double(value);
    }

    /// BEGIN SCRIPT-GENERATED CONSTANT SETUP
    else if(name=="chunk_size"){
        Game_Constants::CHUNK_SIZE=Strings::string_to_long(value);
    }
    else if(name=="tile_size"){
        Game_Constants::TILE_SIZE=Strings::string_to_double(value);
    }
    else if(name=="local_world_size"){
        Game_Constants::LOCAL_WORLD_SIZE=Strings::string_to_long(value);
    }
    else if(name=="maximum_chunks_in_cache"){
        Game_Constants::MAXIMUM_CHUNKS_IN_CACHE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="maximum_chunks_in_pool"){
        Game_Constants::MAXIMUM_CHUNKS_IN_POOL=Strings::string_to_unsigned_long(value);
    }
    else if(name=="chunk_cache_range"){
        Game_Constants::CHUNK_CACHE_RANGE=Strings::string_to_long(value);
    }
    else if(name=="ship_collision_reduction"){
        Game_Constants::SHIP_COLLISION_REDUCTION=Strings::string_to_double(value);
    }
    else if(name=="ship_collision_steps"){
        Game_Constants::SHIP_COLLISION_STEPS=Strings::string_to_long(value);
    }
    /// END SCRIPT-GENERATED CONSTANT SETUP
}
