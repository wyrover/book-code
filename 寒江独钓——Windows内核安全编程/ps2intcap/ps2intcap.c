///
/// @file         ps2intcap.c
/// @author    wowocock,crazy_chu
/// @date       2009-1-27
///

#include <ntddk.h>

// 这一句存在，则本程序编译为替换INT0x93的做法。如果
// 不存在，则为IOAPIC重定位做法。
// #define BUILD_FOR_IDT_HOOK

// 由于这里我们必须明确一个域是多少位，所以我们预先定义几个明
// 确知道多少位长度的变量，以避免不同环境下编译的麻烦.
typedef unsigned char P2C_U8;
typedef unsigned short P2C_U16;
typedef unsigned long P2C_U32;

#define P2C_MAKELONG(low, high) \
((P2C_U32)(((P2C_U16)((P2C_U32)(low) & 0xffff)) | ((P2C_U32)((P2C_U16)((P2C_U32)(high) & 0xffff))) << 16))

#define P2C_LOW16_OF_32(data) \
((P2C_U16)(((P2C_U32)data) & 0xffff))

#define P2C_HIGH16_OF_32(data) \
((P2C_U16)(((P2C_U32)data) >> 16))

// 从sidt指令获得一个如下的结构。从这里可以得到IDT的开始地址
#pragma pack(push,1)
typedef struct P2C_IDTR_ {
	P2C_U16 limit;		// 范围
	P2C_U32 base;		// 基地址（就是开始地址）
} P2C_IDTR, *PP2C_IDTR;
#pragma pack(pop)

// 下面这个函数用sidt指令读出一个P2C_IDTR结构，并返回IDT的地址。
void *p2cGetIdt()
{
	P2C_IDTR idtr;
    // 一句汇编读取到IDT的位置。
	_asm sidt idtr
	return (void *)idtr.base;
}

#pragma pack(push,1)
typedef struct P2C_IDT_ENTRY_ {
		P2C_U16 offset_low;
		P2C_U16 selector;
		P2C_U8 reserved;
		P2C_U8 type:4;
		P2C_U8 always0:1;
		P2C_U8 dpl:2;
		P2C_U8 present:1;
		P2C_U16 offset_high;
} P2C_IDTENTRY, *PP2C_IDTENTRY;
#pragma pack(pop)

#define OBUFFER_FULL 0x02
#define IBUFFER_FULL 0x01

ULONG p2cWaitForKbRead()
{
	int i = 100;
	P2C_U8 mychar;	
	do
	{
        _asm in al,0x64
        _asm mov mychar,al
	    KeStallExecutionProcessor(50);
	    if(!(mychar & OBUFFER_FULL)) break;
	} while (i--);
	if(i) return TRUE;
	return FALSE;
}

ULONG p2cWaitForKbWrite()
{
	int i = 100;
	P2C_U8 mychar;
	do
	{
        _asm in al,0x64
        _asm mov mychar,al
		KeStallExecutionProcessor(50);
		if(!(mychar & IBUFFER_FULL)) break;
	} while (i--);
	if(i) return TRUE;
	return FALSE;
}

// 首先读端口获得按键扫描码打印出来。然后将这个扫
// 描码写回端口，以便别的应用程序能正确接收到按键。
// 如果不想让别的程序截获按键，可以写回一个任意的
// 数据。
void p2cUserFilter()
{
    static P2C_U8 sch_pre = 0;
	P2C_U8	sch;
	p2cWaitForKbRead();
    _asm in al,0x60
    _asm mov sch,al
    KdPrint(("p2c: scan code = %2x\r\n",sch));
   //  把数据写回端口，以便让别的程序可以正确读取。
	if(sch_pre != sch)
	{
		sch_pre = sch;
        _asm mov al,0xd2
        _asm out 0x64,al
		p2cWaitForKbWrite();
        _asm mov al,sch
        _asm out 0x60,al
	}
}

void *g_p2c_old = NULL;

