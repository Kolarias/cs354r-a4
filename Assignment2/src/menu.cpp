#include "menu.h"

namespace Menu
{
void Menu::_register_methods()
{
    register_method("_ready", &Menu::_ready);
    register_method("_process", &Menu::_process);
    register_method("_on_start_pressed", &Menu::_on_start_pressed);
    register_method("_on_quit_pressed", &Menu::_on_quit_pressed);
}

void Menu::_init() 
{
}

void Menu::_ready() 
{
    Button* start_button = Object::cast_to<Button>(Node::get_node("/root/Menu/VBoxContainer/Start"));
    Button* quit_button = Object::cast_to<Button>(Node::get_node("/root/Menu/VBoxContainer/Quit"));
    start_button->connect("pressed", this, "_on_start_pressed");
    quit_button->connect("pressed", this, "_on_quit_pressed");
}

void Menu::_process(float delta)
{
}

void Menu::_on_start_pressed() 
{
    get_tree()->change_scene("res://Level.tscn");
}

void Menu::_on_quit_pressed() 
{
    get_tree()->quit();
}

}
