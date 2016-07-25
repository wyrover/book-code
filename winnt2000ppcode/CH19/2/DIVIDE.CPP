/* divide.cpp  Division of radix-2^32 nonnegative integers
   
   forms the radix-2^32 quotient (u/v) of arrays u[] and v[]
   int u[m + n] dividend
   int v[n]     divisor
   int q[m + 1] quotient
   int r[n]     remainder (if r == 0, no remainder is stored)

   after Knuth, volume 2, section 4.3.1
   Copyright (C) 1994 John K. Gotwals
   ---------------------------------------------------------- */
void divide(const int *u, const int *v, int *q, int *r, int n, int m) {
	int edisav, esisav;
	int e, vtic1, vtic2; // v'1, v'2
	int uticj, uticj1, uticj2; // u'j, u'j+1, u'j+2
	int carry, qhat, rhat;
	int jlimit;
	int *unew = 0;

#pragma comment(exestr, "Copyright (C) 1994 John K. Gotwals")

	// Get more memory if n > 1
	// (Necessary because routine changes u[])
	if (n > 1)
		unew = new int[m + n];

	__asm {
		mov edisav,edi		; edi and esi must be preserved
		mov esisav,esi
		
		; Test for special case of only 1 digit in divisor, n = 1
		cmp n,1
		jg d1

		; Divide nonnegative m+1-place integer u[m+1] by v[0]
		; producing the quotient q[m+1] and remainder r[0]
		mov edx,0			; r = 0
		mov ecx,0			; j = 0
		mov ebx,u			; ebx -> u[0]
		mov edi,q			; edi -> q[0]
		mov esi,v			; esi -> v[0]
		mov esi,[esi]		; esi = v[0]
	s2:
		mov eax,[ebx+4*ecx]	; eax = u[j]
		div esi
		mov	[edi+4*ecx],eax	; store quotient digit
		inc ecx
		cmp ecx,m
		jle s2
		mov ebx,r			; ebx -> r[0]
		cmp ebx,0
		je  retx
		mov [ebx],edx		; save remainder
		jmp retx

; ------------- More than 1 digit in divisor, n > 1 -----------

	d1:	
		; make a copy of the dividend u[m+n]
		pushfd					; save the state of df
		cld
		mov ecx,m
		add ecx,n				; ecx = m + n
		mov esi,u				; esi -> u[0]
		mov edi,unew			; edi -> unew[0]
		rep movsd
		mov ebx,unew
		mov u,ebx				; u -> unew[0]
		popfd

		; Compute the two leading digits of 2^e(v[0]v[1]v[2]) by
		; shifting left e bits and saving in vtic1 and vtic2.
		; It is safe to assume that v[0] != 0
		; It is safe to assume that n > 1
		; Note: 'e' has no connection with exp().
		mov ebx,v				; ebx -> v[0]
		mov eax,[ebx]			; eax = v[0]
		mov edx,[ebx+1*4]		; edx = v[1]
		bsr ecx,eax				; ecx = bit index of msb of v[0]
		neg ecx
		add ecx,31				; ecx = 31 - ecx
		mov e,ecx				; e is initialized
		shld eax,edx,cl			; edx is unchanged
		mov vtic1,eax			; v'1 is initialized
		mov eax,0				; to prepare for possible jump
		cmp n,2					; ok to access v[2]?
		je nis2
		mov eax,[ebx+2*4]		; yes, eax = v[2]
	nis2:
		shld edx,eax,cl
		mov vtic2,edx			; v'2 is initialized

		; rI1 = ecx = i
		; rI2 = esi = j - m
		; rI3 = edi = i + j
		; edx = k = carry
		; rIn are the index registers of Knuth's MIX computer
;	d2:
		mov esi,m			; D2. initialize j
		neg esi				; esi = -m  ==> j = 0
		
		; compute uticj,uticj1,uticj2 from 2^e(u[j]u[j+1]u[j+2]u[j+3])
		; if j+2 or j+3 is >= m + n then subscript is out of range
	d3:						; D3. Calculate qhat
		; calculate jlimit
		mov ebx,esi			; ebx = j - m
		sub ebx,n			; ebx = j - (m + n)
		neg ebx
		mov jlimit,ebx		; jlimit = (m + n) - j
		; compute the uticjs
		mov ecx,e			; ecx = shift bit count
		mov ebx,esi			; ebx = j - m
		add ebx,m			; ebx = j
		lea ebx,[ebx*4]
		add ebx,u			; ebx -> u[j]
		mov eax,0			; to prepare for possible jmp
		cmp ebx,u
		je j0				; jmp if j == 0
		mov eax,[ebx-1*4]	; eax = u[j-1]
	j0:
		mov edx,[ebx]		; edx = u[j]
		shld eax,edx,cl
		mov	uticj,eax
		mov eax,[ebx+1*4]	; eax = u[j+1]
		shld edx,eax,cl
		mov uticj1,edx
		mov edx,0			; to prepare for possible jmp
		cmp jlimit,3
		jl h0				; branch if j+2 is out of range
		mov edx,[ebx+2*4]	; edx = u[j+2]
	h0:
		shld eax,edx,cl
		mov uticj2,eax

		; calculate trial quotient qhat
		mov edx,uticj		; edx:eax = (u'j)b + u'j+1
		mov eax,uticj1
		cmp edx,vtic1		; avoid division overflow
		jae h1 
		div DWORD PTR vtic1	; eax = edx:eax/v'1
		mov qhat,eax		; qhat gets quotient
		mov rhat,edx		; rhat gets remainder
		jmp h2
	h1:
		mov edx,-1			; edx = b - 1, where b is the base
		mov eax,uticj1		; eax = u'j+1
		jmp h4
	h3:
		mov edx,qhat
		dec	edx				; decrease qhat by one
		mov eax,rhat		; adjust rhat accordingly
	h4:
		mov qhat,edx		; qhat = edx
		add eax,vtic1		; eax = eax + v'1
		jc d4				; (if rhat will be >= b, v'2qhat will be < rhatb
		mov rhat,eax		; rhat = eax
		mov eax,qhat
	h2:
		mul DWORD PTR vtic2
		cmp edx,rhat		; test if v'2qhat <= rhatb + uticj2
		jb d4
		ja h3
		cmp eax,uticj2
		ja h3				; if not, qhat is too large
	d4:						; D4. multiply and subtract
		mov edx,0			; will initialize carry below
		mov ecx,n
		dec ecx				; i = n - 1
		mov edi,m
		add edi,n
		add edi,esi			; edi = i + j
		dec edi
	h2a:
		mov carry,edx
		mov ebx,v			; ebx -> v[0]
		mov eax,[ebx+4*ecx]	; eax = v[i]
		mul DWORD PTR qhat	; edx:eax = qhat*v[i]
		add eax,carry		; add the contribution from the digit
		jnc h2b				;   to the right
		inc edx				; there was a carry from the add
	h2b:
		mov ebx,u			; ebx -> u[0]
		sub [ebx+4*edi],eax	; u[i+j] = u[i+j] - qhat*v[i]
		jnc h2c				; jump if result is positive
		inc edx				; borrow
	h2c:
		dec edi
		dec ecx
		jns	h2a				; branch if ecx >= 0
		; handle carry left in edx
		cmp edi,0			; watch out for edi < 0
		jl d5
		sub [ebx+4*edi],edx
		sbb edx,edx			
	d5:						; D5. test remainder
		mov ebx,esi			; ebx = j - m
		add ebx,m			; ebx = j
		lea ebx,[4*ebx]
		add ebx,q			; ebx -> q[j]
		mov eax,qhat
		mov [ebx],eax		; q[j] = qhat
		cmp edx,0			; (here edx = 0 or  1-b)
		je d7
;	d6:						; D6. add back (this branch is taken
		dec eax				;		_very_ infrequently)
		mov [ebx],eax		; q[j] = qhat - 1
		mov ecx,n			; ecx = n
		dec ecx				
		mov edi,m
		add edi,ecx			; edi = m + n
		add edi,esi			; edi = i + j = n + j
		clc					; add v1...vn to uj...uj+n
	h1a:
		mov ebx,v			; ebx -> v[0]
		mov eax,[ebx+4*ecx]	; eax = v[n]
		mov ebx,u			; ebx -> u[0]
		adc [ebx+4*edi],eax	; u[j+n] = u[j+n] + v[n]
		dec edi
		dec ecx
		jns h1a
	d7:						; D7. loop on j
		inc esi
		jle d3				; repeat for 0 <= j <= m
;	d8:						; possibly save remainder
		mov ecx,n			; loop n times
		mov ebx,u			; ebx ->u[o]
		mov esi,m			; index for u
		mov edx,r			; edx ->r[0]
		cmp edx,0
		je  retx
		mov edi,0			; index for r
	d8a:
		mov eax,[ebx+4*esi]	; r[edi] = u[esi]
		mov [edx+4*edi],eax
		inc esi
		inc edi
		loop d8a
	retx:
		mov edi,edisav		; restore edi and esi
		mov esi,esisav
	}
	delete [] unew;
}
