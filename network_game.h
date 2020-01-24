/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef network_game_h
#define network_game_h

#include <network_message_identifiers.h>

#include <string>

#include "raknet/Source/BitStream.h"

enum {
    ///ID_GAME_EXAMPLE=ID_GAME_PACKET_ENUM
};

enum {
    ///ORDERING_CHANNEL_EXAMPLE=ORDERING_CHANNEL_GAME_PACKET_ENUM
};

class Network_Game {
    public:
        static bool receive_game_packet(RakNet::Packet* packet, const RakNet::MessageID& packet_id);

        // Returns an empty string if a new connection should be allowed
        // Otherwise, returns a string containing the reason a new connection is not allowed
        static std::string allow_new_connection();
        static void write_initial_game_data(RakNet::BitStream& bitstream);
        static void read_initial_game_data(RakNet::BitStream& bitstream);
        static void write_update(RakNet::BitStream& bitstream);
        static void read_update(RakNet::BitStream& bitstream);

        // These are used in the lockstep networking model
        static void write_server_ready(RakNet::BitStream& bitstream);
        static void read_server_ready(RakNet::BitStream& bitstream);
        static void write_client_ready(RakNet::BitStream& bitstream);
        static void read_client_ready(RakNet::BitStream& bitstream);
};

#endif
