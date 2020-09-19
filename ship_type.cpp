/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "ship_type.h"

using namespace std;

ShipType::ShipType () {
    name = "";
    displayName = "";
    sprite = "";
    mass = 0.0;
    steerForce = 0.0;
    maximumAngularSpeed = 0.0;
    maximumSpeed = 0.0;
}
