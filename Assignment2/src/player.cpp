#include "player.h"
#include "Input.hpp"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"


namespace Player
{
void Player::_register_methods()
{
    register_method("_ready", &Player::_ready);
    register_method("_process", &Player::_process);
    register_method("_physics_process", &Player::_physics_process);
}

void Player::_init() 
{
    direction = Vector3();
    velocity = 0.5;
}

void Player::_ready() 
{
}

void Player::_process(float delta)
{
    // process user input
    Input *input = Input::get_singleton();
    if (input->is_key_pressed(GlobalConstants::KEY_W)){
        direction.x = 1;
    }
    else if (input->is_key_pressed(GlobalConstants::KEY_S)){
        direction.x = -1;
    }
    else {
        direction.x = 0;
    }
    if (input->is_key_pressed(GlobalConstants::KEY_D)){
        direction.z = 1;
    }
    else if (input->is_key_pressed(GlobalConstants::KEY_A)){
        direction.z = -1;
    }
    else {
        direction.z = 0;
    }
}

void Player::_physics_process(float delta)
{
    godot::Ref<godot::KinematicCollision> collision = move_and_collide((direction * velocity));
}

}