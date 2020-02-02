/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "chunk_cache.h"
#include "game_data.h"
#include "game_constants.h"
#include "game.h"
#include "game_options.h"

#include <log.h>
#include <engine_strings.h>

#include <SDL_thread.h>

using namespace std;

ChunkCachingData::ChunkCachingData () {
    worldDirectory = "";
}

ChunkCachingData::ChunkCachingData (const ChunkCachingData& chunkCachingData) {
    worldDirectory = chunkCachingData.worldDirectory;
    globalChunkDimensions = chunkCachingData.globalChunkDimensions;

    for (const auto& coords : chunkCachingData.chunkPoolGlobalChunkPositions) {
        chunkPoolGlobalChunkPositions.push_back(coords);
    }

    worldGlobalChunkPosition = chunkCachingData.worldGlobalChunkPosition;
}

SDL_mutex* ChunkCache::lock = 0;
SDL_cond* ChunkCache::conditionProduce = 0;
SDL_cond* ChunkCache::conditionStartProduction = 0;
vector<ChunkCachingData> ChunkCache::productionOrders;
bool ChunkCache::productionInProgress = false;
vector<Chunk*> ChunkCache::chunkCachingBuffer;
void ChunkCache::log (string message) {
    if (Game_Options::chunkCacheLog) {
        Log::add_log(message);
    }
}

void ChunkCache::error (string message) {
    if (Game_Options::chunkCacheLog) {
        Log::add_error(message);
    }
}

bool ChunkCache::lockMutex () {
    if (!SDL_LockMutex(lock)) {
        return true;
    } else {
        error("Error locking for chunk caching: " + string(SDL_GetError()));

        return false;
    }
}

void ChunkCache::unlockMutex () {
    if (SDL_UnlockMutex(lock)) {
        error("Error unlocking for chunk caching: " + string(SDL_GetError()));
    }
}

void ChunkCache::waitToProduce () {
    if (SDL_CondWait(conditionProduce, lock)) {
        error("Error waiting on condition to produce: " + string(SDL_GetError()));
    }
}

void ChunkCache::waitToStartProduction () {
    if (SDL_CondWait(conditionStartProduction, lock)) {
        error("Error waiting on condition to start production: " + string(SDL_GetError()));
    }
}

void ChunkCache::signalProduction () {
    if (SDL_CondSignal(conditionProduce)) {
        error("Error signaling condition to produce: " + string(SDL_GetError()));
    }
}

void ChunkCache::signalStartProduction () {
    if (SDL_CondSignal(conditionStartProduction)) {
        error("Error signaling condition to start production: " + string(SDL_GetError()));
    }
}

void ChunkCache::clear () {
    if (lock) {
        SDL_DestroyMutex(lock);
        lock = 0;
    }

    if (conditionProduce) {
        SDL_DestroyCond(conditionProduce);
        conditionProduce = 0;
    }

    if (conditionStartProduction) {
        SDL_DestroyCond(conditionStartProduction);
        conditionStartProduction = 0;
    }

    productionOrders.clear();
    productionInProgress = false;

    for (auto chunk : chunkCachingBuffer) {
        delete chunk;
    }

    chunkCachingBuffer.clear();
}

bool ChunkCache::setup () {
    lock = SDL_CreateMutex();

    if (!lock) {
        error("Error creating lock for chunk caching: " + string(SDL_GetError()));

        return false;
    }

    conditionProduce = SDL_CreateCond();

    if (!conditionProduce) {
        error("Error creating conditionProduce: " + string(SDL_GetError()));

        return false;
    }

    conditionStartProduction = SDL_CreateCond();

    if (!conditionStartProduction) {
        error("Error creating conditionStartProduction: " + string(SDL_GetError()));

        return false;
    }

    return true;
}

void ChunkCache::placeOrder (const ChunkCachingData& chunkCachingData) {
    if (lockMutex()) {
        log("Chunk cache: order placed centered at (" +
            Strings::num_to_string(chunkCachingData.worldGlobalChunkPosition.x) + ", " +
            Strings::num_to_string(chunkCachingData.worldGlobalChunkPosition.y) + ")");

        productionOrders.clear();
        productionOrders.push_back(chunkCachingData);

        unlockMutex();
    }
}

