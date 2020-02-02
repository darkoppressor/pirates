/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "ship.h"
#include "game_data.h"
#include "game_constants.h"
#include "game.h"

#include <engine_math.h>
#include <game_manager.h>
#include <engine.h>

#include <algorithm>

using namespace std;

ShipType* Ship::getType () const {
    return Game_Data::getShipType(type);
}

double Ship::getMass () const {
    return getType()->mass;
}

double Ship::getSteerForce () const {
    return getType()->steerForce;
}

double Ship::getMaximumAngularSpeed () const {
    return getType()->maximumAngularSpeed;
}

double Ship::getMaximumSpeed () const {
    return getType()->maximumSpeed;
}

Collision_Rect<double> Ship::getCollisionBox () const {
    return Collision_Rect<double>(localPosition.x + Game_Constants::SHIP_COLLISION_REDUCTION,
                                  localPosition.y + Game_Constants::SHIP_COLLISION_REDUCTION,
                                  sprite.get_width() - Game_Constants::SHIP_COLLISION_REDUCTION* 2.0,
                                  sprite.get_height() - Game_Constants::SHIP_COLLISION_REDUCTION* 2.0);
}

void Ship::stop () {
    velocity *= 0.0;
    force *= 0.0;
    angularVelocity = 0.0;
    angularForce = 0.0;
}

void Ship::steer () {
    if (steering) {
        if (steerDirectionLeft) {
            angularForce += getSteerForce();
        } else {
            angularForce -= getSteerForce();
        }
    }
}

void Ship::go () {
    if (going) {
        force += Vector(68038.0, angle);
    }
}

bool Ship::tileCollision (const Coords<double>& oldLocalPosition, double oldAngle) {
    Coords<int32_t> tilePosition = getTilePosition();
    Collision_Rect<double> collisionBox = getCollisionBox();
    int32_t tileCheckPadding = max(1, (int32_t) ((collisionBox.w + collisionBox.h) / 2.0 / Game_Constants::TILE_SIZE));

    for (int32_t x = tilePosition.x - tileCheckPadding; x < tilePosition.x + tileCheckPadding + 1; x++) {
        for (int32_t y = tilePosition.y - tileCheckPadding; y < tilePosition.y + tileCheckPadding + 1; y++) {
            if (x >= 0 && y >= 0 && x < Game::getLocalTileWidth() && y < Game::getLocalTileHeight()) {
                Coords<int32_t> tileCoords(x, y);
                Collision_Rect<double> tileBox = Tile::getBox(tileCoords);

                if (Collision::check_rect_rotated(collisionBox, tileBox, angle, 0.0)) {
                    if (Game::getTile(tileCoords).isSolid()) {
                        localPosition = oldLocalPosition;
                        angle = oldAngle;

                        stop();

                        return true;
                    }
                }
            }
        }
    }

    return false;
}

Ship::Ship (const string& type, const Coords<int32_t>& localTilePosition) {
    this->type = type;

    angle = 0.0;
    angularVelocity = 0.0;
    angularForce = 0.0;
    steerDirectionLeft = false;
    steering = false;
    going = false;

    sprite.set_name("ship_" + getType()->sprite);

    localPosition = Coords<double>(
        localTilePosition.x * Game_Constants::TILE_SIZE + Game_Constants::TILE_SIZE / 2.0 - sprite.get_width() / 2.0,
        localTilePosition.y * Game_Constants::TILE_SIZE + Game_Constants::TILE_SIZE / 2.0 - sprite.get_height() / 2.0);

    lastAngularForce = 0.0;
}

Collision_Rect<double> Ship::getBox () const {
    ShipType* type = getType();

    return Collision_Rect<double>(localPosition.x, localPosition.y, sprite.get_width(), sprite.get_height());
}

Coords<int32_t> Ship::getTilePosition () const {
    Collision_Rect<double> box = getBox();

    return Coords<int32_t>(box.center_x() / Game_Constants::TILE_SIZE, box.center_y() / Game_Constants::TILE_SIZE);
}

Coords<int32_t> Ship::getChunkPosition () const {
    Collision_Rect<double> box = getBox();

    return Coords<int32_t>(box.center_x() / Game_Constants::TILE_SIZE / Game_Constants::CHUNK_SIZE,
                           box.center_y() / Game_Constants::TILE_SIZE / Game_Constants::CHUNK_SIZE);
}

Coords<int32_t> Ship::getGlobalTilePosition () const {
    Coords<int32_t> globalChunkPosition = getGlobalChunkPosition();
    Coords<int32_t> tilePosition = getTilePosition();

    return Coords<int32_t>(globalChunkPosition.x * Game_Constants::CHUNK_SIZE +
                           (tilePosition.x - Game_Constants::CHUNK_SIZE),
                           globalChunkPosition.y * Game_Constants::CHUNK_SIZE +
                           (tilePosition.y - Game_Constants::CHUNK_SIZE));
}

Coords<int32_t> Ship::getGlobalChunkPosition () const {
    // The global chunk that corresponds to local chunk 1, 1
    Coords<int32_t> worldGlobalChunkPosition = Game::getGlobalChunkPosition();
    // local chunk coordinates
    Coords<int32_t> chunkPosition = getChunkPosition();

    return Coords<int32_t>(worldGlobalChunkPosition.x + (chunkPosition.x - 1),
                           worldGlobalChunkPosition.y + (chunkPosition.y - 1));
}

