// parse86
// Copyright Ric Vieler, 2006
// disassembler for getx86Instruction

#if _WIN32_WINNT >= 0x0500
#	define NDIS50				1
#else
#	define NDIS40				1
#endif
#define BINARY_COMPATIBLE		0

#ifdef __cplusplus
extern "C" {
#endif

#include <ndis.h>
#include <ntddk.h>
#include "ghost.h"
#include "injectManager.h"


#ifdef __cplusplus
}
#endif

#include "parse86.h"

#pragma code_seg("PAGE")
#pragma optimize( "", off )
// for X86INSTRUCTION struct
#pragma pack(1)

typedef struct _X86_16BIT_INSTRUCTION
{
	BOOL operandIs16;
	BOOL addressIs16;
	PBYTE* jumpAddress;
	LONG* extra;

} X86_16BIT_INSTRUCTION;

// forward declaration for XFER_FUNCTION
struct _X86INSTRUCTION;
typedef struct _X86INSTRUCTION* PX86INSTRUCTION;

typedef PBYTE (*XFER_FUNCTION)(X86_16BIT_INSTRUCTION* op16Ptr, PX86INSTRUCTION opPtr, PBYTE destination, PBYTE source);

typedef struct _X86INSTRUCTION
{
	ULONG 			opcode 		: 8;
	ULONG			size 		: 3;
	ULONG			size16		: 3;
	ULONG			modeOffset	: 3;
	LONG			relOffset 	: 3;
	ULONG			flagMask	: 4;
	XFER_FUNCTION	pXferFunction;

} X86INSTRUCTION;

// flags for flagMask
enum
{
	DYNAMIC_FLAG 	= 0x1u,
	ADDRESS_FLAG 	= 0x2u,
	NOENLARGE_FLAG	= 0x4u,
	SIB_FLAG		= 0x10u,
	NOTSIB_FLAG		= 0x0fu,
};

#pragma pack()

BYTE regMemMode[256] =
{
	0,0,0,0,0x11,4,0,0,0,0,0,0,0x11,4,0,0,	// 00 - 0f
	0,0,0,0,0x11,4,0,0,0,0,0,0,0x11,4,0,0,	// 10 - 1f
	0,0,0,0,0x11,4,0,0,0,0,0,0,0x11,4,0,0,	// 20 - 2f
	0,0,0,0,0x11,4,0,0,0,0,0,0,0x11,4,0,0,	// 30 - 3f
	1,1,1,1,2,1,1,1,1,1,1,1,2,1,1,1,		// 40 - 4f
	1,1,1,1,2,1,1,1,1,1,1,1,2,1,1,1,		// 50 - 5f
	1,1,1,1,2,1,1,1,1,1,1,1,2,1,1,1,		// 60 - 6f
	1,1,1,1,2,1,1,1,1,1,1,1,2,1,1,1,		// 70 - 7f
	4,4,4,4,5,4,4,4,4,4,4,4,5,4,4,4,		// 80 - 8f
	4,4,4,4,5,4,4,4,4,4,4,4,5,4,4,4,		// 90 - 9f
	4,4,4,4,5,4,4,4,4,4,4,4,5,4,4,4,		// a0 - af
	4,4,4,4,5,4,4,4,4,4,4,4,5,4,4,4,		// b0 - bf
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// c0 - cf
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// d0 - df
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// e0 - ef
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0			// f0 - ff
};

// prototypes for X86INSTRUCTION
PBYTE transferData( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source );
PBYTE transferDataPrefix( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source );
PBYTE adjustData( X86_16BIT_INSTRUCTION* op16Ptr, PBYTE destination, PBYTE source, LONG bytes, LONG targetOffset );
PBYTE noTransferOp( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source );
PBYTE transferOp0F( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source );
PBYTE transferOp66( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source );
PBYTE transferOp67( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source );
PBYTE transferOpF6( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source );
PBYTE transferOpF7( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source );
PBYTE transferOpFF( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source );

