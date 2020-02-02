/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_h
#define game_h

#include "world_type.h"
#include "chunk.h"
#include "player.h"
#include "ship.h"
#include "tile.h"

#include <rng.h>
#include <collision.h>
#include <coords.h>

#include <string>
#include <vector>
#include <cstdint>
#include <deque>

class Game {
    private:
        static RNG rng;
        static std::string selectedWorldType;
        static WorldType* worldType;
        // global chunk coordinates
        static Coords<std::int32_t> globalChunkPosition;
        // loaded chunks
        static std::deque<Chunk*> chunkPool;
        // local chunks
        static std::vector<std::vector<std::size_t>> chunks;
        static Player player;
        // ships[0] represents the player's flagship
        static std::vector<Ship> ships;
        /**
         * [setGlobalChunkPosition]
         * @param spawnPosition [the spawn position as global tile coordinates]
         */
        static void setGlobalChunkPosition(const Coords<std::int32_t>& spawnPosition);
        // global chunks
        static std::int32_t getChunkWidth();
        // global chunks
        static std::int32_t getChunkHeight();
        // local chunks
        static std::int32_t getLocalChunkWidth();
        // local chunks
        static std::int32_t getLocalChunkHeight();
        // The returned collision rectangle's values are:
        // x => starting x tile (inclusive)
        // y => starting y tile (inclusive)
        // w => ending x tile (exclusive)
        // h => ending y tile (exclusive)
        static Collision_Rect<std::int32_t> getCameraTileBox();
        static std::size_t getChunk(const Coords<std::int32_t>& globalChunkPosition);
        static void chunkErased(std::size_t chunkIndex);
        /**
         * [cleanChunkPool delete the oldest chunk in the chunk pool that is not in use locally]
         */
        static void cleanChunkPool();

    public:
        static void clear_world();
        static void generate_world();
        static void tick();
        static void ai();
        static void movement();
        static void events();
        static void animate();
        static void render();
        static void render_to_textures();
        static void update_background();
        static void render_background();
        static const std::deque<Chunk*>& getChunkPool();
        static void addToChunkPool(Chunk* chunk);
        static const Coords<std::int32_t>& getGlobalChunkPosition();
        static void updateGlobalChunkPosition(const Coords<std::int32_t>& globalChunkMovement);
        static Tile& getTile(const Coords<std::int32_t>& localTilePosition);
        // global tiles
        static std::int32_t getTileWidth();
        // global tiles
        static std::int32_t getTileHeight();
        // global pixels
        static double getPixelWidth();
        // global pixels
        static double getPixelHeight();
        // local tiles
        static std::int32_t getLocalTileWidth();
        // local tiles
        static std::int32_t getLocalTileHeight();
        // local pixels
        static double getLocalPixelWidth();
        // local pixels
        static double getLocalPixelHeight();
        static std::uint8_t getSeaLevel();
        static std::uint8_t getShallowsLevel();
        static std::uint8_t getSandLevel();
        static const Player& getPlayer();
        static Ship& getPlayerFlagship();
        static bool moveInputState(const std::string& direction);
};

#endif
