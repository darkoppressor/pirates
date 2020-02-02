/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"

#include <engine.h>
#include <game_manager.h>
#include <font.h>
#include <object_manager.h>
#include <options.h>
#include <network_engine.h>
#include <render.h>
#include <engine_strings.h>

using namespace std;

void Engine::render_dev_info () {
    string msg = "";

    if (Game_Manager::in_progress) {
        msg += "Camera Position: " + Strings::num_to_string(Game_Manager::camera.x) + "," + Strings::num_to_string(
            Game_Manager::camera.y) + "\n";
        msg += "Camera Size: " + Strings::num_to_string(Game_Manager::camera.w / Game_Manager::camera_zoom) + "," +
               Strings::num_to_string(Game_Manager::camera.h / Game_Manager::camera_zoom) + "\n";
        msg += "Camera Zoom: " + Strings::num_to_string(Game_Manager::camera_zoom) + "\n";

        /*msg += "\nChunk pool (" + Strings::num_to_string(Game::getChunkPool().size()) + "): \n";

           size_t i = 0;

           for (auto chunk : Game::getChunkPool()) {
            msg += " (" + Strings::num_to_string(chunk->getGlobalChunkPosition().x) + "," + Strings::num_to_string(
                chunk->getGlobalChunkPosition().y) + ")";

            if (i == 7) {
                i = 0;
                msg += "\n";
            } else {
                i++;
            }
           }

           msg += "\n";*/

        msg += "\nPlayer's flagship:\n------------------------------------------------------------\n";
        msg += " Global position:\n";
        msg += "  " + Strings::num_to_string(Game::getPlayerFlagship().getGlobalTilePosition().x) + " tiles, " +
               Strings::num_to_string(Game::getPlayerFlagship().getGlobalTilePosition().y) + " tiles\n";
        msg += "  " + Strings::num_to_string(Game::getPlayerFlagship().getGlobalChunkPosition().x) + " chunks, " +
               Strings::num_to_string(Game::getPlayerFlagship().getGlobalChunkPosition().y) + " chunks\n";
        msg += " Local position:\n";
        msg += "  " + Strings::num_to_string(Game::getPlayerFlagship().getBox().get_center().x) + " meters, " +
               Strings::num_to_string(Game::getPlayerFlagship().getBox().get_center().y) + " meters\n";
        msg += "  " + Strings::num_to_string(Game::getPlayerFlagship().getTilePosition().x) + " tiles, " +
               Strings::num_to_string(Game::getPlayerFlagship().getTilePosition().y) + " tiles\n";
        msg += "  " + Strings::num_to_string(Game::getPlayerFlagship().getChunkPosition().x) + " chunks, " +
               Strings::num_to_string(Game::getPlayerFlagship().getChunkPosition().y) + " chunks\n";
        msg += "\n Velocity: " + Strings::num_to_string(Game::getPlayerFlagship().getVelocity().magnitude) +
               " meters / second, " + Strings::num_to_string(Game::getPlayerFlagship().getVelocity().direction) +
               " degrees\n";
        msg += " Force: " + Strings::num_to_string(Game::getPlayerFlagship().getLastForce().magnitude) + " newtons, " +
               Strings::num_to_string(Game::getPlayerFlagship().getLastForce().direction) + " degrees\n";
        msg += " Angle: " + Strings::num_to_string(Game::getPlayerFlagship().getAngle()) + " degrees\n";
        msg += " Angular velocity: " + Strings::num_to_string(Game::getPlayerFlagship().getAngularVelocity()) +
               " degrees / second\n";
        msg += " Angular force: " + Strings::num_to_string(Game::getPlayerFlagship().getLastAngularForce()) +
               " newtons\n";
    }

    if (msg.length() > 0) {
        Bitmap_Font* font = Object_Manager::get_font("small");
        double y = 2.0;

        if (Options::fps) {
            y += font->spacing_y;

            if (Network_Engine::status != "off") {
                y += font->spacing_y * 2.0;
            }
        }

        Render::render_rectangle(2.0, y, Strings::longest_line(msg) * font->spacing_x, Strings::newline_count(
                                     msg) * font->spacing_y, 0.75, "ui_black");
        font->show(2.0, y, msg, "red");
    }
}
