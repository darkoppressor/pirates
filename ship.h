/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef ship_h
#define ship_h

#include "ship_type.h"

#include <coords.h>
#include <math_vector.h>
#include <collision.h>
#include <sprite.h>

#include <string>
#include <cstdint>

class Ship {
    private:
        std::string type;
        // local pixels
        Coords<double> position;
        double angle;
        Vector velocity;
        Vector force;
        double angularVelocity;
        double angularForce;
        bool steerDirectionLeft;
        bool steering;
        bool braking;
        Sprite sprite;

        // Derived from type:
        ShipType* getType() const;
        double getMass() const;
        double getSteerForce() const;
        double getMaximumAngularSpeed() const;
        double getMaximumSpeed() const;

        // local pixels
        Collision_Rect<double> getCollisionBox() const;
        // local tile coordinates
        Coords<std::int32_t> getTilePosition() const;

        // For displaying dev info:
        double lastAngularForce;

        void stop();
        void steer();
        // Returns true if a collision was detected
        bool tileCollision(const Coords<double>& oldPosition, double oldAngle);

    public:
        Ship (const std::string& type, const Coords<double>& position);

        // local pixels
        Collision_Rect<double> getBox() const;

        // For displaying dev info:
        double getAngle() const;
        double getAngularVelocity() const;
        double getLastAngularForce() const;

        void setSteerDirection(const std::string& direction);
        void accelerate();
        void movement();

        void animate();
        void render() const;
};

#endif
