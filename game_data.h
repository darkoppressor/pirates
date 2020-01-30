/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_data_h
#define game_data_h

#include "world_type.h"
#include "tile_type.h"
#include "ship_type.h"
#include "tile.h"

#include <progress_bar.h>
#include <file_io.h>
#include <coords.h>

#include <vector>
#include <string>
#include <unordered_set>
#include <cstdint>
#include <functional>

class CoordsHasher {
    public:
        size_t operator() (const Coords<std::int32_t>& coords) const {
            return (std::hash<std::int32_t>() (coords.x) ^ (std::hash<std::int32_t>() (coords.y) << 1)) >> 1;
        }
};

class Game_Data {
    private:
        static std::vector<WorldType> worldTypes;
        static std::vector<TileType> tileTypes;
        static std::vector<ShipType> shipTypes;
        // global chunk coordinates of chunks with no corresponding data file
        static std::unordered_set<Coords<std::int32_t>, CoordsHasher> emptyChunks;

    public:
        // The total number of progress bar items in load_data_game()
        static const int game_data_load_item_count;
        static void load_data_game(Progress_Bar& bar);
        // Load any data of the passed tag type
        // Returns false if passed tag type's data could not be loaded
        // Returns true otherwise
        static void load_data_tag_game(std::string tag, File_IO_Load* load);
        static void unload_data_game();
        static void loadWorldTypes(File_IO_Load* load);
        static WorldType* getWorldType(std::string name);
        static void loadTileTypes(File_IO_Load* load);
        static TileType* getTileType(std::string name);
        static void loadShipTypes(File_IO_Load* load);
        static ShipType* getShipType(std::string name);
        static void loadEmptyChunks(const std::string& worldDirectory);
        static void unloadEmptyChunks();
        /**
         * [loadChunk load data from a chunk file into the tiles of a given local chunk]
         * @param worldDirectory      [the world directory for the world to load chunks from]
         * @param globalChunk [global chunk coordinates of the data file to load]
         * @param tiles               [the tiles to load the chunk data into]
         * @param localChunk  [the local chunk coordinates of the chunk of tiles to load the chunk data into]
         */
        static void loadChunk(const std::string& worldDirectory, const Coords<std::int32_t>& globalChunk,
                              std::vector<std::vector<Tile>>& tiles, const Coords<std::int32_t>& localChunk);
};

#endif
