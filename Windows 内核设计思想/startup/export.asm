
%macro ENTRY_FRAME 1
%1:
[bits 32]
align 4														
		push	dword SuCodeSelector
		push	dword %%.TransferMode
		retf
align 4		
%%.TransferMode:
[bits 16]
		push	ebp
		push	ebx
		push	esi
		push	edi
		mov		ebx,esp
							
		mov		ax,SuDataSelector
		mov		ds,ax
		mov		ss,ax
						
		mov		sp,SuStack
		push	ebx 
%endmacro		


%macro EXIT_FRAME 0							
							
		pop		ebx

		mov		dx,KeDataSelector
		mov		ds,dx
		mov		es,dx
		mov		ss,dx
		mov		esp,ebx

		pop		edi
		pop		esi
		pop		ebx
		pop		ebp   

		pop		edx   
	 
		push	dword KeCodeSelector
		push	edx
		db 66h
		retf
%endmacro	


%macro MAKE_STACK_FRAME 2
		mov		ecx,(%1) >> 1
		mov		esi,%2 
		add		esi,20         

		push	KeDataSelector  
		pop		ds           
		push	ss           
		pop		es   
		        
		sub		sp,%1
			      
		xor		edi,edi       
		mov		di,sp          
%%.loop:
		mov		ax,[esi]
	es	mov		[edi],ax
		add		esi,2
		add		edi,2
		loop	%%.loop
		
		push	es          
		pop		ds              
%endmacro
	
;NTSTATUS 
;(_cdecl* ReadWriteSector)(
;	ULONG DriveNum, 
;	ULONG LBNLow, 
;	ULONG LBNHigh, 
;	ULONG BlockCount, 
;	PVOID Buffer, 
;	ULONG ReadWrite);//Read=0x42,Write=0x43
	
ENTRY_FRAME	ReadWriteSector

		MAKE_STACK_FRAME SectorFrame_size, ebx    
		   
		call	RealMode

		push	bp
		mov		bp, sp
		add		bp, 2

		push	ds
		push	si
		push	bx

		push	0
		pop		ds
			
		sub		sp, 16
		mov		si, sp
		mov		byte [si+DAP.PacketSize], DAP_size     
		mov		byte [si+DAP.Reserved], 0          
		mov		al,byte [bp+SectorFrame.BlockCount]
		mov		byte [si+DAP.BlockCount], al         
		mov		byte [si+DAP.BlockCount+1], 0         
		mov		eax, dword [bp+SectorFrame.Buffer]
		mov		bx, ax
		and		bx, 0fh     
		mov		word [si+DAP.BufferOffset], bx         
		shr		eax, 4
		mov		word [si+DAP.BufferSegment], ax        
		mov		eax, dword [bp+SectorFrame.LBNLow]
		mov		dword [si+DAP.LBNLow], eax              
		mov		eax, dword [bp+SectorFrame.LBNHigh]
		mov		dword [si+DAP.LBNHigh], eax             

		mov		ah, byte [bp+SectorFrame.ReadWrite]                                   
		mov		dl, byte [bp+SectorFrame.DriveNum] 
		int		13h
		jc		error

		xor		eax, eax
error:
		and		eax, 0000ffffh
		        
		add		sp, 16
		pop		bx
		pop		si
		pop		ds
		pop		bp

		add		sp, SectorFrame_size

		shl		ecx, 16
		mov		cx, dx
		push	eax
	
		push	word  1
		call	EnableProtectPaging
		add		sp, 2
		
		pop		eax								

EXIT_FRAME
				

;VOID (_cdecl* GetMemoryDescriptor)(
;	PE820FRAME E820Frame);
							
ENTRY_FRAME	GetMemoryDescriptor

		MAKE_STACK_FRAME  4, ebx

		call	RealMode

		push	bp
		mov		bp, sp
		add		bp, 2

		mov		eax, [bp+E820FRAME.ErrorFlag]
		mov		bp, ax
		and		bp, 0fh
		shr		eax, 4
		mov		es,ax                

	es  mov		ebx, [bp+E820FRAME.Key]
	es  mov		ecx, [bp+E820FRAME.Size]
		lea		di, [bp+E820FRAME.BaseAddrLow]
		mov		eax, 0E820h
		mov		edx, 'PAMS' 

		int		15h
	es  mov		[bp+E820FRAME.Key], ebx    
	es  mov		[bp+E820FRAME.Size], ecx   

		sbb		ecx, ecx     
		sub		eax, 'PAMS'   
		or		ecx, eax
	es  mov		[bp+E820FRAME.ErrorFlag], ecx

		pop		bp
		
		add		sp, 4
		
		push	word 1
		call	EnableProtectPaging
		add		sp, 2
	        
EXIT_FRAME	

;VOID 
;(_cdecl* DetectHardware)(
;	ULONG HeapBase, 
;	ULONG HeapSize, 
;	PVOID ConfigurationTree, 
;	PULONG HeapUsed, 
;	PVOID Options, 
;	ULONG OptionsLength);
	
ENTRY_FRAME    DetectHardware

		MAKE_STACK_FRAME  DetectionFrame_size, ebx

		call RealMode

		push	cs
		push	.end 

		push	1000h
		push	0
		retf
.end:
		add		sp, DetectionFrame_size

		push	word  1
		call	EnableProtectPaging
		add		sp, 2

EXIT_FRAME		