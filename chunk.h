/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef chunk_h
#define chunk_h

#include "tile.h"

#include <coords.h>

#include <vector>
#include <cstdint>

class Chunk {
    private:
        // The global chunk coordinates that this chunk is associated with
        Coords<std::int32_t> globalChunkPosition;
        std::vector<std::vector<Tile>> tiles;

    public:
        Chunk ();

        Coords<std::int32_t> getGlobalChunkPosition() const;
        void setGlobalChunkPosition(const Coords<std::int32_t>& globalChunkPosition);

        Tile& getTile(const Coords<std::int32_t>& localTilePosition);
};

#endif
