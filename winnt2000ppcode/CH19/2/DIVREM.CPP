/* divrem.cpp  Division of radix-2^32 nonnegative integers
   
   forms the radix-2^32 quotient (u/v) of array u[] and v

   int u[m] dividend on input
   int u[m] quotient on output
   int v    divisor
   returns remainder r
   Copyright (C) 1994 John K. Gotwals
   ------------------------------------------------------- */
int divrem(int *u, int v, int m) {
	int edisav, esisav, r;

#pragma comment(exestr, "Copyright (C) 1994 John K. Gotwals")

	__asm {
		mov edisav,edi		; edi and esi must be preserved
		mov esisav,esi
		
		; Divide nonnegative j+1-place integer u[j] by v
		; producing the quotient v[j] and remainder r
		mov edx,0			; r = 0
		mov ecx,0			; j = 0
		mov ebx,u			; ebx -> u[0]
		mov edi,m			; edi = m
		mov esi,v			; esi = v
	s2:
		mov eax,[ebx+4*ecx]	; eax = u[j]
		div esi
		mov	[ebx+4*ecx],eax	; store quotient digit
		inc ecx
		cmp ecx,edi
		jl s2
		mov r,edx			; save remainder
	
		mov edi,edisav		; restore edi and esi
		mov esi,esisav
	}

	return r;
}

