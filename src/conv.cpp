#include <stdint.h>
#include <stdio.h>
#include "half.h"

int main(int argc, char* argv)
{
  float full = 1000.25f;
  printf("input: %f\n", full);
  uint32_t nFull = *reinterpret_cast<uint32_t*>(&full);
  uint16_t nHalf = half_from_float(nFull);
  printf("half: %u\n", nHalf);
  uint32_t nFullTest = half_to_float(nHalf);
  printf("full: %lu\n", nFullTest);
  float fullTest = *reinterpret_cast<float*>(&nFullTest);
  printf("full: %f\n", fullTest);
  return 0;
}
