
	xor  ax, ax              
	mov  ss, ax                 
	mov  sp, 7c00h  

	push dword 100
	push dword 1
	call ASR
	add  esp, 8

	push dword 100
	push dword 1
	call Accumulate
	add  esp, 8

	jmp $


; int ASR(int a1, int an)
ASR:
	push	ebp  
	mov		ebp,esp 
	push	ebx  
	push	esi  
	push	edi  

	;return (a1 + an) * (an - a1 + 1) / 2;
	mov		eax,dword [ebp+6] 
	add		eax,dword [ebp+10] 
	mov		ecx,dword [ebp+10] 
	sub		ecx,dword [ebp+6] 
	add		ecx,1 
	imul	eax,ecx 
	cdq              
	sub		eax,edx 
	sar		eax,1 

	pop		edi  
	pop		esi  
	pop		ebx  
	mov		esp,ebp 
	pop		ebp  
	ret 

;int Accumulate(int from, int to)
Accumulate:
sum	equ -4
i	equ -8

	push	ebp  
	mov		ebp,esp 
	sub		esp,12 
	push	ebx  
	push	esi  
	push	edi  

;int Sum = 0;
	mov		dword [ebp+sum],0 
;int i = 0;
	mov		dword [ebp+i],0 
;for(i=from; i<=to; i++)
	mov		eax,dword [ebp+6] 
	mov		dword [ebp+i],eax 
	jmp		._is_i_grater_than_to
._for:
	mov		eax,dword [ebp+i] 
	add		eax,1 
	mov		dword [ebp+i],eax 
._is_i_grater_than_to:
	mov		eax,dword [ebp+i] 
	cmp		eax,dword [ebp+10] 
	jg		._return
;Sum += i;
	mov		eax,dword [ebp+sum] 
	add		eax,dword [ebp+i] 
	mov		dword [ebp+sum],eax 
	jmp		._for
;return Sum;
._return:
	mov		eax,dword [ebp+sum] 

	pop		edi  
	pop		esi  
	pop		ebx  
	mov		esp,ebp 
	pop		ebp  
	ret  

times 510-($-$$) db 0
	dw  0AA55h