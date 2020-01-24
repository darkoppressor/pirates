/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_data.h"
#include "game_options.h"
#include "network_game.h"
#include "game.h"

#include <game_manager.h>
#include <game_option.h>
#include <network_engine.h>
#include <game_world.h>

using namespace std;

void Game_Manager::load_data_game (Progress_Bar& bar) {
    Game_Data::load_data_game(bar);
}

void Game_Manager::load_data_tag_game (string tag, File_IO_Load* load) {
    Game_Data::load_data_tag_game(tag, load);
}

void Game_Manager::unload_data_game () {
    Game_Data::unload_data_game();
}

bool Game_Option::game_get_value (string& value) {
    return Game_Options::get_option(name, value);
}

void Game_Option::game_set_value (string value) {
    Game_Options::set_option(name, value);
}

bool Network_Engine::receive_game_packet (RakNet::Packet* packet, const RakNet::MessageID& packet_id) {
    return Network_Game::receive_game_packet(packet, packet_id);
}

string Network_Engine::allow_new_connection () {
    return Network_Game::allow_new_connection();
}

void Network_Engine::write_initial_game_data (RakNet::BitStream& bitstream) {
    Network_Game::write_initial_game_data(bitstream);
}

void Network_Engine::read_initial_game_data (RakNet::BitStream& bitstream) {
    Network_Game::read_initial_game_data(bitstream);
}

void Network_Engine::write_update (RakNet::BitStream& bitstream) {
    Network_Game::write_update(bitstream);
}

void Network_Engine::read_update (RakNet::BitStream& bitstream) {
    Network_Game::read_update(bitstream);
}

void Network_Engine::write_server_ready (RakNet::BitStream& bitstream) {
    Network_Game::write_server_ready(bitstream);
}

void Network_Engine::read_server_ready (RakNet::BitStream& bitstream) {
    Network_Game::read_server_ready(bitstream);
}

void Network_Engine::write_client_ready (RakNet::BitStream& bitstream) {
    Network_Game::write_client_ready(bitstream);
}

void Network_Engine::read_client_ready (RakNet::BitStream& bitstream) {
    Network_Game::read_client_ready(bitstream);
}

void Game_World::clear_world () {
    Game::clear_world();
}

void Game_World::generate_world () {
    Game::generate_world();
}

void Game_World::tick () {
    Game::tick();
}

void Game_World::ai () {
    Game::ai();
}

void Game_World::movement () {
    Game::movement();
}

void Game_World::events () {
    Game::events();
}

void Game_World::animate () {
    Game::animate();
}

void Game_World::render () {
    Game::render();
}

void Game_World::render_to_textures () {
    Game::render_to_textures();
}

void Game_World::update_background () {
    Game::update_background();
}

void Game_World::render_background () {
    Game::render_background();
}
