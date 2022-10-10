#include "menu.h"

namespace Menu
{
void Menu::_register_methods()
{
    register_method("_ready", &Menu::_ready);
    register_method("_process", &Menu::_process);
    register_method("_on_start_pressed", &Menu::_on_start_pressed);
    register_method("_on_host_pressed", &Menu::_on_host_pressed);
    register_method("_on_join_pressed", &Menu::_on_join_pressed);
    register_method("_on_quit_pressed", &Menu::_on_quit_pressed);
}

void Menu::_init() 
{
}

void Menu::_ready() 
{
    Button* start_button = Object::cast_to<Button>(Node::get_node("/root/Menu/VBoxContainer/Start"));
    Button* host_button = Object::cast_to<Button>(Node::get_node("/root/Menu/VBoxContainer/Host"));
    Button* join_button = Object::cast_to<Button>(Node::get_node("/root/Menu/VBoxContainer/Join"));
    Button* quit_button = Object::cast_to<Button>(Node::get_node("/root/Menu/VBoxContainer/Quit"));
    start_button->connect("pressed", this, "_on_start_pressed");
    host_button->connect("pressed", this, "_on_host_pressed");
    join_button->connect("pressed", this, "_on_join_pressed");
    quit_button->connect("pressed", this, "_on_quit_pressed");
}

void Menu::_process(float delta)
{
}

void Menu::_on_start_pressed() 
{
    get_tree()->change_scene("res://Level.tscn");
}

void Menu::_on_host_pressed() 
{
}

void Menu::_on_join_pressed() 
{
}

void Menu::_on_quit_pressed() 
{
    get_tree()->quit();
}

}
