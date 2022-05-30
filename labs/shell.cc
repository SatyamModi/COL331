#include "labs/shell.h"
#include "labs/vgatext.h"

uint8_t stringLength(char *s)
{
  uint8_t i;
  uint8_t cnt = 0;
  for (i = 0; s[i] != '\0'; i++)
    cnt++;
  return cnt;
}

bool compareString(char *a, const char *b)
{
  char *aa = a;
  const char *bb = b;

  while (*aa == *bb)
  {

    if (*aa == '\0')
      return 1;

    aa++;
    bb++;
  }
  return 0;
}

uint8_t tokenize(shellstate_t &state)
{

  uint8_t argi = 0, cnt = 0;
  bool prev = 1;

  for (uint8_t i = 0; state.command[i] != '\0'; i++)
  {

    if (state.command[i] == ' ' && !prev)
    {

      argi++;
      cnt = 0;
      prev = 1;
    }

    else if (state.command[i] == ' ')
    {
      prev = 1;
    }

    else
    {

      state.args[argi][cnt++] = state.command[i];
      prev = 0;
    }
  }

  if (state.args[argi][0] == '\0')
    argi--;

  return ++argi;
}

uint64_t factorial(uint64_t n)
{

  uint64_t res = 1;
  for (uint64_t i = 2; i <= n; i++)
    res *= i;
  return res;
}

uint64_t numlt(uint64_t n) {
  
    uint64_t count = 0;

    for(uint64_t c=1; c<200000000; c++) 
    {
  	  if(c<n) {
  	    count++;
  	  }
    }
    return count;
}

static inline char dec2char(unsigned int i)
{
  return "0123456789"[i % 10];
}

uint64_t char2dec(char *s)
{

  uint64_t res = 0;

  for (uint8_t i = 0; s[i] != '\0'; i++)
  {

    if (s[i] >= '0' && s[i] <= '9')
    {
      res *= 10;
      res += (s[i] - '0');
    }
    else
    {
      return -1;
    }
  }

  return res;
}

void stringCopy(char *src, char *dst)
{

  uint8_t i = 0;
  for (i = 0; src[i] != '\0'; i++)
    dst[i] = src[i];
  dst[i] = src[i];
}

void dec2str(int x, char arr[])
{
  char num[30];
  int max = 30;
  for (int i = 0; i < max; i++)
  {
    num[i] = dec2char(x % 10);
    x = x / 10;
  }

  int idx = -1;

  for (int i = 29; i >= 0; i--)
  {

    if (num[i] != '0')
    {
      idx = i;
      break;
    }
  }

  if (idx == -1)
  {
    arr[0] = '0';
    return;
  }

  int j = 0;

  for (int i = idx; i >= 0; i--)
  {
    arr[j++] = num[i];
  }
  arr[j] = '\0';
}

void refresh(shellstate_t &state)
{

  for (uint8_t i = 0; i < 50; i++)
    state.result[i] = 0;

  for (uint8_t j = 0; j < 10; j++)
  {

    for (uint8_t i = 0; i < 50; i++)
      state.args[j][i] = 0;
  }

  state.input = 0;
  state.fact = 0;
  state.numlt = 0;
  state.clear = 0;
  state.fcmput = 0;
  state.lcmput = 0;
}
//
// initialize shellstate
//
void shell_init(shellstate_t &state)
{
  state.num_key = 0;
  state.idx = 0;
  state.row = 1;
  state.col = 13;
  state.ch = '$';
  state.fact = false;
  state.numlt = false;
  state.fcmput = false;
  state.lcmput = false;
  state.numTask = 0;
  state.maxTask = 5;
  state.currTask = 0;
  state.limit = 3;
  state.numCommands[0] = 0;     //count for numgt
  state.numCommands[1] = 0;     //count for fibb
  // UPDATE
  for(uint8_t i=0;i<state.maxTask;i++) {
  
  	state.taskResources[i].cmd = 0;
  	state.taskResources[i].done = 1;
  
  }
  // END
  
  state.output = false;
  state.input = false;
  refresh(state);
  state.clear = 1;
  state.bfull = 0;
}