// follows opcode in X86INSTRUCTION
#define transfer1			1, 1, 0, 0, 0, transferData
#define transfer1Dynamic	1, 1, 0, 0, DYNAMIC_FLAG, transferData
#define transfer2			2, 2, 0, 0, 0, transferData
#define transfer2Jump		2, 2, 0, 1, 0, transferData
#define transfer2NoJump		2, 2, 0, 1, NOENLARGE_FLAG, transferData
#define transfer2Dynamic	2, 2, 0, 0, DYNAMIC_FLAG, transferData
#define transfer3			3, 3, 0, 0, 0, transferData
#define transfer3Dynamic	3, 3, 0, 0, DYNAMIC_FLAG, transferData
#define transfer3Or5		5, 3, 0, 0, 0, transferData
#define transfer3Or5Target	5, 3, 0, 1, 0, transferData
#define transfer5Or7Dynamic	7, 5, 0, 0, DYNAMIC_FLAG, transferData
#define transfer3Or5Address	5, 3, 0, 0, ADDRESS_FLAG, transferData
#define transfer4			4, 4, 0, 0, 0, transferData
#define transfer5			5, 5, 0, 0, 0, transferData
#define transfer7			7, 7, 0, 0, 0, transferData
#define transfer2Mod		2, 2, 1, 0, 0, transferData
#define transfer2Mod1		3, 3, 1, 0, 0, transferData
#define transfer2ModOperand	6, 4, 1, 0, 0, transferData
#define transfer3Mod		3, 3, 2, 0, 0, transferData
#define transferPrefix		1, 1, 0, 0, 0, transferDataPrefix
#define transfer0F			1, 1, 0, 0, 0, transferOp0F
#define transfer66			1, 1, 0, 0, 0, transferOp66
#define transfer67			1, 1, 0, 0, 0, transferOp67
#define transferF6			0, 0, 0, 0, 0, transferOpF6
#define transferF7			0, 0, 0, 0, 0, transferOpF7
#define transferFF			0, 0, 0, 0, 0, transferOpFF
#define noTransfer			1, 1, 0, 0, 0, noTransferOp
#define lastEntry			0, 0, 0, 0, 0, NULL

