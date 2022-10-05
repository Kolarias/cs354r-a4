#ifndef SPIKE_H
#define SPIKE_H

#include "Common.h"
#include <Area.hpp>

namespace Spike
{
class Spike : public Area
{
    GODOT_CLASS(Spike, Area);

private:

public:

    static void _register_methods();
    void _init();
    void _ready();
    
};
}

#endif