//
// handle keyboard event.
// key is in scancode format.
// For ex:
// scancode for following keys are:
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | esc |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |  0 |  - |  = |back|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 01  | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 0a | 0b | 0c | 0d | 0e |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | tab |  q |  w |  e |  r |  t |  y |  u |  i |  o |  p |  [ |  ] |entr|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 0f  | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 1a | 1b | 1c |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     |ctrl |  a |  s |  d |  f |  g |  h |  j |  k |  l |  ; |  ' |    |shft|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 1d  | 1e | 1f | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 2a |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
// so and so..
//
// - restrict yourself to: 0-9, a-z, esc, enter, arrows
// - ignore other keys like shift, control keys
// - only handle the keys which you're interested in
// - for example, you may want to handle up(0x48),down(0x50) arrow keys for menu.
//
void shell_update(uint8_t scankey, shellstate_t &stateinout)
{

  hoh_debug("Got: " << unsigned(scankey));
  stateinout.num_key++;
  
  if (stateinout.clear)
  {
    stateinout.clear = 0;
    stateinout.row = 0;
  }

  if (scankey == 0x1c && stateinout.idx > 47)
  {

    stateinout.row += 2;
    stateinout.ch = '$'; // enter
    stateinout.col = 13;
    stateinout.input = true;
    stateinout.command[49] = '\0';
    stateinout.idx = 0;
    return;
  }
  else if (stateinout.idx > 47)
  {
    return;
  }

  switch (scankey)
  {

  // digit
  case 0x02:
    stateinout.ch = '1';
    stateinout.command[stateinout.idx++] = '1';
    stateinout.col++;
    break;
  case 0x03:
    stateinout.ch = '2';
    stateinout.command[stateinout.idx++] = '2';
    stateinout.col++;
    break;
  case 0x04:
    stateinout.ch = '3';
    stateinout.command[stateinout.idx++] = '3';
    stateinout.col++;
    break;
  case 0x05:
    stateinout.ch = '4';
    stateinout.command[stateinout.idx++] = '4';
    stateinout.col++;
    break;
  case 0x06:
    stateinout.ch = '5';
    stateinout.command[stateinout.idx++] = '5';
    stateinout.col++;
    break;
  case 0x07:
    stateinout.ch = '6';
    stateinout.command[stateinout.idx++] = '6';
    stateinout.col++;
    break;
  case 0x08:
    stateinout.ch = '7';
    stateinout.command[stateinout.idx++] = '7';
    stateinout.col++;
    break;
  case 0x09:
    stateinout.ch = '8';
    stateinout.command[stateinout.idx++] = '8';
    stateinout.col++;
    break;
  case 0x0a:
    stateinout.ch = '9';
    stateinout.command[stateinout.idx++] = '9';
    stateinout.col++;
    break;
  case 0x0b:
    stateinout.ch = '0';
    stateinout.command[stateinout.idx++] = '0';
    stateinout.col++;
    break;

  // alphabet
  case 0x10:
    stateinout.ch = 'q';
    stateinout.command[stateinout.idx++] = 'q';
    stateinout.col++;
    break;
  case 0x11:
    stateinout.ch = 'w';
    stateinout.command[stateinout.idx++] = 'w';
    stateinout.col++;
    break;
  case 0x12:
    stateinout.ch = 'e';
    stateinout.command[stateinout.idx++] = 'e';
    stateinout.col++;
    break;
  case 0x13:
    stateinout.ch = 'r';
    stateinout.command[stateinout.idx++] = 'r';
    stateinout.col++;
    break;
  case 0x14:
    stateinout.ch = 't';
    stateinout.command[stateinout.idx++] = 't';
    stateinout.col++;
    break;
  case 0x15:
    stateinout.ch = 'y';
    stateinout.command[stateinout.idx++] = 'y';
    stateinout.col++;
    break;
  case 0x16:
    stateinout.ch = 'u';
    stateinout.command[stateinout.idx++] = 'u';
    stateinout.col++;
    break;
  case 0x17:
    stateinout.ch = 'i';
    stateinout.command[stateinout.idx++] = 'i';
    stateinout.col++;
    break;
  case 0x18:
    stateinout.ch = 'o';
    stateinout.command[stateinout.idx++] = 'o';
    stateinout.col++;
    break;
  case 0x19:
    stateinout.ch = 'p';
    stateinout.command[stateinout.idx++] = 'p';
    stateinout.col++;
    break;
  case 0x1e:
    stateinout.ch = 'a';
    stateinout.command[stateinout.idx++] = 'a';
    stateinout.col++;
    break;
  case 0x1f:
    stateinout.ch = 's';
    stateinout.command[stateinout.idx++] = 's';
    stateinout.col++;
    break;
  case 0x20:
    stateinout.ch = 'd';
    stateinout.command[stateinout.idx++] = 'd';
    stateinout.col++;
    break;
  case 0x21:
    stateinout.ch = 'f';
    stateinout.command[stateinout.idx++] = 'f';
    stateinout.col++;
    break;
  case 0x22:
    stateinout.ch = 'g';
    stateinout.command[stateinout.idx++] = 'g';
    stateinout.col++;
    break;
  case 0x23:
    stateinout.ch = 'h';
    stateinout.command[stateinout.idx++] = 'h';
    stateinout.col++;
    break;
  case 0x24:
    stateinout.ch = 'j';
    stateinout.command[stateinout.idx++] = 'j';
    stateinout.col++;
    break;
  case 0x25:
    stateinout.ch = 'k';
    stateinout.command[stateinout.idx++] = 'k';
    stateinout.col++;
    break;
  case 0x26:
    stateinout.ch = 'l';
    stateinout.command[stateinout.idx++] = 'l';
    stateinout.col++;
    break;
  case 0x2c:
    stateinout.ch = 'z';
    stateinout.command[stateinout.idx++] = 'z';
    stateinout.col++;
    break;
  case 0x2d:
    stateinout.ch = 'x';
    stateinout.command[stateinout.idx++] = 'x';
    stateinout.col++;
    break;
  case 0x2e:
    stateinout.ch = 'c';
    stateinout.command[stateinout.idx++] = 'c';
    stateinout.col++;
    break;
  case 0x2f:
    stateinout.ch = 'v';
    stateinout.command[stateinout.idx++] = 'v';
    stateinout.col++;
    break;
  case 0x30:
    stateinout.ch = 'b';
    stateinout.command[stateinout.idx++] = 'b';
    stateinout.col++;
    break;
  case 0x31:
    stateinout.ch = 'n';
    stateinout.command[stateinout.idx++] = 'n';
    stateinout.col++;
    break;
  case 0x32:
    stateinout.ch = 'm';
    stateinout.command[stateinout.idx++] = 'm';
    stateinout.col++;
    break;

  // other
  case 0x1c:
    stateinout.ch = '$'; // enter
    stateinout.command[stateinout.idx] = '\0';
    stateinout.row += 2;
    stateinout.col = 13;
    stateinout.idx = 0;
    stateinout.input = true;
    break;
  case 0x01:
    stateinout.ch = '#'; // esc
    stateinout.command[stateinout.idx++] = '#';
    stateinout.col++;
    break;
  case 0x48:
    stateinout.ch = '&'; // up
    stateinout.command[stateinout.idx++] = '&';
    stateinout.col++;
    break;
  case 0x50:
    stateinout.ch = '@'; // down
    stateinout.command[stateinout.idx++] = '@';
    stateinout.col++;
    break;
  case 0x39:
    stateinout.ch = ' '; // space
    stateinout.command[stateinout.idx++] = ' ';
    stateinout.col++;
    break;
  }

}

