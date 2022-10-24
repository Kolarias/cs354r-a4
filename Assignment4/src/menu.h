#ifndef MENU_H
#define MENU_H

#include "Common.h"
#include <SceneTree.hpp>
#include "Control.hpp"
#include "Button.hpp"

#include <string>

namespace Menu 
{
class Menu : public Control 
{
    GODOT_CLASS(Menu, Control);

private:


public:

    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);

    void _on_start_pressed();
    void _on_quit_pressed();
};
}

#endif
