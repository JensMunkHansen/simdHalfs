//$Id: hmmintrin.h,v 1.29 2009/03/14 00:19:00 jmh Exp $
#ifndef HMMINTRIN_H
#define HMMINTRIN_H

// TODO: use LZCNT of SSE4a, call __cpuid intrinsic with InfoType=0x80000001 and check bit 5 of CPUInfo[2] (ECX). 

//---------------------------------------------------------------------------
//
// Implementation --
//
// Representation of a float:
//
//      We assume that a float, f, is an IEEE 754 single-precision
//      floating point number, whose bits are arranged as follows:
//
//          31 (msb)
//          | 
//          | 30     23
//          | |      | 
//          | |      | 22                    0 (lsb)
//          | |      | |                     |
//          X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
//
//          s e        m
//
//      S is the sign-bit, e is the exponent and m is the significand.
//
//      If e is between 1 and 254, f is a normalized number:
//
//                  s    e-127
//          f = (-1)  * 2      * 1.m
//
//      If e is 0, and m is not zero, f is a denormalized number:
//
//                  s    -126
//          f = (-1)  * 2      * 0.m
//
//      If e and m are both zero, f is zero:
//
//          f = 0.0
//
//      If e is 255, f is an "infinity" or "not a number" (NAN),
//      depending on whether m is zero or not.
//
//      Examples:
//
//          0 00000000 00000000000000000000000 = 0.0
//          0 01111110 00000000000000000000000 = 0.5
//          0 01111111 00000000000000000000000 = 1.0
//          0 10000000 00000000000000000000000 = 2.0
//          0 10000000 10000000000000000000000 = 3.0
//          1 10000101 11110000010000000000000 = -124.0625
//          0 11111111 00000000000000000000000 = +infinity
//          1 11111111 00000000000000000000000 = -infinity
//          0 11111111 10000000000000000000000 = NAN
//          1 11111111 11111111111111111111111 = NAN
//
// Representation of a HalfFloat:
//
//      Here is the bit-layout for a HalfFloat number, h:
//
//          15 (msb)
//          | 
//          | 14  10
//          | |   |
//          | |   | 9        0 (lsb)
//          | |   | |        |
//          X XXXXX XXXXXXXXXX
//
//          s e     m
//
//      S is the sign-bit, e is the exponent and m is the significand.
//
//      If e is between 1 and 30, h is a normalized number:
//
//                  s    e-15
//          h = (-1)  * 2     * 1.m
//
//      If e is 0, and m is not zero, h is a denormalized number:
//
//                  S    -14
//          h = (-1)  * 2     * 0.m
//
//      If e and m are both zero, h is zero:
//
//          h = 0.0
//
//      If e is 31, h is an "infinity" or "not a number" (NAN),
//      depending on whether m is zero or not.
//
//      Examples:
//
//          0 00000 0000000000 = 0.0
//          0 01110 0000000000 = 0.5
//          0 01111 0000000000 = 1.0
//          0 10000 0000000000 = 2.0
//          0 10000 1000000000 = 3.0
//          1 10101 1111000001 = -124.0625
//          0 11111 0000000000 = +infinity
//          1 11111 0000000000 = -infinity
//          0 11111 1000000000 = NAN
//          1 11111 1111111111 = NAN
//
// Conversion:
//
//      Converting from a float to a HalfFloat requires some non-trivial bit
//      manipulations.  In some cases, this makes conversion relatively
//      slow, but the most common case is accelerated via table lookups.
//
//      Converting back from a HalfFloat to a float is easier because we don't
//      have to do any rounding.  In addition, there are only 65536
//      different HalfFloat numbers; we can convert each of those numbers once
//      and store the results in a table.  Later, all conversions can be
//      done using only simple table lookups.
//
//---------------------------------------------------------------------------

#ifdef _MSC_VER
# pragma warning( disable : 4238 ) // suppress warning C4238: nonstandard extension used : class rvalue used as lvalue
# pragma warning( disable : 4127 ) // suppress warning C4127: conditional expression is constant
#endif

#if (defined(_MSC_VER) && defined(_WIN32))
# define ALIGN16_BEGIN __declspec(align(16))
# define ALIGN16_END 
#elif defined(__GNUC__)
# define ALIGN16_BEGIN
# define ALIGN16_END __attribute__((aligned(16)))
#endif

#if defined(__GNUC__)
//# define _mm_castps_si128(x) (__m128i) x
//# define _mm_castsi128_ps(x) (__m128) x
#endif

#if defined(__GNUC__)
# define GCC_SPLIT_BLOCK(str)  __asm__( "//\n\t// " str "\n\t//\n" );
#elif defined(_WIN64)
# define GCC_SPLIT_BLOCK(str)
#else
# define GCC_SPLIT_BLOCK(str) __asm { }
#endif

#ifdef _MSC_VER
 #define HAVE_EMMINTRIN_H 1
 #define HAVE_PMMINTRIN_H 1
 #define HAVE_TMMINTRIN_H 1
 #define HAVE_SMMINTRIN_H 1
 #define HAVE_NMMINTRIN_H 1
#else
 #include "config.h"
#endif

// MMX
#include <mmintrin.h>

// SSE
#include <xmmintrin.h>

// SSE2
#if defined(HAVE_EMMINTRIN_H)
 #include <emmintrin.h>
#endif

// SSE3
#if defined(HAVE_PMMINTRIN_H)
# include <pmmintrin.h>
#endif

// SSSS3
#if defined(HAVE_TMMINTRIN_H)
# include <tmmintrin.h>
#endif

// SSE4.1
#if defined(HAVE_SMMINTRIN_H)
# include <smmintrin.h>
#endif

// SSE4.2 - CRC
#if defined(HAVE_NMMINTRIN_H)
# include <nmmintrin.h>
#endif

#ifdef __GNUC__
 #include <config.h>
#endif

#ifdef HAVE_STDINT_H
 #include <stdint.h>
#else
# if !(defined(__STDC__) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))
   typedef unsigned int uint32_t; 
   typedef int int32_t; 
   typedef unsigned short uint16_t; 
   typedef short int16_t;
# endif
#endif

#ifdef __GNUC__
# define SPS_INLINE inline
#elif defined(_MSC_VER)
# define SPS_INLINE __forceinline
#endif

// Standard C headers
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include <iostream>

// _mm_empty before float assignments

uint16_t extract_uint16(__m128i vin, size_t inx) {
  _mm_empty();
  ALIGN16_BEGIN uint16_t output[8] ALIGN16_END;
  _mm_store_si128((__m128i*)output,vin);
  return output[inx];
};

uint32_t extract_uint32(__m128 vin, size_t inx) {
  _mm_empty();
  ALIGN16_BEGIN uint32_t output[4] ALIGN16_END;
  _mm_store_si128((__m128i*)output,_mm_castps_si128(vin));
  return output[inx];
};


float extract_f32(__m128 vin, size_t inx) {
  _mm_empty();
  ALIGN16_BEGIN float output[4] ALIGN16_END;
  _mm_store_ps((float*)output,vin);
  return output[inx];
};


// Load immediate
static SPS_INLINE __m128i _mm_li_epi16(const uint16_t& a)
{
  return (_mm_set1_epi16(a));
}

// Decrement
static SPS_INLINE __m128i _mm_dec_epi16(const __m128i& a) 
{
  return _mm_sub_epi16(a,_mm_set1_epi16(0x01));
}

// Increment
static SPS_INLINE __m128i _mm_inc_epi16(const __m128i &a) 
{
  return _mm_add_epi16(a,_mm_set1_epi16(0x01));
}

// Complement
static SPS_INLINE __m128i _mm_not_epi16(const __m128i &a) 
{
  return _mm_xor_si128(a,_mm_set1_epi16((short)0xFFFF));
}

// Negate
static SPS_INLINE __m128i _mm_neg_epu16(const __m128i &a) 
{
  return _mm_inc_epi16(_mm_not_epi16(a));
}

// Extend sign
static SPS_INLINE __m128i _mm_ext_epi16(const __m128i &a) 
{
  return _mm_srai_epi16(a, 15);
}

// And
static SPS_INLINE __m128i _mm_and_epi16(const __m128i &a, const __m128i &b ) 
{
  return _mm_and_si128(a,b);
}

// And integer
static SPS_INLINE __m128i _mm_and1_epi16(const __m128i &a, const uint16_t &b ) 
{
  return _mm_and_si128(a,_mm_set1_epi16((short)b));
}

// Or
static SPS_INLINE __m128i _mm_or_epi16(const __m128i &a, const __m128i &b ) 
{
  return _mm_or_si128(a,b);
}

// Exclusive or
static SPS_INLINE __m128i _mm_xor_epi16(const __m128i &a, const __m128i &b ) 
{
  return _mm_xor_si128(a,b);
}

// And with complement, a & ~b
static SPS_INLINE __m128i _mm_andc_epi16(const __m128i a, __m128i b ) 
{
  return _mm_andnot_si128(b,a);
}

// Shift Right Logical Integer
static SPS_INLINE __m128i _mm_half_srli(const __m128i &a, const int &sa) 
{
  return _mm_srli_epi16(a,sa);
}

// Shift Right Logical
static SPS_INLINE __m128i _mm_half_srl(const __m128i& x, const __m128i& y ) 
{
  const ALIGN16_BEGIN unsigned int bias[4] ALIGN16_END = {127, 127, 127, 127};

  __m128i retval;

#if (defined(_MSC_VER) && defined(_WIN32))
  __asm {   
    mov edi, x
    mov esi, y
    movups xmm0, [edi]
    movups xmm1, [esi]

    paddd       xmm1, bias   // y in xmm1
    pslld       xmm1, 23

    cvtdq2ps    xmm0, xmm0   // x in xmm0
    divps       xmm0, xmm1
    cvttps2dq   xmm0, xmm0
    movups [retval], xmm0      
  }
#elif defined(__linux__) || defined(__ICC) || defined(__GNUC__)
  __asm__ __volatile__
    ( "movups    %[x],   %%xmm0    \n\t"
      "movups    %[y],   %%xmm1    \n\t"
      "paddd     %[bias],%%xmm1    \n\t" 
      "pslld     $23,    %%xmm1    \n\t" 
                                  
      "cvtdq2ps  %%xmm0, %%xmm0    \n\t" 
      "divps     %%xmm1, %%xmm0    \n\t" 
      "cvttps2dq %%xmm0, %%xmm0    \n\t" 
      "movups    %%xmm0, %[output] \n\t" 
      : [output] "=m" (retval)
      : [x] "m" (x), [y] "m" (y), [bias] "m" (*bias)
      : "%xmm0", "%xmm1"
    );
#endif
  return retval;
}

// Shift Left Logical Integer
static SPS_INLINE __m128i _mm_half_slli(const __m128i a, int sa) 
{
  return _mm_slli_epi16(a,sa);
}