//
// do computation
//
void shell_step(shellstate_t &stateinout)
{

  //
  // one way:
  // if a function is enabled in stateinout
  // call that function( with arguments stored in stateinout) ;
  //    stateinout.args[0] = 5;
  //    stateinout.args[1] = 5;
  //

  if(stateinout.result[0]!=0) 
  {
  	for(uint8_t i=0;i<50;i++) stateinout.result[i]=0;
  }
  
  for(uint8_t i=0;i<stateinout.maxTask;i++) 
  {
  	if(stateinout.taskResources[i].result[0]!=0) 
    {
  		for(uint8_t j=0;j<50;j++) stateinout.taskResources[i].result[j]=0;
  	}
  }

  if (stateinout.input == true)
  {

    refresh(stateinout);
    stateinout.argc = tokenize(stateinout);

    if (compareString(stateinout.args[0], "fact"))
    {
      stateinout.fact = 1;
    }
    else if (compareString(stateinout.args[0], "clear"))
    {
      stateinout.clear = 1;
    }
    else if (compareString(stateinout.args[0], "numlt"))
    {
      stateinout.numlt = 1;
    }

    else if (compareString(stateinout.args[0], "numltc") || compareString(stateinout.args[0], "numltf"))
    {
      // ALLOCATE FIRST TASK RESOURCE
      // IN 2.3, 2.4, THIS WILL BE ALLOCATED DIFFERENTLY
      // seomthing like taskResources [ (val++)%5 ]
      hoh_debug("Inside numltc");

      for(uint8_t i = 0; i < stateinout.maxTask; i++)
      {
        if (stateinout.taskResources[i].done)
        {
          stateinout.AllotTaskIndex = i;
          stateinout.taskResources[i].cmd = 1;
          stringCopy(stateinout.args[0], stateinout.taskResources[i].name);
          stateinout.taskResources[i].row = stateinout.row;
          stateinout.lcmput = true;
          break;
        }
      }
    }

    else if (compareString(stateinout.args[0], "numgt") || compareString(stateinout.args[0], "fibb"))
    {
      bool b1 = (stateinout.numTask < stateinout.maxTask);
      bool b2 = (not compareString(stateinout.args[0], "numgt") || (stateinout.numCommands[0] < stateinout.limit));
      bool b3 = (not compareString(stateinout.args[0], "fibb")  || (stateinout.numCommands[1] < stateinout.limit));
      if (b1 == false)
      {
        char msg[50] = "Max. queuing limit exceeded";
        stringCopy(msg, stateinout.result);
      }
      else if (b2 == false || b3 == false)
      {
        char msg[50] = "Max. queuing limit for above task exceeded";
        stringCopy(msg, stateinout.result);
      }
      else
      {
        for(uint8_t i = 0; i < stateinout.maxTask; i++)
        {
          if (stateinout.taskResources[i].done)
          {
            stateinout.AllotTaskIndex = i;
            stateinout.taskResources[i].cmd = 1;
            stringCopy(stateinout.args[0], stateinout.taskResources[i].name);
            stateinout.taskResources[i].row = stateinout.row;
            stateinout.fcmput = true;
            break;
          }
        }
      }
    }

    else
    {
      char msg[50] = "No such command found: ";
      for (uint8_t j = 0; stateinout.args[0][j] != '\0'; j++) {
        	msg[23 + j] = stateinout.args[0][j];
      }
      stringCopy(msg, stateinout.result);
    }

    if (stateinout.fact)
    {

      if (stateinout.argc != 2)
      {
        char msg[50] = "Invalid arguments! Usage: 'fact 5'";
        stringCopy(msg, stateinout.result);
      }

      else
      {
        uint64_t n = char2dec(stateinout.args[1]);
        if (n == -1)
        {
          char msg[50] = "Invalid arguments! Usage: 'fact 5'";
          stringCopy(msg, stateinout.result);
        }
        else
        {
          uint64_t ans = factorial(n);
          dec2str(ans, stateinout.result);
        }
      }
    }
    else if (stateinout.numlt)
    {
      if (stateinout.argc != 2)
      {
        char msg[50] = "Invalid arguments! Usage: 'numlt 5'";
        stringCopy(msg, stateinout.result);
	      stateinout.numlt=false;
      }

      else
      {
        uint64_t n = char2dec(stateinout.args[1]);

        if (n == -1)
        {
          char msg[50] = "Invalid arguments! Usage: 'numlt 5'";
          stringCopy(msg, stateinout.result);
	        stateinout.numlt=false;
        }
        else
        {
	        stateinout.numlt=false;
          uint64_t ans = numlt(n);
          dec2str(ans, stateinout.result);
        }
      }
    }
    else if (stateinout.lcmput)
    {
      uint8_t TaskIndex = stateinout.AllotTaskIndex;
      if (stateinout.argc != 2)
      {
        char msg[50] = "Invalid arguments! Usage: '<long-task-name> 5'";
        stringCopy(msg, stateinout.taskResources[TaskIndex].result);
        stateinout.taskResources[TaskIndex].cmd=false;
        stateinout.lcmput=false;
      }
      else
      {
        uint64_t n = char2dec(stateinout.args[1]);

        if (n == -1)
        {
            char msg[50] = "Invalid arguments! Usage: '<long-task-name> 5'";
            stringCopy(msg, stateinout.taskResources[TaskIndex].result);
            stateinout.taskResources[TaskIndex].cmd=false;
            stateinout.lcmput=false;
        }
        else
        {
          hoh_debug(stateinout.taskResources[TaskIndex].name);
          hoh_debug("<long-task> initiated");
          stateinout.taskResources[TaskIndex].done = 0;
          stateinout.taskResources[TaskIndex].init = 1;
          stateinout.taskResources[TaskIndex].inp_arg = n;
          stateinout.lcmput=false;
        }
      }
    }
    else if (stateinout.fcmput)
    {
      uint8_t TaskIndex = stateinout.AllotTaskIndex;
      if (stateinout.argc != 2)
      {
        char msg[50] = "Invalid arguments! Usage: '<long-task-name> 5'";
        stringCopy(msg, stateinout.taskResources[TaskIndex].result);
        stateinout.taskResources[TaskIndex].cmd=false;
        stateinout.fcmput=false;
      }
      else
      {
        uint64_t n = char2dec(stateinout.args[1]);

        if (n == -1)
        {
          char msg[50] = "Invalid arguments! Usage: '<long-task-name> 5'";
          stringCopy(msg, stateinout.taskResources[TaskIndex].result);
          stateinout.taskResources[TaskIndex].cmd=false;
          stateinout.fcmput=false;
        }
        else
        {
          hoh_debug(stateinout.taskResources[TaskIndex].name);
          hoh_debug("<parallel-task> initiated");
          stateinout.taskResources[TaskIndex].done = 0;
          stateinout.taskResources[TaskIndex].init = 1;
          stateinout.taskResources[TaskIndex].inp_arg = n;
          stateinout.fcmput=false;
          if (compareString(stateinout.taskResources[TaskIndex].name, "numgt"))
          {
            stateinout.numCommands[0] += 1;
          }
          else
          {
            stateinout.numCommands[1] += 1;
          }
        }
      }
    }
      
    for (uint8_t i = 0; i < 50; i++)
      stateinout.command[i] = '\0';
  }
  
  else 
  {
  	for(uint8_t i=0;i<stateinout.maxTask;i++) 
    {
   		if(stateinout.taskResources[i].ret) 
      {
   			stateinout.taskResources[i].ret = 0;
  			dec2str(stateinout.taskResources[i].ret_val, stateinout.taskResources[i].result);
  			
  			hoh_debug(stateinout.taskResources[i].name);
  			hoh_debug("Output of task " << i);
  			hoh_debug(stateinout.taskResources[i].result);		

  			// Deallocate ith task resource
        stateinout.taskResources[i].ret_val = 0;
  			stateinout.taskResources[i].init = 0;
  			stateinout.taskResources[i].done = 1;
  			stateinout.taskResources[i].cmd = 0;
  			stateinout.taskResources[i].ret = 0; 
   		} 	
  	}
  }
}

