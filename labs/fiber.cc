#include "labs/fiber.h"

void numltf(addr_t *pmain_stack, addr_t *pf_stack, uint64_t *pret, bool *pdone, uint64_t n)
{
  addr_t &main_stack = *pmain_stack;
  addr_t &f_stack = *pf_stack;
  uint64_t &ret = *pret;
  bool &done = *pdone;

  uint64_t c = 1, count = 0;

  for (c = 1; c < 1000000; c++)
  {
    if (c < n)
      ret++;
    done = false;
    stack_saverestore(f_stack, main_stack);
  }

  for (;;)
  {
    done = true;
    stack_saverestore(f_stack, main_stack);
  }
}

void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, preempt_t& preempt,  addr_t& f_stack, addr_t f_array, uint32_t f_arraysize, dev_lapic_t& lapic)
{

  // insert your code here
  uint8_t taskIndex = shellstate.AllotTaskIndex;
  if (shellstate.taskResources[taskIndex].cmd && compareString(shellstate.taskResources[taskIndex].name, "numltf")) 
  {
    if (shellstate.taskResources[taskIndex].init == true)
    { 
      shellstate.taskResources[taskIndex].done = false;
      shellstate.taskResources[taskIndex].init = false;
      
      stack_init5(f_stack, f_array, f_arraysize, 
      		  &numltf, &main_stack, &f_stack, 
      		  &shellstate.taskResources[taskIndex].ret_val, 
      		  &shellstate.taskResources[taskIndex].done, 
      		  shellstate.taskResources[taskIndex].inp_arg);
    }
    
    if (shellstate.taskResources[taskIndex].done == false)
    {  
      stack_saverestore(main_stack, f_stack);
    }
    else if (shellstate.taskResources[taskIndex].done == true)
    { 
      shellstate.taskResources[taskIndex].cmd = false;
      shellstate.taskResources[taskIndex].done = true;
      shellstate.taskResources[taskIndex].ret = true;
    }
  }
  
}
