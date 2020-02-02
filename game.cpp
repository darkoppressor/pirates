/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"
#include "game_data.h"
#include "game_constants.h"
#include "chunk_cache.h"

#include <render.h>
#include <game_window.h>
#include <sound_manager.h>
#include <game_manager.h>
#include <object_manager.h>
#include <engine_data.h>
#include <engine.h>
#include <log.h>
#include <engine_strings.h>

#include <SDL.h>

using namespace std;

RNG Game::rng;
string Game::selectedWorldType = "";
WorldType* Game::worldType = 0;
Coords<int32_t> Game::globalChunkPosition;
deque<Chunk*> Game::chunkPool;
vector<vector<size_t>> Game::chunks;
Player Game::player;
vector<Ship> Game::ships;
void Game::setGlobalChunkPosition (const Coords<int32_t>& spawnPosition) {
    globalChunkPosition = Coords<int32_t>(spawnPosition.x / Game_Constants::CHUNK_SIZE,
                                          spawnPosition.y / Game_Constants::CHUNK_SIZE);
}

int32_t Game::getChunkWidth () {
    return worldType ? worldType->width : 0;
}
int32_t Game::getChunkHeight () {
    return worldType ? worldType->height : 0;
}

int32_t Game::getLocalChunkWidth () {
    return Game_Constants::LOCAL_WORLD_SIZE;
}
int32_t Game::getLocalChunkHeight () {
    return Game_Constants::LOCAL_WORLD_SIZE;
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

size_t Game::getChunk (const Coords<int32_t>& globalChunkPosition) {
    bool chunkRetrieved = false;
    size_t chunkIndex = 0;

    // Retrieve the chunk from the chunk pool if possible
    for (size_t i = 0; i < chunkPool.size(); i++) {
        if (chunkPool[i]->getGlobalChunkPosition() == globalChunkPosition) {
            // Renew this chunk's position in the chunk pool

            chunkPool.push_back(chunkPool[i]);
            chunkPool.erase(chunkPool.begin() + i);

            chunkErased(i);

            chunkRetrieved = true;
            chunkIndex = chunkPool.size() - 1;

            break;
        }
    }

    if (!chunkRetrieved) {
        Chunk* chunk = new Chunk;
        Game_Data::loadChunk(worldType->directory, globalChunkPosition, chunk);
        chunkPool.push_back(chunk);

        chunkRetrieved = true;
        chunkIndex = chunkPool.size() - 1;
    }

    if (!chunkRetrieved) {
        Log::add_error("Error accessing chunk");

        Engine::quit();
    }

    return chunkIndex;
}

void Game::chunkErased (size_t chunkIndex) {
    for (int32_t x = 0; x < getLocalChunkWidth(); x++) {
        for (int32_t y = 0; y < getLocalChunkHeight(); y++) {
            if (chunks[x][y] > chunkIndex) {
                chunks[x][y]--;
            }
        }
    }
}

void Game::cleanChunkPool () {
    for (size_t i = 0; i < chunkPool.size() && chunkPool.size() > Game_Constants::MAXIMUM_CHUNKS_IN_POOL; i++) {
        bool chunkInUseLocally = false;

        for (int32_t x = 0; x < getLocalChunkWidth() && !chunkInUseLocally; x++) {
            for (int32_t y = 0; y < getLocalChunkHeight(); y++) {
                if (chunks[x][y] == i) {
                    chunkInUseLocally = true;
                    break;
                }
            }
        }

        if (!chunkInUseLocally) {
            delete chunkPool[i];
            chunkPool.erase(chunkPool.begin() + i);
            chunkErased(i);
        }
    }
}

void Game::clear_world () {
    selectedWorldType = "";
    worldType = 0;
    globalChunkPosition.x = 0;
    globalChunkPosition.y = 0;
    ChunkCache::clear();

    for (auto chunk : chunkPool) {
        delete chunk;
    }

    chunkPool.clear();
    chunks.clear();
    player.clear();
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

    setGlobalChunkPosition(worldType->spawnPosition);

    chunks.resize(getLocalChunkWidth(), vector<size_t>(getLocalChunkHeight()));

    ships.push_back(Ship("0",
                         Coords<int32_t>(Game_Constants::CHUNK_SIZE + worldType->spawnPosition.x -
                                         globalChunkPosition.x *
                                         Game_Constants::CHUNK_SIZE,
                                         Game_Constants::CHUNK_SIZE + worldType->spawnPosition.y -
                                         globalChunkPosition.y * Game_Constants::CHUNK_SIZE)));

    if (!ChunkCache::setup()) {
        Engine::quit();
    }

    updateGlobalChunkPosition(Coords<int32_t>(0, 0));
}

void Game::tick () {
    ChunkCache::manage();
}

void Game::ai () {}

void Game::movement () {
    for (auto& ship : ships) {
        ship.accelerate();
    }

    for (size_t i = 0; i < ships.size(); i++) {
        ships[i].movement(i == 0);
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
            if (x >= 0 && y >= 0 && x < getLocalTileWidth() && y < getLocalTileHeight()) {
                getTile(Coords<int32_t>(x, y)).animate();
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
            if (x >= 0 && y >= 0 && x < getLocalTileWidth() && y < getLocalTileHeight()) {
                getTile(Coords<int32_t>(x, y)).render(Coords<int32_t>(x, y));
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

const deque<Chunk*>& Game::getChunkPool () {
    return chunkPool;
}

void Game::addToChunkPool (Chunk* chunk) {
    chunkPool.push_back(chunk);

    cleanChunkPool();
}

const Coords<int32_t>& Game::getGlobalChunkPosition () {
    return globalChunkPosition;
}

void Game::updateGlobalChunkPosition (const Coords<int32_t>& globalChunkMovement) {
    globalChunkPosition.x += globalChunkMovement.x;
    globalChunkPosition.y += globalChunkMovement.y;

    for (int32_t x = 0, globalX = globalChunkPosition.x - 1; x < getLocalChunkWidth();
         x++, globalX++) {
        for (int32_t y = 0, globalY = globalChunkPosition.y - 1; y < getLocalChunkHeight();
             y++, globalY++) {
            size_t chunkIndex = getChunk(Coords<int32_t>(globalX, globalY));

            if (chunks[x][y] != chunkIndex) {
                chunks[x][y] = chunkIndex;
            }
        }
    }

    cleanChunkPool();

    if (globalChunkMovement != Coords<int32_t>(0, 0)) {
        for (auto& ship : ships) {
            ship.chunkMove(globalChunkMovement);
        }
    }

    ChunkCachingData chunkCachingData;
    chunkCachingData.worldDirectory = worldType->directory;
    chunkCachingData.globalChunkDimensions = Coords<int32_t>(getChunkWidth(), getChunkHeight());

    for (auto chunk : chunkPool) {
        chunkCachingData.chunkPoolGlobalChunkPositions.push_back(chunk->getGlobalChunkPosition());
    }

    chunkCachingData.worldGlobalChunkPosition = globalChunkPosition;

    ChunkCache::placeOrder(chunkCachingData);
}

Tile& Game::getTile (const Coords<int32_t>& localTilePosition) {
    if (localTilePosition.x >= 0 && localTilePosition.y >= 0 && localTilePosition.x < getLocalTileWidth() &&
        localTilePosition.y < getLocalTileHeight()) {
        int32_t chunkX = localTilePosition.x / Game_Constants::CHUNK_SIZE;
        int32_t chunkY = localTilePosition.y / Game_Constants::CHUNK_SIZE;

        return chunkPool[chunks[chunkX][chunkY]]->getTile(Coords<int32_t>(localTilePosition.x - chunkX *
                                                                          Game_Constants::CHUNK_SIZE,
                                                                          localTilePosition.y - chunkY *
                                                                          Game_Constants::CHUNK_SIZE));
    } else {
        Log::add_error("Error accessing local tile (" + Strings::num_to_string(
                           localTilePosition.x) + ", " + Strings::num_to_string(localTilePosition.y) + ")");

        Engine::quit();
    }
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

int32_t Game::getLocalTileWidth () {
    return getLocalChunkWidth() * Game_Constants::CHUNK_SIZE;
}
int32_t Game::getLocalTileHeight () {
    return getLocalChunkWidth() * Game_Constants::CHUNK_SIZE;
}

double Game::getLocalPixelWidth () {
    return getLocalTileWidth() * Game_Constants::TILE_SIZE;
}
double Game::getLocalPixelHeight () {
    return getLocalTileHeight() * Game_Constants::TILE_SIZE;
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
