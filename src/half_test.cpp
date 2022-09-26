//$Id: half_test.cpp,v 1.7 2008/06/06 11:34:45 jmh Exp $
#include <stdio.h>

#ifdef __GNUC__
# include <stdint.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "half.h"
#include "float_test_vector.h"
#include "test_float.h"

#include "half_vector_test.h"

#include "half_test.h"
#include "half_print.h"

void
test_conversion( void ) 
{
    int      test_count;
    int      test_fail_count;

    uint32_t i;
 
    printf("---------------------------------------------\n");
    printf("Test float to half conversion (Special Cases)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < N_TESTS; i++ )
    {
      uint32_t test_in;
      uint16_t test_f16;

      test_in  = float_test_vector_special[i];
      test_f16 = half_from_float( test_in );

      if (test_f16 != half_test_vector_special[i])
      {
         FLOAT_32 in;

         in.u32  = test_in;

         printf("float->half: FAILED ");
         print_float( in.f32 );
         printf(" -> ");
         print_half( test_f16 );
         printf(" !- ");
         print_half( half_test_vector_special[i] );
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");


    printf("---------------------------------------------\n");
    printf("Test float->half->float conversion (Med)     \n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;
    
    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint32_t test_in;
      uint16_t test_f16;
      uint32_t test_f32;
      FLOAT_32 fa;
      FLOAT_32 fb;

      test_in  = float_test_vector_med[i];
      test_f16 = half_from_float( test_in );
      test_f32 = half_to_float( test_f16 );

      fa.u32 = test_in;
      fb.u32 = test_f32;
       
      if (!test_float_compare( fa.f32, fb.f32, 4096 )) // 4096 Ulps max loss from half to float conversion
      {
         FLOAT_32 in;
         FLOAT_32 out;

         in.u32  = test_in;
         out.u32 = test_f32;

         printf("float->half->float: FAILED ");
         print_float( in.f32 );
         printf(" -> ");
         print_half( test_f16 );
         printf(" -> ");
         print_float( out.f32 );
         printf("\n");

         test_fail_count++;
      }
      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test float->half->float conversion (Small)   \n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint32_t test_in;
      uint16_t test_f16;
      uint32_t test_f32;
      FLOAT_32 fa;
      FLOAT_32 fb;

      test_in  = float_test_vector_small[i];
      test_f16 = half_from_float( test_in );
      test_f32 = half_to_float( test_f16 );

      fa.u32 = test_in;
      fb.u32 = test_f32;

      uint16_t h_expected = half_from_float( test_f32 );
      uint16_t h_received = test_f16;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 0 )
      {
         FLOAT_32 in;
         FLOAT_32 out;

         in.u32  = test_in;
         out.u32 = test_f32;

         printf("float->half->float: FAILED ");
         print_float( in.f32 );
         printf(" -> ");
         print_half( test_f16 );
         printf(" -> ");
         print_float( out.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");
}

void
test_add( void ) 
{
    int      test_count;
    int      test_fail_count;
    uint32_t i;
    
    printf("---------------------------------------------\n");
    printf("Test half add (Special)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < N_TESTS; i++ )
    {    
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_special[i];
      test_f_b = float_test_vector_special[ (N_TESTS-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_add( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 + fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      fc.u32   = fc.u32;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 0 )
      {
         printf("half_add: FAILED (dulps=%d)\n    ",h_dulps);
         print_half( test_h_a );
         printf(" + ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" !=\n    ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" + ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half add (Small) (+a) + (+b)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_small[i];
      test_f_b = float_test_vector_small[ (float_test_vector_count-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_add( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 + fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      fc.u32   = fc.u32;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 0 )
      {
         printf("half_add: FAILED (dulps=%d)\n    ",h_dulps);
         print_half( test_h_a );
         printf(" + ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" !=\n    ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" + ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");
    
    printf("---------------------------------------------\n");
    printf("Test half add (Small) (-a) + (+b)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_small[i] ^ 0x80000000;
      test_f_b = float_test_vector_small[ (float_test_vector_count-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_add( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 + fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      fc.u32   = fc.u32;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 0 )
      {
         printf("half_add: FAILED (dulps=%d)\n    ",h_dulps);
         print_half( test_h_a );
         printf(" + ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" !=\n    ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" + ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }
      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half add (Small) (+a) + (-b)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_small[i];
      test_f_b = float_test_vector_small[ (float_test_vector_count-1) - i ] ^ 0x80000000;

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_add( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 + fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      fc.u32   = fc.u32;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 0 )
      {
         printf("half_add: FAILED (dulps=%d)\n    ",h_dulps);
         print_half( test_h_a );
         printf(" + ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" !=\n    ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" + ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");


    printf("---------------------------------------------\n");
    printf("Test half add (Med) (+a) + (+b)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_med[i];
      test_f_b = float_test_vector_med[ (float_test_vector_count-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_add( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 + fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      fc.u32   = fc.u32;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 0 )
      {
         printf("half_add: FAILED ");
         print_half( test_h_a );
         printf(" + ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" != ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" + ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half add (Med) (-a) + (+b)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_med[i] ^ 0x80000000;
      test_f_b = float_test_vector_med[ (float_test_vector_count-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_add( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 + fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      fc.u32   = fc.u32;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 0 )
      {
         printf("half_add: FAILED ");
         print_half( test_h_a );
         printf(" + ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" != ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" + ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half add (Med) (+a) + (-b)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_med[i];
      test_f_b = float_test_vector_med[ (float_test_vector_count-1) - i ] ^ 0x80000000;

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_add( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 + fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      fc.u32   = fc.u32;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 0 )
      {
         printf("half_add: FAILED ");
         print_half( test_h_a );
         printf(" + ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" != ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" + ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");
}

void
test_sub( void ) 
{
    int      test_count;
    int      test_fail_count;
    uint32_t i;
 
    printf("---------------------------------------------\n");
    printf("Test half sub (Special Cases)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < N_TESTS; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_special[i];
      test_f_b = float_test_vector_special[ (N_TESTS-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_sub( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 - fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      fc.u32   = fc.u32;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 0 )
      {
         printf("half_sub: FAILED (dulps=%d)\n    ",h_dulps);
         print_half( test_h_a );
         printf(" - ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" !=\n    ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" - ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half sub (Med), (+a) - (+b)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_med[i];
      test_f_b = float_test_vector_med[ (float_test_vector_count-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_sub( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 - fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      fc.u32   = fc.u32;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 0 )
      {
         printf("half_sub: FAILED ");
         print_half( test_h_a );
         printf(" + ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" != ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" - ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half sub (Med), (-a) - (+b)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_med[i] ^ 0x80000000;
      test_f_b = float_test_vector_med[ (float_test_vector_count-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_sub( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 - fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      fc.u32   = fc.u32;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 0 )
      {
         printf("half_sub: FAILED ");
         print_half( test_h_a );
         printf(" + ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" != ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" - ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half sub (Small), (+a) - (+b)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_small[i];
      test_f_b = float_test_vector_small[ (float_test_vector_count-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_sub( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 - fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      fc.u32   = fc.u32;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 0 )
      {
         printf("half_sub: FAILED ");
         print_half( test_h_a );
         printf(" + ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" != ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" - ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half sub (Small), (-a) - (+b)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_small[i] ^ 0x80000000;
      test_f_b = float_test_vector_small[ (float_test_vector_count-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_sub( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 - fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      fc.u32   = fc.u32;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 0 )
      {
         printf("half_sub: FAILED ");
         print_half( test_h_a );
         printf(" + ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" != ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" - ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");
}


void
test_mul( void ) 
{
    int      test_count;
    int      test_fail_count;
    uint32_t i;
 
    printf("---------------------------------------------\n");
    printf("Test half mul (Special Cases)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < N_TESTS; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_special[i];
      test_f_b = float_test_vector_special[ (N_TESTS-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_mul( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 * fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 0 )
      {
         printf("half_mul: FAILED (dulps=%d)\n    ",h_dulps);
         print_half( test_h_a );
         printf(" * ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" !=\n    ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" * ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");
    
    printf("---------------------------------------------\n");
    printf("Test half mul (Med), (+a) * (+b)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_med[i];
      test_f_b = float_test_vector_med[ (float_test_vector_count-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_mul( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 * fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 1 )
      {
         printf("half_mul: FAILED (dulps=%d)\n    ",h_dulps);
         print_half( test_h_a );
         printf(" * ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" != ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" * ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half mul (Med), (-a) * (+b)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_med[i] ^ 0x80000000;
      test_f_b = float_test_vector_med[ (float_test_vector_count-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_mul( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 * fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 1 )
      {
         printf("half_mul: FAILED (dulps=%d)\n    ",h_dulps);
         print_half( test_h_a );
         printf(" * ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" != ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" * ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half mul (Small), (+a) * (+b)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_small[i];
      test_f_b = float_test_vector_small[ (float_test_vector_count-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_mul( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 * fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 1 )
      {
         printf("half_mul: FAILED (dulps=%d)\n    ",h_dulps);
         print_half( test_h_a );
         printf(" * ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" != ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" * ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half mul (Small), (-a) - (+b)\n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      uint16_t test_h_a;
      uint16_t test_h_b;
      uint16_t test_h_c;
      uint32_t test_f_a;
      uint32_t test_f_b;

      FLOAT_32 fa;
      FLOAT_32 fb;
      FLOAT_32 fc;
      FLOAT_32 fc_h;

      test_f_a = float_test_vector_small[i] ^ 0x80000000;
      test_f_b = float_test_vector_small[ (float_test_vector_count-1) - i ];

      test_h_a = half_from_float( test_f_a );
      test_h_b = half_from_float( test_f_b );
      test_h_c = half_mul( test_h_a, test_h_b );

      fa.u32   = half_to_float( test_h_a );
      fb.u32   = half_to_float( test_h_b );
      fc.f32   = fa.f32 * fb.f32;
      fc_h.u32 = half_to_float( test_h_c );

      uint16_t h_expected = half_from_float( fc.u32 );
      uint16_t h_received = test_h_c;

      fc.u32   = fc.u32;

      uint16_t h_dulps = h_expected - h_received;

      if ( h_received >  h_expected )
      {
        h_dulps = h_received - h_expected;
      }

      if ( h_dulps > 1 )
      {
         printf("half_mul: FAILED (dulps=%d)\n    ",h_dulps);
         print_half( test_h_a );
         printf(" * ");
         print_half( test_h_b );
         printf(" :\n    ");
         print_float( fc_h.f32 );
         printf(" != ");
         print_float( fc.f32 );
         printf("\n    ");
         print_float( fa.f32 );
         printf(" * ");
         print_float( fb.f32 );
         printf("\n    ");
         print_half(h_received);
         printf(" != ");
         print_half(h_expected);
         printf("\n");

         test_fail_count++;
      }

      test_count++;
    }

    printf("\n\n");
    if ( test_fail_count )
    {
      printf("FAILED %d of %d\n", test_fail_count, test_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", test_count);
    }
    printf("\n");
    
}

void print_table(void) {
  FILE* fp = fopen("half_float_lut.cpp","w");
  
  if (fp!=NULL) {
    fprintf(fp,"const __declspec(align(16)) int half_float_lut[65536] = {\n");
    FLOAT_32 fa;
    for (size_t i=0; i < 65535 ; i++) {
      uint32_t test_h = half_to_float((uint16_t)i);
      fa.u32 = test_h;
      //fprintf(fp,"0x%04x ",(unsigned short)i);    
      fprintf(fp,"  0x%08x, ",(unsigned int)fa.u32);
      fprintf(fp," /* %e */",fa.f32);
      fprintf(fp,"\n");   
    }
    uint32_t test_h = half_to_float((uint16_t)65535);
    fa.u32 = test_h;
    fprintf(fp,"  0x%08x};",(unsigned int)fa.u32);
    fprintf(fp," /* %e */",fa.f32);
    
    fclose(fp);
  }
}

  //_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  // Set denormals-are-zero
  //_mm_setcsr(_mm_getcsr() | 0x8040);
  
  /*
  int oldMXCSR = _mm_getcsr(); //read the old MXCSR setting
  int newMXCSR = oldMXCSR | 0x8040; // set DAZ and FZ bits
  _mm_setcsr( newMXCSR ); //write the new MXCSR setting to the MXCSR

    ... // do your work with denormals off here

  //restore old MXCSR settings to turn denormals back on if they were on
  _mm_setcsr( oldMXCSR ); 
  
  #include <fenv.h>
#pragma STDC FENV_ACCESS ON

fenv_t oldEnv;

//Read the old environment and set the new environment using default flags and denormals off
fegetenv( &oldEnv );
fesetenv( FE_DFL_DISABLE_SSE_DENORMS_ENV );

... //do work here

//Restore old floating point environment
fesetenv( &oldEnv );
  */
