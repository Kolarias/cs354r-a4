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

#include <string>

namespace Player 
{
class Player : public KinematicBody 
{
    GODOT_CLASS(Player, KinematicBody);

private:

    Transform start_pos;
    Vector3 movement;
    Vector3 fall_vec;
    bool AD_rotate;
    float velocity;
    float gravity;
    float jump;
    bool on_ledge;
    bool can_grab_ledge;
    KinematicBody* player;
    Label* token_counter;
    Label* hp_counter;
    TextureProgress* hp_gauge;
    Area* player_area;
    RayCast* ray1;
    RayCast* ray2;
    RayCast* ray3;
    RayCast* ray4;
    RayCast* ray5;
    AudioStreamPlayer* token_audio;
    AudioStreamPlayer* damage_audio;

public:

    static void _register_methods();

    void _init();
    void _ready();

    bool is_on_ledge();
    Transform align_with_y(Transform xform, Vector3 new_y);

    void _process(float delta);
    void _physics_process(float delta);

    void collision_handler(Area* area);
    void spike_handler(Node* body);
};
}

#endif
