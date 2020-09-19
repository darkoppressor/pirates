/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef world_type_h
#define world_type_h

#include <coords.h>

#include <string>
#include <cstdint>

class WorldType {
    public:
        std::string name;
        std::string displayName;
        std::string directory;
        // chunks
        std::int32_t width;
        // chunks
        std::int32_t height;
        std::uint8_t seaLevel;
        std::uint8_t shallowsLevel;
        std::uint8_t sandLevel;
        // global tile coordinates
        Coords<std::int32_t> spawnPosition;

        WorldType ();
};

#endif
