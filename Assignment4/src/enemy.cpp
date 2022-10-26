#include "enemy.h"
#include "player.h"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"

namespace Enemy
{
void Enemy::_register_methods()
{
    register_method("_ready", &Enemy::_ready);
    register_method("_process", &Enemy::_process);
    register_method("_physics_process", &Enemy::_physics_process);

    register_property<Enemy, float>("Velocity", &Enemy::velocity, 0.0);
    register_property<Enemy, float>("Gravity", &Enemy::gravity, 9.8);
}

void Enemy::_init() 
{
    start_pos = get_global_transform();
    movement = Vector3();
    gravity = 9.8;
}

void Enemy::_ready() 
{
    enemy = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Enemy"));
    player = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Player"));
    gravity = Object::cast_to<Player::Player>(player)->gravity;
    jump = Object::cast_to<Player::Player>(player)->jump;
    slide_angle = Object::cast_to<Player::Player>(player)->slide_angle;
}

void Enemy::_process(float delta)
{
    // Return to start position if you fell
    if (get_global_transform().origin.y <= -25) {
        set_global_transform(start_pos);
        movement.y = 0;
    }
}

void Enemy::_physics_process(float delta) 
{
    movement.y -= (gravity * delta); // gravity
    Vector3 direction_vel = Vector3(movement);
    direction_vel.x *= float(velocity);
    direction_vel.z *= float(velocity);
    direction_vel.rotate(Vector3(0,1,0), Spatial::get_rotation().y);
    move_and_slide(direction_vel, Vector3::UP, false, 4, slide_angle, true);
}

}
