//$Id: half.h,v 1.2 2008/06/06 11:34:45 jmh Exp $
#ifndef HALF_H
#define HALF_H

#ifdef __GNUC__
 #include <config.h>
#endif

#ifdef _WIN32
# define HAVE_STDINT_H 1
#endif

#ifdef HAVE_STDINT_H
 #include <stdint.h>
#else
# ifndef inline
#  define inline __inline
# endif
# if !(defined(__STDC__) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))
   typedef unsigned int uint32_t; 
   typedef int int32_t; 
   typedef unsigned short uint16_t; 
   typedef short int16_t;
# endif
#endif

uint32_t half_to_float( uint16_t h );
uint16_t half_from_float( uint32_t f );
uint16_t half_add( uint16_t arg0, uint16_t arg1 );
uint16_t half_mul( uint16_t arg0, uint16_t arg1 );

static /*inline*/ uint16_t 
half_sub( uint16_t ha, uint16_t hb ) 
{
  // (a-b) is the same as (a+(-b))
  return half_add( ha, hb ^ 0x8000 );
}

#endif /* HALF_H */
 
