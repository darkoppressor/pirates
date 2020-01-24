/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include <window.h>
#include <log.h>
#include <engine_input.h>
#include <object_manager.h>

using namespace std;

void Window::exec_close_function () {
    if (close_function.length() > 0) {
        if (close_function == "configure_command") {
            Object_Manager::configure_command = -1;
        } else {
            Log::add_error("Invalid close function: '" + close_function + "'");
        }
    }
}
