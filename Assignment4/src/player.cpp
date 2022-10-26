#include "player.h"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"
#include "token.h"
#include "spike.h"
#include "enemy.h"
#include "ally.h"

#define M_PI 3.14159265358979323846

namespace Player
{
void Player::_register_methods()
{
    register_method("_ready", &Player::_ready);
    register_method("_process", &Player::_process);
    register_method("_physics_process", &Player::_physics_process);
    register_method("collision_handler", &Player::collision_handler);

    register_property<Player, bool>("Mouse Rotate", &Player::mouse_rotate, false);
    register_property<Player, bool>("AD Rotate", &Player::AD_rotate, false);
    register_property<Player, float>("Velocity", &Player::velocity, 0.0);
    register_property<Player, float>("Gravity", &Player::gravity, 9.8);
    register_property<Player, float>("Jump Height", &Player::jump, 10);
    register_property<Player, float>("Slide Angle", &Player::slide_angle, 0.785398);
    register_property<Player, bool>("Mute Audio", &Player::mute, false);
    register_property<Player, int>("Spike Damage", &Player::spike_damage, 5);
    register_property<Player, int>("Token Increment", &Player::token_increment, 1);
}

void Player::_init() 
{
    start_pos = get_global_transform();
    movement = Vector3();
    gravity = 9.8;
    jump = 10;
    on_ledge = false;
    can_grab_ledge = true;
    jumped_twice = false;
    gliding = false;
    mute = false;
    spike_damage = 5;
    token_increment = 1;
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
    hp_counter = Object::cast_to<Label>(Node::get_node("/root/Level/Player/GUI/Bars/HealthBar/HP/Background/Number"));
    hp_gauge = Object::cast_to<TextureProgress>(Node::get_node("/root/Level/Player/GUI/Bars/HealthBar/Gauge"));
    token_audio = Object::cast_to<AudioStreamPlayer>(Node::get_node("/root/Level/Player/TokenAudio"));
    damage_audio = Object::cast_to<AudioStreamPlayer>(Node::get_node("/root/Level/Player/DamageAudio"));
    bgm_audio = Object::cast_to<AudioStreamPlayer>(Node::get_node("/root/Level/Player/BackgroundAudio"));
    jump_audio = Object::cast_to<AudioStreamPlayer>(Node::get_node("/root/Level/Player/JumpAudio"));
    glide_audio = Object::cast_to<AudioStreamPlayer>(Node::get_node("/root/Level/Player/GlideAudio"));
    ledge_audio = Object::cast_to<AudioStreamPlayer>(Node::get_node("/root/Level/Player/LedgeAudio"));
    bgm_audio->play();
    bgm_audio->set_stream_paused(mute);
}

bool Player::is_on_ledge(){ 
    return on_ledge;
}

bool Player::can_mouse_rotate(){
    return mouse_rotate;
}

void Player::_process(float delta)
{
    // Get input
    input = Input::get_singleton();

    if (is_on_floor()){
        process_on_floor();
    }
    else if (on_ledge){
        process_on_ledge();
    }
    else if (is_on_ceiling() && movement.y > 0){
        movement.y = -1;
    }
    else {
        process_on_air();
    }

    if (input->is_action_just_pressed("mute")) {
        mute = !mute;
        bgm_audio->set_stream_paused(mute);
    }

    if (input->is_action_just_pressed("reset_scene")) {
        reset_scene();
    }

    // Processes that will always affect the player no matter their state:

    // Gravity
    if (!is_on_floor() && !on_ledge) {
        float scale = gliding ? 0.1 : 1.0;
        movement.y -= (gravity * delta * scale);
    }
    // Return to start position if you fell
    if (get_global_transform().origin.y <= -25) {
        set_global_transform(start_pos);
        movement.y = 0;
    }
}

void Player::_physics_process(float delta) 
{
    Vector3 direction_vel = Vector3(movement);
    direction_vel.x *= float(velocity);
    direction_vel.z *= float(velocity);
    direction_vel.rotate(Vector3(0,1,0), Spatial::get_rotation().y);
    move_and_slide(direction_vel, Vector3::UP, false, 4, slide_angle, true);
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
    Spike::Spike* spike = Object::cast_to<Spike::Spike>(area);
    if (token) {
        // 1) Update token counter on GUI
        int curr_count = stoi(token_counter->get_text().utf8().get_data());
        curr_count += token_increment;
        std::string std_string = std::to_string(curr_count);
        godot::String new_count = godot::String(std_string.c_str());
        token_counter->set_text(new_count);
        // 2) Play pickup sound
        if (!mute) {
            token_audio->play();
        }
        // 3) Delete token from screen
        token->queue_free();
    } else if (spike) {
        // 1) Decrease health counter on GUI and gauge
        int curr_count = stoi(hp_counter->get_text().utf8().get_data());
        curr_count -= spike_damage;
        if (curr_count == 0) {
            // died; reset scene
            reset_scene();
        } else {
            std::string std_string = std::to_string(curr_count);
            godot::String new_count = godot::String(std_string.c_str());
            hp_counter->set_text(new_count);
            hp_gauge->set_value(hp_gauge->get_value() - spike_damage);
            spike->queue_free();
        }
        // 2) Play damage sound
        if (!mute) {
            damage_audio->play();
        }
    }
}

// Helper function that handles all cases of WASD player movement
void Player::wasd_movement(bool on_air){
    float scale = on_air ? 0.7 : 1.0;

    if (!on_ledge && !gliding){
        // Forward/backward movement
        if (input->is_action_pressed("move_forward")) {
            movement.x = 1 * scale;
        }
        else if (input->is_action_pressed("move_backward")){
            movement.x = -1 * scale;
        }
        else {
            movement.x = 0;
        }
    }

    // Move or rotate left and right
    if (AD_rotate && !on_ledge){
        if (input->is_action_pressed("move_right")){
            rotate_y(-0.02);
            
        }
        else if (input->is_action_pressed("move_left")){
            rotate_y(0.02);
        }
    }
    else {
        if (input->is_action_pressed("move_right") && (!on_ledge || ray3->is_colliding())){
            movement.z = 1 * scale;
        }
        else if (input->is_action_pressed("move_left") && (!on_ledge || ray4->is_colliding())){
            movement.z = -1 * scale;
        }
        else {
            movement.z = 0;
        }
    }
}

// Funtion that process input of the user when the player is on the floor
void Player::process_on_floor(){
    // reset boolean variables
    can_grab_ledge = true; 
    jumped_twice = false;
    gliding = false;
    glide_audio->stop();

    wasd_movement(false);

    if (!ray5->is_colliding()){
        if (movement.x > 0){
            movement.x = 0;
        }
        if (input->is_action_pressed("ledge")){
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
                if (!mute) {
                    ledge_audio->play();
                }
                movement = Vector3();
            } else {
                player->rotate_y(-M_PI);
                player->set_translation(old_translation);
            }
        }
    }