// Shift Left Logical, recall x << y = x * 2^y and float = s * 2^(e - 127) * m
static SPS_INLINE __m128i _mm_half_sll(const __m128i& x, const __m128i& y ) 
{
  const ALIGN16_BEGIN unsigned int bias[4] ALIGN16_END = {127, 127, 127, 127};

  __m128i retval;

#if (defined(_MSC_VER) && defined(_WIN32))
  __asm {   
    mov edi, x
    mov esi, y
    movups xmm0, [edi]
    movups xmm1, [esi]

    paddd       xmm1, bias   // y in xmm1
    pslld       xmm1, 23

    cvtdq2ps    xmm0, xmm0   // x in xmm0
    mulps       xmm0, xmm1
    cvttps2dq   xmm0, xmm0
    movups [retval], xmm0      
  }
#elif defined(__linux__) || defined(__ICC) || defined(__GNUC__)
  __asm__ __volatile__
    ( "movups    %[x],   %%xmm0    \n\t"
      "movups    %[y],   %%xmm1    \n\t"
      "paddd     %[bias],%%xmm1    \n\t" 
      "pslld     $23,    %%xmm1    \n\t" 
                                  
      "cvtdq2ps  %%xmm0, %%xmm0    \n\t" 
      "mulps     %%xmm1, %%xmm0    \n\t" 
      "cvttps2dq %%xmm0, %%xmm0    \n\t" 
      "movups    %%xmm0, %[output] \n\t" 
      : [output] "=m" (retval)
      : [x] "m" (x), [y] "m" (y), [bias] "m" (*bias)
      : "%xmm0", "%xmm1"
      );
#endif
  return retval;
}

// And then Shift Right Logical Integer
static SPS_INLINE __m128i _mm_andsrli_epi16(const __m128i a, __m128i b, int sa) 
{
  return _mm_half_srli(_mm_and_epi16(a,b),sa);
}

// Shift Right Logical Integer then Mask
static SPS_INLINE __m128i _mm_srlmi_epi16(const __m128i a, int sa, __m128i mask ) 
{
  return _mm_and_epi16(_mm_half_srli(a,sa),mask);
}

// Shift Right Logical then Mask
static SPS_INLINE __m128i _mm_srlmii_epi16(const __m128i a, int sa, uint16_t mask ) 
{
  return _mm_and_epi16(_mm_half_srli(a,sa),_mm_set1_epi16((short)mask));
}

// Add then Mask
static SPS_INLINE __m128i _mm_addmi_epi16(const __m128i a, __m128i b, __m128i mask ) 
{
  return _mm_and_epi16(_mm_add_epi16(a,b),mask);
}

// Add then Mask
static SPS_INLINE __m128i _mm_addm_epi16(const __m128i a, __m128i b, uint16_t mask ) 
{
  return _mm_and_epi16(_mm_add_epi16(a,b),_mm_set1_epi16((short)mask));
}

// Select Bits on mask
static SPS_INLINE __m128i _mm_selb_epi16(const __m128i &mask, __m128i &a, __m128i &b ) 
{
  const __m128i sel_a  = _mm_and_epi16(a, mask);
  const __m128i sel_b  = _mm_andc_epi16(b, mask);
  const __m128i result = _mm_or_epi16(sel_a, sel_b );

  return (result);
}

// Multiply - low result
static SPS_INLINE __m128i _mm_half_mullo(const __m128i a, const __m128i b ) 
{
  return _mm_mullo_epi16(a,b);
}

//////////////////////////////////////////////////////////////////
// 32-bit
//////////////////////////////////////////////////////////////////

// Load immediate
static SPS_INLINE __m128i _mm_li_epi32(const uint32_t& a) 
{
  return _mm_set1_epi32(a);
}

// Increment
static SPS_INLINE __m128i _mm_inc_epi32(const __m128i a) 
{
  return _mm_add_epi32(a,_mm_set1_epi32(0x01));
}

// Decrement
static SPS_INLINE __m128i _mm_dec_epi32(const __m128i a) 
{
  return _mm_sub_epi32(a,_mm_set1_epi32(0x01));
}

// Complement
static SPS_INLINE __m128i _mm_not_epi32(const __m128i a) 
{
  return _mm_xor_si128(a,_mm_set1_epi32((int)0xFFFFFFFFL));
}

// Negate
static SPS_INLINE __m128i _mm_neg_epu32(const __m128i a) 
{
  return _mm_inc_epi32(_mm_not_epi32(a));
}

// And
static SPS_INLINE __m128i _mm_and_epi32(const __m128i &a, const __m128i &b ) 
{
  return _mm_and_si128(a,b);
}

// And Integer
static SPS_INLINE __m128i _mm_andi_epi32(const __m128i &a, const uint32_t &b ) 
{
  return _mm_and_si128(a,_mm_set1_epi32((int)b));
}

// Or
static SPS_INLINE __m128i _mm_or_epi32(const __m128i &a, const __m128i &b ) 
{
  return _mm_or_si128(a,b);
}

// Exclusive Or
static SPS_INLINE __m128i _mm_xor_epi32(const __m128i &a, const __m128i &b ) 
{
  return _mm_xor_si128(a,b);
}

// And with Complement, a & ~b
static SPS_INLINE __m128i _mm_andc_epi32(const __m128i& a, const __m128i& b ) 
{
  return _mm_andnot_si128(b,a);
}

// Shift Right Logical Integer then Mask Integer
static SPS_INLINE __m128i _mm_srlm_epi32(const __m128i a, int sa, uint32_t mask ) 
{
  return _mm_and_epi32(_mm_srli_epi32(a,sa),_mm_set1_epi32((int)mask));
}

// Add then Mask
static SPS_INLINE __m128i _mm_addm_epi32(const __m128i a, __m128i b, uint32_t mask ) 
{
  return _mm_and_epi32(_mm_add_epi32(a,b),_mm_set1_epi32((int)mask));
}

// Select Bits on mask
static SPS_INLINE __m128i _mm_selb_epi32(const __m128i &mask, const __m128i &a, const __m128i &b ) 
{
  const __m128i sel_a  = _mm_and_epi32(a, mask);
  const __m128i sel_b  = _mm_andc_epi32(b, mask);
  const __m128i result = _mm_or_epi32(sel_a, sel_b );

  return (result);
}

// Comparison, uint16 
static SPS_INLINE __m128i _mm_cmpgt_epu16(const __m128i &val1, const __m128i &val2) 
{
  __m128i mask  = _mm_cmpgt_epi16(val1, val2);
  __m128i t1    = _mm_srai_epi16(val1, 15);
  __m128i t2    = _mm_srai_epi16(val2, 15);
    

  mask          = _mm_andnot_si128(_mm_xor_si128(t1, t2), mask);

  __m128i res   = _mm_andnot_si128(t2, t1);

  return          _mm_or_si128(res, mask);

}

// Max, uint16
static SPS_INLINE __m128i _mm_max_epu16(const __m128i &val1, const __m128i &val2)  {
  __m128i mask    = _mm_cmpgt_epu16(val1, val2);

  __m128i ret     = _mm_and_si128(mask, val1);
          mask    = _mm_andnot_si128(mask, val2);

  return            _mm_or_si128(ret, mask);
}

// Min, uint16
static SPS_INLINE __m128i _mm_min_epu16(const __m128i &val1, const __m128i &val2)  {

  __m128i mask    = _mm_cmpgt_epu16(val1, val2);

  __m128i ret1    = _mm_and_si128(mask, val2);
          mask    = _mm_andnot_si128(mask, val1);

  return            _mm_or_si128(ret1,mask);
}

// Absolute, int16
static SPS_INLINE __m128i _mm_abs_epi16(const __m128i &src)  // returns the absolute value of the 16bit signed value.
{
  __m128i complement, temp,dst  ;
  temp = _mm_srai_epi16(src, 15);
  complement = _mm_xor_si128(src, temp);  //one's complement of src
  dst  = _mm_sub_epi16 (complement, temp);
  temp = _mm_srli_epi16(dst, 15);
  dst  = _mm_sub_epi16(dst, temp);
  return dst;
}

// Absolute, int32
static SPS_INLINE __m128i mm_abs_epi32(const __m128i &src)  // returns the absolute value of the 16bit signed value.
{
  __m128i complement, temp,dst  ;
  temp = _mm_srai_epi32(src, 31);
  complement = _mm_xor_si128(src, temp);  //one's complement of src
  dst  = _mm_sub_epi32 (complement, temp);
  temp = _mm_srli_epi32(dst, 31);
  dst  = _mm_sub_epi32(dst, temp);
  return dst;
}

// Complement
static SPS_INLINE __m128i _mm_not_si128(const __m128i a) 
{
  return _mm_xor_si128(a,_mm_set1_epi32((int)0xFFFFFFFFL));
}

// Extend sign
static SPS_INLINE __m128i _mm_ext_epi32(const __m128i a) 
{
  return _mm_srai_epi32(a, 31);
}

// Select on sign bit
static SPS_INLINE __m128i _mm_sels_epi32(const __m128i test, __m128i a, __m128i b )  {
  const __m128i mask = _mm_srai_epi32(test, 31);
  const __m128i sel_a  = _mm_and_epi16(  a,     mask  );
  const __m128i sel_b  = _mm_andc_epi32( b,     mask  );
  const __m128i result = _mm_or_epi16(   sel_a, sel_b );

  return (result);
}

// Compare greater unsigned long
static SPS_INLINE __m128i _mm_cmpgt_epu32(const __m128i &val1, const __m128i &val2) 
{
  __m128i mask  = _mm_cmpgt_epi32(val1, val2);
  __m128i t1    = _mm_srai_epi32(val1, 31);
  __m128i t2    = _mm_srai_epi32(val2, 31);
    
  mask          = _mm_andnot_si128(_mm_xor_si128(t1, t2), mask);

  __m128i res   = _mm_andnot_si128(t2, t1);
  return          _mm_or_si128(res, mask);
}

// Max unsigned long
static SPS_INLINE __m128i mm_max_epu32(const __m128i &val1, const __m128i &val2) 
{
  __m128i mask    = _mm_cmpgt_epu32(val1, val2);
  __m128i ret     = _mm_and_si128(mask, val1);
  mask            = _mm_andnot_si128(mask, val2);

  return            _mm_or_si128(ret, mask);
}

// Min unsigned long
static SPS_INLINE __m128i mm_min_epu32(const __m128i &val1, const __m128i &val2)  
{
  __m128i mask   = _mm_cmpgt_epu32( val1, val2);
  __m128i ret1   = _mm_and_si128(   mask, val2);
          mask   = _mm_andnot_si128(mask, val1);

  return           _mm_or_si128( ret1, mask);
}

// Min long
static SPS_INLINE __m128i _mm_min_epi32(const __m128i &val1, const __m128i &val2) 
{
  __m128i mask    = _mm_cmplt_epi32( val1, val2);
  __m128i ret1    = _mm_and_si128(   mask, val1);
          mask    = _mm_andnot_si128(mask, val2);

  return            _mm_or_si128(    ret1, mask);
}

// Max long
static SPS_INLINE __m128i mm_max_epi32(const __m128i &val1, const __m128i &val2) 
{
  __m128i mask    = _mm_cmpgt_epi32( val1, val2);
  __m128i ret1    = _mm_and_si128(   mask, val1);
          mask    = _mm_andnot_si128(mask, val2);

  return            _mm_or_si128(    ret1, mask);
}

// Select on sign bit
static SPS_INLINE __m128i _mm_sels_epi16(const __m128i test, __m128i a, __m128i b ) 
{  
  const __m128i mask   = _mm_ext_epi16( test ); // Right shift and jam in sign bits
  const __m128i sel_a  = _mm_and_epi16( a,     mask  );
  const __m128i sel_b  = _mm_andc_epi32(b,     mask  );
  const __m128i result = _mm_or_epi16(  sel_a, sel_b );

  return (result);
}

