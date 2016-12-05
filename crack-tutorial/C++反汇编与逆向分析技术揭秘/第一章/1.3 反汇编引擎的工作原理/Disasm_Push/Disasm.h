
/*
     8888888b.                  888     888 d8b                        
     888   Y88b                 888     888 Y8P                        
     888    888                 888     888                            
     888   d88P 888d888  .d88b. Y88b   d88P 888  .d88b.  888  888  888 
     8888888P"  888P"   d88""88b Y88b d88P  888 d8P  Y8b 888  888  888 
     888        888     888  888  Y88o88P   888 88888888 888  888  888 
     888        888     Y88..88P   Y888P    888 Y8b.     Y88b 888 d88P 
     888        888      "Y88P"     Y8P     888  "Y8888   "Y8888888P"  


                        Dissasembler Engine Core
                        ~~~~~~~~~~~~~~~~~~~~~~~~

 Written by Bengaly (R) 2003-2005.
 As a part of the Proview (a.k.a PVDasm).
 Permission is granted to make and distribute verbatim copies of this
 Program provided the copyright notice and this permission notice are
 Preserved on all copies.

 File: Disasm.h (main)

 Disassembler Core Version: 1.04
*/
#pragma once

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <conio.h>

#ifndef _DISASM_H_
#define _DISASM_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Decoded
{
    // Define Decoded instruction struct

    char    Assembly[256]; // Menemonics
    char    Remarks[256];  // Menemonic addons
    char    Opcode[30];    // Opcode Byte forms
    DWORD   Address;      // Current address of decoded instruction
    BYTE    OpcodeSize;   // Opcode Size
    BYTE    PrefixSize;   // Size of all prefixes used
} DISASSEMBLY;

// Register(s) Size
#define REG8  0
#define REG16 1
#define REG32 2
#define FPU   3 // Not in use.

// 8Bit Registers
#define  REG_AL 0
#define  REG_CL 1
#define  REG_DL 2
#define  REG_BL 3
#define  REG_AH 4
#define  REG_CH 5
#define  REG_DH 6
#define  REG_BH 7

// 16Bit Registers
#define  REG_AX 0
#define  REG_CX 1
#define  REG_DX 2
#define  REG_BX 3
#define  REG_SP 4
#define  REG_BP 5
#define  REG_SI 6
#define  REG_DI 7

// 32bit Registers
#define  REG_EAX 0 
#define  REG_ECX 1
#define  REG_EDX 2
#define  REG_EBX 3
#define  REG_ESP 4
#define  REG_EBP 5
#define  REG_ESI 6
#define  REG_EDI 7

// Segments
#define SEG_ES 0
#define SEG_CS 1
#define SEG_SS 2
#define SEG_DS 3
#define SEG_FS 4
#define SEG_GS 5

// Sib extention
#define SIB_EX 4 
// Disasm Fucntions

void    Mod_RM_SIB(DISASSEMBLY **Disasm,
                   char **Opcode,
                   int pos,
                   bool AddrPrefix,
                   int SEG,
                   DWORD **index,
                   BYTE Bit_d,
                   BYTE Bit_w,
                   char *instruction,
                   BYTE Op,
                   bool PrefixReg,
                   bool PrefixSeg,
                   bool PrefixAddr);

void    Mod_11_RM(BYTE d,
                  BYTE w,
                  char **Opcode,
                  DISASSEMBLY **Disasm,
                  char instruction[],
                  bool PrefixReg,
                  BYTE Op,
                  DWORD **index);


// NEW SET OF INSTRUCTIONS FUNCTIONS

void    Mod_11_RM_EX(BYTE d,
                     BYTE w,
                     char **Opcode,
                     DISASSEMBLY **Disasm,
                     bool PrefixReg,
                     BYTE Op,
                     DWORD **index,
                     bool RepPrefix);

void    Mod_RM_SIB_EX(DISASSEMBLY **Disasm,
                      char **Opcode,
                      int pos,
                      bool AddrPrefix,
                      int SEG,
                      DWORD **index,
                      BYTE Op,
                      bool PrefixReg,
                      bool PrefixSeg,
                      bool PrefixAddr,
                      BYTE Bit_d,
                      BYTE Bit_w,
                      bool RepPrefix);

void    GetInstruction(BYTE Opcode,
                       char *menemonic);
void    GetJumpInstruction(BYTE Opcode,
                           char *menemonic);
DWORD   StringToDword(char *Text);
WORD    StringToWord(char *Text);
void    Decode(DISASSEMBLY *Disasm,
               char *Opcode,
               DWORD *Index);
void    ShowDecoded(DISASSEMBLY Disasm);
void    FlushDecoded(DISASSEMBLY *Disasm);
int     GetNewInstruction(BYTE Op,
                          char *ASM,
                          bool RegPrefix);
void    SwapDword(BYTE *MemPtr,
                  DWORD *Original,
                  DWORD *Mirrored);
void    SwapWord(BYTE *MemPtr,
                 WORD *Original,
                 WORD *Mirrored);

#ifdef __cplusplus
}
#endif
#endif