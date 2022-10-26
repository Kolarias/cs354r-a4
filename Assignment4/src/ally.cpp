#include "ally.h"
#include "player.h"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"
#include "token.h"

namespace Ally
{
void Ally::_register_methods()
{
    register_method("_ready", &Ally::_ready);
    register_method("_process", &Ally::_process);
    register_method("_physics_process", &Ally::_physics_process);

    register_property<Ally, float>("Velocity", &Ally::velocity, 0.0);
    register_property<Ally, float>("Gravity", &Ally::gravity, 9.8);
    register_property<Ally, int>("Token Increment", &Ally::token_increment, 1);
}

void Ally::_init() 
{
    start_pos = get_global_transform();
    movement = Vector3();
    gravity = 9.8;
    token_increment = 1;
}

void Ally::_ready() 
{
    token_counter = Object::cast_to<Label>(Node::get_node("/root/Level/Ally/TokensLabel/Viewport/Tokens"));
    ally = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Ally"));
    ally_area = (Area*)(ally->get_node("AllyArea"));
    ally_area->connect("area_entered", ally, "collision_handler");
    player = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Player"));
    gravity = Object::cast_to<Player::Player>(player)->gravity;
    jump = Object::cast_to<Player::Player>(player)->jump;
    slide_angle = Object::cast_to<Player::Player>(player)->slide_angle;
}

void Ally::_process(float delta)
{
    // Return to start position if you fell
    if (get_global_transform().origin.y <= -25) {
        set_global_transform(start_pos);
        movement.y = 0;
    }
}

void Ally::_physics_process(float delta) 
{
    movement.y -= (gravity * delta); // gravity
    Vector3 direction_vel = Vector3(movement);
    direction_vel.x *= float(velocity);
    direction_vel.z *= float(velocity);
    direction_vel.rotate(Vector3(0,1,0), Spatial::get_rotation().y);
    move_and_slide(direction_vel, Vector3::UP, false, 4, slide_angle, true);
}

void Ally::collision_handler(Area* area)
{
    Token::Token* token = Object::cast_to<Token::Token>(area);
    if (token) {
        // 1) Update token counter on GUI
        int curr_count = stoi(token_counter->get_text().utf8().get_data());
        curr_count += token_increment;
        std::string std_string = std::to_string(curr_count);
        godot::String new_count = godot::String(std_string.c_str());
        token_counter->set_text(new_count);
        // 2) Delete token from screen
        token->queue_free();
    }
}

}