// Multiply 2 signed or unsigned 32 bit integer vector
static SPS_INLINE __m128i _mm_mul32_epi(const __m128i a, const __m128i b)  {
  __m128i a13, b13, prod02, prod13, prod01, prod23, prod0123;
  a13 = _mm_shuffle_epi32(a, 0xF5);              // (-,a3,-,a1)
  b13 = _mm_shuffle_epi32(b, 0xF5);              // (-,b3,-,b1)
  prod02 = _mm_mul_epu32(a, b);                  // (-,a2*b2,-,a0*b0)
  prod13 = _mm_mul_epu32(a13, b13);              // (-,a3*b3,-,a1*b1)
  prod01 = _mm_unpacklo_epi32(prod02,prod13);    // (-,-,a1*b1,a0*b0)
  prod23 = _mm_unpackhi_epi32(prod02,prod13);    // (-,-,a3*b3,a2*b2)
  prod0123 = _mm_unpacklo_epi64(prod01,prod23);  // (ab3,ab2,ab1,ab0)
  return prod0123;
}

// Flip sign of a packed single
static SPS_INLINE __m128 _mm_neg_ps(__m128 x) 
{

  // Is compiled into writing to a single xmms register
  const ALIGN16_BEGIN int clear_signmask[4] ALIGN16_END =
      {(int) 0x80000000L, (int) 0x80000000L, (int) 0x80000000L, (int)0x80000000L};

  return _mm_xor_ps(x,_mm_load_ps((float *) clear_signmask));
/*   
  // Not working using GCC. Do not write to fps registers when using SIMD
  register unsigned int uiMask = 0x80000000L;
#if (defined(_MSC_VER) && defined(_WIN32))
  return _mm_xor_ps( _mm_set_ps1( *(float*)(&uiMask) ), x);
#elif defined(__linux__) || defined(__GNUC__)
  return _mm_xor_ps( _mm_set_ps1((float)uiMask), x);
#endif
*/
}

static SPS_INLINE __m128i _mm_neg_ph(__m128i x) 
{
  return _mm_xor_si128( _mm_set1_epi16(0x8000), x);
}

/*
  TODO: Take care of denormal and NaN cases
        - To convert denormal half values to normalized float values, the log2
          operation can be achieved by converting from integer to floating-point
          format, and extracting the exponent... 
*/

__m128 half_to_floats(const __m128 &input)  {

  static ALIGN16_BEGIN unsigned int half_sign[4]        ALIGN16_END = {0x00008000L, 0x00008000L, 0x00008000L, 0x00008000L};
  static ALIGN16_BEGIN unsigned int half_exponent[4]    ALIGN16_END = {0x00007C00L, 0x00007C00L, 0x00007C00L, 0x00007C00L};
  static ALIGN16_BEGIN unsigned int half_mantissa[4]    ALIGN16_END = {0x000003FFL, 0x000003FFL, 0x000003FFL, 0x000003FFL};
  static ALIGN16_BEGIN unsigned int half_bias_offset[4] ALIGN16_END = {0x0001C000L, 0x0001C000L, 0x0001C000L, 0x0001C000L};

  __m128 retval;
  
#if defined(_MSC_VER) && !defined(_WIN64)
  __asm {
    mov eax, input
    movups xmm0, [eax]
    movaps  xmm1, xmm0  
    movaps  xmm2, xmm0

    andps   xmm0, half_sign
    andps   xmm1, half_exponent
    andps   xmm2, half_mantissa
    paddd   xmm1, half_bias_offset

    pslld   xmm0, 16   // h_f_s_pos_offset
    pslld   xmm1, 13   // h_f_e_pos_offset
    pslld   xmm2, 13   // h_f_e_pos_offset

    orps    xmm1, xmm2
    orps    xmm0, xmm1  
    movups [retval], xmm0
  }
#elif defined(__GNUC__)
  __asm__ __volatile__
    ( "movups %[input],            %%xmm0    \n\t"
      "movaps %%xmm0,              %%xmm1    \n\t"
      "movaps %%xmm0,              %%xmm2    \n\t"
   
      "andps  %[half_sign],        %%xmm0    \n\t"
      "andps  %[half_exponent],    %%xmm1    \n\t"
      "andps  %[half_mantissa],    %%xmm2    \n\t"
      "paddd  %[half_bias_offset], %%xmm1    \n\t"

      "pslld  $16,                 %%xmm0    \n\t"   // h_f_s_pos_offset
      "pslld  $13,                 %%xmm1    \n\t"   // h_f_e_pos_offset
      "pslld  $13,                 %%xmm2    \n\t"   // h_f_e_pos_offset

      "orps   %%xmm2,              %%xmm1    \n\t"
      "orps   %%xmm1,              %%xmm0    \n\t"
      "movups %%xmm0,              %[output] \n\t"
      : [output] "=m" (retval)
      : [input] "m" (input),
        [half_sign] "m" (*half_sign),
        [half_exponent] "m" (*half_exponent),
        [half_mantissa] "m" (*half_mantissa),
        [half_bias_offset] "m" (*half_bias_offset)
      : "%xmm0", "%xmm1", "%xmm2"
    );
#else
#error("Not supported")
#endif

  return retval;
}

// TODO: Take care of under- / overflows

/* float_to_halfs

   Max finite half precision number is 65504.0
 
   Converting an integer into a floating-point number can be done without losing
   any precision, as long as it fits into the mantissa field, which is 23 bits
   long. And because of an implicit 1 and the sign bit you can use signed
   integers up to 25 bits long. Anything that falls out of this range will loose
   some of the lower precision bits, which may or may not be critical for your
   application. Also, this version only works with positive shift values for
   shifting left, and when the result overflows it returns 0x80000000 (0x7FFF).
    
*/

__m128 float_to_halfs(const __m128 &input) { 

  const ALIGN16_BEGIN unsigned int abs[4]      ALIGN16_END = {0x7FFFFFFFL, 0x7FFFFFFFL, 0x7FFFFFFFL, 0x7FFFFFFFL};
  const ALIGN16_BEGIN unsigned int infinity[4] ALIGN16_END = {0x47FFEFFFL, 0x47FFEFFFL, 0x47FFEFFFL, 0x47FFEFFFL};
  const ALIGN16_BEGIN unsigned int denormal[4] ALIGN16_END = {0x38800000L, 0x38800000L, 0x38800000L, 0x38800000L};
  const ALIGN16_BEGIN unsigned int mantissa[4] ALIGN16_END = {0x007FFFFFL, 0x007FFFFFL, 0x007FFFFFL, 0x007FFFFFL};
  const ALIGN16_BEGIN unsigned int oneDot[4]   ALIGN16_END = {0x00800000L, 0x00800000L, 0x00800000L, 0x00800000L};
  const ALIGN16_BEGIN unsigned int exponent[4] ALIGN16_END = {0x7F800000L, 0x7F800000L, 0x7F800000L, 0x7F800000L};
  const ALIGN16_BEGIN unsigned int fixup[4]    ALIGN16_END = {0x48000000L, 0x48000000L, 0x48000000L, 0x48000000L};
  const ALIGN16_BEGIN unsigned int round1[4]   ALIGN16_END = {0x00000001L, 0x00000001L, 0x00000001L, 0x00000001L};
  const ALIGN16_BEGIN unsigned int round2[4]   ALIGN16_END = {0x00000FFFL, 0x00000FFFL, 0x00000FFFL, 0x00000FFFL};
  const ALIGN16_BEGIN unsigned int sign[4]     ALIGN16_END = {0x80000000L, 0x80000000L, 0x80000000L, 0x80000000L};
  const ALIGN16_BEGIN unsigned int base[4]     ALIGN16_END = {0x00007FFFL, 0x00007FFFL, 0x00007FFFL, 0x00007FFFL};
  const ALIGN16_BEGIN unsigned int adjust[4]   ALIGN16_END = {0x07000000L, 0x07000000L, 0x07000000L, 0x07000000L};

  __m128 retval;

#if (defined(_MSC_VER) && defined(_WIN32))
  __asm {
    
    mov eax, input
    movups xmm0, [eax]
    
    movaps      xmm1, xmm0

    // Compute masks
    andps       xmm0, abs
    movaps      xmm2, xmm0
    movaps      xmm3, xmm0
    cmpnltps    xmm2, infinity
    cmpltps     xmm3, denormal

    // Denormal case
    movaps      xmm4, xmm0
    andps       xmm4, exponent
    paddd       xmm4, adjust
    movaps      xmm6, xmm0
    andps       xmm6, mantissa
    orps        xmm6, oneDot
    cvtdq2ps    xmm6, xmm6
    mulps       xmm6, xmm4
    cvttps2dq   xmm6, xmm6

    // Normal case and combine
    paddd       xmm0, fixup
    andps       xmm6, xmm3
    andnps      xmm3, xmm0
    orps        xmm6, xmm3

    // Correct rounding
    movaps      xmm0, xmm6
    psrld       xmm0, 13
    andps       xmm0, round1
    paddd       xmm0, round2
    paddd       xmm0, xmm6

    // Combine with sign and infinity
    psrld       xmm0, 13
    andps       xmm1, sign
    psrld       xmm1, 16
    orps        xmm0, xmm2
    andps       xmm0, base
    orps        xmm0, xmm1   // Result in lower words of each element
    movups [retval], xmm0
  }
#elif defined(__GNUC__)
  __asm__ __volatile__
    ( "movups      %[input],    %%xmm0    \n\t"
      "movaps      %%xmm0,      %%xmm1    \n\t"
                                         
      // Compute masks                   
      "andps       %[abs],      %%xmm0    \n\t"
      "movaps      %%xmm0,      %%xmm2    \n\t"
      "movaps      %%xmm0,      %%xmm3    \n\t"
      "cmpnltps    %[infinity], %%xmm2    \n\t"
      "cmpltps     %[denormal], %%xmm3    \n\t"
                                         
      // Denormal case
      "movaps      %%xmm0,      %%xmm4    \n\t"
      "andps       %[exponent], %%xmm4    \n\t"
      "paddd       %[adjust],   %%xmm4    \n\t"
      "movaps      %%xmm0,      %%xmm6    \n\t"
      "andps       %[mantissa], %%xmm6    \n\t"
      "orps        %[oneDot],   %%xmm6    \n\t"
      "cvtdq2ps    %%xmm6,      %%xmm6    \n\t"
      "mulps       %%xmm4,      %%xmm6    \n\t"
      "cvttps2dq   %%xmm6,      %%xmm6    \n\t"
                                         
      // Normal case and combine         
      "paddd       %[fixup],    %%xmm0    \n\t"
      "andps       %%xmm3,      %%xmm6    \n\t"
      "andnps      %%xmm0,      %%xmm3    \n\t"
      "orps        %%xmm3,      %%xmm6    \n\t"
                                         
      // Correct rounding                
      "movaps      %%xmm6,      %%xmm0    \n\t"
      "psrld       $13,         %%xmm0    \n\t"
      "andps       %[round1],   %%xmm0    \n\t"
      "paddd       %[round2],   %%xmm0    \n\t"
      "paddd       %%xmm6,      %%xmm0    \n\t"
                                         
      // Combine with sign and infinity
      "psrld       $13,         %%xmm0    \n\t"
      "andps       %[sign],     %%xmm1    \n\t"
      "psrld       $16,         %%xmm1    \n\t"
      "orps        %%xmm2,      %%xmm0    \n\t"
      "andps       %[base],     %%xmm0    \n\t"
      "orps        %%xmm1,      %%xmm0    \n\t"
      "movups      %%xmm0,      %[output] \n\t"  // Result in lower words of each element
      : [output]   "=m" (retval)
      : [input]    "m" (input),
        [abs]      "m" (*abs),
        [infinity] "m" (*infinity),
        [denormal] "m" (*denormal),
        [mantissa] "m" (*mantissa),
        [oneDot]   "m" (*oneDot),
        [exponent] "m" (*exponent),
        [fixup]    "m" (*fixup),
        [round1]   "m" (*round1),
        [round2]   "m" (*round2),
        [sign]     "m" (*sign),
        [base]     "m" (*base),
        [adjust]   "m" (*adjust)
      : "%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm6"
    );

#endif
  
  return retval;
}


