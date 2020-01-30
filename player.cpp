/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "player.h"
#include "game_constants.h"

using namespace std;

Player::Player () {
    reset();
}

void Player::reset () {
    globalChunkPosition.x = 0;
    globalChunkPosition.y = 0;
}

const Coords<int32_t>& Player::getGlobalChunkPosition () const {
    return globalChunkPosition;
}

void Player::setGlobalChunkPosition (const Coords<int32_t>& spawnPosition) {
    globalChunkPosition = Coords<int32_t>(spawnPosition.x / Game_Constants::CHUNK_SIZE,
                                          spawnPosition.y / Game_Constants::CHUNK_SIZE);
}
