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

    register_property<Player, bool>("Rotate", &Player::AD_rotate, false);
    register_property<Player, float>("Velocity", &Player::velocity, 0.0);
}

void Player::_init() 
{
    movement = Vector3();
}

void Player::_ready() 
{
}

void Player::_process(float delta)
{
    // Process user input
    Input *input = Input::get_singleton();
        if (input->is_key_pressed(GlobalConstants::KEY_W)){
        movement.x = 1;
    }
    else if (input->is_key_pressed(GlobalConstants::KEY_S)){
        movement.x = -1;
    }
    else {
        movement.x = 0;
    }

    if (AD_rotate){
        if (input->is_key_pressed(GlobalConstants::KEY_D)){
            rotate_y(-0.02);
            
        }
        else if (input->is_key_pressed(GlobalConstants::KEY_A)){
            rotate_y(0.02);
        }
    }
    else {
        if (input->is_key_pressed(GlobalConstants::KEY_D)){
            movement.z = 1;
        }
        else if (input->is_key_pressed(GlobalConstants::KEY_A)){
            movement.z = -1;
        }
        else {
            movement.z = 0;
        }
    }
}

void Player::_physics_process(float delta)
{
    Vector3 direction = Vector3(movement);
    direction.rotate(Vector3(0,1,0), Spatial::get_rotation().y);
    godot::Ref<godot::KinematicCollision> collision = move_and_collide(direction * velocity);
}

}