// intel op codes and disassembly parameters
X86INSTRUCTION instructionMap[257] =
{ 
	{ 0x00, transfer2Mod },
	{ 0x01, transfer2Mod },
	{ 0x02, transfer2Mod },
	{ 0x03, transfer2Mod },
	{ 0x04, transfer2 },
	{ 0x05, transfer3Or5 },
	{ 0x06, transfer1 },
	{ 0x07, transfer1 },
	{ 0x08, transfer2Mod },
	{ 0x09, transfer2Mod },
	{ 0x0A, transfer2Mod },
	{ 0x0B, transfer2Mod },
	{ 0x0C, transfer2 },
	{ 0x0D, transfer3Or5 },
	{ 0x0E, transfer1 },
	{ 0x0F, transfer0F },
	{ 0x10, transfer2Mod },
	{ 0x11, transfer2Mod },
	{ 0x12, transfer2Mod },
	{ 0x13, transfer2Mod },
	{ 0x14, transfer2 },
	{ 0x15, transfer3Or5 },
	{ 0x16, transfer1 },
	{ 0x17, transfer1 },
	{ 0x18, transfer2Mod },
	{ 0x19, transfer2Mod },
	{ 0x1A, transfer2Mod },
	{ 0x1B, transfer2Mod },
	{ 0x1C, transfer2 },
	{ 0x1D, transfer3Or5 },
	{ 0x1E, transfer1 },
	{ 0x1F, transfer1 },
	{ 0x20, transfer2Mod },
	{ 0x21, transfer2Mod },
	{ 0x22, transfer2Mod },
	{ 0x23, transfer2Mod },
	{ 0x24, transfer2 },
	{ 0x25, transfer3Or5 },
	{ 0x26, transferPrefix },
	{ 0x27, transfer1 },
	{ 0x28, transfer2Mod },
	{ 0x29, transfer2Mod },
	{ 0x2A, transfer2Mod },
	{ 0x2B, transfer2Mod },
	{ 0x2C, transfer2 },
	{ 0x2D, transfer3Or5 },
	{ 0x2E, transferPrefix },
	{ 0x2F, transfer1 },
	{ 0x30, transfer2Mod },
	{ 0x31, transfer2Mod },
	{ 0x32, transfer2Mod },
	{ 0x33, transfer2Mod },
	{ 0x34, transfer2 },
	{ 0x35, transfer3Or5 },
	{ 0x36, transferPrefix },
	{ 0x37, transfer1 },
	{ 0x38, transfer2Mod },
	{ 0x39, transfer2Mod },
	{ 0x3A, transfer2Mod },
	{ 0x3B, transfer2Mod },
	{ 0x3C, transfer2 },
	{ 0x3D, transfer3Or5 },
	{ 0x3E, transferPrefix },
	{ 0x3F, transfer1 },
	{ 0x40, transfer1 },
	{ 0x41, transfer1 },
	{ 0x42, transfer1 },
	{ 0x43, transfer1 },
	{ 0x44, transfer1 },
	{ 0x45, transfer1 },
	{ 0x46, transfer1 },
	{ 0x47, transfer1 },
	{ 0x48, transfer1 },
	{ 0x49, transfer1 },
	{ 0x4A, transfer1 },
	{ 0x4B, transfer1 },
	{ 0x4C, transfer1 },
	{ 0x4D, transfer1 },
	{ 0x4E, transfer1 },
	{ 0x4F, transfer1 },
	{ 0x50, transfer1 },
	{ 0x51, transfer1 },
	{ 0x52, transfer1 },
	{ 0x53, transfer1 },
	{ 0x54, transfer1 },
	{ 0x55, transfer1 },
	{ 0x56, transfer1 },
	{ 0x57, transfer1 },
	{ 0x58, transfer1 },
	{ 0x59, transfer1 },
	{ 0x5A, transfer1 },
	{ 0x5B, transfer1 },
	{ 0x5C, transfer1 },
	{ 0x5D, transfer1 },
	{ 0x5E, transfer1 },
	{ 0x5F, transfer1 },
	{ 0x60, transfer1 },
	{ 0x61, transfer1 },
	{ 0x62, transfer2Mod },
	{ 0x63, transfer2Mod },
	{ 0x64, transferPrefix },
	{ 0x65, transferPrefix },
	{ 0x66, transfer66 },
	{ 0x67, transfer67 },
	{ 0x68, transfer3Or5 },
	{ 0x69, transfer2ModOperand },
	{ 0x6A, transfer2 },
	{ 0x6B, transfer2Mod1 },
	{ 0x6C, transfer1 },
	{ 0x6D, transfer1 },
	{ 0x6E, transfer1 },
	{ 0x6F, transfer1 },
	{ 0x70, transfer2Jump },
	{ 0x71, transfer2Jump },
	{ 0x72, transfer2Jump },
	{ 0x73, transfer2Jump },
	{ 0x74, transfer2Jump },
	{ 0x75, transfer2Jump },
	{ 0x76, transfer2Jump },
	{ 0x77, transfer2Jump },
	{ 0x78, transfer2Jump },
	{ 0x79, transfer2Jump },
	{ 0x7A, transfer2Jump },
	{ 0x7B, transfer2Jump },
	{ 0x7C, transfer2Jump },
	{ 0x7D, transfer2Jump },
	{ 0x7E, transfer2Jump },
	{ 0x7F, transfer2Jump },
	{ 0x80, transfer2Mod1 },
	{ 0x81, transfer2ModOperand },
	{ 0x82, transfer2 },
	{ 0x83, transfer2Mod1 },
	{ 0x84, transfer2Mod },
	{ 0x85, transfer2Mod },
	{ 0x86, transfer2Mod },
	{ 0x87, transfer2Mod },
	{ 0x88, transfer2Mod },
	{ 0x89, transfer2Mod },
	{ 0x8A, transfer2Mod },
	{ 0x8B, transfer2Mod },
	{ 0x8C, transfer2Mod },
	{ 0x8D, transfer2Mod },
	{ 0x8E, transfer2Mod },
	{ 0x8F, transfer2Mod },
	{ 0x90, transfer1 },
	{ 0x91, transfer1 },
	{ 0x92, transfer1 },
	{ 0x93, transfer1 },
	{ 0x94, transfer1 },
	{ 0x95, transfer1 },
	{ 0x96, transfer1 },
	{ 0x97, transfer1 },
	{ 0x98, transfer1 },
	{ 0x99, transfer1 },
	{ 0x9A, transfer5Or7Dynamic },
	{ 0x9B, transfer1 },
	{ 0x9C, transfer1 },
	{ 0x9D, transfer1 },
	{ 0x9E, transfer1 },
	{ 0x9F, transfer1 },
	{ 0xA0, transfer3Or5Address },
	{ 0xA1, transfer3Or5Address },
	{ 0xA2, transfer3Or5Address },
	{ 0xA3, transfer3Or5Address },
	{ 0xA4, transfer1 },
	{ 0xA5, transfer1 },
	{ 0xA6, transfer1 },
	{ 0xA7, transfer1 },
	{ 0xA8, transfer2 },
	{ 0xA9, transfer3Or5 },
	{ 0xAA, transfer1 },
	{ 0xAB, transfer1 },
	{ 0xAC, transfer1 },
	{ 0xAD, transfer1 },
	{ 0xAE, transfer1 },
	{ 0xAF, transfer1 },
	{ 0xB0, transfer2 },
	{ 0xB1, transfer2 },
	{ 0xB2, transfer2 },
	{ 0xB3, transfer2 },
	{ 0xB4, transfer2 },
	{ 0xB5, transfer2 },
	{ 0xB6, transfer2 },
	{ 0xB7, transfer2 },
	{ 0xB8, transfer3Or5 },
	{ 0xB9, transfer3Or5 },
	{ 0xBA, transfer3Or5 },
	{ 0xBB, transfer3Or5 },
	{ 0xBC, transfer3Or5 },
	{ 0xBD, transfer3Or5 },
	{ 0xBE, transfer3Or5 },
	{ 0xBF, transfer3Or5 },
	{ 0xC0, transfer2Mod1 },
	{ 0xC1, transfer2Mod1 },
	{ 0xC2, transfer3 },
	{ 0xC3, transfer1 },
	{ 0xC4, transfer2Mod },
	{ 0xC5, transfer2Mod },
	{ 0xC6, transfer2Mod1 },
	{ 0xC7, transfer2ModOperand },
	{ 0xC8, transfer4 },
	{ 0xC9, transfer1 },
	{ 0xCA, transfer3Dynamic },
	{ 0xCB, transfer1Dynamic },
	{ 0xCC, transfer1Dynamic },
	{ 0xCD, transfer2Dynamic },
	{ 0xCE, transfer1Dynamic },
	{ 0xCF, transfer1Dynamic },
	{ 0xD0, transfer2Mod },
	{ 0xD1, transfer2Mod },
	{ 0xD2, transfer2Mod },
	{ 0xD3, transfer2Mod },
	{ 0xD4, transfer2 },
	{ 0xD5, transfer2 },
	{ 0xD6, noTransfer },
	{ 0xD7, transfer1 },
	{ 0xD8, transfer2Mod },
	{ 0xD9, transfer2Mod },
	{ 0xDA, transfer2Mod },
	{ 0xDB, transfer2Mod },
	{ 0xDC, transfer2Mod },
	{ 0xDD, transfer2Mod },
	{ 0xDE, transfer2Mod },
	{ 0xDF, transfer2Mod },
	{ 0xE0, transfer2NoJump },
	{ 0xE1, transfer2NoJump },
	{ 0xE2, transfer2NoJump },
	{ 0xE3, transfer2Jump },
	{ 0xE4, transfer2 },
	{ 0xE5, transfer2 },
	{ 0xE6, transfer2 },
	{ 0xE7, transfer2 },
	{ 0xE8, transfer3Or5Target },
	{ 0xE9, transfer3Or5Target },
	{ 0xEA, transfer5Or7Dynamic },
	{ 0xEB, transfer2Jump },
	{ 0xEC, transfer1 },
	{ 0xED, transfer1 },
	{ 0xEE, transfer1 },
	{ 0xEF, transfer1 },
	{ 0xF0, transferPrefix },
	{ 0xF1, noTransfer },
	{ 0xF2, transferPrefix },
	{ 0xF3, transferPrefix },
	{ 0xF4, transfer1 },
	{ 0xF5, transfer1 },
	{ 0xF6, transferF6 },
	{ 0xF7, transferF7 },
	{ 0xF8, transfer1 },
	{ 0xF9, transfer1 },
	{ 0xFA, transfer1 },
	{ 0xFB, transfer1 },
	{ 0xFC, transfer1 },
	{ 0xFD, transfer1 },
	{ 0xFE, transfer2Mod },
	{ 0xFF, transferFF },
	{ 0x00, lastEntry }
};