// TODO: Verify against D3DXFloat32To16Array of Direct3D

/*
  Using D3DXFloat32To16Array/D3DXFloat16To32Array behavior, which does
  not have a representation for NaN nor infinity. It can represent up to
  131008.0, but clamps anything higher to the representation of this number
  (0x7FFF). Hence you can store a 16-bit integer texture in a 16-bit
  floating-point texture.*/

__m128 float_to_halfs_d3dx(const __m128 &input) { 

  const ALIGN16_BEGIN unsigned int abs[4]      ALIGN16_END = {0x7FFFFFFFL, 0x7FFFFFFFL, 0x7FFFFFFFL, 0x7FFFFFFFL};
  const ALIGN16_BEGIN unsigned int infinity[4] ALIGN16_END = {0x47FFEFFFL, 0x47FFEFFFL, 0x47FFEFFFL, 0x47FFEFFFL};
  const ALIGN16_BEGIN unsigned int denormal[4] ALIGN16_END = {0x38800000L, 0x38800000L, 0x38800000L, 0x38800000L};
  const ALIGN16_BEGIN unsigned int fixup[4]    ALIGN16_END = {0x48000000L, 0x48000000L, 0x48000000L, 0x48000000L};
  const ALIGN16_BEGIN unsigned int round1[4]   ALIGN16_END = {0x00000001L, 0x00000001L, 0x00000001L, 0x00000001L};
  const ALIGN16_BEGIN unsigned int round2[4]   ALIGN16_END = {0x00000FFFL, 0x00000FFFL, 0x00000FFFL, 0x00000FFFL};
  const ALIGN16_BEGIN unsigned int sign[4]     ALIGN16_END = {0x80000000L, 0x80000000L, 0x80000000L, 0x80000000L};
  const ALIGN16_BEGIN unsigned int base[4]     ALIGN16_END = {0x00007FFFL, 0x00007FFFL, 0x00007FFFL, 0x00007FFFL};
  const ALIGN16_BEGIN unsigned int integer[4]  ALIGN16_END = {0x52000000L, 0x52000000L, 0x52000000L, 0x52000000L};

  __m128 retval;

#if (defined(_MSC_VER) && defined(_WIN32))
  __asm {
    mov eax, input
    movups xmm0, [eax]
    movaps      xmm1, xmm0

    // Compute masks
    andps       xmm0, abs
    movaps      xmm2, xmm0
    movaps      xmm3, xmm0
    cmpnltps    xmm2, infinity
    cmpltps     xmm3, denormal

    // Denormal case
    movaps      xmm6, xmm0
    mulps       xmm6, integer
    cvttps2dq   xmm6, xmm6

    // Normal case and combine
    paddd       xmm0, fixup
    andps       xmm6, xmm3
    andnps      xmm3, xmm0
    orps        xmm6, xmm3

    // Correct rounding
    movaps      xmm0, xmm6
    psrld       xmm0, 13
    andps       xmm0, round1
    paddd       xmm0, round2
    paddd       xmm0, xmm6

    // Combine with sign and infinity
    psrld       xmm0, 13
    andps       xmm1, sign
    psrld       xmm1, 16
    orps        xmm0, xmm2
    andps       xmm0, base
    orps        xmm0, xmm1   // Result in lower words of each element
    movups [retval], xmm0
  }
#elif defined(__GNUC__)
  __asm__ __volatile__
    ( "movups      %[input],    %%xmm0    \n\t"
      "movaps      %%xmm0,      %%xmm1    \n\t"
                                         
      // Compute masks                   
      "andps       %[abs],      %%xmm0    \n\t"
      "movaps      %%xmm0,      %%xmm2    \n\t"
      "movaps      %%xmm0,      %%xmm3    \n\t"
      "cmpnltps    %[infinity], %%xmm2    \n\t"
      "cmpltps     %[denormal], %%xmm3    \n\t"
                                         
      // Denormal case
      "movaps      %%xmm0,      %%xmm6    \n\t"
      "mulps       %[adjust],   %%xmm6    \n\t"
      "cvttps2dq   %%xmm6,      %%xmm6    \n\t"
      
      // Normal case and combine         
      "paddd       %[fixup],    %%xmm0    \n\t"
      "andps       %%xmm3,      %%xmm6    \n\t"
      "andnps      %%xmm0,      %%xmm3    \n\t"
      "orps        %%xmm3,      %%xmm6    \n\t"
                                         
      // Correct rounding                
      "movaps      %%xmm6,      %%xmm0    \n\t"
      "psrld       $13,         %%xmm0    \n\t"
      "andps       %[round1],   %%xmm0    \n\t"
      "paddd       %[round2],   %%xmm0    \n\t"
      "paddd       %%xmm6,      %%xmm0    \n\t"
                                         
      // Combine with sign and infinity
      "psrld       $13,         %%xmm0    \n\t"
      "andps       %[sign],     %%xmm1    \n\t"
      "psrld       $16,         %%xmm1    \n\t"
      "orps        %%xmm2,      %%xmm0    \n\t"
      "andps       %[base],     %%xmm0    \n\t"
      "orps        %%xmm1,      %%xmm0    \n\t"
      "movups      %%xmm0,      %[output] \n\t"  // Result in lower words of each element
      : [output]   "=m" (retval)
      : [input]    "m" (input),
        [abs]      "m" (*abs),
        [infinity] "m" (*infinity),
        [denormal] "m" (*denormal),
        [fixup]    "m" (*fixup),
        [round1]   "m" (*round1),
        [round2]   "m" (*round2),
        [sign]     "m" (*sign),
        [base]     "m" (*base),
        [adjust]   "m" (*integer)
      : "%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm6"
    );
#endif
  
  return retval;
}

static SPS_INLINE __m128i 
_mm_float_to_half(__m128 f) 
{

GCC_SPLIT_BLOCK("LOADS")
  
  const int     one                        =              0x00000001L;
  const __m128i f_s_mask                   = _mm_li_epi32(0x80000000L);
  const __m128i f_e_mask                   = _mm_li_epi32(0x7F800000L);
  const __m128i f_m_mask                   = _mm_li_epi32(0x007FFFFFL);
  const __m128i f_m_hidden_bit             = _mm_li_epi32(0x00800000L);
  const __m128i f_m_round_bit              = _mm_li_epi32(0x00001000L);
  const __m128i f_snan_mask                = _mm_li_epi32(0x7FC00000L); // 0 1.......1  0..........0
                                                                        //31        22 21          0
  const int     f_e_pos                    =              0x00000017L;
  const int     h_e_pos                    =              0x0000000AL;
  const __m128i h_e_mask                   = _mm_li_epi32(0x00007C00L); // 0 1...10........0
                                                                        //15     9        
  const __m128i h_snan_mask                = _mm_li_epi32(0x00007E00L); // 0 1....10.......0
                                                                        //15      8   
  const __m128i h_e_mask_value             = _mm_li_epi32(0x0000001FL);
  const int     f_h_s_pos_offset           =              0x00000010L;
  const __m128i f_h_bias_offset            = _mm_li_epi32(0x00000070L);
  const int     f_h_m_pos_offset           =              0x0000000DL;
  const __m128i h_nan_min                  = _mm_li_epi32(0x00007C01L);
  const __m128i f_h_e_biased_flag          = _mm_li_epi32(0x0000008FL);

  GCC_SPLIT_BLOCK("CALCULATIONS")
  
  // Extract sign (s)
  const __m128i f_s                        = _mm_and_epi16( _mm_castps_si128(f), f_s_mask); 
  
  const __m128i h_s                        = _mm_srli_epi32( f_s, f_h_s_pos_offset );
  
  // Extract exponent (e) and subtract bias of 112: s * 2 ^ (e-112) * m 
  const __m128i f_e                        = _mm_and_epi16( _mm_castps_si128(f), f_e_mask );
  const __m128i f_e_amount                 = _mm_srli_epi32( f_e,             f_e_pos          );
  const __m128i f_e_half_bias              = _mm_sub_epi32( f_e_amount,      f_h_bias_offset  );

  // Extract mantissa (m) and round-up if f_m_round_bit is set
  const __m128i f_m                      = _mm_and_epi16( _mm_castps_si128(f),               f_m_mask         );
  
  const __m128i m_nan                      = _mm_srli_epi32( f_m,               f_h_m_pos_offset ); // Rigth shift mantissa 13 bit: single(23) -> half(10)
  const __m128i f_m_round_mask             = _mm_and_epi16(  f_m,               f_m_round_bit    );
  const __m128i f_m_round_offset           = _mm_slli_epi32( f_m_round_mask,    one              );
  const __m128i f_m_rounded                = _mm_add_epi32(  f_m,               f_m_round_offset );

  // Is float NaN?
  const __m128i f_snan                     = _mm_and_epi16( _mm_castps_si128(f),      f_snan_mask      );
  const __m128i f_m_denorm_sa              = _mm_sub_epi32(_mm_set1_epi32(one), f_e_half_bias    ); // (1-exp)
  const __m128i f_m_with_hidden            = _mm_or_epi16(   f_m_rounded,       f_m_hidden_bit   ); // Is rounded mantissa (including hidden bit) too big? If so, divide by 2^(1-exp)
  const __m128i f_m_denorm                 = _mm_half_srl(   f_m_with_hidden,   f_m_denorm_sa    );
  const __m128i h_m_denorm                 = _mm_srli_epi32( f_m_denorm,        f_h_m_pos_offset ); // Rigth shift mantissa 13 bit: single(23) -> half(10)   
  const __m128i f_m_rounded_overflow       = _mm_and_epi16(  f_m_rounded,       f_m_hidden_bit   ); // Is rounded mantissa causing float overflow?
  const __m128i h_em_nan                   = _mm_or_epi16(   h_e_mask,          m_nan            ); // Is it a NaN value?
 
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Normal results:
  ////////////////////////////////////////////////////////////////////////////////////////////
  
  const __m128i h_e_norm_overflow_offset   = _mm_inc_epi32(  f_e_half_bias );   // Increase exponent by one (in case) of overflow 
  const __m128i h_e_norm_overflow          = _mm_slli_epi32( h_e_norm_overflow_offset, h_e_pos          );
  const __m128i h_e_norm                   = _mm_slli_epi32( f_e_half_bias,            h_e_pos          );  // Left shift exponent 10 bits: S(1) E(5) M(10)
  const __m128i h_m_norm                   = _mm_srli_epi32( f_m_rounded,              f_h_m_pos_offset );  // Rigth shift mantissa 13 bits: single(23) -> half(10)
  const __m128i h_em_norm                  = _mm_or_epi16(   h_e_norm,                 h_m_norm         );  // Combine exponent and mantissa to obtain normal result
  
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Handle denormals
  ////////////////////////////////////////////////////////////////////////////////////////////

  const __m128i is_h_ndenorm_msb           = _mm_sub_epi32( f_h_bias_offset,    f_e_amount    );      // Exponent < 112: -> Denorm result
  const __m128i is_h_denorm_msb            = _mm_not_epi16( is_h_ndenorm_msb );                       // sel
  const __m128i is_f_e_flagged_msb         = _mm_sub_epi32( f_h_e_biased_flag,  f_e_half_bias );      // Exponent msb: 10001111 - f_e_half_bias, sel
  const __m128i is_f_m_eqz_msb             = _mm_dec_epi32( f_m   );                                  // Zero mantissa 
  const __m128i is_h_nan_eqz_msb           = _mm_dec_epi32( m_nan );
  const __m128i is_f_inf_msb               = _mm_and_epi16( is_f_e_flagged_msb, is_f_m_eqz_msb   );   // Zero float mantissa + exponent msb : Float inf  
  const __m128i is_f_nan_underflow_msb     = _mm_and_epi16( is_f_e_flagged_msb, is_h_nan_eqz_msb );   // Zero half mantissa + exponent msb : Underflow -> NaN min (negative infinity), sel
  const __m128i is_e_overflow_msb          = _mm_sub_epi32( h_e_mask_value,     f_e_half_bias    );   // Exponent larger than 31 (5 bits) : Exponent overflow 
  const __m128i is_h_inf_msb               = _mm_or_epi16(  is_e_overflow_msb,  is_f_inf_msb     );   // Float inf or exponent overflow -> NaN Infinity (positive infinity), sel
  const __m128i is_f_nsnan_msb             = _mm_sub_epi32( f_snan,             f_snan_mask      );   // Float NaN -> NaN
  const __m128i is_f_snan_msb              = _mm_not_epi16( is_f_nsnan_msb );                         // sel  
  const __m128i is_m_norm_overflow_msb     = _mm_neg_epu32( f_m_rounded_overflow );                   // Round overflow -> Overflow, sel

  GCC_SPLIT_BLOCK("SELECT RESULT")
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Select appropriate output
  ////////////////////////////////////////////////////////////////////////////////////////////
    
  const __m128i h_em_overflow_result       = _mm_sels_epi32( is_m_norm_overflow_msb, h_e_norm_overflow, h_em_norm);                    // Overflow -> normal (no exception is thrown)  
  const __m128i h_em_nan_result            = _mm_sels_epi32( is_f_e_flagged_msb,     h_em_nan,          h_em_overflow_result      );   // Max exponent non-zero mantissa -> NaN result
  const __m128i h_em_nan_underflow_result  = _mm_sels_epi32( is_f_nan_underflow_msb, h_nan_min,         h_em_nan_result           );   // Negative infinity -> 0x7C01  
  const __m128i h_em_inf_result            = _mm_sels_epi32( is_h_inf_msb,           h_e_mask,          h_em_nan_underflow_result );   // Positive infinity -> 0x7C00
  const __m128i h_em_denorm_result         = _mm_sels_epi32( is_h_denorm_msb,        h_m_denorm,        h_em_inf_result           );   // Denormals
  const __m128i h_em_snan_result           = _mm_sels_epi32( is_f_snan_msb,          h_snan_mask,       h_em_denorm_result        );   // NaN -> 0x7E00
  const __m128i h_result                   = _mm_or_epi16(   h_s,                    h_em_snan_result );                               // Put sign back

  return h_result;
}