//
// shellstate --> renderstate
//
void shell_render(const shellstate_t &shell, renderstate_t &render)
{

  //
  // renderstate. number of keys pressed = shellstate. number of keys pressed
  //
  // renderstate. menu highlighted = shellstate. menu highlighted
  //
  // renderstate. function result = shellstate. output argument
  //
  // etc.
  //
  render.num_key = shell.num_key;
  render.ch = shell.ch;
  render.idx = shell.idx;
  render.row = shell.row;
  render.col = shell.col;
  render.output = shell.output;
  render.input = shell.input;
  render.clear = shell.clear;
  render.maxTask = shell.maxTask;
  // UPDATE
  for(uint8_t i=0;i<shell.maxTask;i++) 
  {
     render.taskResources[i] = shell.taskResources[i];
  }
  // ENd
  for (int i = 0; i < 50; i++)
  {
    render.command[i] = shell.command[i];
    render.result[i] = shell.result[i];
  }
}

//
// compare a and b
//

bool render_eq(const renderstate_t &a, const renderstate_t &b)
{

  bool b1 = (a.num_key == b.num_key) && (a.ch == b.ch) && (a.idx == b.idx) && (a.row == b.row) && (a.col == b.col);
  bool b2 = (a.output == b.output) && (a.input == b.input) && (a.clear == b.clear); 


  for(uint8_t i=0;i<a.maxTask;i++) 
  {
    if(b.taskResources[i].cmd != a.taskResources[i].cmd) return false;
    if(b.taskResources[i].ret != a.taskResources[i].ret) return false;
    if(b.taskResources[i].init != a.taskResources[i].init) return false;
    if(b.taskResources[i].done != a.taskResources[i].done) return false;
    if(b.taskResources[i].ret_val != a.taskResources[i].ret_val) return false;
    if(b.taskResources[i].inp_arg != a.taskResources[i].inp_arg) return false;
    if(b.taskResources[i].row != a.taskResources[i].row) return false;

  	for(uint8_t j=0;j<50;j++) 
    {
  		if(b.taskResources[i].result[j]!=a.taskResources[i].result[j]) return false;
  		if(b.taskResources[i].name[j]!=a.taskResources[i].name[j]) return false;
  	}
  }

  if ((b1 && b2) == 0)
    return false;

  bool b3 = 1;
  for (uint8_t i = 0; i < 50; i++)
  {
    if ((a.command[i] != b.command[i]) || (a.result[i] != b.result[i]))
      b3 = 0;
  }

  if (b3 == 0)
    return false;

  return true;
}

