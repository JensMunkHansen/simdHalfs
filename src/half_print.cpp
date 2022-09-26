// $Id: half_print.cpp,v 1.3 2008/05/28 14:29:40 jmh Exp $
#include <stdio.h>

#include "half_print.h"

void 
print_float( float arg )
{
  FLOAT_32 f;
  int      i;

  f.f32 = arg;
  printf("0x%08x ",(unsigned int)f.u32);
  printf(" b");
  for ( i = 31; i >= 0; i-- ) 
  {
    int bit = (int)(( f.u32 & (1 << i) ) >> i);
    printf("%d",bit);
  }         
  printf(" %f",f.f32);
}

void 
print_half( uint16_t h )
{
  int i;

  printf("0x%04x ",(unsigned int)h);
  printf(" b");
  for ( i = 15; i >= 0; i-- ) 
  {
    int bit = (int)(( h & (1 << i) ) >> i);
    printf("%d",bit);
  }         
}