// intel extended op codes and disassembly parameters
X86INSTRUCTION extendedInstructionMap[257] =
{
	{ 0x00, transfer2Mod },
	{ 0x01, transfer2Mod },
	{ 0x02, transfer2Mod },
	{ 0x03, transfer2Mod },
	{ 0x04, noTransfer },
	{ 0x05, noTransfer },
	{ 0x06, transfer2 },
	{ 0x07, noTransfer },
	{ 0x08, transfer2 },
	{ 0x09, transfer2 },
	{ 0x0A, noTransfer },
	{ 0x0B, transfer2 },
	{ 0x0C, noTransfer },
	{ 0x0D, noTransfer },
	{ 0x0E, noTransfer },
	{ 0x0F, noTransfer },
	{ 0x10, noTransfer },
	{ 0x11, noTransfer },
	{ 0x12, noTransfer },
	{ 0x13, noTransfer },
	{ 0x14, noTransfer },
	{ 0x15, noTransfer },
	{ 0x16, noTransfer },
	{ 0x17, noTransfer },
	{ 0x18, noTransfer },
	{ 0x19, noTransfer },
	{ 0x1A, noTransfer },
	{ 0x1B, noTransfer },
	{ 0x1C, noTransfer },
	{ 0x1D, noTransfer },
	{ 0x1E, noTransfer },
	{ 0x1F, noTransfer },
	{ 0x20, transfer2Mod },
	{ 0x21, transfer2Mod },
	{ 0x22, transfer2Mod },
	{ 0x23, transfer2Mod },
	{ 0x24, noTransfer },
	{ 0x25, noTransfer },
	{ 0x26, noTransfer },
	{ 0x27, noTransfer },
	{ 0x28, noTransfer },
	{ 0x29, noTransfer },
	{ 0x2A, noTransfer },
	{ 0x2B, noTransfer },
	{ 0x2C, noTransfer },
	{ 0x2D, noTransfer },
	{ 0x2E, noTransfer },
	{ 0x2F, noTransfer },
	{ 0x30, transfer2 },
	{ 0x31, transfer2 },
	{ 0x32, transfer2 },
	{ 0x33, transfer2 },
	{ 0x34, transfer2 },
	{ 0x35, transfer2 },
	{ 0x36, noTransfer },
	{ 0x37, noTransfer },
	{ 0x38, noTransfer },
	{ 0x39, noTransfer },
	{ 0x3A, noTransfer },
	{ 0x3B, noTransfer },
	{ 0x3C, noTransfer },
	{ 0x3D, noTransfer },
	{ 0x3E, noTransfer },
	{ 0x3F, noTransfer },
	{ 0x40, transfer2Mod },
	{ 0x41, transfer2Mod },
	{ 0x42, transfer2Mod },
	{ 0x43, transfer2Mod },
	{ 0x44, transfer2Mod },
	{ 0x45, transfer2Mod },
	{ 0x46, transfer2Mod },
	{ 0x47, transfer2Mod },
	{ 0x48, transfer2Mod },
	{ 0x49, transfer2Mod },
	{ 0x4A, transfer2Mod },
	{ 0x4B, transfer2Mod },
	{ 0x4C, transfer2Mod },
	{ 0x4D, transfer2Mod },
	{ 0x4E, transfer2Mod },
	{ 0x4F, transfer2Mod },
	{ 0x50, noTransfer },
	{ 0x51, noTransfer },
	{ 0x52, noTransfer },
	{ 0x53, noTransfer },
	{ 0x54, noTransfer },
	{ 0x55, noTransfer },
	{ 0x56, noTransfer },
	{ 0x57, noTransfer },
	{ 0x58, noTransfer },
	{ 0x59, noTransfer },
	{ 0x5A, noTransfer },
	{ 0x5B, noTransfer },
	{ 0x5C, noTransfer },
	{ 0x5D, noTransfer },
	{ 0x5E, noTransfer },
	{ 0x5F, noTransfer },
	{ 0x60, transfer2Mod },
	{ 0x61, noTransfer },
	{ 0x62, transfer2Mod },
	{ 0x63, transfer2Mod },
	{ 0x64, transfer2Mod },
	{ 0x65, transfer2Mod },
	{ 0x66, transfer2Mod },
	{ 0x67, transfer2Mod },
	{ 0x68, transfer2Mod },
	{ 0x69, transfer2Mod },
	{ 0x6A, transfer2Mod },
	{ 0x6B, transfer2Mod },
	{ 0x6C, noTransfer },
	{ 0x6D, noTransfer },
	{ 0x6E, transfer2Mod },
	{ 0x6F, transfer2Mod },
	{ 0x70, noTransfer },
	{ 0x71, transfer2Mod1 },
	{ 0x72, transfer2Mod1 },
	{ 0x73, transfer2Mod1 },
	{ 0x74, transfer2Mod },
	{ 0x75, transfer2Mod },
	{ 0x76, transfer2Mod },
	{ 0x77, transfer2 },
	{ 0x78, noTransfer },
	{ 0x79, noTransfer },
	{ 0x7A, noTransfer },
	{ 0x7B, noTransfer },
	{ 0x7C, noTransfer },
	{ 0x7D, noTransfer },
	{ 0x7E, transfer2Mod },
	{ 0x7F, transfer2Mod },
	{ 0x80, transfer3Or5Target },
	{ 0x81, transfer3Or5Target },
	{ 0x82, transfer3Or5Target },
	{ 0x83, transfer3Or5Target },
	{ 0x84, transfer3Or5Target },
	{ 0x85, transfer3Or5Target },
	{ 0x86, transfer3Or5Target },
	{ 0x87, transfer3Or5Target },
	{ 0x88, transfer3Or5Target },
	{ 0x89, transfer3Or5Target },
	{ 0x8A, transfer3Or5Target },
	{ 0x8B, transfer3Or5Target },
	{ 0x8C, transfer3Or5Target },
	{ 0x8D, transfer3Or5Target },
	{ 0x8E, transfer3Or5Target },
	{ 0x8F, transfer3Or5Target },
	{ 0x90, transfer2Mod },
	{ 0x91, transfer2Mod },
	{ 0x92, transfer2Mod },
	{ 0x93, transfer2Mod },
	{ 0x94, transfer2Mod },
	{ 0x95, transfer2Mod },
	{ 0x96, transfer2Mod },
	{ 0x97, transfer2Mod },
	{ 0x98, transfer2Mod },
	{ 0x99, transfer2Mod },
	{ 0x9A, transfer2Mod },
	{ 0x9B, transfer2Mod },
	{ 0x9C, transfer2Mod },
	{ 0x9D, transfer2Mod },
	{ 0x9E, transfer2Mod },
	{ 0x9F, transfer2Mod },
	{ 0xA0, transfer2 },
	{ 0xA1, transfer2 },
	{ 0xA2, transfer2 },
	{ 0xA3, transfer2Mod },
	{ 0xA4, transfer2Mod1 },
	{ 0xA5, transfer2Mod },
	{ 0xA6, noTransfer },
	{ 0xA7, noTransfer },
	{ 0xA8, transfer2 },
	{ 0xA9, transfer2 },
	{ 0xAA, transfer2 },
	{ 0xAB, transfer2Mod },
	{ 0xAC, transfer2Mod1 },
	{ 0xAD, transfer2Mod },
	{ 0xAE, transfer2Mod },
	{ 0xAF, transfer2Mod },
	{ 0xB0, transfer2Mod },
	{ 0xB1, transfer2Mod },
	{ 0xB2, transfer2Mod },
	{ 0xB3, transfer2Mod },
	{ 0xB4, transfer2Mod },
	{ 0xB5, transfer2Mod },
	{ 0xB6, transfer2Mod },
	{ 0xB7, transfer2Mod },
	{ 0xB8, noTransfer },
	{ 0xB9, noTransfer },
	{ 0xBA, transfer2Mod1 },
	{ 0xBB, transfer2Mod },
	{ 0xBC, transfer2Mod },
	{ 0xBD, transfer2Mod },
	{ 0xBE, transfer2Mod },
	{ 0xBF, transfer2Mod },
	{ 0xC0, transfer2Mod },
	{ 0xC1, transfer2Mod },
	{ 0xC2, noTransfer },
	{ 0xC3, noTransfer },
	{ 0xC4, noTransfer },
	{ 0xC5, noTransfer },
	{ 0xC6, noTransfer },
	{ 0xC7, transfer2Mod },
	{ 0xC8, transfer2 },
	{ 0xC9, transfer2 },
	{ 0xCA, transfer2 },
	{ 0xCB, transfer2 },
	{ 0xCC, transfer2 },
	{ 0xCD, transfer2 },
	{ 0xCE, transfer2 },
	{ 0xCF, transfer2 },
	{ 0xD0, noTransfer },
	{ 0xD1, transfer2Mod },
	{ 0xD2, transfer2Mod },
	{ 0xD3, transfer2Mod },
	{ 0xD4, noTransfer },
	{ 0xD5, transfer2Mod },
	{ 0xD6, noTransfer },
	{ 0xD7, noTransfer },
	{ 0xD8, transfer2Mod },
	{ 0xD9, transfer2Mod },
	{ 0xDA, noTransfer },
	{ 0xDB, transfer2Mod },
	{ 0xDC, transfer2Mod },
	{ 0xDD, transfer2Mod },
	{ 0xDE, noTransfer },
	{ 0xDF, transfer2Mod },
	{ 0xE0, noTransfer },
	{ 0xE1, transfer2Mod },
	{ 0xE2, transfer2Mod },
	{ 0xE3, noTransfer },
	{ 0xE4, noTransfer },
	{ 0xE5, transfer2Mod },
	{ 0xE6, noTransfer },
	{ 0xE7, noTransfer },
	{ 0xE8, transfer2Mod },
	{ 0xE9, transfer2Mod },
	{ 0xEA, noTransfer },
	{ 0xEB, transfer2Mod },
	{ 0xEC, transfer2Mod },
	{ 0xED, transfer2Mod },
	{ 0xEE, noTransfer },
	{ 0xEF, transfer2Mod },
	{ 0xF0, noTransfer },
	{ 0xF1, transfer2Mod },
	{ 0xF2, transfer2Mod },
	{ 0xF3, transfer2Mod },
	{ 0xF4, noTransfer },
	{ 0xF5, transfer2Mod },
	{ 0xF6, noTransfer },
	{ 0xF7, noTransfer },
	{ 0xF8, transfer2Mod },
	{ 0xF9, transfer2Mod },
	{ 0xFA, transfer2Mod },
	{ 0xFB, noTransfer },
	{ 0xFC, transfer2Mod },
	{ 0xFD, transfer2Mod },
	{ 0xFE, transfer2Mod },
	{ 0xFF, noTransfer },
	{ 0x00, lastEntry }
};