__declspec(naked) p2cInterruptProc()
{
	__asm
	{
		pushad					// 保存所有的通用寄存器
		pushfd					// 保存标志寄存器
		call p2cUserFilter	// 调一个我们自己的函数。 这个函数将实现
								    // 一些我们自己的功能
		popfd					// 恢复标志寄存器
		popad					// 恢复通用寄存器
		jmp	g_p2c_old		// 跳到原来的中断服务程序
	}
}

// 这个函数修改IDT表中的第0x93项，修改为p2cInterruptProc。
// 在修改之前要保存到g_p2c_old中。
void p2cHookInt93(BOOLEAN hook_or_unhook)
{
    PP2C_IDTENTRY idt_addr = (PP2C_IDTENTRY)p2cGetIdt();
    idt_addr += 0x93;
    KdPrint(("p2c: the current address = %x.\r\n",
        (void *)P2C_MAKELONG(idt_addr->offset_low,idt_addr->offset_high)));
    if(hook_or_unhook)
    {
        KdPrint(("p2c: try to hook interrupt.\r\n"));
        // 如果g_p2c_old是NULL，那么进行hook
        g_p2c_old = (void *)P2C_MAKELONG(idt_addr->offset_low,idt_addr->offset_high);
        idt_addr->offset_low = P2C_LOW16_OF_32(p2cInterruptProc);
        idt_addr->offset_high = P2C_HIGH16_OF_32(p2cInterruptProc);
    }
    else
    {
        KdPrint(("p2c: try to recovery interrupt.\r\n"));
        // 如果g_p2c_old不是NULL，那么取消hook.
        idt_addr->offset_low = P2C_LOW16_OF_32(g_p2c_old);
        idt_addr->offset_high = P2C_HIGH16_OF_32(g_p2c_old);
    }
    KdPrint(("p2c: the current address = %x.\r\n",
        (void *)P2C_MAKELONG(idt_addr->offset_low,idt_addr->offset_high)));
}

// 在idt表中找到一个空闲的idtentry的位置。然后返回这个id.这是为
// 了能填入我们新的键盘中断处理入口。如果找不到就返回0。这
// 种情况下无法安装新的中断处理。
P2C_U8 p2cGetIdleIdtVec()
{
    P2C_U8 i;
    PP2C_IDTENTRY idt_addr = (PP2C_IDTENTRY)p2cGetIdt();

    // 从vec20搜索到2a即可。
	for(i=0x20;i<0x2a;i++)
	{
        // 如果类型为0说明是空闲位置，返回即可。
        if(idt_addr[i].type == 0)
		{
			return i;
		}
	}
    return 0;
}


P2C_U8 p2cCopyANewIdt93(P2C_U8 id,void *interrupt_proc)
{
    // 我们写入一个新的中断门。这个门完全拷贝原来的0x93
    // 上的idtentry，只是中断处理函数的地址不同。
    PP2C_IDTENTRY idt_addr = (PP2C_IDTENTRY)p2cGetIdt();
    idt_addr[id] = idt_addr[0x93];
    idt_addr[id].offset_low = P2C_LOW16_OF_32(interrupt_proc);
    idt_addr[id].offset_high = P2C_HIGH16_OF_32(interrupt_proc);
    return id;
}

