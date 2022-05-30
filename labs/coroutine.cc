#include "labs/coroutine.h"

void numltc(coroutine_t* pf_coro, f_t* pf_locals, uint64_t* pret, bool* pdone, uint64_t n)
{
    coroutine_t& f_coro = *pf_coro;
    uint64_t& ret = *pret;
    bool& done = *pdone;
    uint64_t&i = pf_locals->i;
    uint64_t&j = pf_locals->j;
    h_begin(f_coro);

    for(i=1; i<1000000; i++)
    {
        if (i < n) ret++;
        done=false;
        h_yield(f_coro);
    }
    done=true;
    h_end(f_coro);
}

void shell_step_coroutine(shellstate_t& shellstate, coroutine_t& f_coro, f_t& f_locals)
{

    //insert your code here
    uint8_t taskIndex = shellstate.AllotTaskIndex;
    if (shellstate.taskResources[taskIndex].cmd && compareString(shellstate.taskResources[taskIndex].name, "numltc"))
    {
        if (shellstate.taskResources[taskIndex].done == false)
        {
            numltc(&f_coro, &f_locals, &shellstate.taskResources[taskIndex].ret_val, &shellstate.taskResources[taskIndex].done, shellstate.taskResources[taskIndex].inp_arg);
        }
        else if (shellstate.taskResources[taskIndex].done==true)
        {
            shellstate.taskResources[taskIndex].cmd = false;
            shellstate.taskResources[taskIndex].done = true;
            shellstate.taskResources[taskIndex].ret = true;
            coroutine_reset(f_coro);
            f_locals.i=0;
        }
    }
}
