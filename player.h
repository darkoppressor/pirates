/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef player_h
#define player_h

#include <coords.h>

#include <cstdint>

class Player {
    private:
        // global chunk coordinates
        Coords<std::int32_t> globalChunkPosition;

    public:
        Player ();

        void reset();

        const Coords<std::int32_t>& getGlobalChunkPosition() const;
        /**
         * [setGlobalChunkPosition]
         * @param spawnPosition [the spawn position as global tile coordinates]
         */
        void setGlobalChunkPosition(const Coords<std::int32_t>& spawnPosition);
};

#endif
