/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "main.h"
#include "game_data.h"

#include <main_startup.h>

using namespace std;

int main (int argc, char* args[]) {
    return main_startup(argc, args, Game_Data::game_data_load_item_count);
}
