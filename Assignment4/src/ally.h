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

    // Editable in the Godot editor
    float velocity;

    // Non-editable in Godot editor
    Transform start_pos;
    Transform goal_pos;
    Player::Player* player;
    KinematicBody* ally;
    Vector3 movement;
    Label* token_counter;
    Area* ally_area;
    Area* ally_search_area;
    bool found_token;

public:

    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);
    void _physics_process(float delta);
    
    void collision_handler(Area* area);
    void search_handler(Area* area);
    void handle_searching();
    void handle_collecting();
    void handle_returning();
};
}

#endif
