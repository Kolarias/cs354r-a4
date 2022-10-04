#include "player.h"
#include "Input.hpp"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"
#include "token.h"

#define M_PI 3.14159265358979323846

namespace Player
{
void Player::_register_methods()
{
    register_method("_ready", &Player::_ready);
    register_method("_process", &Player::_process);
    register_method("_physics_process", &Player::_physics_process);
    register_method("collision_handler", &Player::collision_handler);

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
    can_grab_ledge = true;
}

void Player::_ready() 
{
    ray1 = Object::cast_to<RayCast>(Node::get_node("/root/Level/Player/LedgeCollider1"));
    ray2 = Object::cast_to<RayCast>(Node::get_node("/root/Level/Player/LedgeCollider2"));
    ray3 = Object::cast_to<RayCast>(Node::get_node("/root/Level/Player/WallCollider1"));
    ray4 = Object::cast_to<RayCast>(Node::get_node("/root/Level/Player/WallCollider2"));
    ray5 = Object::cast_to<RayCast>(Node::get_node("/root/Level/Player/FloorCollider1"));
    player = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Player"));
    player_area = (Area*)(player->get_node("PlayerArea"));
    player_area->connect("area_entered", player, "collision_handler");
    token_counter = Object::cast_to<Label>(Node::get_node("/root/Level/Player/GUI/Bars/TokenCounter/Tokens/Background/Number"));
    token_audio = Object::cast_to<AudioStreamPlayer>(Node::get_node("/root/Level/TokenAudio"));
    damage_audio = Object::cast_to<AudioStreamPlayer>(Node::get_node("/root/Level/DamageAudio"));
}

bool Player::is_on_ledge(){ 
    return on_ledge;
}

void Player::_process(float delta)
{
    // Get input
    Input *input = Input::get_singleton();

    // If we've touched the floor, we can grab ledges again
    if (is_on_floor() && !can_grab_ledge) {
        can_grab_ledge = true;
    }

    // Forward/backward movement
    if (!on_ledge) {
        // Don't allow movement forward if we're approaching a ledge (floorcollider isn't colliding)
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

    // Move or rotate left and right
    if (AD_rotate){
        if (input->is_action_pressed("move_right")){
            rotate_y(-0.02);
            
        }
        else if (input->is_action_pressed("move_left")){
            rotate_y(0.02);
        }
    }
    else {
        // If we're on a ledge, only allow left/right movement if there's remaining wall to move to there
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
    
    // If we're on a ledge, check if we wanna drop
    if (on_ledge) {
        if (input->is_action_just_pressed("ledge")) {
            on_ledge = false;
            can_grab_ledge = false;
        }
    // Otherwise, if we're not on a ledge...
    } else {
        // Check for ledge in front of us while walking
        if (is_on_floor() && !ray5->is_colliding()) {
            if(input->is_action_pressed("ledge")) {
                // Possibly a better way to do this - basically it teleports down to around
                // where the ledge should be, and sees if it can align itself with a ledge
                // there. If it can, it aligns with the ledge and stays, otherwise it teleports
                // back.
                Vector3 new_translation = Vector3(2, -1.75, 2);
                Vector3 old_translation = player->get_translation();
                player->translate(new_translation);
                player->rotate_y(M_PI);
                ray1->force_raycast_update();
                if (ray1->is_colliding()) {
                    player->set_transform(align_with_y(player->get_transform(), (ray1->get_collision_normal())));
                    on_ledge = true;
                    fall_vec = Vector3();
                    movement = Vector3();
                } else {
                    player->rotate_y(-M_PI);
                    player->set_translation(old_translation);
                }
            }
        }
        // Check for ledge while in air
        if (ray1->is_colliding() && !(ray2->is_colliding()) && can_grab_ledge) {
            Vector3 new_position = Vector3(ray1->get_collision_point().x, 
                player->get_translation().y, ray1->get_collision_point().z);
            player->set_translation(new_position);
            // Figuring out the right math to align to ledges took a looooong time lmao
            player->set_transform(align_with_y(player->get_transform(), (ray1->get_collision_normal())));
            on_ledge = true;
            fall_vec = Vector3();
            movement = Vector3();
        }
    }

    // Gravity
    if (!is_on_floor() && !on_ledge) {
        fall_vec.y -= (gravity * delta);
    }

    // Jump
    if (input->is_action_just_pressed("jump") & (is_on_floor() || on_ledge)) {
        fall_vec.y = jump;
        // Stop grabbing onto current ledge and stop grabbing ledges until we land
        if (on_ledge) {
            on_ledge = false;
            can_grab_ledge = false;
        }
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

// HELPER TAKEN FROM THIS FORUM POST: https://godotengine.org/qa/132087/how-to-make-the-character-face-the-plane-you-are-climbing-on
Transform Player::align_with_y(Transform xform, Vector3 normal) 
{
    xform.basis.y = normal;
    xform.basis.x = -xform.basis.z.cross(normal);
    xform.basis = xform.basis.orthonormalized();
    xform.basis = xform.basis.rotated(xform.basis.z, -M_PI/2);
    return xform;
}

void Player::collision_handler(Area* area)
{
    Token::Token* token = Object::cast_to<Token::Token>(area);
    if (token) {
        // 1) Update token counter on GUI
        int curr_count = stoi(token_counter->get_text().utf8().get_data());
        curr_count += 1;
        std::string std_string = std::to_string(curr_count);
        godot::String new_count = godot::String(std_string.c_str());
        token_counter->set_text(new_count);
        // 2) Play pickup sound
        token_audio->play();
        // 3) Delete token from screen
        token->queue_free();
    }
}
}   
