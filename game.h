/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_h
#define game_h

#include "world_type.h"
#include "tile.h"
#include "player.h"
#include "ship.h"

#include <rng.h>
#include <collision.h>

#include <string>
#include <vector>
#include <cstdint>

class Game {
    private:
        static RNG rng;
        static std::string selectedWorldType;
        static WorldType* worldType;
        static std::vector<std::vector<Tile>> tiles;
        static Player player;
        // ships[0] represents the player's flagship
        static std::vector<Ship> ships;
        // chunks
        static std::int32_t getChunkWidth();
        // chunks
        static std::int32_t getChunkHeight();
        // The returned collision rectangle's values are:
        // x => starting x tile (inclusive)
        // y => starting y tile (inclusive)
        // w => ending x tile (exclusive)
        // h => ending y tile (exclusive)
        static Collision_Rect<std::int32_t> getCameraTileBox();
        static const Player& getPlayer();

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
        // tiles
        static std::int32_t getTileWidth();
        // tiles
        static std::int32_t getTileHeight();
        // pixels
        static double getPixelWidth();
        // pixels
        static double getPixelHeight();
        static const std::vector<std::vector<Tile>>& getTiles();
        static Ship& getPlayerFlagship();
        static void playerFlagshipSteer(const std::string& direction);
        static bool moveInputState(const std::string& direction);
};

#endif
