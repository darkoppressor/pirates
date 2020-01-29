/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef tile_h
#define tile_h

#include "tile_type.h"

#include <collision.h>
#include <coords.h>
#include <sprite.h>

#include <string>
#include <cstdint>

class Tile {
    private:
        std::string type;
        Sprite sprite;

        // Derived from type:
        TileType* getType() const;

    public:
        Tile ();
        Tile (const std::string& type);

        // Derived from type:
        bool isSolid() const;

        // local pixels
        static Collision_Rect<double> getBox(const Coords<std::int32_t>& tileCoords);

        void animate();
        void render(const Coords<std::int32_t>& tileCoords) const;
};

#endif