PBYTE transferInstruction( PBYTE destination, PBYTE source, PBYTE* jumpAddress, LONG* extra )
{
	X86_16BIT_INSTRUCTION op16 = { 0 };
	X86INSTRUCTION* opPtr = { 0 };

	*jumpAddress = TARGETLESS_X86INSTRUCTION;
	*extra = 0;

	op16.operandIs16 = 0;
	op16.addressIs16 = 0;
	op16.jumpAddress = jumpAddress;
	op16.extra = extra;

	opPtr = &instructionMap[source[0]];
	return opPtr->pXferFunction( &op16, opPtr, destination, source );
}

PBYTE transferData( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source )
{
	LONG bytes = 0;
	LONG fixedBytes = (opPtr->flagMask & ADDRESS_FLAG)
		? (op16Ptr->addressIs16 ? opPtr->size16 : opPtr->size)
		: (op16Ptr->operandIs16 ? opPtr->size16 : opPtr->size);
	bytes = fixedBytes;
	if( opPtr->modeOffset > 0 )
	{
		BYTE rmMode = source[opPtr->modeOffset];
		BYTE flags = regMemMode[rmMode];
		
		if( flags & SIB_FLAG )
		{
			if( ( source[opPtr->modeOffset + 1] & 0x07 ) == 0x05 )
			{
				if( ( rmMode & 0xc0 ) == 0x00 )
					bytes += 4;
				else if( ( rmMode & 0xc0 ) == 0x40 )
					bytes += 1;
				else if( ( rmMode & 0xc0 ) == 0x80 )
					bytes += 4;
			}
		}
		bytes += flags & NOTSIB_FLAG;
	}
	memcpy( destination, source, bytes );

	if( opPtr->relOffset )
		*op16Ptr->jumpAddress = adjustData( op16Ptr, destination, source, fixedBytes, opPtr->relOffset );
	if( opPtr->flagMask & NOENLARGE_FLAG )
		*op16Ptr->extra = -*op16Ptr->extra;
	if( opPtr->flagMask & DYNAMIC_FLAG )
		*op16Ptr->jumpAddress = DYNAMIC_X86INSTRUCTION;
	return source + bytes;
}

