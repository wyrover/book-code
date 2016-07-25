/* addsub.cpp
   Multiple-precision addition and subtraction of
   unsigned integers

   add() forms the radix-2^32 sum w[] of arrays u[] and v[]
   int u[n]
   int v[n]
   int w[n]

   after Knuth, volume 2, section 4.3.1
   Copyright (C) 1994 John K. Gotwals
   -------------------------------------------------------- */

int add(const int *u, const int *v, int *w, int n) {
	int edisav, esisav, carry;

#pragma comment(exestr, "Copyright (C) 1994 John K. Gotwals")

	__asm {
		mov edisav,edi		; edi and esi must be preserved
		mov esisav,esi

		mov ecx,n			; ecx = n;
		mov ebx,u			; ebx -> u[0]
		mov esi,v			; esi -> v[0]
		mov edi,w			; edi -> w[0]
		clc
		h1:
			mov eax,[ebx+4*ecx-4]	; eax = u[j-1]
			adc eax,[esi+4*ecx-4]	; eax = u[j-1] + v[j-1]
			mov [edi+4*ecx-4],eax	; w[j-1] = u[j-1] + v[j-1]
		loop h1
		mov eax,0
		adc	eax,0
		mov carry,eax		; carry gets 0 or 1 from carry flag

		mov edi,edisav		; restore edi and esi
		mov esi,esisav
	}
	return carry;
}

/* sub() forms the radix-2^32 difference w[] of arrays u[] and v[]
   int u[n], v[n], w[n]	where u[] >= v[]
   --------------------------------------------------------------- */
void sub(const int *u, const int *v, int *w, int n) {
	int edisav, esisav;

	__asm {
		mov edisav,edi		; edi and esi must be preserved
		mov esisav,esi

		mov ecx,n			; ecx = n;
		mov ebx,u			; ebx -> u[0]
		mov esi,v			; esi -> v[0]
		mov edi,w			; edi -> w[0]
		clc
		h1:
			mov eax,[ebx+4*ecx-4]	; eax = u[j-1]
			sbb eax,[esi+4*ecx-4]	; eax = u[j-1] + v[j-1]
			mov [edi+4*ecx-4],eax	; w[j] = u[j-1] + v[j-1]
		loop h1

		mov edi,edisav		; restore edi and esi
		mov esi,esisav
	}
}
