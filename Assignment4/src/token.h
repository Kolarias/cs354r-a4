#ifndef TOKEN_H
#define TOKEN_H

#include "Common.h"
#include <Area.hpp>

namespace Token
{
class Token : public Area
{
    GODOT_CLASS(Token, Area);

private:

public:

    static void _register_methods();
    void _init();
    void _ready();

};
}

#endif