PBYTE transferDataPrefix( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source )
{
	transferData( op16Ptr, opPtr, destination, source );
	
	opPtr = &instructionMap[source[1]];
	return opPtr->pXferFunction(op16Ptr, opPtr, destination + 1, source + 1);
}

PBYTE adjustData( X86_16BIT_INSTRUCTION* op16Ptr, PBYTE destination, PBYTE source, LONG bytes, LONG targetOffset )
{
	LONG oldOffset = 0;
	LONG newOffset = 0;
	PBYTE target;
	LONG targetSize = bytes - targetOffset;
	PVOID targetAddr = &destination[targetOffset];
	
	switch( targetSize )
	{
	  case 1:
		oldOffset = (LONG)*((PCHAR)targetAddr);
		*op16Ptr->extra = 3;
		break;
	  case 2:
		oldOffset = (LONG)*((PSHORT)targetAddr);
		*op16Ptr->extra = 2;
		break;
	  case 4:
		oldOffset = (LONG)*((PLONG)targetAddr);
		*op16Ptr->extra = 0;
		break;
	}
	
	target = source + bytes + oldOffset;
	newOffset = oldOffset - (destination - source);
	
	switch( targetSize )
	{
	  case 1:
		*((PCHAR)targetAddr) = (CHAR)newOffset;
		break;
	  case 2:
		*((PSHORT)targetAddr) = (SHORT)newOffset;
		break;
	  case 4:
		*((PLONG)targetAddr) = (LONG)newOffset;
		break;
	}
	ASSERT( destination + bytes + newOffset == target );
	return target;
}

