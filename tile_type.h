/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef tile_type_h
#define tile_type_h

#include <string>

class TileType {
    public:
        std::string name;
        std::string sprite;
        bool solid;

        TileType ();
};

#endif
