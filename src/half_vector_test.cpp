// $Id: half_vector_test.cpp,v 1.15 2008/10/25 12:29:21 jmh Exp $

#include "hmmintrin.h"
#include "float_test_vector.h"
#include "test_float.h"
#include "half_print.h"

#include "half_vector_test.h"

#ifdef __GNUC__
 #include "mm_malloc.h"
#endif

#define HALF_DULPS_MUL_LIMIT 1



void test_conversion_vector( void ) 
{
    int      vtest_count;
    int      vtest_fail_count;

    uint32_t i;
    uint32_t j;
 
    printf("---------------------------------------------\n");
    printf("Test float to half conversion (Special Cases)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < N_TESTS; i++ )
    {   
      if (i%4 == 0) {
        __m128 test_in = _mm_load_ps((float*)&float_test_vector_special[i]);
        __m128i test_f16 = _mm_float_to_half(test_in);
        
        for (j=0;j<4;j++) {
          if (( extract_uint16(test_f16,j*2) != half_test_vector_special[i+j])) {
            printf("float->half: FAILED ");
            FLOAT_32 in;
            in.u32 = float_test_vector_special[i+j];
            print_float( in.f32 );
            printf(" -> ");
            print_half( extract_uint16(test_f16, 2*j) );
            printf(" !- ");
            print_half( half_test_vector_special[i+j] );
            printf("\n");
            vtest_fail_count++;
          }
          vtest_count++;
        }
      }
    }

    printf("\n\n");
    if ( vtest_fail_count )
    {
      getchar();
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test float->half->float conversion (Med)     \n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;
    
    for ( i=0; i < float_test_vector_count; i++ )
    {
      __m128 vtest_f32;
      
      if (i%4 == 0) {
              
        __m128 vtest_in = _mm_load_ps((float*)&float_test_vector_med[i]);
        __m128i vtest_f16  = _mm_float_to_half(vtest_in);
#ifdef __GNUC__
        vtest_f32 = (__m128)_mm_half_to_float(vtest_f16);
#else        
        vtest_f32 = *(__m128*)&_mm_half_to_float(vtest_f16);
#endif        
        for (j=0;j<4;j++) {
          if (!test_float_compare( extract_f32(vtest_in,j), extract_f32(vtest_f32,j), 4096 )) {

            printf("float->half->float: FAILED ");
            print_float( extract_f32(vtest_in, j) );
            printf(" -> ");
            print_half( extract_uint16(vtest_f16,j*2) );
            printf(" -> ");
            print_float( extract_f32(vtest_f32, j) );
            printf("\n");
            vtest_fail_count++;
          }
          vtest_count++;
        }      
      }
    }

    printf("\n\n");
    if ( vtest_fail_count )
    {
      getchar();
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test float->half->float conversion (Small)   \n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");


    vtest_count      = 0;
    vtest_fail_count = 0;
    
    for ( i=0; i < float_test_vector_count; i++ )
    {
      FLOAT_32 fa;
      FLOAT_32 fb;

      __m128 vtest_in; 
      __m128i vtest_f16;
      __m128 vtest_f32;
        
      if (i%4 == 0) {    
        vtest_in = _mm_load_ps((float*)&float_test_vector_small[i]);
        vtest_f16  = _mm_float_to_half(vtest_in);
#ifdef __GNUC__
        vtest_f32 = (__m128)_mm_half_to_float(vtest_f16);
#else        
        vtest_f32 = *(__m128*)&_mm_half_to_float(vtest_f16);
#endif        
        __m128i vh_expected = _mm_float_to_half(vtest_f32);
        
        for (j=0;j<4;j++) {

          _mm_empty();
          fa.u32 = extract_uint32(vtest_in, j);
          fb.u32 = extract_uint32(vtest_f32, j);

          uint16_t h_expected = extract_uint16(vh_expected,2*j);
          uint16_t h_received = extract_uint16(vtest_f16,2*j);
          _mm_empty();
          uint16_t h_dulps = h_expected - h_received;
          
          if ( h_received >  h_expected ) {
            h_dulps = h_received - h_expected;
          }

          if ( h_dulps > 0 ) {
            FLOAT_32 in;
            FLOAT_32 out;

            in.u32  = extract_uint32(vtest_in, j);
            out.u32 = extract_uint32(vtest_f32, j);

            printf("float->half->float: FAILED ");
            print_float( in.f32 );
            printf(" -> ");
            print_half( extract_uint16(vtest_f16, 2*j) );
            printf(" -> ");
            print_float( out.f32 );
            printf("\n    ");
            print_half(h_received);
            printf(" != ");
            print_half(h_expected);
            printf("\n");

            vtest_fail_count++;
          }
          vtest_count++; 
        }   
      }
    }

    printf("\n\n");
    if ( vtest_fail_count )
    {
      getchar();
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");

}

void
test_add_vector( void ) {
    int      test_count;
    int      test_fail_count;
    int      vtest_count;
    int      vtest_fail_count;
    uint32_t i,j;
    
    printf("---------------------------------------------\n");
    printf("Test half add (Special)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    test_count      = 0;
    test_fail_count = 0;
    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < N_TESTS; i++ )
    {
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128  vtest_f_a;
      __m128  vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128  vfc;
      __m128i vfc_h;
     
      // Dummy
      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
 
      if (i%4 == 0 ) {
        vtest_f_a = _mm_load_ps((float*)&float_test_vector_special[i]);
        vtest_f_b = _mm_load_ps((float*)&float_test_vector_special[ (N_TESTS-4) - 4*(i/4) ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));

        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );        
        vtest_h_c = _mm_add_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float(vtest_h_a);
        vfb   = _mm_half_to_float(vtest_h_b);

#ifdef __GNUC__        
        vfc    = _mm_add_ps((__m128)vfa,(__m128)vfb);
#else
        vfc    = _mm_add_ps(*(__m128*)&vfa,*(__m128*)&vfb);
#endif        
        vfc_h = _mm_half_to_float(*(__m128i*)&vtest_h_c); 
        
        __m128i vh_expected = _mm_float_to_half(vfc);
        __m128i vh_received = vtest_h_c;
        
#ifdef __GNUC__        
        vffa = (__m128)vfa;
        vffb = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif
        
        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(vh_expected, j*2) - extract_uint16(vh_received, j*2);

          if ( extract_uint16(vh_received, j*2) >  extract_uint16(vh_expected, j*2) ) {
            h_dulps = extract_uint16(vh_received, j*2) - extract_uint16(vh_expected, j*2);
          }

          if ( h_dulps > 0 ) {
            printf("half_add: FAILED (dulps=%d)\n    ",h_dulps);
            
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" + ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" + ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            
            print_half(extract_uint16(vh_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(vh_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }
          vtest_count++;
        }
      }
    }
    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half add (Small) (+a) + (+b)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ ) {   
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128  vtest_f_a;
      __m128  vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128  vfc;
      __m128i vfc_h;
     
      // Dummy
      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
 
      if (i%4 == 0 ) {
        vtest_f_a = _mm_load_ps((float*)&float_test_vector_small[i]);
        vtest_f_b = _mm_load_ps((float*)&float_test_vector_small[ (float_test_vector_count-4) - 4*(i/4) ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));

        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );        
        vtest_h_c = _mm_add_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float(vtest_h_a);
        vfb   = _mm_half_to_float(vtest_h_b);
        
#ifdef __GNUC__
        vfc    = _mm_add_ps((__m128)vfa,(__m128)vfb);
        vfc_h = _mm_half_to_float((__m128i)vtest_h_c); 
#else
        vfc    = _mm_add_ps(*(__m128*)&vfa,*(__m128*)&vfb);
        vfc_h = _mm_half_to_float(*(__m128i*)&vtest_h_c); 
#endif
        
        __m128i vh_expected = _mm_float_to_half(vfc);
        __m128i vh_received = vtest_h_c;
        
#ifdef __GNUC__
        vffa   = (__m128)vfa;
        vffb   = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif
        
        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(vh_expected, 2*j) - extract_uint16(vh_received, 2*j);

          if ( extract_uint16(vh_received, 2*j) >  extract_uint16(vh_expected, 2*j) ) {
            h_dulps = extract_uint16(vh_received, 2*j) - extract_uint16(vh_expected, 2*j);
          }

          if ( h_dulps > 0 ) {
            printf("half_add: FAILED (dulps=%d)\n    ",h_dulps);
            
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" + ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" + ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            
            print_half(extract_uint16(vh_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(vh_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }
          vtest_count++;
        }
      }
    }
    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");
    
    printf("---------------------------------------------\n");
    printf("Test half add (Small) (-a) + (+b)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ ) {
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128  vtest_f_a;
      __m128  vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128  vfc;
      __m128i vfc_h;
     
      // Dummy
      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
      _mm_empty();
      if (i % 4 == 0 ) {
        
        vtest_f_a = _mm_neg_ps(_mm_load_ps((float*)&float_test_vector_small[i]));

        vtest_f_b = _mm_load_ps((float*)&float_test_vector_small[ (float_test_vector_count-4) - 4*(i/4) ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));
        
        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );        

        vtest_h_c = _mm_add_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float(vtest_h_a);
        vfb   = _mm_half_to_float(vtest_h_b); 
        
#ifdef __GNUC__
        vfc    = _mm_add_ps((__m128)vfa,(__m128)vfb);
        vfc_h = _mm_half_to_float((__m128i)vtest_h_c); 
#else
        vfc    = _mm_add_ps(*(__m128*)&vfa,*(__m128*)&vfb);
        vfc_h = _mm_half_to_float(*(__m128i*)&vtest_h_c); 
#endif

        
        __m128i vh_expected = _mm_float_to_half(vfc);
        __m128i vh_received = vtest_h_c;
        
#ifdef __GNUC__        
        vffa = (__m128)vfa;
        vffb = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif
        
        for (j=0;j<4;j++) {

          _mm_empty();
          uint16_t h_dulps = extract_uint16(vh_expected, 2*j) - extract_uint16(vh_received, 2*j);

          if ( extract_uint16(vh_received, 2*j) >  extract_uint16(vh_expected, 2*j) ) {
            h_dulps = extract_uint16(vh_received, 2*j) - extract_uint16(vh_expected, 2*j);
          }

          if ( h_dulps > 0 ) { // i=4(5) ,3192288129, 961007201
            printf("half_add: FAILED (dulps=%d) index %ld\n    ",h_dulps, i*4 + j);
            
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" + ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" + ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            
            print_half(extract_uint16(vh_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(vh_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }
          vtest_count++;
        }
      }
    }
    
    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");
    
    printf("---------------------------------------------\n");
    printf("Test half add (Small) (+a) + (-b)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < float_test_vector_count ; i++ ) {
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128  vtest_f_a;
      __m128  vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128  vfc;
      __m128i vfc_h;
     
      // Dummy
      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
 
      if (i%4 == 0 ) {
        vtest_f_a = _mm_load_ps((float*)&float_test_vector_small[i]);
        vtest_f_b = _mm_neg_ps(_mm_load_ps((float*)&float_test_vector_small[ (float_test_vector_count-4) - 4*(i/4) ]));
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));

        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );        
        vtest_h_c = _mm_add_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float(vtest_h_a);
        vfb   = _mm_half_to_float(vtest_h_b);
        
#ifdef __GNUC__        
        vfc    = _mm_add_ps((__m128)vfa,(__m128)vfb);
        vfc_h = _mm_half_to_float((__m128i)vtest_h_c); 
#else
        vfc    = _mm_add_ps(*(__m128*)&vfa,*(__m128*)&vfb);
        vfc_h = _mm_half_to_float(*(__m128i*)&vtest_h_c); 
#endif
        
        __m128i vh_expected = _mm_float_to_half(vfc);
        __m128i vh_received = vtest_h_c;
        
#ifdef __GNUC__        
        vffa = (__m128)vfa;
        vffb = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif
        
        for (j=0;j<4;j++) {
          _mm_empty();
          uint16_t h_dulps = extract_uint16(vh_expected, 2*j) - extract_uint16(vh_received, 2*j);

          if ( extract_uint16(vh_received, 2*j) >  extract_uint16(vh_expected, 2*j) ) {
            h_dulps = extract_uint16(vh_received, 2*j) - extract_uint16(vh_expected, 2*j);
          }

          if ( h_dulps > 0 ) {
            printf("half_add: FAILED (dulps=%d) index %ld\n    ",h_dulps, i*4 + j);
            
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" + ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" + ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            
            print_half(extract_uint16(vh_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(vh_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }
          vtest_count++;
        }
      }
    }

    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");


    printf("---------------------------------------------\n");
    printf("Test half add (Med) (+a) + (+b)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i <  float_test_vector_count; i++ ) {
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128  vtest_f_a;
      __m128  vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128  vfc;
      __m128i vfc_h;
     
      // Dummy
      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
 
      if (i%4 == 0 ) {
        vtest_f_a = _mm_load_ps((float*)&float_test_vector_med[i]);
        vtest_f_b = _mm_load_ps((float*)&float_test_vector_med[ (float_test_vector_count-4) - 4*(i/4) ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));

        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );        
        vtest_h_c = _mm_add_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float(vtest_h_a);
        vfb   = _mm_half_to_float(vtest_h_b);
        
        vfc    = _mm_add_ps(*(__m128*)&vfa,*(__m128*)&vfb);
        vfc_h = _mm_half_to_float(*(__m128i*)&vtest_h_c); 
        
        __m128i vh_expected = _mm_float_to_half(vfc);
        __m128i vh_received = vtest_h_c;
        
#ifdef __GNUC__        
        vffa = (__m128)vfa;
        vffb = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif
        
        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(vh_expected, 2*j) - extract_uint16(vh_received, 2*j);

          if ( extract_uint16(vh_received, 2*j) >  extract_uint16(vh_expected, 2*j) ) {
            h_dulps = extract_uint16(vh_received, 2*j) - extract_uint16(vh_expected, 2*j);
          }

          if ( h_dulps > 0 ) {
            printf("half_add: FAILED (dulps=%d)\n    ",h_dulps);
            
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" + ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" + ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            
            print_half(extract_uint16(vh_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(vh_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }
          vtest_count++;
        }
      }
    }

    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");
    
    printf("---------------------------------------------\n");
    printf("Test half add (Med) (-a) + (+b)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ ) {

      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128  vtest_f_a;
      __m128  vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128  vfc;
      __m128i vfc_h;
     
      // Dummy
      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
 
      if (i%4 == 0 ) {
        vtest_f_a = _mm_neg_ps(_mm_load_ps((float*)&float_test_vector_med[i]));
        vtest_f_b = _mm_load_ps((float*)&float_test_vector_med[ (float_test_vector_count-4) - 4*(i/4) ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));

        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );        
        vtest_h_c = _mm_add_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float(vtest_h_a);
        vfb   = _mm_half_to_float(vtest_h_b);
        
        vfc    = _mm_add_ps(*(__m128*)&vfa,*(__m128*)&vfb);
        vfc_h = _mm_half_to_float(*(__m128i*)&vtest_h_c); 
        
        __m128i vh_expected = _mm_float_to_half(vfc);
        __m128i vh_received = vtest_h_c;
        
#ifdef __GNUC__        
        vffa = (__m128)vfa;
        vffb = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif
        
        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(vh_expected, 2*j) - extract_uint16(vh_received, 2*j);

          if ( extract_uint16(vh_received, 2*j) >  extract_uint16(vh_expected, 2*j) ) {
            h_dulps = extract_uint16(vh_received, 2*j) - extract_uint16(vh_expected, 2*j);
          }

          if ( h_dulps > 0 ) {
            printf("half_add: FAILED (dulps=%d)\n    ",h_dulps);
            
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" + ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" + ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            
            print_half(extract_uint16(vh_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(vh_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }
          vtest_count++;
        }
      }
    }

    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half add (Med) (+a) + (-b)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ ) {

      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128  vtest_f_a;
      __m128  vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128  vfc;
      __m128i vfc_h;
     
      // Dummy
      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
 
      if (i%4 == 0 ) {
        vtest_f_a = _mm_load_ps((float*)&float_test_vector_med[i]);
        vtest_f_b = _mm_neg_ps(_mm_load_ps((float*)&float_test_vector_med[ (float_test_vector_count-4) - 4*(i/4) ]));
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));

        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );        
        vtest_h_c = _mm_add_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float(vtest_h_a);
        vfb   = _mm_half_to_float(vtest_h_b);
        
        vfc    = _mm_add_ps(*(__m128*)&vfa,*(__m128*)&vfb);
        vfc_h = _mm_half_to_float(*(__m128i*)&vtest_h_c); 
        
        __m128i vh_expected = _mm_float_to_half(vfc);
        __m128i vh_received = vtest_h_c;
        
#ifdef __GNUC__        
        vffa = (__m128)vfa;
        vffb = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif
        
        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(vh_expected, 2*j) - extract_uint16(vh_received, 2*j);

          if ( extract_uint16(vh_received, 2*j) >  extract_uint16(vh_expected, 2*j) ) {
            h_dulps = extract_uint16(vh_received, 2*j) - extract_uint16(vh_expected, 2*j);
          }

          if ( h_dulps > 0 ) {
            printf("half_add: FAILED (dulps=%d)\n    ",h_dulps);
            
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" + ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" + ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            
            print_half(extract_uint16(vh_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(vh_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }
          vtest_count++;
        }
      }
    }

    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");
}

void
test_sub_vector( void ) 
{
    int      vtest_count;
    int      vtest_fail_count;
    uint32_t i,j;
 
    printf("---------------------------------------------\n");
    printf("Test half sub (Special Cases)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < N_TESTS ; i++ )
    {
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128 vtest_f_a;
      __m128 vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128 vfc;
      __m128i vfc_h;

      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
      if (i%4 == 0) {
        vtest_f_a = _mm_load_ps((float*)&float_test_vector_special[i]);
        vtest_f_b = _mm_load_ps((float*)&float_test_vector_special[ (N_TESTS-4) - (i/4)*4 ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));
        
        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );
        vtest_h_c = _mm_sub_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float( vtest_h_a );
        vfb   = _mm_half_to_float( vtest_h_b );
        vfc   = _mm_sub_ps(*(__m128*)&vfa,*(__m128*)&vfb);
        vfc_h = _mm_half_to_float( vtest_h_c );

#ifdef __GNUC__        
        vffa = (__m128)vfa;
        vffb = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif
        
        __m128i h_expected = _mm_float_to_half( vfc );
        __m128i h_received = vtest_h_c;

        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(h_expected, 2*j) - extract_uint16(h_received, 2*j);

          if ( extract_uint16(h_received, 2*j) >  extract_uint16(h_expected, 2*j) )
          {
            h_dulps = extract_uint16(h_received, 2*j) - extract_uint16(h_expected, 2*j);
          }

          if ( h_dulps > 0 )
          {
            printf("half_sub: FAILED (dulps=%d) index %ld\n    ",h_dulps, i*4 + j);
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" - ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" - ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            print_half(extract_uint16(h_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(h_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }

          vtest_count++;
        }
      }
    }
    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half sub (Med), (+a) - (+b)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");
    
    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128 vtest_f_a;
      __m128 vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128 vfc;
      __m128i vfc_h;

      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
      if (i%4 == 0) {
        vtest_f_a = _mm_load_ps((float*)&float_test_vector_med[i]);
        vtest_f_b = _mm_load_ps((float*)&float_test_vector_med[ (float_test_vector_count-4) - (i/4)*4 ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));
        
        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );
        vtest_h_c = _mm_sub_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float( vtest_h_a );
        vfb   = _mm_half_to_float( vtest_h_b );
        vfc   = _mm_sub_ps(*(__m128*)&vfa,*(__m128*)&vfb);
        vfc_h = _mm_half_to_float( vtest_h_c );

#ifdef __GNUC__        
        vffa = (__m128)vfa;
        vffb = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif
        
        __m128i h_expected = _mm_float_to_half( vfc );
        __m128i h_received = vtest_h_c;

        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(h_expected, 2*j) - extract_uint16(h_received, 2*j);

          if ( extract_uint16(h_received, 2*j) >  extract_uint16(h_expected, 2*j) )
          {
            h_dulps = extract_uint16(h_received, 2*j) - extract_uint16(h_expected, 2*j);
          }

          if ( h_dulps > 0 )
          {
            printf("half_sub: FAILED (dulps=%d)\n    ",h_dulps);
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" - ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" - ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            print_half(extract_uint16(h_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(h_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }

          vtest_count++;
        }
      }
    }

    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half sub (Med), (-a) - (+b)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128 vtest_f_a;
      __m128 vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128 vfc;
      __m128i vfc_h;

      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
      if (i%4 == 0) {
        vtest_f_a = _mm_neg_ps(_mm_load_ps((float*)&float_test_vector_med[i]));
        vtest_f_b = _mm_load_ps((float*)&float_test_vector_med[ (float_test_vector_count-4) - (i/4)*4 ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));
        
        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );
        vtest_h_c = _mm_sub_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float( vtest_h_a );
        vfb   = _mm_half_to_float( vtest_h_b );
        vfc   = _mm_sub_ps(*(__m128*)&vfa,*(__m128*)&vfb);
        vfc_h = _mm_half_to_float( vtest_h_c );

#ifdef __GNUC__        
        vffa = (__m128)vfa;
        vffb = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif
        
        __m128i h_expected = _mm_float_to_half( vfc );
        __m128i h_received = vtest_h_c;

        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(h_expected, 2*j) - extract_uint16(h_received, 2*j);

          if ( extract_uint16(h_received, 2*j) >  extract_uint16(h_expected, 2*j) )
          {
            h_dulps = extract_uint16(h_received, 2*j) - extract_uint16(h_expected, 2*j);
          }

          if ( h_dulps > 0 )
          {
            printf("half_sub: FAILED (dulps=%d)\n    ",h_dulps);
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" - ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" - ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            print_half(extract_uint16(h_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(h_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }

          vtest_count++;
        }
      }
    }

    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");
    
    printf("---------------------------------------------\n");
    printf("Test half sub (Small), (+a) - (+b)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128 vtest_f_a;
      __m128 vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128 vfc;
      __m128i vfc_h;

      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
      if (i%4 == 0) {
        vtest_f_a = _mm_load_ps((float*)&float_test_vector_small[i]);
        vtest_f_b = _mm_load_ps((float*)&float_test_vector_small[ (float_test_vector_count-4) - (i/4)*4 ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));
        
        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );
        vtest_h_c = _mm_sub_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float( vtest_h_a );
        vfb   = _mm_half_to_float( vtest_h_b );
        vfc   = _mm_sub_ps(*(__m128*)&vfa,*(__m128*)&vfb);
        vfc_h = _mm_half_to_float( vtest_h_c );

#ifdef __GNUC__        
        vffa = (__m128)vfa;
        vffb = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif
        
        __m128i h_expected = _mm_float_to_half( vfc );
        __m128i h_received = vtest_h_c;

        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(h_expected, 2*j) - extract_uint16(h_received, 2*j);

          if ( extract_uint16(h_received, 2*j) >  extract_uint16(h_expected, 2*j) )
          {
            h_dulps = extract_uint16(h_received, 2*j) - extract_uint16(h_expected, 2*j);
          }

          if ( h_dulps > 0 )
          {
            printf("half_sub: FAILED (dulps=%d) index %ld\n    ",h_dulps, i*4 + j);
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" - ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" - ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            print_half(extract_uint16(h_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(h_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }

          vtest_count++;
        }
      }
    }

    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");
    
    printf("---------------------------------------------\n");
    printf("Test half sub (Small), (-a) - (+b)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128 vtest_f_a;
      __m128 vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128 vfc;
      __m128i vfc_h;

      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
      if (i%4 == 0) {
        vtest_f_a = _mm_neg_ps(_mm_load_ps((float*)&float_test_vector_small[i]));
        vtest_f_b = _mm_load_ps((float*)&float_test_vector_small[ (float_test_vector_count-4) - (i/4)*4 ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));
        
        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );
        vtest_h_c = _mm_sub_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float( vtest_h_a );
        vfb   = _mm_half_to_float( vtest_h_b );
        vfc   = _mm_sub_ps(*(__m128*)&vfa,*(__m128*)&vfb);
        vfc_h = _mm_half_to_float( vtest_h_c );

#ifdef __GNUC__        
        vffa = (__m128)vfa;
        vffb = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif        
        __m128i h_expected = _mm_float_to_half( vfc );
        __m128i h_received = vtest_h_c;

        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(h_expected, 2*j) - extract_uint16(h_received, 2*j);

          if ( extract_uint16(h_received, 2*j) >  extract_uint16(h_expected, 2*j) )
          {
            h_dulps = extract_uint16(h_received, 2*j) - extract_uint16(h_expected, 2*j);
          }

          if ( h_dulps > 0 )
          {
            printf("half_sub: FAILED (dulps=%d)\n    ",h_dulps);
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" - ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" - ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            print_half(extract_uint16(h_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(h_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }

          vtest_count++;
        }
      }
    }
    
    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");
}


void test_mul_vector( void ) 
{
    int      vtest_count;
    int      vtest_fail_count;
    uint32_t i,j;
 
    printf("---------------------------------------------\n");
    printf("Test half mul (Special Cases)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < N_TESTS; i++ ) {
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128 vtest_f_a;
      __m128 vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128 vfc;
      __m128i vfc_h;

      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
      if (i%4 == 0) {
        vtest_f_a = _mm_load_ps((float*)&float_test_vector_special[i]);
        vtest_f_b = _mm_load_ps((float*)&float_test_vector_special[ (N_TESTS-4) - (i/4)*4 ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));
        
        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );
        vtest_h_c = _mm_mul_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float( vtest_h_a );
        vfb   = _mm_half_to_float( vtest_h_b );
        vfc   = _mm_mul_ps(*(__m128*)&vfa,*(__m128*)&vfb);
        vfc_h = _mm_half_to_float( vtest_h_c );

#ifdef __GNUC__        
        vffa = (__m128)vfa;
        vffb = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif
        
        __m128i h_expected = _mm_float_to_half( vfc );
        __m128i h_received = vtest_h_c;

        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(h_expected, 2*j) - extract_uint16(h_received, 2*j);

          if ( extract_uint16(h_received, 2*j) >  extract_uint16(h_expected, 2*j) )
          {
            h_dulps = extract_uint16(h_received, 2*j) - extract_uint16(h_expected, 2*j);
          }

          if ( h_dulps > HALF_DULPS_MUL_LIMIT )
          {
            printf("half_mul: FAILED (dulps=%d)\n    ",h_dulps);
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" * ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" * ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            print_half(extract_uint16(h_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(h_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }

          vtest_count++;
        }
      }
    }
    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");

    printf("---------------------------------------------\n");
    printf("Test half mul (Med), (+a) * (+b)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");
    
    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128 vtest_f_a;
      __m128 vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128 vfc;
      __m128i vfc_h;

      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
      if (i%4 == 0) {
        // TODO: introduce union (float* int*) to avoid breaking of strict alising
        vtest_f_a = _mm_load_ps((float*)&float_test_vector_med[i]);
        vtest_f_b = _mm_load_ps((float*)&float_test_vector_med[ (float_test_vector_count-4) - (i/4)*4 ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));
        
        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );
        vtest_h_c = _mm_mul_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float( vtest_h_a );
        vfb   = _mm_half_to_float( vtest_h_b );

#ifdef __GNUC__        
        vfc   = _mm_mul_ps((__m128)vfa,(__m128)vfb);
#else
        vfc   = _mm_mul_ps(*(__m128*)&vfa,*(__m128*)&vfb);
#endif        
        vfc_h = _mm_half_to_float( vtest_h_c );

#ifdef __GNUC__        
        vffa = (__m128)vfa;
        vffb = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif
        
        __m128i h_expected = _mm_float_to_half( vfc );
        __m128i h_received = vtest_h_c;

        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(h_expected, 2*j) - extract_uint16(h_received, 2*j);

          if ( extract_uint16(h_received, 2*j) >  extract_uint16(h_expected, 2*j) )
          {
            h_dulps = extract_uint16(h_received, 2*j) - extract_uint16(h_expected, 2*j);
          }

          if ( h_dulps > HALF_DULPS_MUL_LIMIT ) {
            printf("half_mul: FAILED (dulps=%d)\n    ",h_dulps);
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" * ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" * ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            print_half(extract_uint16(h_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(h_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }

          vtest_count++;
        }
      }
    }

    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");
    
    printf("---------------------------------------------\n");
    printf("Test half mul (Med), (-a) * (+b)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128 vtest_f_a;
      __m128 vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128 vfc;
      __m128i vfc_h;

      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
      if (i%4 == 0) {
        vtest_f_a = _mm_neg_ps(_mm_load_ps((float*)&float_test_vector_med[i]));
        vtest_f_b = _mm_load_ps((float*)&float_test_vector_med[ (float_test_vector_count-4) - (i/4)*4 ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));
        
        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );
        vtest_h_c = _mm_mul_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float( vtest_h_a );
        vfb   = _mm_half_to_float( vtest_h_b );
        vfc   = _mm_mul_ps(*(__m128*)&vfa,*(__m128*)&vfb);
        vfc_h = _mm_half_to_float( vtest_h_c );

        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
        
        __m128i h_expected = _mm_float_to_half( vfc );
        __m128i h_received = vtest_h_c;

        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(h_expected, 2*j) - extract_uint16(h_received, 2*j);

          if ( extract_uint16(h_received, 2*j) >  extract_uint16(h_expected, 2*j) )
          {
            h_dulps = extract_uint16(h_received, 2*j) - extract_uint16(h_expected, 2*j);
          }

          if ( h_dulps > HALF_DULPS_MUL_LIMIT )
          {
            printf("half_mul: FAILED (dulps=%d)\n    ",h_dulps);
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" * ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" * ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            print_half(extract_uint16(h_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(h_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }

          vtest_count++;
        }
      }
    }

    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");
    
    printf("---------------------------------------------\n");
    printf("Test half mul (Small), (+a) * (+b)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128 vtest_f_a;
      __m128 vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128 vfc;
      __m128i vfc_h;

      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
      if (i%4 == 0) {
        vtest_f_a = _mm_load_ps((float*)&float_test_vector_small[i]);
        vtest_f_b = _mm_load_ps((float*)&float_test_vector_small[ (float_test_vector_count-4) - (i/4)*4 ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));
        
        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );
        vtest_h_c = _mm_mul_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float( vtest_h_a );
        vfb   = _mm_half_to_float( vtest_h_b );
        vfc   = _mm_mul_ps(*(__m128*)&vfa,*(__m128*)&vfb);
        vfc_h = _mm_half_to_float( vtest_h_c );

        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
        
        __m128i h_expected = _mm_float_to_half( vfc );
        __m128i h_received = vtest_h_c;

        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(h_expected, 2*j) - extract_uint16(h_received, 2*j);

          if ( extract_uint16(h_received, 2*j) >  extract_uint16(h_expected, 2*j) )
          {
            h_dulps = extract_uint16(h_received, 2*j) - extract_uint16(h_expected, 2*j);
          }

          if ( h_dulps > HALF_DULPS_MUL_LIMIT )
          {
            printf("half_mul: FAILED (dulps=%d) index %ld\n    ",h_dulps,i*4+j);
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" * ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" * ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            print_half(extract_uint16(h_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(h_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }

          vtest_count++;
        }
      }
    }

    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");
    
    printf("---------------------------------------------\n");
    printf("Test half mul (Small), (-a) * (+b)\n");
    printf("SSE version                                  \n");
    printf("---------------------------------------------\n");

    vtest_count      = 0;
    vtest_fail_count = 0;

    for ( i=0; i < float_test_vector_count; i++ )
    {
      __m128i vtest_h_a;
      __m128i vtest_h_b;
      __m128i vtest_h_c;
      __m128 vtest_f_a;
      __m128 vtest_f_b;

      __m128i vfa;
      __m128i vfb;
      __m128 vfc;
      __m128i vfc_h;

      __m128 vffa;
      __m128 vffb;
      __m128 vffc_h;
      
      if (i%4 == 0) {
        vtest_f_a = _mm_neg_ps(_mm_load_ps((float*)&float_test_vector_small[i]));
        vtest_f_b = _mm_load_ps((float*)&float_test_vector_small[ (float_test_vector_count-4) - (i/4)*4 ]);
        vtest_f_b = _mm_shuffle_ps(vtest_f_b, vtest_f_b, _MM_SHUFFLE (0, 1, 2, 3));
        
        vtest_h_a = _mm_float_to_half( vtest_f_a );
        vtest_h_b = _mm_float_to_half( vtest_f_b );
        vtest_h_c = _mm_mul_ph( vtest_h_a, vtest_h_b );

        vfa   = _mm_half_to_float( vtest_h_a );
        vfb   = _mm_half_to_float( vtest_h_b );
#ifdef __GNUC__
        vfc   = _mm_mul_ps((__m128)vfa,(__m128)vfb);
#else
        vfc   = _mm_mul_ps(*(__m128*)&vfa,*(__m128*)&vfb);
#endif        
        vfc_h = _mm_half_to_float( vtest_h_c );

#ifdef __GNUC__
        vffa   = (__m128)vfa;
        vffb   = (__m128)vfb;
        vffc_h = (__m128)vfc_h;
#else
        vffa = *(__m128*)&vfa;
        vffb = *(__m128*)&vfb;
        vffc_h = *(__m128*)&vfc_h;
#endif
        
        __m128i h_expected = _mm_float_to_half( vfc );
        __m128i h_received = vtest_h_c;

        for (j=0;j<4;j++) {

          uint16_t h_dulps = extract_uint16(h_expected, 2*j) - extract_uint16(h_received, 2*j);

          if ( extract_uint16(h_received, 2*j) >  extract_uint16(h_expected, 2*j) )
          {
            h_dulps = extract_uint16(h_received, 2*j) - extract_uint16(h_expected, 2*j);
          }

          if ( h_dulps > HALF_DULPS_MUL_LIMIT )
          {
            printf("half_mul: FAILED (dulps=%d) index %ld\n    ",h_dulps,i*4+j);
            print_half( extract_uint16(vtest_h_a, 2*j) );
            printf(" * ");
            print_half( extract_uint16(vtest_h_b, 2*j) );
            printf(" :\n    ");
            print_float( extract_f32(vffc_h, j) );
            printf(" !=\n    ");
            print_float( extract_f32(vfc, j) );
            printf("\n    ");
            print_float( extract_f32(vffa, j) );
            printf(" * ");
            print_float( extract_f32(vffb, j) );
            printf("\n    ");
            print_half(extract_uint16(h_received, 2*j));
            printf(" != ");
            print_half(extract_uint16(h_expected, 2*j));
            printf("\n");

            vtest_fail_count++;
          }

          vtest_count++;
        }
      }
    }
    
    printf("\n\n");
    if ( vtest_fail_count )
    {
      printf("FAILED %d of %d\n", vtest_fail_count, vtest_count);
      getchar();
    }
    else
    {
      printf("PASSED ALL (%d)\n", vtest_count);
    }
    printf("\n");
}

void speed_vector(void) {

  __m128i* mydata = (__m128i*) _mm_malloc( 16384 * sizeof(__m128i),16);
  
  for (size_t i=0;i<1e3;i++) {
    for (size_t j=0;j< 16384;j++) {
      __m128i result = _mm_mul_ph(mydata[j],mydata[ 16384-1 - j]);
    }
  }
  
  _mm_free(mydata);
}
