#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "Common.h"
#include <SceneTree.hpp>
#include "Control.hpp"
#include "Button.hpp"
#include "Input.hpp"
#include "InputEvent.hpp"
#include "Control.hpp"

#include <string>

namespace PauseMenu 
{
class PauseMenu : public Control 
{
    GODOT_CLASS(PauseMenu, Control);

private:

    Input* input;
    bool isPaused;
    Control* pauseMenu;

public:

    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);

    void _input(InputEvent* event);

    void _on_resume_pressed();
    void _on_main_menu_pressed();
    void _on_quit_pressed();
    void handle_music();
};
}

#endif
