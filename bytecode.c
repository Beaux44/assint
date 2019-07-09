#include <stdio.h>
#include <stdlib.h>
#include "order32.h"
#define bool  _Bool
#define true  1
#define false 0
#define SF (flags.sf)
#define ZF (flags.zf)
#define OF (flags.of)

struct {
  bool sf: 1;
  bool zf: 1;
  bool of: 1;
} flags = {0};

int main(int argc, char **argv) {
  // REGISTERS
  short *const AX = calloc(1, sizeof(short));
  short *const BX = calloc(1, sizeof(short));
  short *const CX = calloc(1, sizeof(short));
  short *const DX = calloc(1, sizeof(short));
  char  *const AH = ((char*) AX) + O32_IS_LITTLE;
  char  *const AL = ((char*) AX) + O32_IS_BIG;
  char  *const BH = ((char*) BX) + O32_IS_LITTLE;
  char  *const BL = ((char*) BX) + O32_IS_BIG;
  char  *const CH = ((char*) CX) + O32_IS_LITTLE;
  char  *const CL = ((char*) CX) + O32_IS_BIG;
  char  *const DH = ((char*) DX) + O32_IS_LITTLE;
  char  *const DL = ((char*) DX) + O32_IS_BIG;
  *AX = 0x6F3F;
  *BX = 0x6F3F;

  return 0;
}
