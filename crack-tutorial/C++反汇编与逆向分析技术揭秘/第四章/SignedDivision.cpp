#include <iostream.h>
#include <malloc.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <stdlib.h>   

#define for if(0){}else for

// 对于除数在3到13之间，直接查表，表结构如下
struct SignedMagicNumber
{
  int nMagic; 
  int nExpInc;
};

// 对于除数为2的幂，有其他处理，故表内无对应值
struct SignedMagicNumber MagicTable[] = {
  {1, 1},           // 0 
  {1, 1},           // 1
  {1, 1},           // 2
  {0x55555556, 0},
  {0, 0},           // 4
  {0x66666667, 1},
  {0x2AAAAAAB, 0},
  {0x92492493, 2},
  {0, 0},           // 8
  {0x38E38E39, 1},
  {0x66666667, 2},
  {0x2E8BA2E9, 1},
  {0x2AAAAAAB, 1}
};


#define EXP31 0x80000000

// 以下代码还原修改自VC++6.0 bin目录下c2.dll(版本12.0.9782.0)，文件偏移5FACE，
// 原程序的返回值定义为结构体，这里修改为参数返回
int GetMagic(unsigned int nDivC, int *nOutExpInc)
{
//   if ((int)nDivC >= 3 && nDivC < 13)
//   {
//     *nOutExpInc = MagicTable[nDivC].nExpInc;
//     return MagicTable[nDivC].nMagic;
//   }

  unsigned int nAbsDivC = abs(nDivC);
  int nExcBase = 31;

  // t = 2^31 if nDivC > 0
  // or t = 2^31 + 1 if nDivC < 0
  unsigned int t = (nDivC >> 31) + EXP31;

  // |nc| = t - 1 - rem(t, |nDivC|)
  unsigned int nLargestMultiple  = t - t % nAbsDivC - 1;
  unsigned int q1 = EXP31 / nLargestMultiple;
  unsigned int r1 = EXP31 - nLargestMultiple * q1;
  unsigned int nMagicNumber = EXP31 / nAbsDivC;
  unsigned int r2 = EXP31 - nAbsDivC * nMagicNumber;

  do
  {
    r1 *= 2;
    q1 *= 2;
    ++nExcBase;
    if ( r1 >= nLargestMultiple )
    {
      ++q1;
      r1 -= nLargestMultiple;
    }
    r2 *= 2;
    nMagicNumber *= 2;
    if ( r2 >= nAbsDivC )
    {
      ++nMagicNumber;
      r2 -= nAbsDivC;
    }
  }
  while ( q1 < nAbsDivC - r2 || q1 == nAbsDivC - r2 && !r1 );

  nMagicNumber++;

  if ( (int)nDivC < 0 )
    nMagicNumber = -(int)nMagicNumber;

  *nOutExpInc = nExcBase - 32;

  return nMagicNumber;
}

int main(int argc)
{
  int nExpInc;
  int nMagicNumber;


  int nDividend = argc-201; // 这是被除数
  int nDivisor = -100;      // 这是除数
  int nQuotient;            // 这里存放商

  // GetMagic用来计算magic number，
  // 第一个参数指定除数，第二个参数OUT指数相对32的增量
  // 这个例子用来模拟计算70 / -7的结果
  do 
  {
    nMagicNumber = GetMagic(nDivisor, &nExpInc);
    printf("nMagicNumber = 0x%08x, ExpInc = %d\r\n", nMagicNumber, nExpInc);

    if (nDivisor >= 0)
    {
      __asm
      {
        mov eax, nMagicNumber // 编译器会做成imm寻址，nMagicNumber早已在编译期间算出
        mov esi, nDividend
        imul esi

        // 编译器不会产生这里的跳转，
        // 因为编译阶段就计算出nMagicNumber的取值了，
        // 所以编译期间就可以决定是否产生其后的add指令,
        // nMagicNumber小于0x80000000(负数)则不需增加add
        test nMagicNumber, 80000000h
        jz NEXT1
        add edx, esi
NEXT1:
        mov ecx, nExpInc
        sar edx, cl
        shr esi, 31
        add edx, esi
        mov nQuotient, edx
      }
    }
    else
    {
      __asm
      {
        mov eax, nMagicNumber
        mov esi, nDividend
        imul esi

        test nMagicNumber, 80000000h
        jnz NEXT2
        sub edx, esi
NEXT2:
        mov ecx, nExpInc
        sar edx, cl
        mov ecx, edx
        shr ecx, 31
        add edx, ecx
        mov nQuotient, edx
      }
    }
    
    printf("%d / %d = %d\r\n", nDividend, nDivisor, nQuotient);
    printf("%d / %d = %d\r\n", nDividend, nDivisor, nDividend / nDivisor);
    if (nQuotient != nDividend / nDivisor)
    {
      puts("Error");
      break;
    }
    
    nDivisor++;
    if (nDivisor == 0 || nDivisor == -1 || nDivisor == 1)
    {
      nDivisor = 2;
    }
    nDividend += 10;
  }
  while(nDivisor <= 100);
  
  return 0;
}