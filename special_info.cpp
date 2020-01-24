/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include <special_info.h>
#include <log.h>
#include <object_manager.h>

using namespace std;

string Special_Info::get_special_info_text (string special_info) {
    string text = "";

    if (special_info.length() > 0) {
        if (special_info == "configure_command") {
            Object_Manager::output_command_configuration_info(text);
        } else {
            Log::add_error("Invalid special info text: '" + special_info + "'");
        }
    }

    return text;
}

string Special_Info::get_special_info_sprite (string special_info) {
    string str_sprite_name = "";

    if (special_info.length() > 0) {
        if (special_info == "example") {
            str_sprite_name = "";
        } else {
            Log::add_error("Invalid special info sprite: '" + special_info + "'");
        }
    }

    return str_sprite_name;
}
