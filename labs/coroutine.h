#pragma once

#include "labs/shell.h"
#include "util/coroutine.h"

// state for your coroutine implementation:
struct f_t{

    //insert your code here
    uint64_t i;
    uint64_t j;
};


void shell_step_coroutine(shellstate_t& shellstate, coroutine_t& f_coro, f_t& f_locals);


