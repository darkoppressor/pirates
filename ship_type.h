/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef ship_type_h
#define ship_type_h

#include <string>

class ShipType {
    public:
        std::string name;
        std::string displayName;
        std::string sprite;
        // pixels
        double width;
        // pixels
        double height;
        // kilograms
        double mass;
        // newtons
        double steerForce;
        // degrees / second
        double maximumAngularSpeed;
        // meters / second
        double maximumSpeed;

        ShipType ();
};

#endif