double Ship::getAngle () const {
    return angle;
}

double Ship::getAngularVelocity () const {
    return angularVelocity;
}

double Ship::getLastAngularForce () const {
    return lastAngularForce;
}

Vector Ship::getVelocity () const {
    return velocity;
}

Vector Ship::getLastForce () const {
    return lastForce;
}

void Ship::setSteerDirection (const string& direction) {
    if (direction != "none") {
        if (direction == "left") {
            steerDirectionLeft = true;
        } else if (direction == "right") {
            steerDirectionLeft = false;
        }

        steering = true;
    } else {
        steering = false;
    }
}

void Ship::setGoing (bool going) {
    this->going = going;
}

void Ship::accelerate () {
    steer();
    go();

    Vector acceleration = force / getMass();

    velocity += acceleration;

    if (velocity.magnitude > getMaximumSpeed()) {
        velocity.magnitude = getMaximumSpeed();
    } else if (velocity.magnitude < -getMaximumSpeed()) {
        velocity.magnitude = -getMaximumSpeed();
    }

    lastForce = force;
    force *= 0.0;

    double angularAcceleration = angularForce / getMass();

    angularVelocity += angularAcceleration;

    if (angularVelocity > getMaximumAngularSpeed()) {
        angularVelocity = getMaximumAngularSpeed();
    } else if (angularVelocity < -getMaximumAngularSpeed()) {
        angularVelocity = -getMaximumAngularSpeed();
    }

    lastAngularForce = angularForce;
    angularForce = 0.0;
}

void Ship::movement (bool isPlayer) {
    Vector_Components vc = velocity.get_components();
    double movementX = vc.a / Engine::UPDATE_RATE;
    double movementY = vc.b / Engine::UPDATE_RATE;
    double angularMovement = angularVelocity / Engine::UPDATE_RATE;

    for (int32_t i = 0; i < Game_Constants::SHIP_COLLISION_STEPS; i++) {
        Coords<double> oldLocalPosition = localPosition;
        double oldAngle = angle;

        localPosition.x += movementX / Game_Constants::SHIP_COLLISION_STEPS;
        localPosition.y += movementY / Game_Constants::SHIP_COLLISION_STEPS;
        angle += angularMovement / Game_Constants::SHIP_COLLISION_STEPS;
        Math::clamp_angle(angle);

        if (tileCollision(oldLocalPosition, oldAngle)) {
            break;
        }
    }

    Collision_Rect<double> collisionBox = getCollisionBox();

    if (isPlayer) {
        // QQQ handle player ship going outside of the global world

        Coords<int32_t> chunkPosition = getChunkPosition();
        Coords<int32_t> globalChunkMovement(chunkPosition.x - 1, chunkPosition.y - 1);

        if (globalChunkMovement != Coords<int32_t>(0, 0)) {
            Game::updateGlobalChunkPosition(globalChunkMovement);
        }
    } else {
        if (localPosition.x < 0.0) {
            localPosition.x = 0.0;
            stop();
        }

        if (localPosition.y < 0.0) {
            localPosition.y = 0.0;
            stop();
        }

        if (collisionBox.x + collisionBox.w >=
            (Game_Constants::LOCAL_WORLD_SIZE - 1) * Game_Constants::CHUNK_SIZE* Game_Constants::TILE_SIZE) {
            localPosition.x = Game::getLocalPixelWidth() - collisionBox.w;
            stop();
        }

        if (collisionBox.y + collisionBox.h >=
            (Game_Constants::LOCAL_WORLD_SIZE - 1) * Game_Constants::CHUNK_SIZE* Game_Constants::TILE_SIZE) {
            localPosition.y = Game::getLocalPixelHeight() - collisionBox.h;
            stop();
        }
    }
}

void Ship::chunkMove (const Coords<int32_t>& globalChunkMovement) {
    if (globalChunkMovement.x == 1) {
        localPosition.x -= Game_Constants::CHUNK_SIZE * Game_Constants::TILE_SIZE;
    } else if (globalChunkMovement.x == -1) {
        localPosition.x += Game_Constants::CHUNK_SIZE * Game_Constants::TILE_SIZE;
    }

    if (globalChunkMovement.y == 1) {
        localPosition.y -= Game_Constants::CHUNK_SIZE * Game_Constants::TILE_SIZE;
    } else if (globalChunkMovement.y == -1) {
        localPosition.y += Game_Constants::CHUNK_SIZE * Game_Constants::TILE_SIZE;
    }
}

void Ship::animate () {
    sprite.animate();
}

void Ship::render () const {
    Collision_Rect<double> box = getBox();

    if (Collision::check_rect_rotated(box * Game_Manager::camera_zoom, Game_Manager::camera, angle, 0.0)) {
        sprite.render(box.x * Game_Manager::camera_zoom - Game_Manager::camera.x,
                      box.y * Game_Manager::camera_zoom - Game_Manager::camera.y, 1.0, 1.0, 1.0, angle);
    }
}