// TODO: SSE4a use LZCNT

// __lzcnt64 __lzcnt __lzcnt16

// Count Leading Zeros
static SPS_INLINE __m128i _mm_cntlz_epi32(const __m128i x) 
{
  const __m128i x0  = _mm_srli_epi32(  x,   1 );
  const __m128i x1  = _mm_or_si128(    x,   x0 );
  const __m128i x2  = _mm_srli_epi32(  x1,  2 );
  const __m128i x3  = _mm_or_si128(    x1,  x2 );
  const __m128i x4  = _mm_srli_epi32(  x3,  4 );
  const __m128i x5  = _mm_or_si128(    x3,  x4 );
  const __m128i x6  = _mm_srli_epi32(  x5,  8 );
  const __m128i x7  = _mm_or_si128(    x5,  x6 );
  const __m128i x8  = _mm_srli_epi32(  x7,  16 );
  const __m128i x9  = _mm_or_si128(    x7,  x8 );
  const __m128i xA  = _mm_not_si128(   x9 );
  const __m128i xB  = _mm_srli_epi32(  xA,  1 );
  const __m128i xC  = _mm_andi_epi32(  xB,  0x55555555L );
  const __m128i xD  = _mm_sub_epi32(   xA,  xC );
  const __m128i xE  = _mm_andi_epi32(  xD,  0x33333333L );
  const __m128i xF  = _mm_srli_epi32(  xD,  2 );
  const __m128i x10 = _mm_andi_epi32(  xF,  0x33333333L );
  const __m128i x11 = _mm_add_epi32(   xE,  x10 );
  const __m128i x12 = _mm_srli_epi32(  x11, 4 );
  const __m128i x13 = _mm_add_epi32(   x11, x12 );
  const __m128i x14 = _mm_andi_epi32(  x13, 0x0F0F0F0FL );
  const __m128i x15 = _mm_srli_epi32(  x14, 8 );
  const __m128i x16 = _mm_add_epi32(   x14, x15 );
  const __m128i x17 = _mm_srli_epi32(  x16, 16 );
  const __m128i x18 = _mm_add_epi32(   x16, x17 );
  const __m128i x19 = _mm_andi_epi32(  x18, 0x0000003FL );
  return ( x19 );
}

// Count Leading Zeros (addition)
static SPS_INLINE __m128i _mm_cntlz_epi16(const __m128i x ) 
{
  const __m128i x0  = _mm_half_srli(    x,  1 );
  const __m128i x1  = _mm_or_epi16(     x,  x0 );
  const __m128i x2  = _mm_half_srli(    x1, 2 );
  const __m128i x3  = _mm_or_epi16(     x1, x2 );
  const __m128i x4  = _mm_half_srli(    x3, 4 );
  const __m128i x5  = _mm_or_epi16(     x3, x4 );
  const __m128i x6  = _mm_half_srli(    x5, 8 );
  const __m128i x7  = _mm_or_epi16(     x5, x6 );
  const __m128i x8  = _mm_not_epi16(    x7 );
  const __m128i x9  = _mm_srlmii_epi16( x8, 1,   0x5555 );
  const __m128i xA  = _mm_sub_epi16(    x8, x9 );
  const __m128i xB  = _mm_and1_epi16(   xA, 0x3333 );
  const __m128i xC  = _mm_srlmii_epi16( xA, 2,   0x3333);
  const __m128i xD  = _mm_add_epi16(    xB, xC );
  const __m128i xE  = _mm_half_srli(    xD, 4 );
  const __m128i xF  = _mm_addm_epi16(   xD, xE,  0x0F0F);
  const __m128i x10 = _mm_half_srli(    xF, 8 );
  const __m128i x11 = _mm_addm_epi16(   xF, x10, 0x001F );
  return ( x11 );
}

// Count Leading Zeros (multiplication)
static SPS_INLINE __m128i _mm_cntlz_epi16_32(const __m128i x ) 
{
  const __m128i x0  = _mm_srli_epi32( x,  1 );
  const __m128i x1  = _mm_or_epi16(   x,  x0 );
  const __m128i x2  = _mm_srli_epi32( x1, 2 );
  const __m128i x3  = _mm_or_epi16(   x1, x2 );
  const __m128i x4  = _mm_srli_epi32( x3, 4 );
  const __m128i x5  = _mm_or_epi16(   x3, x4 );
  const __m128i x6  = _mm_srli_epi32( x5, 8 );
  const __m128i x7  = _mm_or_epi16(   x5, x6 );
  const __m128i x8  = _mm_not_epi16(  x7 );
  const __m128i x9  = _mm_srlm_epi32( x8, 1,  0x5555 );
  const __m128i xA  = _mm_sub_epi32(  x8, x9 );
  const __m128i xB  = _mm_and1_epi16( xA, 0x3333 );
  const __m128i xC  = _mm_srlm_epi32( xA, 2,  0x3333);
  const __m128i xD  = _mm_add_epi32(  xB, xC );
  const __m128i xE  = _mm_srli_epi32( xD, 4 );
  const __m128i xF  = _mm_addm_epi16( xD, xE, 0x0F0F);
  const __m128i x10 = _mm_srli_epi32( xF, 8 );
  const __m128i x11 = _mm_addm_epi32( xF, x10,0x001F );
  return ( x11 );
}

