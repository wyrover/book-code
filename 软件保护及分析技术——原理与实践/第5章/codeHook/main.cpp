#include <windows.h>

#pragma comment(linker,"/SECTION:.text,ERW")
void func1()
{
    MessageBoxA(0,"消息1","提示",MB_OK);
}

void hookedproc()
{
    MessageBoxA(0,"Hooked消息","提示",MB_OK);
}
void* glpCode = 0;
void __declspec(naked) hookentry()
{
    __asm{
        pushfd
        pushad
        call hookedproc
        popad
        popfd
        jmp glpCode
    }
}

BYTE gCodeBackup[32];
void hookproc1()
{
    __asm int 3	//定位指令，编译后直接用调试调试定位到这里观察效果
    //取出func1入口地址，也是第一行汇编指令地址
    BYTE* lpFunc1 = (BYTE*)func1;
    //这里我们已经预先计算出我们会影响6个字节的原始指令大小
    memcpy(gCodeBackup,lpFunc1,6);

    //申请一段可执行内存空间存放原始指令片段
    glpCode = (void*)VirtualAlloc(0,1000,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
    //迁移原始指令片段到新内存空间
    //此处为指令迁移代码，需要设计到代码移位技术
    DWORD szMove = 16;
    BYTE* lpTemp = (BYTE*)glpCode;
    int len = 0;
    int pos = 0;
    do
    {
        //本示例需要读者自行解决LDE引擎
        len = LDE64((BYTE*)(lpFunc1+pos));
        BYTE* codeIP	=	lpFunc1+pos;
        //调整短jmc
        if ((BYTE)codeIP[0] >= 0x70 && (BYTE)codeIP[0] <= 0x7F )
        {
            WORD*	pJmpCode	=	(WORD*)(lpTemp);
            *pJmpCode			=	(codeIP[0] * 0x100) + 0x100F;
            *(ULONG*)(lpTemp+2) = *(ULONG*)((ULONG)codeIP+1)+(ULONG)codeIP-(ULONG)lpTemp;
            lpTemp+=6;
        }else if ((BYTE)codeIP[0] == 0x0F &&((BYTE)codeIP[1] >= 0x80 && (BYTE)codeIP[1] <= 0x8F))
        {
            //调整长jmc
            *(WORD*)(lpTemp) = *(WORD*)codeIP;
            *(ULONG_PTR*)(lpTemp+2) = *(ULONG_PTR*)((ULONG)codeIP+2)+(ULONG)codeIP-(ULONG)lpTemp;
            lpTemp+=6;
        }
        else if((BYTE)codeIP[0] == 0xE9 || (BYTE)codeIP[0] == 0xE8)
        {
            //调整jmp,CALL
            *(lpTemp) = codeIP[0];
            *(ULONG*)(lpTemp+1) = *(ULONG*)((ULONG)codeIP+1)+(ULONG)codeIP-(ULONG)lpTemp;
            lpTemp+=5;
        }
        else
        {
            //其它指令直接复制
            memcpy((char*)(lpTemp),(char*)(lpFunc1+pos),len);
            lpTemp+=len;
        }
        pos += len;
    } while(pos<6);

    //在迁移后的指令片段后面添加跳转，跳转到原始指令主体
    BYTE* lpCode = (BYTE*)glpCode+pos;
    lpCode[0] = 0x68;
    *(ULONG_PTR*)&lpCode[1] = (ULONG_PTR)lpFunc1+szMove;
    lpCode[5] = 0xC3;
    //HOOK代码指令
    lpFunc1[0] = 0x68;
    *(ULONG_PTR*)&lpFunc1[1] = (ULONG_PTR)hookentry;
    lpFunc1[5] = 0xC3;
}

void unhookproc1()
{
    BYTE* lpFunc1 = (BYTE*)func1;
    memcpy(lpFunc1,gCodeBackup,6);
}

int main()
{
    hookproc1();
    func1();
    return 0;
}

