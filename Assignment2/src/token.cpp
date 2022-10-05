#include "token.h"

namespace Token
{
void Token::_register_methods()
{
    register_method("_ready", &Token::_ready);
}

void Token::_ready() 
{
}

void Token::_init()
{
}
}