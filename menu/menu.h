#pragma once
#include "../globals.h"

class c_menu : public c_singleton<c_menu>{
public:
    struct {
        bool misc_bhop = true;
        bool misc_autostrafe = true;
    } config;
    
    void on_paint();
};
#define menu c_menu::instance()