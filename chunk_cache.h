/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef chunk_cache_h
#define chunk_cache_h

#include "chunk.h"

#include <coords.h>

#include <SDL_mutex.h>

#include <vector>
#include <string>
#include <cstdint>

class ChunkCachingData {
    public:
        std::string worldDirectory;
        Coords<std::int32_t> globalChunkDimensions;
        std::vector<Coords<std::int32_t>> chunkPoolGlobalChunkPositions;
        Coords<std::int32_t> worldGlobalChunkPosition;

        ChunkCachingData ();
        ChunkCachingData (const ChunkCachingData& chunkCachingData);
};

class ChunkCache {
    private:
        static SDL_mutex* lock;
        static SDL_cond* conditionProduce;
        static SDL_cond* conditionStartProduction;

        // lockable objects:
        static std::vector<ChunkCachingData> productionOrders;
        static bool productionInProgress;
        static std::vector<Chunk*> chunkCachingBuffer;

        /**
         * [log log a message if the chunk cache log option is enabled]
         * @param message [the message to be logged]
         */
        static void log(std::string message);
        /**
         * [error log an error message if the chunk cache log option is enabled]
         * @param message [the message to be logged]
         */
        static void error(std::string message);

        /**
         * [lockMutex]
         * @return [true if the lock was successful]
         */
        static bool lockMutex();
        static void unlockMutex();
        static void waitToProduce();
        static void waitToStartProduction();
        static void signalProduction();
        static void signalStartProduction();

    public:
        static void clear();
        /**
         * [setup]
         * @return [true if successful, false if any errors]
         */
        static bool setup();
        static void placeOrder(const ChunkCachingData& chunkCachingData);
        static void manage();

        /**
         * [cacheChunk]
         * @param  chunkCachingData
         * @param  globalChunkPosition
         * @param  chunk            [the chunk for data to be loaded into]
         * @return                  [true if the chunk was loaded, false if the chunk was already in the pool]
         */
        static bool cacheChunk(ChunkCachingData* chunkCachingData, const Coords<std::int32_t>& globalChunkPosition,
                               Chunk* chunk);
        static int cacheChunksInBackground(void* data);
};

#endif
