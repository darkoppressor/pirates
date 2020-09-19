/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "chunk.h"
#include "game_constants.h"

#include <log.h>
#include <engine.h>
#include <engine_strings.h>

using namespace std;

Chunk::Chunk () {
    tiles.resize(Game_Constants::CHUNK_SIZE, vector<Tile>(Game_Constants::CHUNK_SIZE));
}

Coords<int32_t> Chunk::getGlobalChunkPosition () const {
    return globalChunkPosition;
}

void Chunk::setGlobalChunkPosition (const Coords<int32_t>& globalChunkPosition) {
    this->globalChunkPosition = globalChunkPosition;
}

Tile& Chunk::getTile (const Coords<int32_t>& localTilePosition) {
    if (localTilePosition.x >= 0 && localTilePosition.x < Game_Constants::CHUNK_SIZE && localTilePosition.y >= 0 &&
        localTilePosition.y < Game_Constants::CHUNK_SIZE) {
        return tiles[localTilePosition.x][localTilePosition.y];
    } else {
        Log::add_error("Error accessing chunk-local tile (" + Strings::num_to_string(
                           localTilePosition.x) + ", " + Strings::num_to_string(localTilePosition.y) + ")");

        Engine::quit();
    }
}
