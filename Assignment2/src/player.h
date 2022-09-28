#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include <KinematicBody.hpp>

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

public:

    static void _register_methods();

    void _init();
    void _ready();

    void _process(float delta);
    void _physics_process(float delta);
};
}

#endif
