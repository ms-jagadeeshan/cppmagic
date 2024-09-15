#pragma once

#ifdef ENABLE_TRACEBACK
#define BACKWARD_HAS_DW 1
#include "backward-cpp/backward.hpp"
namespace backward
{
static backward::SignalHandling sh;
}
#endif
