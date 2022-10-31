#include "token.h"

namespace Token
{
void Token::_register_methods()
{
    register_method("_ready", &Token::_ready);
}

void Token::_ready() 
{
    set_collision_layer(2);
}

void Token::_init()
{
}
}