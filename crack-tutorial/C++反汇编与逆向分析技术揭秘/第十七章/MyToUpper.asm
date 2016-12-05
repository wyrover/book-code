.486                  	; create 32 bit code
.model flat, c      	; 32 bit memory model
option casemap :none   	; case sensitive
      
.code

Format db 'CBaseOne ',0Dh,0Ah,0

Format1 db 'CBaseOne ',0Dh,0Ah,0

Format2 dd offset Format, ; 
offset Format1

ToUpper      proc arg_0:DWORD               ; 函数入口
	push    esi
	mov  	esi, arg_0
	push  	edi
	mov  	edi, esi
	or     	ecx, 0FFFFFFFFh
	xor 	eax, eax
	repne scasb
	not  	ecx
	dec    	ecx
	xor    	edx, edx
	test  	ecx, ecx
	jle    	short loc_40102D

loc_401018:  			; 地址标号
	mov 	al, [edx+esi]
	cmp 	al, 61h
	jl      	short loc_401028
	cmp   	al, 7Ah
	jg      	short loc_401028
	sub     	al, 20h
	mov  	[edx+esi], al
loc_401028:   		; 地址标号
	inc     	edx
	cmp  	edx, ecx
	jl      	short loc_401018
loc_40102D:     		; 地址标号
	pop  	edi
	pop   	esi
	ret
ToUpper     endp
end