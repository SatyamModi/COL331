#include "labs/fiber_scheduler.h"

//
// stackptrs:      Type: addr_t[stackptrs_size].  array of stack pointers (generalizing: main_stack and f_stack)
// stackptrs_size: number of elements in 'stacks'.
//
// arrays:      Type: uint8_t [arrays_size]. array of memory region for stacks (generalizing: f_array)
// arrays_size: size of 'arrays'. equal to stackptrs_size*STACK_SIZE.
//
// Tip: divide arrays into stackptrs_size parts.
// Tip: you may implement a circular buffer using arrays inside shellstate_t
//      if you choose linked lists, create linked linked using arrays in
//      shellstate_t. (use array indexes as next pointers)
// Note: malloc/new is not available at this point.
//


void numgt(addr_t *pmain_stack, addr_t *pf_stack, uint64_t *pret, bool *pdone, uint64_t n, preempt_t *preempt)
{
  addr_t &main_stack = *pmain_stack;
  addr_t &f_stack = *pf_stack;
  uint64_t &ret = *pret;
  bool volatile &done = *pdone;
  	
  uint64_t c = 1, count = 0;

  for (c = 1; c < 200000000; c++)
  {
    if (c >= n)
      ret++;

    done = false;
  }
  
  for(;;)
  {
    done = true;
  }
}

uint64_t fibb_util(uint64_t n, addr_t *pmain_stack, addr_t *pf_stack, preempt_t *preempt) 
{
  if(n<=2) return 1;
  else 
  {
    addr_t &main_stack = *pmain_stack;
    addr_t &f_stack = *pf_stack;
    preempt->yield = 1;
    stack_saverestore(f_stack, main_stack);
    preempt->yield = 0;
  
    return fibb_util(n-1, pmain_stack, pf_stack, preempt)+fibb_util(n-2, pmain_stack, pf_stack, preempt);
  }
}

void fibb(addr_t *pmain_stack, addr_t *pf_stack, uint64_t *pret, bool *pdone, uint64_t n, preempt_t *ppreempt)
{
  addr_t &main_stack = *pmain_stack;
  addr_t &f_stack = *pf_stack;
  uint64_t &ret = *pret;
  bool &done = *pdone;
  preempt_t& preempt = *ppreempt;

  ret = fibb_util(n, pmain_stack, pf_stack, &preempt);
  for (;;)
  {
    done = true;
    preempt.yield = 1;
    stack_saverestore(f_stack, main_stack);
  }
}

void shell_step_fiber_scheduler(shellstate_t& shellstate,addr_t& main_stack, preempt_t& preempt, addr_t stackptrs[], size_t stackptrs_size, addr_t arrays, size_t arrays_size, dev_lapic_t& lapic){

  //insert your code here
  uint8_t taskIndex = shellstate.AllotTaskIndex;
  bool b1 = compareString(shellstate.taskResources[taskIndex].name, "numgt");
  bool b2 = compareString(shellstate.taskResources[taskIndex].name, "fibb");
  if (shellstate.taskResources[taskIndex].cmd && (b1 || b2))
  {
    if (shellstate.taskResources[taskIndex].init == true)
    {
      shellstate.taskResources[taskIndex].done = false;
      shellstate.taskResources[taskIndex].init = false;

      hoh_debug("taskIndex " << taskIndex);
      size_t f_arraysize = arrays_size / stackptrs_size;
      addr_t f_array = addr_t(arrays) + (taskIndex*f_arraysize);
      addr_t& f_stack = stackptrs[taskIndex]; 
      if (b1)
      {
        stack_init6(f_stack, f_array, f_arraysize, 
                    &numgt, &main_stack, &f_stack, 
                    &shellstate.taskResources[taskIndex].ret_val,
                    &shellstate.taskResources[taskIndex].done,
                    shellstate.taskResources[taskIndex].inp_arg, &preempt);
      }
      else
      {
        stack_init6(f_stack, f_array, f_arraysize, 
                    &fibb, &main_stack, &f_stack, 
                    &shellstate.taskResources[taskIndex].ret_val,
                    &shellstate.taskResources[taskIndex].done,
                    shellstate.taskResources[taskIndex].inp_arg, &preempt);
      }
      shellstate.numTask += 1;
      shellstate.currTask = taskIndex;
    }
  }

  //To know that the task is only a parallel task, we use bool b1 and b2
  b1 = compareString(shellstate.taskResources[shellstate.currTask].name, "numgt");
  b2 = compareString(shellstate.taskResources[shellstate.currTask].name, "fibb");
  if (shellstate.taskResources[shellstate.currTask].done == false && (b1 || b2))
  {
        if(b1)
        {
            preempt.yield = 0;
            preempt.saved_stack = stackptrs[shellstate.currTask];
            lapic.reset_timer_count(5000000);
            stack_saverestore(main_stack, stackptrs[shellstate.currTask]);
            preempt.yield = 1;
            stackptrs[shellstate.currTask] = preempt.saved_stack;
        }  
        else 
        {
          preempt.yield = 0;
          stack_saverestore(main_stack, stackptrs[shellstate.currTask]);
          preempt.yield = 1;
        }
  }

  if (shellstate.taskResources[shellstate.currTask].done == true && shellstate.taskResources[shellstate.currTask].cmd == true && (b1 || b2))
  {
    hoh_debug("Completed task " << shellstate.currTask);
    shellstate.taskResources[shellstate.currTask].cmd = false;
    shellstate.taskResources[shellstate.currTask].done = true;
    shellstate.taskResources[shellstate.currTask].ret = true;
    shellstate.numTask -= 1;
    if (compareString(shellstate.taskResources[shellstate.currTask].name, "numgt"))
    { 
      shellstate.numCommands[0] -= 1;
    }
    else
    {
      shellstate.numCommands[1] -= 1;
    }
  }

  //To know that there is a parallel task running, then only scheduling works
  if (shellstate.numTask > 0)
  {
    shellstate.currTask = (shellstate.currTask+1)%shellstate.maxTask;
    uint8_t count = 0;
    while (count < 5)
    {
      uint8_t i = (shellstate.currTask+count)%shellstate.maxTask;
      if (shellstate.taskResources[i].cmd)
      {
        shellstate.currTask = i;
        break;
      }
      else
      {
        count += 1;
      }
    }
  }
}
