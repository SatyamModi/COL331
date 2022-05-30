#include "util/io.h"

namespace vgatext{

   static inline void writechar(int loc, uint8_t c, uint8_t bg, uint8_t fg, addr_t base){
     //your code goes here
        uint16_t vga_rep = ((uint16_t)bg << 12) | ((uint16_t)fg << 8) | (uint16_t)c;
        mmio::write16(base, loc<<1, vga_rep);
   }

}//namespace vgatext
