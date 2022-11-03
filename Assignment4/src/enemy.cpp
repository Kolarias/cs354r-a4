#include "enemy.h"
#include "player.h"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"
#include <stdlib.h>
#include <ctime>

namespace Enemy
{
    
enum enemy_states
{
    SEARCHING, ATTACKING, RETREATING
};

void Enemy::_register_methods()
{
    register_method("_ready", &Enemy::_ready);
    register_method("_process", &Enemy::_process);
    register_method("_physics_process", &Enemy::_physics_process);
    register_method("collision_handler", &Enemy::collision_handler);
    register_method("player_entered", &Enemy::player_entered);

    register_property<Enemy, float>("Gravity", &Enemy::gravity, 9.8);
}

void Enemy::_init() 
{
    movement = Vector3();
    gravity = 9.8;
    state = SEARCHING;
    velocity = 5.0;
}

void Enemy::_ready() 
{
    srand (static_cast <unsigned> (time(0)));
    start_pos = get_global_transform();
    enemy = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Enemy"));
    player = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Player"));
    enemy_area = Object::cast_to<Area>(Node::get_node("/root/Level/Enemy/EnemyArea"));
    enemy_search_area = Object::cast_to<Area>(Node::get_node("/root/Level/Enemy/EnemySearchArea"));
    enemy_area->connect("area_entered", enemy, "collision_handler");
    enemy_search_area->connect("area_entered", enemy, "player_entered");
    enemy_search_area->connect("area_exited", enemy, "player_exited");
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

    // State specific

    if (state == SEARCHING) {
        // just walk in some direction meaninglessly until player enters the area
        handle_searching();
    } else if (state == ATTACKING) {
        // player has entered the area; player is now the goal pos
        Godot::print("Attacking player");
        goal_pos = player->get_translation();
        move_to_goal();
        // once picking up the token, return to the player
    } else if (state == RETREATING) {
        // if returned to start position, return to searching
        Vector3 curr_pos = get_global_transform().origin;
        if ((curr_pos.x - start_pos.origin.x) < 0.01 && (curr_pos.z - start_pos.origin.z) < 0.01) {
            Godot::print("Returned to starting position. Going back to searching state.");
            state = SEARCHING;
        } else {
            // otherwise, continue going back to start position
            goal_pos = start_pos.origin;
            move_to_goal();
        }
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

void Enemy::collision_handler(Area* area)
{
    Player::Player* player_node = Object::cast_to<Player::Player>(area->get_parent());
    
    // hit player
    if (player_node) {
        // player node will handle damage, just start retreating
        Godot::print("Enemy hit player - retreating from player");
        state = RETREATING;
    }
}

void Enemy::player_entered(Area* area) 
{
    Player::Player* player_node = Object::cast_to<Player::Player>(area->get_parent());
    
    // go toward player player
    if (player_node) {
        Godot::print("Player has entered enemy area, attacking");
        state = ATTACKING;
    }
}

void Enemy::player_exited(Area* area) 
{
    Player::Player* player_node = Object::cast_to<Player::Player>(area->get_parent());
    
    // retreat
    if (player_node) {
        Godot::print("Player has exited area, retreating");
        state = RETREATING;
    }
}

void Enemy::handle_searching()
{
    // we want the enemy to wander while the player isn't in range. find a random point
    // within 50 units of the start_pos and go there. once there, find another random
    // point and repeat until enemy comes into range
    Vector3 curr_pos = get_global_transform().origin;
    if ((curr_pos.x - wander_pos.x) > 0.01 && (curr_pos.z - wander_pos.z) > 0.01 ) {
        move_to_goal();
    } else {
        Godot::print("CREATING NEW RANDOM POS...");
        float new_x = (start_pos.origin.x - 15) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((start_pos.origin.x + 15)-(start_pos.origin.x - 15))));
        float new_z = (start_pos.origin.z - 15) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((start_pos.origin.z + 15)-(start_pos.origin.z - 15))));
        Vector3 new_pos = Vector3(new_x, curr_pos.y, new_z);
        wander_pos = new_pos;
        goal_pos = wander_pos;
    }
}

void Enemy::move_to_goal()
{
    // look at the goal pos - automatically finds the shortest angle path to do this
    look_at(goal_pos, Vector3::UP);
    // look_at automatically defines forwards as the -z axis; have to rotate to adjust for this
    rotate_object_local(Vector3::UP, Math_PI / 2.0);
    Vector3 rotation = get_rotation();
    // look_at looks for the goal_pos in all 3 directions; this tilts the enemy up/down. have to adjust
    rotate_object_local(Vector3(0, 0, 1), -rotation.z);
    movement.x = 1;
}

}
