/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"
#include "game_data.h"
#include "game_constants.h"

#include <render.h>
#include <game_window.h>
#include <sound_manager.h>
#include <game_manager.h>
#include <object_manager.h>
#include <engine_data.h>
#include <engine.h>
#include <log.h>

using namespace std;

RNG Game::rng;
string Game::selectedWorldType;
WorldType* Game::worldType;
vector<vector<Tile>> Game::tiles;
Player Game::player;
vector<Ship> Game::ships;
int32_t Game::getChunkWidth () {
    return worldType ? worldType->width : 0;
}
int32_t Game::getChunkHeight () {
    return worldType ? worldType->height : 0;
}

Collision_Rect<int32_t> Game::getCameraTileBox () {
    int32_t cameraTileX = (int32_t) (Game_Manager::camera.x / (Game_Constants::TILE_SIZE * Game_Manager::camera_zoom));
    int32_t cameraTileY = (int32_t) (Game_Manager::camera.y / (Game_Constants::TILE_SIZE * Game_Manager::camera_zoom));
    int32_t endTileX = cameraTileX +
                       (int32_t) (Game_Manager::camera.w / (Game_Constants::TILE_SIZE * Game_Manager::camera_zoom)) + 2;
    int32_t endTileY = cameraTileY +
                       (int32_t) (Game_Manager::camera.h / (Game_Constants::TILE_SIZE * Game_Manager::camera_zoom)) + 2;

    return Collision_Rect<int32_t>(cameraTileX, cameraTileY, endTileX, endTileY);
}

void Game::clear_world () {
    selectedWorldType = "";
    worldType = 0;
    tiles.clear();
    player.reset();
    ships.clear();
    Game_Data::unloadEmptyChunks();
}

void Game::generate_world () {
    clear_world();

    // QQQ For now we will hardcode the world name
    selectedWorldType = "caribbean";

    RNG rngSeeder;
    rng.seed(rngSeeder.random_range(0, numeric_limits<uint32_t>::max()));

    worldType = Game_Data::getWorldType(selectedWorldType);

    Game_Data::loadEmptyChunks(worldType->directory);

    player.setGlobalChunkPosition(worldType->spawnPosition);

    tiles.resize(Game_Constants::LOCAL_WORLD_SIZE* Game_Constants::CHUNK_SIZE,
                 vector<Tile>(Game_Constants::LOCAL_WORLD_SIZE* Game_Constants::CHUNK_SIZE));

    for (int32_t x = 0, globalX = player.getGlobalChunkPosition().x - 1; x < Game_Constants::LOCAL_WORLD_SIZE;
         x++, globalX++) {
        for (int32_t y = 0, globalY = player.getGlobalChunkPosition().y - 1; y < Game_Constants::LOCAL_WORLD_SIZE;
             y++, globalY++) {
            Game_Data::loadChunk(worldType->directory, Coords<int32_t>(globalX, globalY), tiles, Coords<int32_t>(x, y));
        }
    }

    ships.push_back(Ship("0",
                         Coords<int32_t>(Game_Constants::CHUNK_SIZE + worldType->spawnPosition.x -
                                         player.getGlobalChunkPosition().x *
                                         Game_Constants::CHUNK_SIZE,
                                         Game_Constants::CHUNK_SIZE + worldType->spawnPosition.y -
                                         player.getGlobalChunkPosition().y *
                                         Game_Constants::CHUNK_SIZE)));
}

void Game::tick () {}

void Game::ai () {}

void Game::movement () {
    for (size_t i = 0; i < ships.size(); i++) {
        ships[i].accelerate();
    }

    for (size_t i = 0; i < ships.size(); i++) {
        ships[i].movement();
    }
}

void Game::events () {
    Sound_Manager::set_listener(getPlayerFlagship().getBox().center_x(),
                                getPlayerFlagship().getBox().center_y(), Game_Manager::camera_zoom);
}

void Game::animate () {
    Collision_Rect<int32_t> cameraTileBox = getCameraTileBox();

    // Animate each on-screen tile
    for (int32_t x = cameraTileBox.x; x < cameraTileBox.w; x++) {
        for (int32_t y = cameraTileBox.y; y < cameraTileBox.h; y++) {
            if (x >= 0 && y >= 0 && x < getTileWidth() && y < getTileHeight()) {
                tiles[x][y].animate();
            }
        }
    }

    getPlayerFlagship().animate();
}

void Game::render () {
    Collision_Rect<int32_t> cameraTileBox = getCameraTileBox();

    // Render each on-screen tile
    for (int32_t x = cameraTileBox.x; x < cameraTileBox.w; x++) {
        for (int32_t y = cameraTileBox.y; y < cameraTileBox.h; y++) {
            if (x >= 0 && y >= 0 && x < getTileWidth() && y < getTileHeight()) {
                tiles[x][y].render(Coords<int32_t>(x, y));
            }
        }
    }

    getPlayerFlagship().render();
}

void Game::render_to_textures () {
    /**Rtt_Manager::set_render_target("example");
       ///Render something here
       Rtt_Manager::reset_render_target();*/
}

void Game::update_background () {}

void Game::render_background () {
    Render::render_rectangle(0.0, 0.0, Game_Window::width(), Game_Window::height(), 1.0, "ui_black");
}

int32_t Game::getTileWidth () {
    return getChunkWidth() * Game_Constants::CHUNK_SIZE;
}
int32_t Game::getTileHeight () {
    return getChunkHeight() * Game_Constants::CHUNK_SIZE;
}

double Game::getPixelWidth () {
    return getTileWidth() * Game_Constants::TILE_SIZE;
}
double Game::getPixelHeight () {
    return getTileHeight() * Game_Constants::TILE_SIZE;
}

uint8_t Game::getSeaLevel () {
    return worldType ? worldType->seaLevel : 0;
}

uint8_t Game::getShallowsLevel () {
    return worldType ? worldType->shallowsLevel : 0;
}

uint8_t Game::getSandLevel () {
    return worldType ? worldType->sandLevel : 0;
}

const vector<vector<Tile>>& Game::getTiles () {
    return tiles;
}

const Player& Game::getPlayer () {
    return player;
}

Ship& Game::getPlayerFlagship () {
    if (ships.size() > 0) {
        return ships[0];
    } else {
        Log::add_error("Error accessing player's flagship");

        Engine::quit();
    }
}

bool Game::moveInputState (const string& direction) {
    if (Object_Manager::game_command_state(direction)) {
        return true;
    }

    if (direction == "left" &&
        Object_Manager::game_command_state("move_stick_horizontal") < -Engine_Data::controller_dead_zone) {
        return true;
    } else if (direction == "right" &&
               Object_Manager::game_command_state("move_stick_horizontal") > Engine_Data::controller_dead_zone) {
        return true;
    }

    return false;
}
