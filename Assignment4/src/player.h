#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include <KinematicBody.hpp>
#include <RayCast.hpp>
#include <Transform.hpp>
#include <Area.hpp>
#include <Label.hpp>
#include <TextureProgress.hpp>
#include <AudioStreamPlayer.hpp>
#include <SceneTree.hpp>
#include <string>

namespace Player 
{
class Player : public KinematicBody 
{
    GODOT_CLASS(Player, KinematicBody);

private:

    // Editable in Godot editor
    float velocity;

    // Non-editable in Godot editor
    Transform start_pos;
    KinematicBody* player;
    Vector3 movement;
    bool on_ledge;
    bool can_grab_ledge;
    bool jumped_twice;
    bool gliding;
    TextureProgress* hp_gauge;
    Area* player_area;
    RayCast* ray1;
    RayCast* ray2;
    RayCast* ray3;
    RayCast* ray4;
    RayCast* ray5;
    AudioStreamPlayer* bgm_audio;
    AudioStreamPlayer* jump_audio;
    AudioStreamPlayer* glide_audio;
    AudioStreamPlayer* ledge_audio;
    Input* input;


public:
    // Editable in Godot editor
    bool AD_rotate;
    bool mouse_rotate;
    bool mute;
    float gravity;
    float jump;
    float slide_angle;
    int spike_damage;
    int token_increment;
    Label* token_counter;
    Label* hp_counter;

    AudioStreamPlayer* token_audio;
    AudioStreamPlayer* damage_audio;

    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);
    void _physics_process(float delta);

    bool is_on_ledge();
    bool can_mouse_rotate();
    Transform align_with_y(Transform xform, Vector3 new_y);
    void reset_scene();

    void collision_handler(Area* area);
    void process_on_floor();
    void process_on_air();
    void process_on_ledge();
    void wasd_movement(bool on_air);
};
}

#endif
