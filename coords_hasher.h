/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef coords_hasher_h
#define coords_hasher_h

#include <coords.h>

#include <cstdint>
#include <functional>

class CoordsHasher {
    public:
        size_t operator() (const Coords<std::int32_t>& coords) const {
            return (std::hash<std::int32_t>() (coords.x) ^ (std::hash<std::int32_t>() (coords.y) << 1)) >> 1;
        }
};

#endif
