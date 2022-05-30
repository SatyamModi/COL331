#pragma once
#include "util/config.h"
#include "util/debug.h"

struct longtask {
	
   char result[50];
   char name[50];
   bool cmd, init, done, ret;
   uint64_t inp_arg, ret_val;
   uint8_t row;

};

struct shellstate_t
{
  uint16_t num_key;
  char ch;
  uint8_t idx;
  uint8_t row, col;
  char command[50];
  char args[10][50];
  uint8_t argc;
  char result[50];
  bool fact, numlt;
  bool output, input, clear, bfull, lcmput, fcmput;   //lcmput is the bool to know coroutine or fiber commands | fcmput is the bool for parallel commands
  uint8_t AllotTaskIndex;         //To know the alloted task index
  uint8_t numTask;                //To know the total no. of running tasks
  uint8_t maxTask;                //To know the maximum no. of running tasks
  uint8_t currTask;               //To know the index of current Executing task
  uint8_t numCommands[2];         //To know number of queued commands, 0 => numgt, 1 => fibb
  uint8_t limit;
  longtask taskResources[5];
  
};

struct renderstate_t
{
  uint16_t num_key;
  char ch;
  uint8_t idx;
  uint8_t row;
  uint8_t col;
  uint8_t maxTask; 
  char command[50];
  char result[50];
  bool output;
  bool input;
  bool clear;
  bool bfull;
 
  longtask taskResources[5];
};

// computation helper functions
uint8_t stringLength(char *s);
bool compareString(char *a, const char *b);
void stringCopy(char *src, char *dst);
uint64_t factorial(uint64_t n);
uint64_t numlt(uint64_t n);
uint64_t char2dec(char *s);
void dec2str(int x, char arr[]);
static inline char dec2char(unsigned int i);

// shell functions
void refresh(shellstate_t &state);
void shell_init(shellstate_t &state);
void shell_update(uint8_t scankey, shellstate_t &stateinout);
void shell_step(shellstate_t &stateinout);
void shell_render(const shellstate_t &shell, renderstate_t &render);
bool render_eq(const renderstate_t &a, const renderstate_t &b);
void render(const renderstate_t &state, int w, int h, addr_t display_base);

// IO helper functions
static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawtext(int x, int y, const char *str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumberinhex(int x, int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumberindec(int x, int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void writecharxy(int x, int y, uint8_t c, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
