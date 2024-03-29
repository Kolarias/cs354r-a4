#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"
#include <KinematicBody.hpp>
#include <Transform.hpp>
#include <Area.hpp>
#include <Label.hpp>
#include <TextureProgress.hpp>
#include <SceneTree.hpp>
#include <Node.hpp>
#include <string>
#include "RandomNumberGenerator.hpp"
#include "player.h"

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
    float velocity;

    int state;

    // Editable in the Godot editor
    int enemy_damage;

    // Non-editable in Godot editor
    RandomNumberGenerator rng;
    Transform start_pos;
    Vector3 goal_pos;
    Vector3 wander_pos;
    Player::Player* player;
    KinematicBody* enemy;
    Area* enemy_area;
    Area* visibility;
    Vector3 movement;

public:

    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);
    void _physics_process(float delta);

    void collision_handler(Area* area);
    void handle_searching();
    void move_to_goal();
};
}

#endif
