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

    // Editable in the Godot editor
    float velocity;

    // Non-editable in Godot editor
    Transform start_pos;
    KinematicBody* player;
    KinematicBody* ally;
    Vector3 movement;
    Label* token_counter;
    Area* ally_area;

public:

    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);
    void _physics_process(float delta);
    
    void collision_handler(Area* area);
};
}

#endif