__m128i 
_mm_half_to_float( const __m128i h )
{
  
  GCC_SPLIT_BLOCK("LOADS")
  const __m128i h_e_mask              = _mm_li_epi32( 0x00007C00 );
  const __m128i h_m_mask              = _mm_li_epi32( 0x000003FF );
  const __m128i h_s_mask              = _mm_li_epi32( 0x00008000 );
  const __m128i h_f_s_pos_offset      = _mm_li_epi32( 0x00000010 );
  const __m128i h_f_e_pos_offset      = _mm_li_epi32( 0x0000000D );
  const __m128i h_f_bias_offset       = _mm_li_epi32( 0x0001C000 );
  const __m128i f_e_mask              = _mm_li_epi32( 0x7F800000 );
  const __m128i f_m_mask              = _mm_li_epi32( 0x007FFFFF );
  const __m128i h_f_e_denorm_bias     = _mm_li_epi32( 0x0000007E );
  const __m128i h_f_m_denorm_sa_bias  = _mm_li_epi32( 0x00000008 );
  const __m128i f_e_pos               = _mm_li_epi32( 0x00000017 );
  const __m128i h_e_mask_minus_one    = _mm_li_epi32( 0x00007BFF );
  GCC_SPLIT_BLOCK("CALCULATIONS")
  const __m128i h_e                   = _mm_and_si128(  h,                   h_e_mask );
  const __m128i h_m                   = _mm_and_si128(  h,                   h_m_mask );
  const __m128i h_s                   = _mm_and_si128(  h,                   h_s_mask );
  const __m128i h_e_f_bias            = _mm_add_epi32(  h_e,                 h_f_bias_offset );
  const __m128i h_m_nlz               = _mm_cntlz_epi32(h_m );
  const __m128i f_s                   = _mm_half_sll(   h_s,                 h_f_s_pos_offset );
  const __m128i f_e                   = _mm_half_sll(   h_e_f_bias,          h_f_e_pos_offset );
  const __m128i f_m                   = _mm_half_sll(   h_m,                 h_f_e_pos_offset );
  const __m128i f_em                  = _mm_or_epi16(   f_e,                 f_m              );
  const __m128i h_f_m_sa              = _mm_sub_epi32(  h_m_nlz,             h_f_m_denorm_sa_bias );
  const __m128i f_e_denorm_unpacked   = _mm_sub_epi32(  h_f_e_denorm_bias,   h_f_m_sa             );
  const __m128i h_f_m                 = _mm_half_sll(   h_m,                 h_f_m_sa             );
  const __m128i f_m_denorm            = _mm_and_si128(  h_f_m,               f_m_mask             );
  const __m128i f_e_denorm            = _mm_half_sll(   f_e_denorm_unpacked, f_e_pos              );
  const __m128i f_em_denorm           = _mm_or_epi16(   f_e_denorm,          f_m_denorm           );
  const __m128i f_em_nan              = _mm_or_epi16(   f_e_mask,            f_m                  );
  const __m128i is_e_eqz_msb          = _mm_dec_epi32(  h_e );
  const __m128i is_m_nez_msb          = _mm_neg_epu32(  h_m );
  const __m128i is_e_flagged_msb      = _mm_sub_epi32(  h_e_mask_minus_one,  h_e );
  const __m128i is_zero_msb           = _mm_andc_epi32( is_e_eqz_msb,        is_m_nez_msb );
  const __m128i is_inf_msb            = _mm_andc_epi32( is_e_flagged_msb,    is_m_nez_msb );
  const __m128i is_denorm_msb         = _mm_and_si128(  is_m_nez_msb,        is_e_eqz_msb );
  const __m128i is_nan_msb            = _mm_and_si128(  is_e_flagged_msb,    is_m_nez_msb ); 
  const __m128i is_zero               = _mm_ext_epi32(  is_zero_msb );
  const __m128i f_zero_result         = _mm_andc_epi32( f_em, is_zero );
  GCC_SPLIT_BLOCK("SELECT RESULT")
  const __m128i f_denorm_result       = _mm_sels_epi32( is_denorm_msb, f_em_denorm, f_zero_result );
  const __m128i f_inf_result          = _mm_sels_epi32( is_inf_msb,    f_e_mask,    f_denorm_result );
  const __m128i f_nan_result          = _mm_sels_epi32( is_nan_msb,    f_em_nan,    f_inf_result    );
  const __m128i f_result              = _mm_or_epi16(   f_s, f_nan_result );
 
  return (f_result);
  
}

// half_add
// --------
//
//  (SUM)        __m128i z = half_add( x, y );
//  (DIFFERENCE) __m128i z = half_add( x, -y );
//
//  * Difference of ZEROs is always +ZERO
//  * Sum round with guard + round + sticky bit (grs)
//  * QNaN + [x]  = QNaN
//  * [x]  + +INF = +INF
//  * [x]  - -INF = -INF
//  * INF  - INF  = SNaN
//
//  Will have exactly (0 ulps difference) the same result as:
//  (Round up)
//
//     union FLOAT_32
//     {
//       float    f32;
//       uint32_t u32;
//     };
//
//     union FLOAT_32 fx = { .u32 = half_to_float( x ) };
//     union FLOAT_32 fy = { .u32 = half_to_float( y ) };
//     union FLOAT_32 fz = { .f32 = fx.f32 + fy.f32    };
//     __m128i       z  = float_to_half( fz );
//
__m128i _mm_add_ph(const __m128i x, __m128i y )
{

  GCC_SPLIT_BLOCK("LOADS")

  // Constants
  const __m128i one                       = _mm_li_epi16(       0x0001);
  const int     msb_to_lsb_sa             =                     0x000F;
  const __m128i h_s_mask                  = _mm_li_epi16((short)0x8000);
  const __m128i h_e_mask                  = _mm_li_epi16(       0x7C00);
  const __m128i h_m_mask                  = _mm_li_epi16(       0x03FF);
  const __m128i h_m_msb_mask              = _mm_li_epi16(       0x2000);
  const int     h_m_msb_sa                =                     0x000D;
  const __m128i h_m_hidden                = _mm_li_epi16(       0x0400);
  const int     h_e_pos                   =                     0x000A;
  const __m128i h_e_bias_minus_one        = _mm_li_epi16(       0x000E);
  const __m128i h_m_grs_carry             = _mm_li_epi16(       0x4000);
  const int     h_m_grs_carry_pos         =                     0x000E;
  const int     h_grs_size                =                     0x0003;
  const __m128i h_snan                    = _mm_li_epi16((short)0xFE00);
  const __m128i h_e_mask_minus_one        = _mm_li_epi16(       0x7BFF);

  const __m128i h_grs_round_carry         = _mm_half_slli( one,               h_grs_size );
  const __m128i h_grs_round_mask          = _mm_sub_epi16( h_grs_round_carry, one);
  
  GCC_SPLIT_BLOCK("CALCULATIONS")

  const __m128i x_e                       = _mm_and_epi16( x, h_e_mask );
  const __m128i y_e                       = _mm_and_epi16( y, h_e_mask );
  const __m128i is_y_e_larger_msb         = _mm_sub_epi16( x_e, y_e );
  const __m128i a                         = _mm_sels_epi16( is_y_e_larger_msb, y, x);
  const __m128i a_s                       = _mm_and_epi16( a,   h_s_mask );
  const __m128i a_e                       = _mm_and_epi16( a,   h_e_mask );
  const __m128i a_m_no_hidden_bit         = _mm_and_epi16( a,   h_m_mask );
  const __m128i a_em_no_hidden_bit        = _mm_or_epi16(  a_e, a_m_no_hidden_bit );
  const __m128i b                         = _mm_sels_epi16( is_y_e_larger_msb, x, y);
  const __m128i b_s                       = _mm_and_epi16( b,   h_s_mask );
  const __m128i b_e                       = _mm_and_epi16( b,   h_e_mask );
  const __m128i b_m_no_hidden_bit         = _mm_and_epi16( b,   h_m_mask );
  const __m128i b_em_no_hidden_bit        = _mm_or_epi16(  b_e, b_m_no_hidden_bit );
  const __m128i is_diff_sign_msb          = _mm_xor_epi16( a_s, b_s );
  const __m128i is_a_inf_msb              = _mm_sub_epi16( h_e_mask_minus_one, a_em_no_hidden_bit );
  const __m128i is_b_inf_msb              = _mm_sub_epi16( h_e_mask_minus_one, b_em_no_hidden_bit );
  const __m128i is_undenorm_msb           = _mm_dec_epi16( a_e );
  const __m128i is_undenorm               = _mm_ext_epi16( is_undenorm_msb );
  const __m128i is_both_inf_msb           = _mm_and_epi16( is_a_inf_msb, is_b_inf_msb );
  const __m128i is_invalid_inf_op_msb     = _mm_and_epi16( is_both_inf_msb, b_s );
  const __m128i is_a_e_nez_msb            = _mm_neg_epu16( a_e );
  const __m128i is_b_e_nez_msb            = _mm_neg_epu16( b_e );
  const __m128i is_a_e_nez                = _mm_ext_epi16( is_a_e_nez_msb );
  const __m128i is_b_e_nez                = _mm_ext_epi16( is_b_e_nez_msb );
  const __m128i a_m_hidden_bit            = _mm_and_epi16( is_a_e_nez, h_m_hidden );
  const __m128i b_m_hidden_bit            = _mm_and_epi16( is_b_e_nez, h_m_hidden );
  const __m128i a_m_no_grs                = _mm_or_epi16( a_m_no_hidden_bit, a_m_hidden_bit );
  const __m128i b_m_no_grs                = _mm_or_epi16( b_m_no_hidden_bit, b_m_hidden_bit );
  const __m128i diff_e                    = _mm_sub_epi16( a_e,        b_e );
  const __m128i a_e_unbias                = _mm_sub_epi16( a_e,        h_e_bias_minus_one );
  const __m128i a_m                       = _mm_half_slli( a_m_no_grs, h_grs_size );
  const __m128i a_e_biased                = _mm_half_srli( a_e,        h_e_pos );
  const __m128i m_sa_unbias               = _mm_half_srli( a_e_unbias, h_e_pos );
  const __m128i m_sa_default              = _mm_half_srli( diff_e,     h_e_pos );
  const __m128i m_sa_unbias_mask          = _mm_andc_epi16(is_a_e_nez_msb,   is_b_e_nez_msb );
  const __m128i m_sa                      = _mm_sels_epi16(m_sa_unbias_mask,  m_sa_unbias, m_sa_default );
  const __m128i b_m_no_sticky             = _mm_half_slli( b_m_no_grs,        h_grs_size );

  const __m128i sh_m                      = _mm_half_srl(  b_m_no_sticky,     m_sa);

  const __m128i sticky_overflow           = _mm_half_sll(  one,               m_sa);

  const __m128i sticky_mask               = _mm_dec_epi16( sticky_overflow); 
  const __m128i sticky_collect            = _mm_and_epi16( b_m_no_sticky,     sticky_mask );
  const __m128i is_sticky_set_msb         = _mm_neg_epu16( sticky_collect );
  
  const __m128i sticky                    = _mm_half_srli( is_sticky_set_msb, msb_to_lsb_sa);

  const __m128i b_m                       = _mm_or_epi16( sh_m, sticky);
  const __m128i is_c_m_ab_pos_msb         = _mm_sub_epi16( b_m, a_m );
  const __m128i c_inf                     = _mm_or_epi16(  a_s, h_e_mask );
  const __m128i c_m_sum                   = _mm_add_epi16( a_m, b_m );
  const __m128i c_m_diff_ab               = _mm_sub_epi16( a_m, b_m );
  const __m128i c_m_diff_ba               = _mm_sub_epi16( b_m, a_m );
  const __m128i c_m_smag_diff             = _mm_sels_epi16( is_c_m_ab_pos_msb, c_m_diff_ab, c_m_diff_ba);
  const __m128i c_s_diff                  = _mm_sels_epi16( is_c_m_ab_pos_msb, a_s,         b_s         );
  const __m128i c_s                       = _mm_sels_epi16( is_diff_sign_msb,  c_s_diff,    a_s         );
  const __m128i c_m_smag_diff_nlz         = _mm_cntlz_epi16( c_m_smag_diff );
  const __m128i diff_norm_sa              = _mm_sub_epi16( c_m_smag_diff_nlz, one);
  const __m128i is_diff_denorm_msb        = _mm_sub_epi16( a_e_biased, diff_norm_sa);
  const __m128i is_diff_denorm            = _mm_ext_epi16( is_diff_denorm_msb );
  const __m128i is_a_or_b_norm_msb        = _mm_neg_epu16( a_e_biased );
  const __m128i diff_denorm_sa            = _mm_dec_epi16( a_e_biased );
  const __m128i c_m_diff_denorm           = _mm_half_sll( c_m_smag_diff, diff_denorm_sa);
  const __m128i c_m_diff_norm             = _mm_half_sll( c_m_smag_diff, diff_norm_sa);
  const __m128i c_e_diff_norm             = _mm_sub_epi16( a_e_biased,  diff_norm_sa);
  const __m128i c_m_diff_ab_norm          = _mm_sels_epi16( is_diff_denorm_msb, c_m_diff_denorm, c_m_diff_norm);
  const __m128i c_e_diff_ab_norm          = _mm_andc_epi16( c_e_diff_norm, is_diff_denorm );
  const __m128i c_m_diff                  = _mm_sels_epi16( is_a_or_b_norm_msb, c_m_diff_ab_norm, c_m_smag_diff );
  const __m128i c_e_diff                  = _mm_sels_epi16( is_a_or_b_norm_msb, c_e_diff_ab_norm, a_e_biased    );
  const __m128i is_diff_eqz_msb           = _mm_dec_epi16( c_m_diff );
  const __m128i is_diff_exactly_zero_msb  = _mm_and_epi16( is_diff_sign_msb, is_diff_eqz_msb );
  const __m128i is_diff_exactly_zero      = _mm_ext_epi16( is_diff_exactly_zero_msb );
  const __m128i c_m_added                 = _mm_sels_epi16( is_diff_sign_msb, c_m_diff, c_m_sum );
  const __m128i c_e_added                 = _mm_sels_epi16( is_diff_sign_msb, c_e_diff, a_e_biased );
  const __m128i c_m_carry                 = _mm_and_epi16( c_m_added, h_m_grs_carry );
  const __m128i is_c_m_carry_msb          = _mm_neg_epu16( c_m_carry );  
  const __m128i c_e_hidden_offset         = _mm_andsrli_epi16( c_m_added,   h_m_grs_carry, h_m_grs_carry_pos );
  const __m128i c_m_sub_hidden            = _mm_half_srl(      c_m_added,        one);
  const __m128i c_m_no_hidden             = _mm_sels_epi16(    is_c_m_carry_msb, c_m_sub_hidden, c_m_added );
  const __m128i c_e_no_hidden             = _mm_add_epi16(     c_e_added,         c_e_hidden_offset  );
  const __m128i c_m_no_hidden_msb         = _mm_and_epi16(     c_m_no_hidden,     h_m_msb_mask       );
  const __m128i undenorm_m_msb_odd        = _mm_half_srli(     c_m_no_hidden_msb, h_m_msb_sa         );
  const __m128i undenorm_fix_e            = _mm_and_epi16(     is_undenorm,       undenorm_m_msb_odd );
  const __m128i c_e_fixed                 = _mm_add_epi16(     c_e_no_hidden,     undenorm_fix_e     );
  const __m128i c_m_round_amount          = _mm_and_epi16(     c_m_no_hidden,     h_grs_round_mask   );
  const __m128i c_m_rounded               = _mm_add_epi16(     c_m_no_hidden,     c_m_round_amount   );
  const __m128i c_m_round_overflow        = _mm_andsrli_epi16( c_m_rounded, h_m_grs_carry, h_m_grs_carry_pos );
  const __m128i c_e_rounded               = _mm_add_epi16(     c_e_fixed,         c_m_round_overflow );
  const __m128i c_m_no_grs                = _mm_srlmi_epi16(   c_m_rounded, h_grs_size,  h_m_mask );
  const __m128i c_e                       = _mm_half_slli(     c_e_rounded,       h_e_pos );
  const __m128i c_em                      = _mm_or_epi16(      c_e,               c_m_no_grs );
  const __m128i c_normal                  = _mm_or_epi16(      c_s,               c_em );
  const __m128i c_inf_result              = _mm_sels_epi16(    is_a_inf_msb,   c_inf,         c_normal );
  const __m128i c_zero_result             = _mm_andc_epi16(    c_inf_result,      is_diff_exactly_zero );
  const __m128i c_result                  = _mm_sels_epi16(    is_invalid_inf_op_msb, h_snan, c_zero_result );

  return (c_result);
}