//
// Given a render state, we need to write it into vgatext buffer
//
void render(const renderstate_t &state, int w, int h, addr_t vgatext_base)
{

//	hoh_debug("ooo");
//	hoh_debug(state.taskResources[0].result);

  // KEY PRESSED STATUS BAR
  const char *p = "Keys pressed: ";
  drawtext(45, 0, p, 14, 0, 2, w, h, vgatext_base);
  drawnumberindec(59, 0, state.num_key, 10, 0, 2, w, h, vgatext_base);
  
  // TYPING CHARS
  if(state.ch!='$') {
  	writecharxy(state.col, state.row, state.ch, 0, 7, w, h, vgatext_base);
  }

  // CLEAR SCREEN
  if (state.clear)
  {
    fillrect(0, 0, w, h, 0, 7, w, h, vgatext_base);
  }

  // PRINT PROMPT
  if (state.ch=='$' && !state.clear)
  {
    //hoh_debug("hello");
    drawtext(0, state.row, "col331-user:$ ", 14, 0, 3, w, h, vgatext_base);
  }
  
  // PRINT SHORT TASK RESULT (IF ANY)
  if (state.result[0]!=0 && !state.clear) {
  
     drawtext(0, state.row - 1, state.result, 50, 0, 7, w, h, vgatext_base);
  }
  
  // PRINT LONG TASK RESULTS (IF ANY)
  for(uint8_t i=0;i<state.maxTask;i++) {
	  if (state.taskResources[i].result[0]!=0) {
		//hoh_debug(i);
		//hoh_debug(state.taskResources[i].row);
	     drawtext(0, state.taskResources[i].row-1, state.taskResources[i].result, 50, 0, 7, w, h, vgatext_base);
	  }
  }
  
  // ????? (DON'T REMEMBER)
  if (state.clear)
  {
    drawtext(0, 0, state.result, 50, 0, 7, w, h, vgatext_base);
    drawtext(state.col - 13, 0, "col331-user:$ ", 14, 0, 3, w, h, vgatext_base);
  }
}

