/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_data.h"

using namespace std;

///vector<Example_Game_Tag> Game_Data::example_game_tags;

///Don't forget to increment this for each progress item in load_data_game() below
const int Game_Data::game_data_load_item_count = 0;
void Game_Data::load_data_game (Progress_Bar& bar) {
    ///bar.progress("Loading example game tag");
    ///Data_Manager::load_data("example_game_tag");
}

void Game_Data::load_data_tag_game (string tag, File_IO_Load* load) {
    /**if(tag=="example_game_tag"){
        load_example_game_tag(load);
       }*/
}

void Game_Data::unload_data_game () {
    ///example_game_tags.clear();
}

/**void Game_Data::load_example_game_tag(File_IO_Load* load){
    example_game_tags.push_back(Example_Game_Tag());

    vector<string> lines=Data_Reader::read_data(load,"</example_game_tag>");

    for(size_t i=0;i<lines.size();i++){
        string& line=lines[i];

        if(Data_Reader::check_prefix(line,"name:")){
            example_game_tags.back().name=line;
        }
    }
   }*/

/**Example_Game_Tag* Game_Data::get_example_game_tag(string name){
    Example_Game_Tag* ptr_object=0;

    for(size_t i=0;i<example_game_tags.size();i++){
        if(example_game_tags[i].name==name){
            ptr_object=&example_game_tags[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing example game tag '"+name+"'");
    }

    return ptr_object;
   }*/
