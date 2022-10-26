#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"
#include <KinematicBody.hpp>
#include <Transform.hpp>
#include <Area.hpp>
#include <Label.hpp>
#include <TextureProgress.hpp>
#include <SceneTree.hpp>
#include <string>

namespace Enemy
{
class Enemy : public KinematicBody 
{
    GODOT_CLASS(Enemy, KinematicBody);

private:

    // (Taken from the player class when enemy is instanced)
    float gravity;
    float jump;
    float slide_angle;

    // Editable in the Godot editor
    float velocity;

    // Non-editable in Godot editor
    Transform start_pos;
    KinematicBody* player;
    KinematicBody* enemy;
    Vector3 movement;

public:

    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);
    void _physics_process(float delta);
};
}

#endif
