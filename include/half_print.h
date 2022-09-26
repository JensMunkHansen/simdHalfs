// $Id: half_print.h,v 1.4 2008/06/06 11:34:45 jmh Exp $
#ifndef HALF_PRINT_H
#define HALF_PRINT_H

#ifdef __GNUC__
 #include <config.h>
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

typedef union FLOAT_32 FLOAT_32;
union FLOAT_32
{
    float    f32;
    uint32_t u32;
};

void 
print_float( float arg );

void 
print_half( uint16_t h );

#endif