static SPS_INLINE __m128i _mm_cntlz16(const __m128i x ) 
{
  const __m128i x0  = _mm_half_srli(    x,  1 );
  const __m128i x1  = _mm_or_epi16(     x,  x0 );
  const __m128i x2  = _mm_half_srli(    x1, 2 );
  const __m128i x3  = _mm_or_epi16(     x1, x2 );
  const __m128i x4  = _mm_half_srli(    x3, 4 );
  const __m128i x5  = _mm_or_epi16(     x3, x4 );
  const __m128i x6  = _mm_half_srli(    x5, 8 );
  const __m128i x7  = _mm_or_epi16(     x5, x6 );
  const __m128i x8  = _mm_not_epi16(    x7 );
  const __m128i x9  = _mm_srlmii_epi16( x8, 1,      0x5555 );
  const __m128i xA  = _mm_sub_epi16(    x8, x9 );
  const __m128i xB  = _mm_and1_epi16(   xA, 0x3333 );
  const __m128i xC  = _mm_srlmii_epi16( xA, 2,      0x3333 );
  const __m128i xD  = _mm_add_epi16(    xB, xC );
  const __m128i xE  = _mm_half_srli(    xD, 4 );
  const __m128i xF  = _mm_addm_epi16(   xD, xE,     0x0F0F );
  const __m128i x10 = _mm_half_srli(    xF, 8 );
  const __m128i x11 = _mm_addm_epi16(   xF, x10,    0x001F );
  return ( x11 );
}

void _mm_half_interleave(__m128i& out, __m128i input1, __m128i input2) {
  out = _mm_packs_epi16(input1,input2);
}     

//TODO: use interleave operations
  // half_mul
  // --------
  //
  //  May have 0 or 1 ulp difference from the following result:
  //  (Round to nearest) 
  //  NOTE: Rounding mode differs between conversion and multiply
  //
  //     union FLOAT_32
  //     {
  //       float    f32;
  //       uint32_t u32;
  //     };
  //
  //     union FLOAT_32 fx = { .u32 = half_to_float( x ) };
  //     union FLOAT_32 fy = { .u32 = half_to_float( y ) };
  //     union FLOAT_32 fz = { .f32 = fx.f32 * fy.f32    };
  //     uint16_t       z  = float_to_half( fz );
  //

