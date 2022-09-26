//$Id: main.cpp,v 1.13 2009/03/12 19:03:18 jmh Exp $
#include <stdio.h>

#ifdef __GNUC__
# include <stdint.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include "test_float.h"
#include "float_test_vector.h"

#ifdef _REFERENCE
# include "half_test.h"
# include "half.h"
#endif

#include "half_vector_test.h"

#include "mytime.h"

#include <malloc.h>

#ifdef __GNUC__
 #include "mm_malloc.h"
#endif

int main( void ) {

  double start, stop;
  
  size_t nIter = 1000;
  size_t nData = 65536;

#ifdef _REFERENCE
  test_conversion();
  test_add();
  test_sub();
  test_mul();
#endif
  
  test_conversion_vector();
  test_add_vector();
  test_sub_vector();
  test_mul_vector();

#ifdef _REFERENCE
  printf("Half-precision float multiplications\n");

  start = mytime();

  uint16_t* mydata = (uint16_t*) _mm_malloc(65536*sizeof(uint16_t),16);

  for (size_t i=0;i<nIter;i++) {
    for (size_t j=0;j< nData;j++) {
      uint16_t result = half_mul(mydata[j],mydata[65535-j]);
    }
  }

  _mm_free(mydata);

  stop = mytime() - start;
  
  printf("%d multiplications done in %g seconds\n\n", nIter*nData, stop);

#endif

  // Floats
  printf("Single-precision float multiplications\n");

  start = mytime();

  float* myfloatdata = (float*) _mm_malloc(65536*sizeof(float),16);
  float result = 0.0f;

  for (size_t i=0;i<nIter;i++) {
    for (size_t j=0;j< nData;j++) {
      result += myfloatdata[j]*myfloatdata[65535-j];
    }
  }

  _mm_free(myfloatdata);

  stop = mytime() - start;
  
  printf("%d multiplications done in %g seconds\n\n", nIter*nData, stop);
  
  printf("Half-precision float multiplications (SSE)\n");
  
  start = mytime();

  speed_vector();

  stop = mytime() - start;

  printf("%d multiplications done in %g seconds\n\n", nIter*nData,stop);
     
  // print_table();
  return (EXIT_SUCCESS);
}
