#include "camera.h"

namespace PlayerCam
{
void PlayerCam::_register_methods()
{
    register_method("_ready", &PlayerCam::_ready);
    register_method("_process", &PlayerCam::_process);
    register_method("_physics_process", &PlayerCam::_physics_process);
}

void PlayerCam::_init() 
{
}

void PlayerCam::_ready() 
{
}

void PlayerCam::_process(float delta)
{
}

}