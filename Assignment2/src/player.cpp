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
    register_property<Player, float>("Gravity", &Player::gravity, 9.8);
    register_property<Player, float>("Jump Height", &Player::jump, 10);
}

void Player::_init() 
{
    start_pos = get_global_transform();
    movement = Vector3();
    fall_vec = Vector3();
    gravity = 9.8;
    jump = 10;
    on_ledge = false;
}

void Player::_ready() 
{
    ray1 = Object::cast_to<RayCast>(Node::get_node("/root/Level/Player/LedgeCollider1"));
    ray2 = Object::cast_to<RayCast>(Node::get_node("/root/Level/Player/LedgeCollider2"));
    ray3 = Object::cast_to<RayCast>(Node::get_node("/root/Level/Player/WallCollider1"));
    ray4 = Object::cast_to<RayCast>(Node::get_node("/root/Level/Player/WallCollider2"));
    ray5 = Object::cast_to<RayCast>(Node::get_node("/root/Level/Player/FloorCollider1"));
    player = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Player"));
}

bool Player::is_on_ledge(){ 
    return on_ledge;
}

void Player::_process(float delta)
{
    // Process user input
    Input *input = Input::get_singleton();
    if (!on_ledge) {
        if (input->is_action_pressed("move_forward") && (!is_on_floor() || ray5->is_colliding())) {
            movement.x = 0.5;
        }
        else if (input->is_action_pressed("move_backward")){
            movement.x = -0.5;
        }
        else {
            movement.x = 0;
        }
    }

    if (AD_rotate){
        if (input->is_action_pressed("move_right")){
            rotate_y(-0.02);
            
        }
        else if (input->is_action_pressed("move_left")){
            rotate_y(0.02);
        }
    }
    else {
        if (input->is_action_pressed("move_right") && (!on_ledge || ray3->is_colliding())){
            movement.z = 0.5;
        }
        else if (input->is_action_pressed("move_left") && (!on_ledge || ray4->is_colliding())){
            movement.z = -0.5;
        }
        else {
            movement.z = 0;
        }
    }

    // Check for ledge
    if (ray1->is_colliding() && !(ray2->is_colliding()) && !on_ledge) {
        on_ledge = true;
        fall_vec = Vector3();
        movement = Vector3();
        Vector3 new_position = Vector3(ray1->get_collision_point().x, 
            player->get_translation().y, ray1->get_collision_point().z);
        player->set_translation(new_position);
        player->rotate_y(0);
    }


    // Gravity
    if (!is_on_floor() && !on_ledge) {
        fall_vec.y -= (gravity * delta);
    }

    // Jump
    if (input->is_action_just_pressed("jump") & (is_on_floor() || on_ledge)) {
        fall_vec.y = jump;
        on_ledge = false;
    }

    // Return to start position if you fell
    if (get_global_transform().origin.y <= -25) {
        set_global_transform(start_pos);
        fall_vec.y = 0;
    }
}

void Player::_physics_process(float delta)
{
    move_and_slide(fall_vec, Vector3::UP);
    Vector3 direction = Vector3(movement);
    direction.rotate(Vector3(0,1,0), Spatial::get_rotation().y);
    godot::Ref<godot::KinematicCollision> collision = move_and_collide(direction * velocity * delta);
}

}