__m128i _mm_mul_ph(__m128i x, __m128i y) {

  GCC_SPLIT_BLOCK("LOADS")

  // Constants
  const int     one                                =              0x00000001L  ;
  const __m128i h_s_mask                           = _mm_li_epi32(0x00008000L );
  const __m128i h_e_mask                           = _mm_li_epi32(0x00007C00L );
  const __m128i h_m_mask                           = _mm_li_epi32(0x000003FFL );
  const __m128i h_m_hidden                         = _mm_li_epi32(0x00000400L );
  const int     h_e_pos                            =              0x0000000AL  ;
  const __m128i h_e_bias                           = _mm_li_epi32(0x0000000FL );
  const int     h_m_bit_count                      =              0x0000000AL  ;
  const int     h_m_bit_half_count                 =              0x00000005L  ;
  const __m128i h_nan_min                          = _mm_li_epi32(0x00007C01L );
  const __m128i h_e_mask_minus_one                 = _mm_li_epi32(0x00007BFFL );
  const __m128i h_snan                             = _mm_li_epi32(0x0000FE00L );
  const __m128i m_round_overflow_bit               = _mm_li_epi32(0x00000020L );
  const __m128i m_hidden_bit                       = _mm_li_epi32(0x00100000L );

  GCC_SPLIT_BLOCK("CALCULATIONS")
  
  const __m128i a_s                                = _mm_and_epi16(  x,   h_s_mask );
  const __m128i b_s                                = _mm_and_epi16(  y,   h_s_mask );
  
  // Common result
  const __m128i c_s                                = _mm_xor_epi16(  a_s, b_s      );
  
  const __m128i x_e                                = _mm_and_epi16(  x,   h_e_mask );
  const __m128i x_e_eqz_msb                        = _mm_dec_epi32(  x_e );
  const __m128i a                                  = _mm_sels_epi32( x_e_eqz_msb, y, x );
  const __m128i b                                  = _mm_sels_epi32( x_e_eqz_msb, x, y );
  const __m128i a_e                                = _mm_and_epi16(  a,   h_e_mask );
  const __m128i b_e                                = _mm_and_epi16(  b,   h_e_mask );
  const __m128i a_m                                = _mm_and_epi16(  a,   h_m_mask );
  const __m128i b_m                                = _mm_and_epi16(  b,   h_m_mask );

  const __m128i a_e_amount                         = _mm_srli_epi32( a_e,                   h_e_pos                 );
  const __m128i b_e_amount                         = _mm_srli_epi32( b_e,                   h_e_pos                 );
  
  const __m128i a_m_with_hidden                    = _mm_or_epi16(   a_m,                   h_m_hidden              );
  const __m128i b_m_with_hidden                    = _mm_or_epi16(   b_m,                   h_m_hidden              );
  
  const __m128i c_m_normal                         = _mm_mul32_epi(  a_m_with_hidden,       b_m_with_hidden         );
  const __m128i c_m_denorm_biased                  = _mm_mul32_epi(  a_m_with_hidden,       b_m                     );
  
  const __m128i c_e_denorm_unbias_e                = _mm_sub_epi32(  h_e_bias,              a_e_amount              );
  const __m128i c_m_denorm_round_amount            = _mm_and_epi16(  c_m_denorm_biased,     h_m_mask                );
  const __m128i c_m_denorm_rounded                 = _mm_add_epi32(  c_m_denorm_biased,     c_m_denorm_round_amount );
  const __m128i c_m_denorm_inplace                 = _mm_srli_epi32( c_m_denorm_rounded,    h_m_bit_count           );
  const __m128i c_m_denorm_unbiased                = _mm_half_srl(   c_m_denorm_inplace,    c_e_denorm_unbias_e     );
  const __m128i c_m_denorm                         = _mm_and_epi16(  c_m_denorm_unbiased,   h_m_mask                );
  const __m128i c_e_amount_biased                  = _mm_add_epi32(  a_e_amount,            b_e_amount              );
  const __m128i c_e_amount_unbiased                = _mm_sub_epi32(  c_e_amount_biased,     h_e_bias                );
  const __m128i is_c_e_unbiased_underflow          = _mm_ext_epi32(  c_e_amount_unbiased );
  const __m128i c_e_underflow_half_sa              = _mm_neg_epu32(  c_e_amount_unbiased );
  const __m128i c_e_underflow_sa                   = _mm_slli_epi32( c_e_underflow_half_sa, one );
  const __m128i c_m_underflow                      = _mm_half_srl(   c_m_normal,            c_e_underflow_sa);
  const __m128i c_e_underflow_added                = _mm_andc_epi32( c_e_amount_unbiased,   is_c_e_unbiased_underflow );
  const __m128i c_m_underflow_added                = _mm_selb_epi32( is_c_e_unbiased_underflow, c_m_underflow, c_m_normal );
  const __m128i is_mul_overflow_test               = _mm_and_epi16(  c_e_underflow_added,   m_round_overflow_bit );
  const __m128i is_mul_overflow_msb                = _mm_neg_epu32(  is_mul_overflow_test );
  const __m128i c_e_norm_radix_corrected           = _mm_inc_epi32(  c_e_underflow_added );
  const __m128i c_m_norm_radix_corrected           = _mm_srli_epi32( c_m_underflow_added,   one );
  const __m128i c_m_norm_hidden_bit                = _mm_and_epi16(  c_m_norm_radix_corrected,  m_hidden_bit );
  const __m128i is_c_m_norm_no_hidden_msb          = _mm_dec_epi32(  c_m_norm_hidden_bit );
  const __m128i c_m_norm_lo                        = _mm_srli_epi32( c_m_norm_radix_corrected, h_m_bit_half_count );
  const __m128i c_m_norm_lo_nlz                    = _mm_cntlz_epi16_32( c_m_norm_lo );
  const __m128i is_c_m_hidden_nunderflow_msb       = _mm_sub_epi32(  c_m_norm_lo_nlz, c_e_norm_radix_corrected );
  const __m128i is_c_m_hidden_underflow_msb        = _mm_not_epi32(  is_c_m_hidden_nunderflow_msb );
  const __m128i is_c_m_hidden_underflow            = _mm_ext_epi32(  is_c_m_hidden_underflow_msb  );
  const __m128i c_m_hidden_underflow_normalized_sa = _mm_srli_epi32( c_m_norm_lo_nlz, one );
  
  const __m128i c_m_hidden_underflow_normalized    = _mm_half_sll(   c_m_norm_radix_corrected, c_m_hidden_underflow_normalized_sa);
  const __m128i c_m_hidden_normalized              = _mm_half_sll(   c_m_norm_radix_corrected, c_m_norm_lo_nlz );
  
  const __m128i c_e_hidden_normalized              = _mm_sub_epi32(  c_e_norm_radix_corrected, c_m_norm_lo_nlz );
  const __m128i c_e_hidden                         = _mm_andc_epi32( c_e_hidden_normalized, is_c_m_hidden_underflow );
  const __m128i c_m_hidden                         = _mm_sels_epi32( is_c_m_hidden_underflow_msb, c_m_hidden_underflow_normalized, c_m_hidden_normalized );
  const __m128i c_m_normalized                     = _mm_sels_epi32( is_c_m_norm_no_hidden_msb, c_m_hidden, c_m_norm_radix_corrected );
  const __m128i c_e_normalized                     = _mm_sels_epi32( is_c_m_norm_no_hidden_msb, c_e_hidden, c_e_norm_radix_corrected );
  const __m128i c_m_norm_round_amount              = _mm_and_epi16(  c_m_normalized, h_m_mask );
  const __m128i c_m_norm_rounded                   = _mm_add_epi32(  c_m_normalized, c_m_norm_round_amount );
  const __m128i is_round_overflow_test             = _mm_and_epi16(  c_e_normalized, m_round_overflow_bit  );
  const __m128i is_round_overflow_msb              = _mm_neg_epu32(  is_round_overflow_test );
  const __m128i c_m_norm_inplace                   = _mm_srli_epi32( c_m_norm_rounded,    h_m_bit_count );
  const __m128i c_m                                = _mm_and_epi16(  c_m_norm_inplace,    h_m_mask      );
  const __m128i c_e_norm_inplace                   = _mm_slli_epi32( c_e_normalized,      h_e_pos       );
  const __m128i c_e                                = _mm_and_epi16(  c_e_norm_inplace,    h_e_mask      );
  
  const __m128i c_em_nan                           = _mm_or_epi16(   h_e_mask,  a_m        );
  const __m128i c_nan                              = _mm_or_epi16(   a_s,       c_em_nan   );
  const __m128i c_denorm                           = _mm_or_epi16(   c_s,       c_m_denorm );
  const __m128i c_inf                              = _mm_or_epi16(   c_s,       h_e_mask   );
  
  // Norm
  const __m128i c_em_norm                          = _mm_or_epi16(   c_e,       c_m        );
  
  // Denormals
  const __m128i is_a_e_flagged_msb                 = _mm_sub_epi32(  h_e_mask_minus_one, a_e );
  const __m128i is_b_e_flagged_msb                 = _mm_sub_epi32(  h_e_mask_minus_one, b_e );
  const __m128i is_a_e_eqz_msb                     = _mm_dec_epi32(  a_e );
  const __m128i is_a_m_eqz_msb                     = _mm_dec_epi32(  a_m );
  const __m128i is_b_e_eqz_msb                     = _mm_dec_epi32(  b_e );
  const __m128i is_b_m_eqz_msb                     = _mm_dec_epi32(  b_m );
  const __m128i is_b_eqz_msb                       = _mm_and_epi16(  is_b_e_eqz_msb,          is_b_m_eqz_msb         );
  const __m128i is_a_eqz_msb                       = _mm_and_epi16(  is_a_e_eqz_msb,          is_a_m_eqz_msb         );
  const __m128i is_c_nan_via_a_msb                 = _mm_andc_epi32( is_a_e_flagged_msb,      is_b_e_flagged_msb     );
  const __m128i is_c_nan_via_b_msb                 = _mm_andc_epi32( is_b_e_flagged_msb,      is_b_m_eqz_msb         );
  const __m128i is_c_nan_msb                       = _mm_or_epi16(   is_c_nan_via_a_msb,      is_c_nan_via_b_msb     );
  const __m128i is_c_denorm_msb                    = _mm_andc_epi32( is_b_e_eqz_msb,          is_a_e_flagged_msb     );
  const __m128i is_a_inf_msb                       = _mm_and_epi16(  is_a_e_flagged_msb,      is_a_m_eqz_msb         );
  const __m128i is_c_snan_msb                      = _mm_and_epi16(  is_a_inf_msb,            is_b_eqz_msb           );
  const __m128i is_c_nan_min_via_a_msb             = _mm_and_epi16(  is_a_e_flagged_msb,      is_b_eqz_msb           );
  const __m128i is_c_nan_min_via_b_msb             = _mm_and_epi16(  is_b_e_flagged_msb,      is_a_eqz_msb           );
  const __m128i is_c_nan_min_msb                   = _mm_or_epi16(   is_c_nan_min_via_a_msb,  is_c_nan_min_via_b_msb );
  const __m128i is_c_inf_msb                       = _mm_or_epi16(   is_a_e_flagged_msb,      is_b_e_flagged_msb     );
  const __m128i is_overflow_msb                    = _mm_or_epi16(   is_round_overflow_msb,   is_mul_overflow_msb    );

  GCC_SPLIT_BLOCK("SELECT RESULT")
  
  // Overflow
  const __m128i c_em_overflow_result               = _mm_sels_epi32( is_overflow_msb, h_e_mask, c_em_norm );
  
  const __m128i c_common_result                    = _mm_or_epi16(   c_s, c_em_overflow_result );
  const __m128i c_zero_result                      = _mm_sels_epi32( is_b_eqz_msb,     c_s,       c_common_result  );
  const __m128i c_nan_result                       = _mm_sels_epi32( is_c_nan_msb,     c_nan,     c_zero_result );
  const __m128i c_nan_min_result                   = _mm_sels_epi32( is_c_nan_min_msb, h_nan_min, c_nan_result     );
  const __m128i c_inf_result                       = _mm_sels_epi32( is_c_inf_msb,     c_inf,     c_nan_min_result   );
  const __m128i c_denorm_result                    = _mm_sels_epi32( is_c_denorm_msb,  c_denorm,  c_inf_result);
  const __m128i c_result                           = _mm_sels_epi32( is_c_snan_msb,    h_snan,    c_denorm_result );

  return c_result;
}

// Multiply 8 16-bit floating points
static SPS_INLINE __m128i _mm_mul_ph8(const __m128i &x, const __m128i &y)  {
  // Extend to 32 bit
  const __m128i x1 = _mm_unpacklo_epi16(x,_mm_setzero_si128());
  const __m128i x2 = _mm_unpackhi_epi16(x,_mm_setzero_si128());
  const __m128i y1 = _mm_unpacklo_epi16(y,_mm_setzero_si128());
  const __m128i y2 = _mm_unpackhi_epi16(y,_mm_setzero_si128());
  const __m128i retval1 = _mm_mul_ph(x1,y1);
  const __m128i retval2 = _mm_mul_ph(x2,y2);
  return _mm_packs_epi16(retval1,retval2);
}

static SPS_INLINE __m128i 
_mm_sub_ph(__m128i ha, __m128i hb ) 
{
  // (a-b) is the same as (a+(-b))
  return _mm_add_ph( ha, _mm_neg_ph(hb) );
}


/*
  Lexi, while we are at it, is there any chance that we will finally get gather/scatter SIMD instructions at least for 32-bit int/float datatypes? Those would be usefull for so many things because they would reduce the pressure on GPRs for address calculation.

  I would make them as follows:

  GMOVPS  xmmreg, xmmreg/mem128, reg32, imm8 ; gather
  SMOVPS  xmmreg/mem128, xmmreg, reg32, imm8 ; scatter

  xmmreg  for scatter contains four 32-bit floats to be writen out
  for gather it receives values which are read from memory

  xmmreg/mem128   contains four offsets from base pointer

  reg32   GPR containing base address pointer

  imm8    shuffle value like for SHUFPS

  Fictive code example:

  lea     esi, dataset
  lea     edx, [esi + rowsize]
  lea     eax, destination
  lea     edi, offset_table
  mov     ecx, dword ptr [count]
  loop:
  movdqa  xmm1, xmmword ptr [edi]
  movdqa  xmm3, xmmword ptr [edi + 16]
  gmovps  xmm0, xmm1, esi, 0xDD
  gmovps  xmm2, xmm3, edx, 0x88
  movaps  xmm6, xmmword ptr [eax]
  subps   xmm2, xmm0
  mulps   xmm2, xmm7
  addps   xmm2, xmm0
  addps   xmm2, xmm6
  movaps  xmmword ptr [eax], xmm0
  add     esi, 16
  add     edx, 16
  add     eax, 16
  add     edi, 32
  sub     ecx, 1
  jnz     loop

  NOTE: I know that the above loop could be written much better (perhaps using single register as index, etc) but it is just an example off the top of my head. Without an instruction such as gmovps one has to perform anywhere between eight and four loads, two shuffles and a bunch of GPR pointer math to get a vector from scattered data. I cannot be 100% sure that it would be faster, hopefully someone in Intel can test it in some simulator.

  Another thing I always wanted to have is FRACPS, that could help your compiler with some non-vectorizable loops and is generaly very usefull.

  fracps          xmm1, xmm0

  This would simply do the following as one operation

  movaps          xmm1, xmm0
  cvttps2dq       xmm0, xmm0
  cvtdq2ps        xmm0, xmm0
  subps           xmm1, xmm0 ; xmm1 has fractional part

  You could even add an imm8 parameter and use roundps instead of cvttps2dq.

  One step beyond that would be fipsdq (frac-int-ps-dq).

  fipsdq          xmm2, xmm1, xmm0 (could be implicit source)

  movaps          xmm1, xmm0
  cvttps2dq       xmm2, xmm0 ; xmm2 has integer part
  cvtdq2ps        xmm0, xmm2
  subps           xmm1, xmm0 ; xmm1 has fractional part

  In my opinion those two instructions I just proposed would have great chances of working much faster than those the above code since it probably wouldn't require full conversions float->int->float.

  This would bring considerable speedup for interpolation where you need to separate integer and fractional part for indexing and multiplication. Of course, in both versions it would be nice if xmm0 (source) doesn't get trashed so it can be reused. Again, code is just an example, not exactly what it should be.

  So Lexi, could you please pass this to proper department so they consider it for some next SIMD set?
  Regards,
  Igor Levicki
  http://www.levicki.net/


*/
#endif /* HMMINTRIN_H */


