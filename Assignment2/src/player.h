#ifndef PLAYER_H
#define PLAYER_H

#include <Godot.hpp>
#include <KinematicBody.hpp>

namespace godot {

class Player : public KinematicBody {
    GODOT_CLASS(Player, KinematicBody)

private:

public:

    static void _register_methods();

    void _init();
    void _ready();

    void _process(float delta);
    void _physics_process(float delta);
};

}

#endif