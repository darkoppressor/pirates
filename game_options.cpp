/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_options.h"

#include <engine_strings.h>

using namespace std;

bool Game_Options::chunkCacheLog = false;
bool Game_Options::get_option (string name, string& value) {
    if (name == "cl_chunk_cache_log") {
        value = Strings::bool_to_string(chunkCacheLog);

        return true;
    }

    return false;
}

void Game_Options::set_option (string name, string value) {
    if (name == "cl_chunk_cache_log") {
        chunkCacheLog = Strings::string_to_bool(value);
    }
}
