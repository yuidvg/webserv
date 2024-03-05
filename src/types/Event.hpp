#pragma once
#include "aliases.hpp"
#include "external.hpp"

struct Event
{
    const bool isEof;
    const bool error;
    const uintptr_t identifier;
    const EVENT_TYPE type;
    const intptr_t amount;
};
