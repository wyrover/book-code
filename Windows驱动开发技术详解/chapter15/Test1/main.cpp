#include <Windows.h>
#include <stdio.h>

UCHAR In_8 (PUCHAR Port)
{
   UCHAR Value;
	__asm
	{
		mov edx, Port
		in al, dx
		mov Value, al
		//插入几个空指令
		nop
		nop
	}

   return(Value);
}

USHORT In_16 (PUSHORT Port)
{
   USHORT Value;

	__asm
	{
		mov edx, Port
		in ax, dx
		mov Value, ax
		//插入几个空指令
		nop
		nop
	}
   return(Value);
}

ULONG In_32 (PULONG Port)
{
   ULONG Value;
	__asm
	{
		mov edx, Port
		in eax, dx
		mov Value, eax
		//插入几个空指令
		nop
		nop
	}
   return(Value);
}

void Out_32(PULONG Port,ULONG Value)
{
	__asm
	{
		mov edx, Port
		mov eax, Value
		out dx,eax
		//插入几个空指令
		nop
		nop
	}
}
void Out_16 (PUSHORT Port,USHORT Value)
{
	__asm
	{
		mov edx, Port
		mov ax, Value
		out dx,ax
		//插入几个空指令
		nop
		nop
	}
}

void Out_8 (PUCHAR Port,UCHAR Value)
{
	__asm
	{
		mov edx, Port
		mov al, Value
		out dx,al
		//插入几个空指令
		nop
		nop
	}
}

int main()
{
	Out_8((PUCHAR)0x378,0);
	return 0;
}