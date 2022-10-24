#include "pausemenu.h"
#include "player.h"

namespace PauseMenu
{
void PauseMenu::_register_methods()
{
    register_method("_ready", &PauseMenu::_ready);
    register_method("_process", &PauseMenu::_process);
    register_method("_input", &PauseMenu::_input);
    register_method("_on_resume_pressed", &PauseMenu::_on_resume_pressed);
    register_method("_on_main_menu_pressed", &PauseMenu::_on_main_menu_pressed);
    register_method("_on_quit_pressed", &PauseMenu::_on_quit_pressed);
}

void PauseMenu::_init() 
{
    isPaused = false;
}

void PauseMenu::_ready() 
{
    pauseMenu = Object::cast_to<Control>(Node::get_node("/root/Level/PauseMenu"));

    Button* resume_button = Object::cast_to<Button>(Node::get_node("/root/Level/PauseMenu/CenterContainer/VBoxContainer/Resume"));
    Button* main_menu_button = Object::cast_to<Button>(Node::get_node("/root/Level/PauseMenu/CenterContainer/VBoxContainer/MainMenu"));
    Button* quit_button = Object::cast_to<Button>(Node::get_node("/root/Level/PauseMenu/CenterContainer/VBoxContainer/Quit"));
    resume_button->connect("pressed", pauseMenu, "_on_resume_pressed");
    main_menu_button->connect("pressed", pauseMenu, "_on_main_menu_pressed");
    quit_button->connect("pressed", pauseMenu, "_on_quit_pressed");

    // I don't know if this is necessary:
    // Object::cast_to<Player::Player>(Node::get_node("/root/Level/Player"))->connect("input_event", pauseMenu, "_input");
}

void PauseMenu::_process(float delta)
{
}

void PauseMenu::_input(InputEvent* event)
{
    if (event->is_action_pressed("pause")) {
        input = Input::get_singleton();
        input->set_mouse_mode(input->MOUSE_MODE_VISIBLE);
        isPaused = !isPaused;
        get_tree()->set_pause(isPaused);
        pauseMenu->set_visible(isPaused);
        if(!isPaused){
            handle_music();
        }
    }
}

void PauseMenu::_on_resume_pressed() 
{
    input = Input::get_singleton();
    isPaused = !isPaused;
    get_tree()->set_pause(isPaused);
    pauseMenu->set_visible(isPaused);
    if (!isPaused){
        handle_music();
    }
    input->set_mouse_mode(input->MOUSE_MODE_CAPTURED);
}

void PauseMenu::handle_music(){
    AudioStreamPlayer* bgm_audio = Object::cast_to<AudioStreamPlayer>(Node::get_node("/root/Level/Player/BackgroundAudio"));
    Player::Player *player = Object::cast_to<Player::Player>(Node::get_node("/root/Level/Player"));
    bgm_audio->set_stream_paused(player->mute);
}

void PauseMenu::_on_main_menu_pressed() 
{
    isPaused = !isPaused;
    get_tree()->set_pause(isPaused);
    get_tree()->change_scene("res://Menu.tscn");
}

void PauseMenu::_on_quit_pressed() 
{
    get_tree()->quit();
}

}
