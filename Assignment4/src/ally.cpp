#include "ally.h"
#include "player.h"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"
#include "token.h"

namespace Ally
{

enum ally_states 
{
    SEARCHING, COLLECTING, RETURNING
};

void Ally::_register_methods()
{
    register_method("_ready", &Ally::_ready);
    register_method("_process", &Ally::_process);
    register_method("_physics_process", &Ally::_physics_process);

    register_property<Ally, float>("Velocity", &Ally::velocity, 5.0);
    register_property<Ally, float>("Gravity", &Ally::gravity, 9.8);
    register_property<Ally, int>("Token Increment", &Ally::token_increment, 1);
}

void Ally::_init() 
{
    start_pos = get_global_transform();
    movement = Vector3();
    gravity = 9.8;
    token_increment = 1;
    state = SEARCHING;
    found_token = false;
}

void Ally::_ready() 
{
    token_counter = Object::cast_to<Label>(Node::get_node("/root/Level/Ally/TokensLabel/Viewport/Tokens"));
    ally = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Ally"));
    ally_area = (Area*)(ally->get_node("AllyArea"));
    ally_area->connect("area_entered", ally, "collision_handler");
    ally_search_area = (Area*)(ally->get_node("AllySearchArea"));
    ally_search_area->connect("area_entered", ally, "search_handler");
    player = Object::cast_to<Player::Player>(Node::get_node("/root/Level/Player"));
    gravity = Object::cast_to<Player::Player>(player)->gravity;
    jump = Object::cast_to<Player::Player>(player)->jump;
    slide_angle = Object::cast_to<Player::Player>(player)->slide_angle;
}

void Ally::_process(float delta)
{
    // Basic processes that always happen

    // Return to start position if you fell
    if (get_global_transform().origin.y <= -25) {
        set_global_transform(start_pos);
        movement.y = 0;
    }

    // State specific

    if (state == SEARCHING) {
        // just walk in some direction meaninglessly until you a token enters the area
        handle_searching();
    } else if (state == COLLECTING) {
        // token has entered the area; token is now the goal pos. walk towards the token
        handle_collecting();
        // once picking up the token, return to the player
    } else {
        // walk towards player (goal_pos is player).
        handle_returning();
        // once dropped tokens off to player, return to searching
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
    Player::Player* player_node = Object::cast_to<Player::Player>(area->get_parent());
    
    if (token) {
        // 1) Update token counter on GUI
        int curr_count = stoi(token_counter->get_text().utf8().get_data());
        curr_count += token_increment;
        std::string std_string = std::to_string(curr_count);
        godot::String new_count = godot::String(std_string.c_str());
        token_counter->set_text(new_count);
        // 2) Delete token from screen
        token->queue_free();
        state = RETURNING;
        Godot::print("Ally picked up token");
    }
    // drop off tokens to player
    if (player_node) {
        int curr_count = stoi(token_counter->get_text().utf8().get_data());
        int player_token_count = stoi(player->token_counter->get_text().utf8().get_data());
        player_token_count += curr_count;
        std::string std_string = std::to_string(player_token_count);
        godot::String new_count = godot::String(std_string.c_str());
        player->token_counter->set_text(new_count);
        state = SEARCHING;
        Godot::print("Ally dropped off tokens");
    }
}

void Ally::search_handler(Area* area) {

    Token::Token* token = Object::cast_to<Token::Token>(area);

    if (state == SEARCHING) {
        if (token) {
            // only update goal_pos if we haven't found a token yet; don't want to keep updating goal
            // if multiple tokens have entered the area
            goal_pos = token->get_global_transform();
            state = COLLECTING;
            Godot::print("Ally found token");
        }
    }
}

void Ally::handle_searching()
{
    // just move in some direction; once a token enters the area for search_handler state will change to collecting
    movement.x = 1;
}

void Ally::handle_collecting()
{
    // orient towards goal_pos
}

void Ally::handle_returning()
{
    // orient towards player and move
    goal_pos = player->get_global_transform();
}

}
