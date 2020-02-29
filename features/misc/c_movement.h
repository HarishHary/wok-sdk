#pragma once
#include "../../globals.h"

class c_movement : public c_singleton<c_movement> {
public:
    void bhop();
    void autostrafe();
};
#define movement c_movement::instance()