void ChunkCache::manage () {
    if (lockMutex()) {
        if (!chunkCachingBuffer.empty()) {
            log("Chunk cache: management consuming " + Strings::num_to_string(chunkCachingBuffer.size()) + " chunks");
        }

        for (auto chunk : chunkCachingBuffer) {
            Game::addToChunkPool(chunk);
        }

        chunkCachingBuffer.clear();

        unlockMutex();

        signalProduction();

        if (lockMutex()) {
            if (!productionOrders.empty()) {
                log("Chunk cache: new production order present");

                if (productionInProgress) {
                    log("Chunk cache: production in progress, waiting to start production");

                    waitToStartProduction();
                }

                log("Chunk cache: starting production");

                productionInProgress = true;
                ChunkCachingData* data = new ChunkCachingData(productionOrders.back());
                productionOrders.clear();

                unlockMutex();

                SDL_Thread* producer =
                    SDL_CreateThread(cacheChunksInBackground, "cacheChunksInBackground", (void*) data);

                if (producer) {
                    SDL_DetachThread(producer);
                } else {
                    error("Error creating thread for cacheChunksInBackground: " + string(SDL_GetError()));
                }
            } else {
                unlockMutex();
            }
        }
    }
}

bool ChunkCache::cacheChunk (ChunkCachingData* chunkCachingData, const Coords<int32_t>& globalChunkPosition,
                             Chunk* chunk) {
    for (const auto& chunkPoolCoords : chunkCachingData->chunkPoolGlobalChunkPositions) {
        if (chunkPoolCoords == globalChunkPosition) {
            log("Chunk cache: skipping chunk load as it is already in the pool");

            return false;
        }
    }

    Game_Data::loadChunk(chunkCachingData->worldDirectory, globalChunkPosition, chunk);

    return true;
}

int ChunkCache::cacheChunksInBackground (void* data) {
    ChunkCachingData* chunkCachingData = (ChunkCachingData*) data;

    for (int32_t x = chunkCachingData->worldGlobalChunkPosition.x - Game_Constants::CHUNK_CACHE_RANGE;
         x <= chunkCachingData->worldGlobalChunkPosition.x + Game_Constants::CHUNK_CACHE_RANGE; x++) {
        for (int32_t y = chunkCachingData->worldGlobalChunkPosition.y - Game_Constants::CHUNK_CACHE_RANGE;
             y <= chunkCachingData->worldGlobalChunkPosition.y + Game_Constants::CHUNK_CACHE_RANGE; y++) {
            if (x >= 0 && x < chunkCachingData->globalChunkDimensions.x && y >= 0 &&
                y < chunkCachingData->globalChunkDimensions.y) {
                log("Chunk cache: loading chunk (" + Strings::num_to_string(x) + ", " + Strings::num_to_string(y) +
                    ")");

                Chunk* chunk = new Chunk;

                if (cacheChunk(chunkCachingData, Coords<int32_t>(x, y), chunk)) {
                    if (lockMutex()) {
                        log("Chunk cache: chunk loaded into memory");

                        if (!productionOrders.empty()) {
                            log("Chunk cache: new production order present, halting production");

                            productionInProgress = false;

                            unlockMutex();

                            signalStartProduction();

                            delete (ChunkCachingData*) data;
                            data = 0;

                            return 0;
                        }

                        if (chunkCachingBuffer.size() >= Game_Constants::MAXIMUM_CHUNKS_IN_CACHE) {
                            log("Chunk cache: cache full, waiting to produce");

                            waitToProduce();
                        }

                        log("Chunk cache: caching loaded chunk");

                        chunkCachingBuffer.push_back(chunk);

                        if (!productionOrders.empty()) {
                            log("Chunk cache: new production order present, halting production");

                            productionInProgress = false;

                            unlockMutex();

                            signalStartProduction();

                            delete (ChunkCachingData*) data;
                            data = 0;

                            return 0;
                        }

                        unlockMutex();
                    }
                }
            } else {
                log("Chunk cache: skipping chunk (" + Strings::num_to_string(x) + ", " + Strings::num_to_string(
                        y) + ") as it is outside of the world");
            }
        }
    }

    if (lockMutex()) {
        log("Chunk cache: production finished");

        productionInProgress = false;

        unlockMutex();

        signalStartProduction();
    }

    delete (ChunkCachingData*) data;
    data = 0;

    return 0;
}
