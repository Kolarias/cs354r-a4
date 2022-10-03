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
    Vector3 fall_vec;
    bool AD_rotate;
    float velocity;
    float gravity;
    float jump;
    bool on_ledge;
    bool can_grab_ledge;
    KinematicBody* player;
    RayCast* ray1;
    RayCast* ray2;
    RayCast* ray3;
    RayCast* ray4;
    RayCast* ray5;

public:

    static void _register_methods();

    void _init();
    void _ready();

    bool is_on_ledge();

    void _process(float delta);
    void _physics_process(float delta);
};
}

#endif
