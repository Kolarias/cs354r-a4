#include "ally.h"
#include "player.h"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"
#include "token.h"
#include "Navigation.hpp"

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
    register_method("collision_handler", &Ally::collision_handler);


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
    velocity = 5.0;
}

void Ally::_ready() 
{
    token_counter = Object::cast_to<Label>(Node::get_node("/root/Level/Ally/TokensLabel/Viewport/Tokens"));
    ally = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Ally"));
    ally_area = (Area*)(ally->get_node("AllyArea"));
    ally_area->connect("area_entered", ally, "collision_handler");
    visibility = (Area*)(ally->get_node("Visibility"));
    visibility->set_collision_mask(2);
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
        move_to_goal();
        // once picking up the token, return to the player
    } else if (state == RETURNING) {
        // walk towards player (goal_pos is player).
        goal_pos = player->get_translation();
        move_to_goal();
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
        goal_pos = player->get_translation();
    }
    // drop off tokens to player
    if (player_node) {
        // 1) Update token counter on GUI
        int curr_count = stoi(token_counter->get_text().utf8().get_data());
        if (curr_count == 0){
            return;
        }
        int player_token_count = stoi(player->token_counter->get_text().utf8().get_data());
        player_token_count += curr_count;
        std::string std_string = std::to_string(player_token_count);
        godot::String new_count = godot::String(std_string.c_str());
        player->token_counter->set_text(new_count);
        token_counter->set_text("0");
        if (!player->mute) {
            player->token_audio->play();
        }
        state = SEARCHING;
    }
}

void Ally::handle_searching()
{
    // just move in some direction; once a token enters the area for search_handler state will change to collecting
    // maybe move behind player, like following the player and just scan for the closest token.

    // scans for tokens and set the closest one as a goal.
    Array overlapping_bodies = visibility->get_overlapping_areas();
    Vector3 current_position = get_translation();
    int min_dist = 0;
    Token::Token* token_goal = nullptr;
    // iterate over the over the areas
    if (overlapping_bodies.empty()){
        return;
    }
    for (int i = 0; i < overlapping_bodies.size(); i ++){
        Token::Token* token = Object::cast_to<Token::Token>(overlapping_bodies[i]);
        if (token){
            // get distance to token
            Vector3 token_pos = token->get_translation();
            int distance = current_position.distance_to(token_pos);
            if (token_pos.y > 0 && token_pos.y <= 4 && (distance <= min_dist || min_dist == 0)){
                min_dist = distance;
                token_goal = token;
            }
        }
    }
    if (token_goal != nullptr){
        goal_pos = token_goal->get_translation();
        state = COLLECTING;
    }
    else {
        // no token found; just follow the player
        goal_pos = player->get_translation();
        move_to_goal();
    }
}

void Ally::move_to_goal()
{
    // look at the goal pos - automatically finds the shortest angle path to do thi
    look_at(goal_pos, Vector3::UP);
    // look_at automatically defines forwards as the -z axis; have to rotate to adjust for this
    rotate_object_local(Vector3::UP, Math_PI / 2.0);
    Vector3 rotation = get_rotation();
    // look_at looks for the goal_pos in all 3 directions; this tilts the ally up/down. have to adjust
    rotate_object_local(Vector3(0, 0, 1), -rotation.z);
    movement.x = 1;
}

}