    // Jump
    if (input->is_action_just_pressed("jump")) {
        if (!mute) {
            jump_audio->play();
        }
        movement.y = jump;
    }
    else if (movement.y < -0.1){
        movement.y = -0.1;
    }
}

// Function that process user input when player is on the air
void Player::process_on_air(){
    if (input->is_action_just_pressed("glide")){
        gliding = true;
        if (!mute) {
            glide_audio->play();
        }
        if (movement.y > 0){
            movement.y = 0;
        }
    }
    if (input->is_action_pressed("glide")){
        movement.x = 1;
    }
    else {
        gliding = false;
        glide_audio->stop();
        // Double Jump
        if (input->is_action_just_pressed("jump") & !jumped_twice){
            // adjusted double jump - this can be changed so both jumps are of equal height
            if (!mute) {
                jump_audio->play();
            }
            movement.y = jump * 0.75;
            jumped_twice = true;
        }
    }
    wasd_movement(true);

    // Check for ledge
    if (ray1->is_colliding() && !ray2->is_colliding() && (ray3->is_colliding() || ray4->is_colliding()) && can_grab_ledge) {
        // Don't ledge snap to allies or enemies!
        Ally::Ally* obj1 = Object::cast_to<Ally::Ally>(ray1->get_collider());
        Enemy::Enemy* obj2 = Object::cast_to<Enemy::Enemy>(ray1->get_collider());
        if (!obj1 && !obj2) {
            on_ledge = true;
            gliding = false;
            glide_audio->stop();
            if (!mute) {
                ledge_audio->play();
            }
            Vector3 new_position = Vector3(ray1->get_collision_point().x, 
                player->get_translation().y, ray1->get_collision_point().z);
            player->set_translation(new_position);
            // Figuring out the right math to align to ledges took a looooong time lmao
            player->set_transform(align_with_y(player->get_transform(), (ray1->get_collision_normal())));
            movement = Vector3();
        }
    }
}

// Function that process user input when player is hanging on a ledge
void Player::process_on_ledge(){
    movement = Vector3();
    gliding = false;
    wasd_movement(false);
    // Ledge Key
    if (input->is_action_just_pressed("ledge") || input->is_action_just_pressed("jump")) {
        on_ledge = false;
        can_grab_ledge = false;

        Vector3 rotation_degrees = Vector3();
        rotation_degrees.y = get_rotation_degrees().y;
        set_rotation_degrees(rotation_degrees);

        if (input->is_action_just_pressed("jump")){
            movement.y = jump;
        }
    }
}

void Player::reset_scene() 
{
    get_tree()->reload_current_scene();
}

}
