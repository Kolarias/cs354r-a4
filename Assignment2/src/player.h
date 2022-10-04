#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include <KinematicBody.hpp>
#include <RayCast.hpp>
#include <Transform.hpp>

namespace Player 
{
class Player : public KinematicBody 
{
    GODOT_CLASS(Player, KinematicBody);

private:

    Transform start_pos;
    Vector3 movement;
    bool AD_rotate;
    float velocity;
    float gravity;
    float jump;
    bool on_ledge;
    bool can_grab_ledge;
    bool jumped_twice;
    KinematicBody* player;
    RayCast* ray1;
    RayCast* ray2;
    RayCast* ray3;
    RayCast* ray4;
    RayCast* ray5;
    Input* input;


public:

    static void _register_methods();

    void _init();
    void _ready();

    bool is_on_ledge();
    Transform align_with_y(Transform xform, Vector3 new_y);

    void _process(float delta);
    void _physics_process(float delta);

    void process_on_floor();
    void process_on_air();
    void process_on_ledge();
    void wasd_movement(bool on_air);
};
}

#endif
