//$Id: test_float.cpp,v 1.6 2008/10/17 12:27:03 jmh Exp $
#include "test_float.h"
#include <stdio.h>

inline int test_float_is_infinite(float a)
{
    // An infinity has an exponent of 255 (shift left 23 positions) and
    // a zero mantissa. There are two infinities - positive and negative.
    const int32_t      kInfAsInt = 0x7F800000;
    const int32_t      mInfAsInt = 0xFF800000;
    const TEST_FLOAT32 A = {a};
  
    return ( (int32_t) A.u32 == kInfAsInt ) || ( (int32_t) A.u32 == mInfAsInt );
}
inline int test_float_is_nan(float a)
{
    // A NAN has an exponent of 255 (shifted left 23 positions) and
    // a non-zero mantissa.
    const TEST_FLOAT32 A = {a};
    
    const int exp = A.u32 & 0x7F800000;
    const int mantissa  = A.u32 & 0x007FFFFF;

    return (exp == 0x7F800000 && mantissa != 0);
}

inline int test_float_sign(float a)
{
    // The sign bit of a number is the high bit.
    const TEST_FLOAT32 A         = {a};

    return ( (A.u32 & 0x80000000) != 0 );
}

// This is the 'final' version of the AlmostEqualUlps function.
// The optional checks are included for completeness, but in many
// cases they are not necessary, or even not desirable.

//inline int test_float_compare(float a, float b, int maxUlps)
int test_float_compare(float a, float b, int maxUlps)
{
    // There are several optional checks that you can do, depending
    // on what behavior you want from your floating point comparisons.
    // These checks should not be necessary and they are included
    // mainly for completeness.

#ifdef  TEST_FLOAT_INFINITYCHECK
    // If A or B are infinity (positive or negative) then
    // only return 1 if they are exactly equal to each other -
    // that is, if they are both infinities of the same sign.
    // This check is only needed if you will be generating
    // infinities and you don't want them 'close' to numbers
    // near FLT_MAX.
    if (test_float_is_infinite(a) || test_float_is_infinite(b))
        return a == b;
#endif

#ifdef  TEST_FLOAT_NANCHECK
    // If A or B are a NAN, return 0. NANs are equal to nothing,
    // not even themselves.
    // This check is only needed if you will be generating NANs
    // and you use a maxUlps greater than 4 million or you want to
    // ensure that a NAN does not equal itself.
    if (test_float_is_nan(a) || test_float_is_nan(b))
        return 0;
#endif

#ifdef  TEST_FLOAT_SIGNCHECK
    // After adjusting floats so their representations are lexicographically
    // ordered as twos-complement integers a very small positive number
    // will compare as 'close' to a very small negative number. If this is
    // not desireable, and if you are on a platform that supports
    // subnormals (which is the only place the problem can show up) then
    // you need this check.
    // The check for A == B is because zero and negative zero have different
    // signs but are equal to each other.
    if (test_float_sign(a) != test_float_sign(b))
        return a == b;
#endif

    const TEST_FLOAT32 A = {a};
    const TEST_FLOAT32 B  = {b};

    int aInt = A.u32;
    // Make aInt lexicographically ordered as a twos-complement int
    
/*
    uint32_t test_comb_sign_msb = aInt;
    uint32_t mask_comb          = (uint32_t)(((int32_t)test_comb_sign_msb)>>31);
    aInt             = ( (0x80000000 - aInt) & mask_comb ) | ( aInt & (~mask_comb) );
*/
    if (aInt < 0)
        aInt = 0x80000000 - aInt;
    
    // Make bInt lexicographically ordered as a twos-complement int
    int bInt = B.u32;

/*
    test_comb_sign_msb = bInt;
    mask_comb          = (uint32_t)(((int32_t)test_comb_sign_msb)>>31);
    bInt             = ( (0x80000000 - bInt) & mask_comb ) | ( bInt & (~mask_comb) );
*/

    if (bInt < 0)
        bInt = 0x80000000 - bInt;

    // Now we can compare aInt and bInt to find out how far apart A and B
    // are.
    const int intDiff = test_abs_int32(aInt - bInt);

    return (intDiff <= maxUlps);
}
