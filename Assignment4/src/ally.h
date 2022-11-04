#ifndef ALLY_H
#define ALLY_H

#include "Common.h"
#include <KinematicBody.hpp>
#include <Transform.hpp>
#include <Area.hpp>
#include <Label.hpp>
#include <TextureProgress.hpp>
#include <SceneTree.hpp>
#include <string>
#include "player.h"

namespace Ally
{
class Ally : public KinematicBody 
{
    GODOT_CLASS(Ally, KinematicBody);

private:

    // (Taken from the player class when ally is instanced)
    float gravity;
    float jump;
    float slide_angle;
    int token_increment;

    int state;

    // Non-editable in Godot editor
    float velocity;
    Transform start_pos;
    Vector3 goal_pos;
    Player::Player* player;
    KinematicBody* ally;
    Vector3 movement;
    Vector3 next_token_pos;
    Label* token_counter;
    Area* ally_area;
    Area* visibility;

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
