#ifndef PLAYERCAM_H
#define PLAYERCAM_H

#include "Common.h"
#include <Camera.hpp>
#include <Input.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <RayCast.hpp>
#include <algorithm>
#include <cstdlib>
#include <Godot.hpp>

namespace PlayerCam {

class PlayerCam : public Camera {
    GODOT_CLASS(PlayerCam, Camera);

private:
    float mouse_sensitivity;
    Input *input;

public:
    static void _register_methods();

    void _init();
    void _ready();
    void _input(InputEvent* event);

    void _process(float delta);
};
}

#endif