// 搜索IOAPIC获得键盘中断，或者设置这个值。
P2C_U8 p2cSeachOrSetIrq1(P2C_U8 new_ch)
{
    // 选择寄存器。选择寄存器虽然是32位的寄存器，但是只使用
    // 低8位，其他的位都被保留。
	P2C_U8 *io_reg_sel;

    // 窗口寄存器，用来读写被选择寄存器选择的值，是32位的。
	P2C_U32 *io_win;
	P2C_U32 ch,ch1;

    // 定义一个物理地址，这个地址为0xfec00000。正是IOAPIC
    // 寄存器组在Windows上的开始地址
	PHYSICAL_ADDRESS	phys ;
	PVOID paddr;
	RtlZeroMemory(&phys,sizeof(PHYSICAL_ADDRESS));
	phys.u.LowPart = 0xfec00000;

    // 物理地址是不能直接读写的。MmMapIoSpace把物理地址映射
    // 为系统空间的虚拟地址。0x14是这片空间的长度。
	paddr = MmMapIoSpace(phys, 0x14, MmNonCached);

    // 如果映射失败了就返回0.
	if (!MmIsAddressValid(paddr))
		return 0;

    // 选择寄存器的偏移为0
	io_reg_sel = (P2C_U8 *)paddr;
    // 窗口寄存器的偏移为0x10.
	io_win = (P2C_U32 *)((P2C_U8 *)(paddr) + 0x10);

    // 选择第0x12，刚好是irq1的项
	*io_reg_sel = 0x12;
	ch = *io_win;

    // 如果new_ch不为0，我们就设置新值。并返回旧值。
    if(new_ch != 0)
    {
        ch1 = *io_win;
        ch1 &= 0xffffff00;
        ch1 |= (P2C_U32)new_ch;
        *io_win = ch1;
        KdPrint(("p2cSeachOrSetIrq1: set %2x to irq1.\r\n",(P2C_U8)new_ch));
    }

    // 窗口寄存器里读出的值是32位的，但是我们只需要
    // 一个字节就可以了。这个字节就是中断向量的值。
    // 一会我们要修改这个值。
    ch &= 0xff;
	MmUnmapIoSpace(paddr, 0x14);
    KdPrint(("p2cSeachOrSetIrq1: the old vec of irq1 is %2x.\r\n",(P2C_U8)ch));
	return (P2C_U8)ch;
}

void p2cResetIoApic(BOOLEAN set_or_recovery)
{
    static P2C_U8 idle_id = 0;
    PP2C_IDTENTRY idt_addr = (PP2C_IDTENTRY)p2cGetIdt();
    P2C_U8 old_id = 0;

    if(set_or_recovery)
    {
        // 如果是设置新的ioapic定位，那么首先在g_p2c_old中保存
        // 原函数的入口。
        idt_addr = (PP2C_IDTENTRY)p2cGetIdt();
        idt_addr += 0x93;
        g_p2c_old = (void *)P2C_MAKELONG(idt_addr->offset_low,idt_addr->offset_high);
 
        // 然后获得一个空闲位，将irq1处理中断门复制一个进去。
        // 里面的跳转函数填写为我们的新的处理函数。
        idle_id = p2cGetIdleIdtVec();
        if(idle_id != 0)
        {
            p2cCopyANewIdt93(idle_id,p2cInterruptProc);
            // 然后重新定位到这个中断。
            old_id = p2cSeachOrSetIrq1(idle_id);
            // 在32位WindowsXP下这个中断默认应该是定位到0x93的。
            ASSERT(old_id == 0x93);
        }
    }
    else
    {
        // 如果是要恢复...
        old_id = p2cSeachOrSetIrq1(0x93);
        ASSERT(old_id == idle_id);
        // 现在那个中断门没用了，设置type = 0使之空闲
        idt_addr[old_id].type = 0;
    }
}

#define  DELAY_ONE_MICROSECOND  (-10)
#define  DELAY_ONE_MILLISECOND (DELAY_ONE_MICROSECOND*1000)
#define  DELAY_ONE_SECOND (DELAY_ONE_MILLISECOND*1000)

void p2cUnload(PDRIVER_OBJECT drv)
{
	LARGE_INTEGER interval;
#ifdef BUILD_FOR_IDT_HOOK
    p2cHookInt93(FALSE);
#else
    p2cResetIoApic(FALSE);
#endif
    KdPrint (("p2c: unloading\n")); 
	// 睡眠5秒。等待所有irp处理结束
	interval.QuadPart = (5*1000 * DELAY_ONE_MILLISECOND);		
	KeDelayExecutionThread(KernelMode,FALSE,&interval);
}

NTSTATUS DriverEntry( 
                     IN PDRIVER_OBJECT DriverObject, 
                     IN PUNICODE_STRING RegistryPath 
                     ) 
{ 
    ULONG i; 
    KdPrint (("p2c: entering DriverEntry\n")); 
    // 卸载函数。
    DriverObject->DriverUnload = p2cUnload;
#ifdef BUILD_FOR_IDT_HOOK
    p2cHookInt93(TRUE);
#else
    p2cResetIoApic(TRUE);
#endif
    return  STATUS_SUCCESS; 
}