//
//
// helper functions
//
//

static void writecharxy(int x, int y, uint8_t c, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base)
{
  vgatext::writechar(y * w + x, c, bg, fg, vgatext_base);
}

static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base)
{
  for (int y = y0; y < y1; y++)
  {
    for (int x = x0; x < x1; x++)
    {
      writecharxy(x, y, 0, bg, fg, w, h, vgatext_base);
    }
  }
}

static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base)
{

  writecharxy(x0, y0, 0xc9, bg, fg, w, h, vgatext_base);
  writecharxy(x1 - 1, y0, 0xbb, bg, fg, w, h, vgatext_base);
  writecharxy(x0, y1 - 1, 0xc8, bg, fg, w, h, vgatext_base);
  writecharxy(x1 - 1, y1 - 1, 0xbc, bg, fg, w, h, vgatext_base);

  for (int x = x0 + 1; x + 1 < x1; x++)
  {
    writecharxy(x, y0, 0xcd, bg, fg, w, h, vgatext_base);
  }

  for (int x = x0 + 1; x + 1 < x1; x++)
  {
    writecharxy(x, y1 - 1, 0xcd, bg, fg, w, h, vgatext_base);
  }

  for (int y = y0 + 1; y + 1 < y1; y++)
  {
    writecharxy(x0, y, 0xba, bg, fg, w, h, vgatext_base);
  }

  for (int y = y0 + 1; y + 1 < y1; y++)
  {
    writecharxy(x1 - 1, y, 0xba, bg, fg, w, h, vgatext_base);
  }
}

static void drawtext(int x, int y, const char *str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base)
{

  for (int i = 0; i < maxw; i++)
  {
    writecharxy(x + i, y, str[i], bg, fg, w, h, vgatext_base);
    if (!str[i])
    {
      break;
    }
  }
}

static void drawnumberinhex(int x, int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base)
{
  enum
  {
    max = sizeof(uint32_t) * 2 + 1
  };
  char a[max];
  for (int i = 0; i < max - 1; i++)
  {
    a[max - 1 - i - 1] = hex2char(number % 16);
    number = number / 16;
  }
  a[max - 1] = '\0';

  drawtext(x, y, a, maxw, bg, fg, w, h, vgatext_base);
}

static void drawnumberindec(int x, int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base)
{
  enum
  {
    max = sizeof(uint32_t) * 2 + 1
  };
  char a[max];
  for (int i = 0; i < max - 1; i++)
  {
    a[max - 1 - i - 1] = dec2char(number % 10);
    number = number / 10;
  }
  a[max - 1] = '\0';

  drawtext(x, y, a, maxw, bg, fg, w, h, vgatext_base);
}