PBYTE noTransferOp( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source )
{
	return source + 1;
	UNREFERENCED_PARAMETER( destination );
	UNREFERENCED_PARAMETER( opPtr );
	UNREFERENCED_PARAMETER( op16Ptr );
}

PBYTE transferOp0F( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source )
{
	transferData( op16Ptr, opPtr, destination, source );	
	opPtr = &extendedInstructionMap[source[1]];
	return opPtr->pXferFunction( op16Ptr, opPtr, destination + 1, source + 1 );
}

PBYTE transferOp66( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source )
{	
	op16Ptr->operandIs16 = 1;
	return transferDataPrefix( op16Ptr, opPtr, destination, source );
}

PBYTE transferOp67( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source )
{	
	op16Ptr->addressIs16 = 1;
	return transferDataPrefix( op16Ptr, opPtr, destination, source );
}

PBYTE transferOpF6( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source )
{
	if( (source[1] & 0x38) == 0x00 )
	{
		X86INSTRUCTION ce = { 0xf6, transfer2Mod1 };
		return ce.pXferFunction( op16Ptr, &ce, destination, source );
	}
	{
		X86INSTRUCTION ce = { 0xf6, transfer2Mod };
		return ce.pXferFunction( op16Ptr, &ce, destination, source );
	}
}

