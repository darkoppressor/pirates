/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "tile.h"
#include "game_constants.h"
#include "game_data.h"
#include "game.h"

#include <game_manager.h>

using namespace std;

TileType* Tile::getType () const {
    return Game_Data::getTileType(type);
}

Tile::Tile () {
    elevation = 0;
    type = "";
}

void Tile::setup (uint8_t elevation) {
    this->elevation = elevation;

    if (elevation <= Game::getSeaLevel()) {
        type = "sea";
    } else if (elevation <= Game::getShallowsLevel()) {
        type = "shallows";
    } else if (elevation <= Game::getSandLevel()) {
        type = "sand";
    } else {
        type = "grass";
    }

    sprite.set_name("tile_" + getType()->sprite);
}

bool Tile::isSolid () const {
    return getType()->solid;
}

Collision_Rect<double> Tile::getBox (const Coords<int32_t>& tileCoords) {
    return Collision_Rect<double>(tileCoords.x * Game_Constants::TILE_SIZE, tileCoords.y * Game_Constants::TILE_SIZE,
                                  Game_Constants::TILE_SIZE, Game_Constants::TILE_SIZE);
}

void Tile::animate () {
    sprite.animate();
}

void Tile::render (const Coords<int32_t>& tileCoords) const {
    Collision_Rect<double> box = getBox(tileCoords);

    if (Collision::check_rect(box * Game_Manager::camera_zoom, Game_Manager::camera)) {
        sprite.render(box.x * Game_Manager::camera_zoom - Game_Manager::camera.x,
                      box.y * Game_Manager::camera_zoom - Game_Manager::camera.y);
    }
}
