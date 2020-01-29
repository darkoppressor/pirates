/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_data.h"

#include <data_manager.h>
#include <data_reader.h>
#include <engine_strings.h>
#include <log.h>

#include <boost/algorithm/string.hpp>

using namespace std;

vector<WorldType> Game_Data::worldTypes;
vector<TileType> Game_Data::tileTypes;
vector<ShipType> Game_Data::shipTypes;

///Don't forget to increment this for each progress item in load_data_game() below
const int Game_Data::game_data_load_item_count = 3;
void Game_Data::load_data_game (Progress_Bar& bar) {
    bar.progress("Loading worlds");
    Data_Manager::load_data("world");

    bar.progress("Loading tiles");
    Data_Manager::load_data("tile");

    bar.progress("Loading ships");
    Data_Manager::load_data("ship");
}

void Game_Data::load_data_tag_game (string tag, File_IO_Load* load) {
    if (tag == "world") {
        loadWorldTypes(load);
    } else if (tag == "tile") {
        loadTileTypes(load);
    } else if (tag == "ship") {
        loadShipTypes(load);
    }
}

void Game_Data::unload_data_game () {
    worldTypes.clear();
    tileTypes.clear();
    shipTypes.clear();
}

void Game_Data::loadWorldTypes (File_IO_Load* load) {
    worldTypes.push_back(WorldType());

    vector<string> lines = Data_Reader::read_data(load, "</world>");

    for (size_t i = 0; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "name:")) {
            worldTypes.back().name = line;
        } else if (Data_Reader::check_prefix(line, "displayName:")) {
            worldTypes.back().displayName = line;
        } else if (Data_Reader::check_prefix(line, "directory:")) {
            worldTypes.back().directory = line;
        } else if (Data_Reader::check_prefix(line, "dimensions:")) {
            vector<string> dimensions;
            boost::algorithm::split(dimensions, line, boost::algorithm::is_any_of("x"));

            if (dimensions.size() >= 2) {
                worldTypes.back().width = Strings::string_to_long(dimensions[0]);
                worldTypes.back().height = Strings::string_to_long(dimensions[1]);
            }
        } else if (Data_Reader::check_prefix(line, "spawnPosition:")) {
            vector<string> spawnPosition;
            boost::algorithm::split(spawnPosition, line, boost::algorithm::is_any_of(","));

            if (spawnPosition.size() >= 2) {
                worldTypes.back().spawnPosition.x = Strings::string_to_long(spawnPosition[0]);
                worldTypes.back().spawnPosition.y = Strings::string_to_long(spawnPosition[1]);
            }
        }
    }
}

WorldType* Game_Data::getWorldType (string name) {
    WorldType* ptr_object = 0;

    for (size_t i = 0; i < worldTypes.size(); i++) {
        if (worldTypes[i].name == name) {
            ptr_object = &worldTypes[i];

            break;
        }
    }

    if (ptr_object == 0) {
        Log::add_error("Error accessing world '" + name + "'");
    }

    return ptr_object;
}

void Game_Data::loadTileTypes (File_IO_Load* load) {
    tileTypes.push_back(TileType());

    vector<string> lines = Data_Reader::read_data(load, "</tile>");

    for (size_t i = 0; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "name:")) {
            tileTypes.back().name = line;
        } else if (Data_Reader::check_prefix(line, "sprite:")) {
            tileTypes.back().sprite = line;
        } else if (Data_Reader::check_prefix(line, "solid:")) {
            tileTypes.back().solid = Strings::string_to_bool(line);
        }
    }
}

TileType* Game_Data::getTileType (string name) {
    TileType* ptr_object = 0;

    for (size_t i = 0; i < tileTypes.size(); i++) {
        if (tileTypes[i].name == name) {
            ptr_object = &tileTypes[i];

            break;
        }
    }

    if (ptr_object == 0) {
        Log::add_error("Error accessing tile '" + name + "'");
    }

    return ptr_object;
}

void Game_Data::loadShipTypes (File_IO_Load* load) {
    shipTypes.push_back(ShipType());

    vector<string> lines = Data_Reader::read_data(load, "</ship>");

    for (size_t i = 0; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "name:")) {
            shipTypes.back().name = line;
        } else if (Data_Reader::check_prefix(line, "displayName:")) {
            shipTypes.back().displayName = line;
        } else if (Data_Reader::check_prefix(line, "sprite:")) {
            shipTypes.back().sprite = line;
        } else if (Data_Reader::check_prefix(line, "dimensions:")) {
            vector<string> dimensions;
            boost::algorithm::split(dimensions, line, boost::algorithm::is_any_of("x"));

            if (dimensions.size() >= 2) {
                shipTypes.back().width = Strings::string_to_double(dimensions[0]);
                shipTypes.back().height = Strings::string_to_double(dimensions[1]);
            }
        } else if (Data_Reader::check_prefix(line, "mass:")) {
            shipTypes.back().mass = Strings::string_to_double(line);
        } else if (Data_Reader::check_prefix(line, "steerForce:")) {
            shipTypes.back().steerForce = Strings::string_to_double(line);
        } else if (Data_Reader::check_prefix(line, "maximumAngularSpeed:")) {
            shipTypes.back().maximumAngularSpeed = Strings::string_to_double(line);
        } else if (Data_Reader::check_prefix(line, "maximumSpeed:")) {
            shipTypes.back().maximumSpeed = Strings::string_to_double(line);
        }
    }
}

ShipType* Game_Data::getShipType (string name) {
    ShipType* ptr_object = 0;

    for (size_t i = 0; i < shipTypes.size(); i++) {
        if (shipTypes[i].name == name) {
            ptr_object = &shipTypes[i];

            break;
        }
    }

    if (ptr_object == 0) {
        Log::add_error("Error accessing ship '" + name + "'");
    }

    return ptr_object;
}
