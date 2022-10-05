#include "spike.h"

namespace Spike
{
void Spike::_register_methods()
{
    register_method("_ready", &Spike::_ready);
}

void Spike::_ready() 
{
}

void Spike::_init()
{
}
}