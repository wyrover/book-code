;;-----------------------------------------------------------------------------
;;     Author : habe (NujiNuji/MikoMiko)
;;       Mail : ----------------------
;;        Web : http://openlibsys.org/
;;    License : The modified BSD license
;;
;;                                    Copyright 2006 habe. All rights reserved.
;;                          Copyright 2009 OpenLibSys.org. All rights reserved.
;;-----------------------------------------------------------------------------
;; CPUID.ASM - by NujiNuji (MikoMiko)
;; $Id$


	.code
;; typedef unsigned int uint32;
;; typedef unsigned __int64 uint64;
;; assert(sizeof(uint32)*8 == 32);
;; assert(sizeof(uint64)*8 == 64);

;; void __fastcall CPUIDx64(DWORD index, DWORD *pEAX, DWORD *pEBX, DWORD *pECX, DWORD *pEDX);
_CPUIDx64	PROC
	PUSH	RBP
	MOV	RBP, RSP

	;; RSP+30H [edx]
	;; RSP+28H [ROOM FOR ecx]
	;; RSP+20H [ROOM FOR ebx]
	;; RSP+18H [ROOM FOR eax]
	;; RSP+14H [PADDING FOR index]
	;; RSP+10H [ROOM FOR index]
	;; RSP+08H [RETURN-ADDRESS]
	;; RSP+00H [RBP]
	;; RBP == RSP
	MOV	DWORD PTR [RBP+10H], ECX
	MOV	DWORD PTR [RBP+14H], 0
	MOV	QWORD PTR [RBP+18H], RDX
	MOV	QWORD PTR [RBP+20H], R8
	MOV	QWORD PTR [RBP+28H], R9

	;; RCX, RDX are volatile
	PUSH	RBX
	PUSH	RDI

	MOV	EAX, DWORD PTR [RBP+10H]
	MOV	RCX, 0

	CPUID

	;; EDI = pEAX;
	MOV	RDI, QWORD PTR [RBP+18H]
	;; *EDI = EAX;
	MOV	DWORD PTR [RDI], EAX

	;; *pEBX = EBX;
	MOV	RDI, QWORD PTR [RBP+20H]
	MOV	DWORD PTR [RDI], EBX

	;; *pECX = ECX;
	MOV	RDI, QWORD PTR [RBP+28H]
	MOV	DWORD PTR [RDI], ECX

	;; *pEDX = EDX
	MOV	RDI, QWORD PTR [RBP+30H]
	MOV	DWORD PTR [RDI], EDX

	POP	RDI
	POP	RBX

	XOR	RAX, RAX
	POP	RBP
	RET
_CPUIDx64	ENDP


	PUBLIC	_CPUIDx64

	END
;; THE END OF FILE
