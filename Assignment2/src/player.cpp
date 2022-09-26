#include "player.h"

namespace Player
{
void Player::_register_methods()
{
    register_method("_ready", &Player::_ready);
    register_method("_process", &Player::_process);
    register_method("_physics_process", &Player::_physics_process);
}

void Player::_init() 
{
}

void Player::_ready() 
{
}

void Player::_process(float delta)
{
}

void Player::_physics_process(float delta)
{
}

}