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
    Vector3 direction;
    float velocity;

public:
    static void _register_methods();

    void _init();
    void _ready();

    void _process(float delta);
    void _physics_process(float delta);
};
}

#endif
