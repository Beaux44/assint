#include <stdio.h>
#include <stdlib.h>
#include "order32.h"
#define bool  _Bool
#define true  1
#define false 0
#define SF (flags.sf)
#define ZF (flags.zf)
#define OF (flags.of)
#define EQ() (ZF)
#define NE() (!ZF)
#define LT() ((SF ^ OF) && NE())
#define LE() ((SF ^ OF) || EQ())
#define GT() (!(SF ^ OF) && NE())
#define GE() (!(SF ^ OF) || EQ())

void test(short *, short *);
void cmp(short *, short *);
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
  cmp(AX, BX);
  printf("AX:%X,BX:%X\n", *AX, *BX);
  printf("SF:%d, ZF:%d, OF:%d\n", SF, ZF, OF);
  printf("GT:%d\nGE:%d\nLT:%d\nLE:%d\nEQ:%d\nNE:%d\n", GT(), GE(), LT(), LE(), EQ(), NE());

  return 0;
}

void test(short *arg1, short *arg0) {
  short commonBits = *arg1 & *arg0;
  SF = commonBits & 0x8000;
  ZF = !commonBits;
  OF = false;
}

void cmp(short *minuend, short *subtrahend) {
  short a = *minuend,
        b = *subtrahend,
        difference = a - b;
  SF = difference & 0x8000;
  ZF = !difference;
  OF = (a > 0 && b > SHRT_MAX - a) || (a < 0 && b < SHRT_MIN - a);
}
