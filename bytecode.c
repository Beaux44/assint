#include <stdio.h>
#include <stdlib.h>
#include "order32.h"
#define bool  _Bool
#define true  1
#define false 0
#define SF (flags.sf)
#define ZF (flags.zf)
#define OF (flags.of)
#define CF (flags.of)
#define EQ() (ZF)
#define NE() (!ZF)
#define LT() ((SF ^ OF) && NE())
#define LE() ((SF ^ OF) || EQ())
#define GT() (!(SF ^ OF) && NE())
#define GE() (!(SF ^ OF) || EQ())
#define AB() (!(CF || EQ()))
#define AE() (!CF && EQ())
#define BL() (CF)
#define BE() (CF || EQ())

void test(unsigned *, unsigned *);
void cmp(unsigned *, unsigned *);
struct {
  bool sf: 1;
  bool zf: 1;
  bool of: 1;
} flags = {0};

int main(int argc, char **argv) {
  // REGISTERS
  unsigned *const REGS = calloc(9, sizeof(unsigned));
  if(!REGS) {
    puts("wtf");
    return -1;
  }

  unsigned *const EAX = REGS + 0,   // Accumulator register
           *const EBX = REGS + 1,   // Base register
           *const ECX = REGS + 2,   // Counter register
           *const EDX = REGS + 3,   // Data register
           *const ESP = REGS + 4,   // Stack pointer
           *const EBP = REGS + 5,   // Base pointer
           *const EIP = REGS + 6,   // Instruction pointer
           *const EDI = REGS + 7,   // Destination Index
           *const ESI = REGS + 8;   // Source Index

  /*
    This may be excessively long winded for what it's attempting to get across, of which I'm unsure.

    x86 processors have two 8-bit registers corresponding to the high and low bits of a 16-bit register
    that is the least significant 16-bits of a 32-bit register for all the integer registers.

    O32_IS_LITTLE and O32_IS_BIG are macros that determine whether the host machine is little or big
    endian; as endianness is an important factor since the high registers must be the high bits, and the
    low registers must be the low bits.
  */
  unsigned short *const AX = ((unsigned short*) EAX) + O32_IS_BIG,
                 *const BX = ((unsigned short*) EBX) + O32_IS_BIG,
                 *const CX = ((unsigned short*) ECX) + O32_IS_BIG,
                 *const DX = ((unsigned short*) EDX) + O32_IS_BIG,
                 *const SP = ((unsigned short*) ESP) + O32_IS_BIG,
                 *const BP = ((unsigned short*) EBP) + O32_IS_BIG,
                 *const IP = ((unsigned short*) EIP) + O32_IS_BIG,
                 *const DI = ((unsigned short*) EDI) + O32_IS_BIG,
                 *const SI = ((unsigned short*) ESI) + O32_IS_BIG;

  unsigned char *const AH = ((unsigned char*) AX) + O32_IS_LITTLE,
                *const AL = ((unsigned char*) AX) + O32_IS_BIG,
                *const BH = ((unsigned char*) BX) + O32_IS_LITTLE,
                *const BL = ((unsigned char*) BX) + O32_IS_BIG,
                *const CH = ((unsigned char*) CX) + O32_IS_LITTLE,
                *const CL = ((unsigned char*) CX) + O32_IS_BIG,
                *const DH = ((unsigned char*) DX) + O32_IS_LITTLE,
                *const DL = ((unsigned char*) DX) + O32_IS_BIG;

  // Note that the content of the code segment is intentionally not const, as there were no
  // restrictions employed by the hardware on a program that disallowed it from editing its
  // own code, and that it's a char* rather than a short* so that it has single byte granularity
  char  *const CS = malloc(1024*64 * sizeof(short)); // Code segment
  short *const SS = malloc(1024*64 * sizeof(short)), // Stack segment
        *const DS = malloc(1024*64 * sizeof(short)), // Data segment
        *const ES = malloc(1024*64 * sizeof(short)); // Extra segment

  // Read code into CS
  if(argc != 2) {
    printf("Takes exactly 1 argument, got %d.\n", argc - 1);
    return -1;
  }

  char *fileName = NULL;
  if(argc > 1) {
    fileName = argv[1];
    long length;
    FILE *fp = fopen(fileName, "rb");

    if(fp) {
      fseek(fp, 0, SEEK_END);
      length = ftell(fp);
      if(length) {
        fread(CS, 1, length, fp);
      }
      fclose(fp);
    }
  }

  return 0;
}


void test(unsigned *arg1, unsigned *arg0) {
  unsigned commonBits = *arg1 & *arg0;
  SF = commonBits & 0x80000000;
  ZF = !commonBits;
  OF = false;
}

void cmp(unsigned *minuend, unsigned *subtrahend) {
  unsigned a = *minuend,
           b = *subtrahend,
           difference = a - b;
  SF = difference & 0x80000000;
  ZF = !difference;
  OF = (a > 0 && b > SHRT_MAX - a) || (a < 0 && b < SHRT_MIN - a);
}
