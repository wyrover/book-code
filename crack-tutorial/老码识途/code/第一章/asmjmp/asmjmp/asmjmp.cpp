// asmjmp.cpp : 定义控制台应用程序的入口点。
//

int i, gi;
void * address;
void main()
{	  		
		_asm {
			mov address, offset _lb1;
			jmp address;
			}
		i = 2;
		_lb1:
		gi = 12;
}