PBYTE transferOpF7( X86_16BIT_INSTRUCTION* op16Ptr, X86INSTRUCTION* opPtr, PBYTE destination, PBYTE source )
{
	if( (source[1] & 0x38) == 0x00 )
	{
		X86INSTRUCTION ce = { 0xf7, transfer2ModOperand };
		return ce.pXferFunction( op16Ptr, &ce, destination, source );
	}
	{
		X86INSTRUCTION ce = { 0xf7, transfer2Mod };
		return ce.pXferFunction( op16Ptr, &ce, destination, source );
	}
}

PBYTE transferOpFF( X86_16BIT_INSTRUCTION* op16Ptr, PX86INSTRUCTION opPtr, PBYTE destination, PBYTE source )
{	
	if( source[1] == 0x15 || source[1] == 0x25 )
	{
		PBYTE* jumpAddress = *(PBYTE**) &source[2];
		*op16Ptr->jumpAddress = *jumpAddress;
	}
	else if( (source[1] & 0x38) == 0x10 || (source[1] & 0x38) == 0x18 ||
			 (source[1] & 0x38) == 0x20 || (source[1] & 0x38) == 0x28 )
	{
		*op16Ptr->jumpAddress = DYNAMIC_X86INSTRUCTION;
	}
	{
		X86INSTRUCTION ce = { 0xff, transfer2Mod };
		return ce.pXferFunction( op16Ptr, &ce, destination, source );
	}
}

//called by isJump when getx86Instruction wasn't enough to determine type
ULONG getNextInstruction( PCHAR codePtr, ULONG initial, PCHAR destinationBuffer, ULONG destinationBufferLength )
{
	PBYTE source = NULL;
	PBYTE destination = NULL;
	ULONG bytesCopied = 0;
	PBYTE target = NULL;
	LONG  extra = 0;

	memset( destinationBuffer, 0, destinationBufferLength );
	source = (PBYTE)codePtr;
	destination = (PBYTE)destinationBuffer;
	for( bytesCopied = 0; bytesCopied < initial; )
	{
		source = transferInstruction( destination, source, &target, &extra );
		if( !source )
		{
			memset( destinationBuffer, 0, destinationBufferLength );
			bytesCopied = 0;
			break;
		}
		bytesCopied = (DWORD)source - (DWORD)codePtr;
		if( bytesCopied >= destinationBufferLength )
		{
			ASSERT( FALSE );
			break;
		}
		destination = (PBYTE)destinationBuffer + bytesCopied;
	}
	return bytesCopied;
}

// called by trampoline to check for jump type instruction
BOOL isJump( PCHAR instruction, ULONG instructionLength )
{
	BYTE firstByte;
	BYTE secondByte;
	PCHAR thisInstruction;
	ULONG thisInstructionLength;
	ULONG nextInstructionLength;
	char instructionBuffer[MAX_INSTRUCTION] = { 0 };

	thisInstruction = instruction;
	thisInstructionLength = instructionLength;
	while( thisInstructionLength > 0 )
	{
		// check all jump op codes
		firstByte = thisInstruction[0];
		secondByte = thisInstruction[1];
		if( IS_BETWEEN( firstByte, 0x70, 0x7f ) )
			return TRUE;
		else if( IS_BETWEEN( firstByte, 0xca, 0xcb ) )
			return TRUE;
		else if( IS_BETWEEN( firstByte, 0xe0, 0xe3 ) )
			return TRUE;
		else if( IS_BETWEEN( firstByte, 0xe8, 0xeb ) )
			return TRUE;
		else if( IS_EQUAL( firstByte, 0xcf ) )
			return TRUE;
		else if( IS_EQUAL( firstByte, 0xf3 ) )
			return TRUE;
		else if( IS_EQUAL( firstByte, 0xff ) )
		{
			if( secondByte == 0x15 || secondByte == 0x25 )
				return TRUE;
			if( (secondByte & 0x38) == 0x10 || (secondByte & 0x38) == 0x18 ||
				(secondByte & 0x38) == 0x20 || (secondByte & 0x38) == 0x28 )
				return TRUE;
		}
		else if( IS_EQUAL( firstByte, 0x0f ) )
		{
			if( IS_BETWEEN( secondByte, 0x80, 0x8f ) )
				return TRUE;
		}
		memset( instructionBuffer, 0, sizeof(instructionBuffer) );
		nextInstructionLength = getNextInstruction( thisInstruction, 1, instructionBuffer, MAX_INSTRUCTION );
		if( nextInstructionLength <= 0 )
			break;
		thisInstructionLength -= nextInstructionLength;
		thisInstruction += nextInstructionLength;
	}
	return FALSE;
}

#pragma optimize( "", on )

