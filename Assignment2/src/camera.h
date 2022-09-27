#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include <Camera.hpp>

namespace PlayerCam 
{
class PlayerCam : public Camera 
{
    GODOT_CLASS(PlayerCam, Camera);

private:

public:
    static void _register_methods();

    void _init();
    void _ready();

    void _process(float delta);
};
}

#endif