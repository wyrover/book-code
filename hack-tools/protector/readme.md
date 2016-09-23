# 利用钩子技术控制进程创建

一、 简介


　　最近，我了解到一个叫做 Sanctuary 的相当有趣的安全产品。它能够阻止任何程序的运行 - 这些程序没有显示在软件列表中 - 该表中的程序被允许在一个特定的机器上运行。结果，PC 用户得到保护而免于各种插件间谍软件、蠕虫和特洛伊木马的侵袭 - 就算能够进入他 / 她的计算机，它们也没有机会执行，并因此没有机会对该机器造成任何损害。当然，我觉得这个特征相当有趣；并且，在稍作思考以后，我就有了一个自己的实现。因此，本文将描述如何通过钩住本机 API 的方式来实现监控一个进程的创建并在系统级上对之进行控制。

　　本文大胆假设，目标进程是以一种用户模式 (外壳函数，CreateProcess()，用一系列的本机 API 调用的手工的进程创建，等等) 创建的。尽管从理论上，一个进程能够以内核方式启动；不过从实际来看，如此的可能性是可以忽略不计的，因此我们不必为此担心。为什么？请逻辑地思考一下 - 为了以内核方式启动一个进程，用户必须装载一个驱动程序，该驱动程序反过来首先要暗示某种用户模式代码的执行。因此，为了防止未被授权程序的执行，我们可以安全地在系统级上以用户模式限制我们自己控制的进程的创建。

　　二、 定义策略

　　首先让我们明确，之所以这样做的目的是为了在系统级上监视和控制进程创建。

　　进程创建是一件相当复杂的事情 - 它包含相当多的工作 (如果你不相信我，可以反汇编 CreateProcess()，这样你就会亲眼看到这点)。为了启动一个进程，可以使用下列步骤：

　　1. 可执行文件必须被以 FILE_EXECUTE 存取方式打开。

　　2. 可执行映像必须被装载进 RAM。

　　3. 必须建立进程执行对象 (EPROCESS，KPROCESS 和 PEB 结构)。

　　4. 必须为新建进程分配地址空间。

　　5. 必须建立进程的主线程的线程执行对象 (ETHREAD，KTHREAD 和 TEBstructures)。

　　6. 必须为主线程分配堆栈。

　　7. 必须建立进程的主线程的执行上下文。

　　8. 必须通知 Win32 子系统有关该新进程的创建情况。

　　为确保这些步骤中的任何一步的成功，所有其前面的步骤必须是成功执行的 (你不能够在没有一个可执行区句柄的情况下建立一个可执行进程对象；没有文件句柄的情况下你无法映射一个可执行区，等等)。因此，如果我们决定退出任何这些步骤，所有后面的步骤也会失败，以至于整个进程创建会失败。上面所有的步骤都可以通过调用某些本机 API 函数的方式来实现，这是可以理解的。因此，为了监视和控制进程创建，我们所有要做的就是钩住这些 API 函数 - 它们无法旁路掉要创建一新进程所要执行的代码。

　　我们应该钩住哪些本机 API 函数呢? 尽管 NtCreateProcess() 似乎是问题的最显然的答案，但是，这个答案是错误的 - 有可能不需要调用这个函数也可以创建一个新的进程。例如，CreateProcess() 可以创建与进程相关的内核模式结构而不是调用 NtCreateProcess()。因此，这样以来钩住 NtCreateProcess() 对我们毫无帮助。

　　为了监视进程的创建，我们必须或者钩住 NtCreateFile() 和 NtOpenFile()，或者钩住 NtCreateSection()- 不经调用这些 API 是绝对无法运行任何可执行文件的。如果我们决定监视对 NtCreateFile() 和 NtOpenFile() 的调用，那么我们必须区别开进程创建和常规的文件 IO 操作。这项任务并不总是那么容易。例如，如果一些可执行文件正在被以 FILE_ALL_ACCESS 存取方式打开，我们该怎么办？这仅是一个 IO 操作还是一个进程创建的一部分？在这点上，是很难判断的 - 我们需要了解调用线程下一步要干什么。因此，钩住 NtCreateFile() 和 NtOpenFile() 不是最好的可能性选择。

　　钩住 NtCreateSection() 是更为合理的 - 如果我们想拦截对 NtCreateSection() 的调用，发出的请求是作为一个映像 (SEC_IMAGE 属性) 映射可执行文件 (SEC_IMAGE 属性)，同时请求允许执行的页面保护；那么，我们可以确信该进程将要被启动。在这一点上，我们是能够作出决定的，并且在我们不想要创建该进程的情况下，让 NtCreateSection() 返回 STATUS_ACCESS_DENIED。因此，为了完全控制目标机器上的进程创建，所有我们要做的是在系统级上钩住 NtCreateSection()。

　　象来自于 ntdll.dll 中的任何其它代理一样，NtCreateSection() 用服务索引加载 EAX，使 EDX 指向函数参数，并且把执行权传递到 KiDispatchService() 内核模式例程（这是通过 Windows NT/2000 中的 INT 0x2E 指令或者 Windows XP 下的 SYSENTER 指令实现的）。在校验完函数参数之后，KiDispatchService() 把执行权传递到服务的实际实现部分 - 它的地址可用于服务描述表（指向这个表的指针由 ntoskrnl.exe 作为 KeServiceDescriptorTable 变量所输出，所以它对于内核模式驱动程序是可用的）中。服务描述表通过下列结构所描述： 


``` cpp
struct SYS_SERVICE_TABLE { 
    void **ServiceTable; 
    unsigned long CounterTable; 
    unsigned long ServiceLimit; 
    void **ArgumentsTable; 
};
```

这个结构中的 ServiceTable 字段指向一个数组 - 它拥有所有实现系统服务的函数的地址。因此，为了在系统级上钩住任何本机 API 函数，所有我们必须做的是把我们的代理函数的地址写入被 KeServiceDescriptorTable 的 ServiceTable 字段所指向的数组的第 i 个入口 (i 是服务索引)。
至此，看起来我们已了解了在系统级上监视和控制进程创建的一切。现在让我们开始实际的工作。



三、 控制进程创建

　　我们的解决方案由一个内核模式驱动程序和一个用户模式应用程序组成。为了开始监视进程创建，我们的应用程序要把服务索引（相应于 NtCreateSection()）以及交换缓冲区的地址传递到我们的驱动程序。这是由下列代码所完成的：


``` cpp
// 打开设备
device=CreateFile("\\\\.\\PROTECTOR",GENERIC_READ|GENERIC_WRITE, 
0,0,OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM,0);
// 得到 NtCreateSection 的索引并把它连同输出缓冲区的地址传递给设备
DWORD * addr=(DWORD *)
(1+(DWORD)GetProcAddress(GetModuleHandle("ntdll.dll"),"NtCreateSection"));
ZeroMemory(outputbuff,256);
controlbuff[0]=addr[0];
controlbuff[1]=(DWORD)&outputbuff[0];
DeviceIoControl(device,1000,controlbuff,256,controlbuff,256,&dw,0);
```

此代码是显然的 - 唯一需要注意的是我们得到服务索引的方式。所有来自于 ntdll.dll 的代理都从一行代码 MOV EAX,ServiceIndex 开始 - 它可以适用于任何版本和风味的 Windows NT。这是一条 5 字节长的指令，以 MOV EAX 操作码作第一字节，服务索引作为留下的 4 字节。因此，为了得到相应于一些特别的本机 API 函数的服务索引，所有你要做的是从该地址读取 4 个字节，- 位于从这个代理开始 1 字节距离的地方。

　　现在让我们看一下我们的驱动程序做什么，当它收到来自我们的应用程序的 IOCTL 时：


``` cpp

NTSTATUS DrvDispatch(IN PDEVICE_OBJECT device,IN PIRP Irp)
{
　UCHAR*buff=0; ULONG a,base;
　PIO_STACK_LOCATION loc=IoGetCurrentIrpStackLocation(Irp);
　if(loc->Parameters.DeviceIoControl.IoControlCode==1000)
　{
　　buff=(UCHAR*)Irp->AssociatedIrp.SystemBuffer;
　　// 钩住服务调度表
　　memmove(&Index,buff,4);
　　a=4*Index+(ULONG)KeServiceDescriptorTable->ServiceTable;
　　base=(ULONG)MmMapIoSpace(MmGetPhysicalAddress((void*)a),4,0);
　　a=(ULONG)&Proxy;
　　_asm
　　{
　　　mov eax,base
　　　mov ebx,dword ptr[eax]
　　　mov RealCallee,ebx
　　　mov ebx,a
　　　mov dword ptr[eax],ebx
　　}
　　MmUnmapIoSpace(base,4);
　　memmove(&a,&buff[4],4);
　　output=(char*)MmMapIoSpace(MmGetPhysicalAddress((void*)a),256,0);
　}
　Irp->IoStatus.Status=0;
　IoCompleteRequest(Irp,IO_NO_INCREMENT);
　return 0;
}
```

　　正如你所见，这里没有什么特别的 - 我们只是通过 MmMapIoSpace() 来把交换缓冲区映射到内核中，另外把我们的代理函数的地址写到服务表 (当然，我们这是在把实际的服务执行的地址保存到全局变量 RealCallee 以后这样做的)。为了改写服务表的适当入口，我们通过 MmMapIoSpace() 来映射目标地址。为什么我们要这样做？不管怎么说，我们已经可以存取服务表了，不是吗？问题是，服务表可能驻留在一段只读内存中。因此，我们必须检查一下是否我们有对目标空间写的权限，而如果我们没有这个权限，那么在改写服务表之前，我们必须改变页面保护。你不认为这样以来工作太多了吗？因此，我们仅用 MmMapIoSpace() 来映射我们的目标地址，这样以来，我们就不必担心任何的页面保护问题了 - 从现在开始，我们假定已有到目标页面写的权限了。现在让我们看一下我们的代理函数:


``` cpp
// 这个函数用来确定是否我们应该允许 NtCreateSection() 调用成功
ULONG __stdcall check(PULONG arg)
{
　HANDLE hand=0;PFILE_OBJECT file=0;
　POBJECT_HANDLE_INFORMATION info;ULONG a;char*buff;
　ANSI_STRING str; LARGE_INTEGER li;li.QuadPart=-10000;
　// 检查标志。如果所要求的存取方式不是 PAGE_EXECUTE,
　// 这并不要紧
　if((arg[4]&0xf0)==0)return 1;
　if((arg[5]&0x01000000)==0)return 1;
　// 经由文件句柄得到文件名
　hand=(HANDLE)arg[6];
　ObReferenceObjectByHandle(hand,0,0,KernelMode,&file,&info);
　if(!file)return 1;
　　RtlUnicodeStringToAnsiString(&str,&file->FileName,1);
　　a=str.Length;buff=str.Buffer;
　　while(1)
　　{
　　　if(buff[a]=='.'){a++;break;}
　　　a--;
　　}
　　ObDereferenceObject(file);
　　// 如果它是不可执行的, 这也不要紧
　　// 返回 1
　　if(_stricmp(&buff[a],"exe")){RtlFreeAnsiString(&str);return 1;}
　　　// 现在，我们要询问用户的选择。
　　　// 把文件名写入缓冲区，并等待直到用户显示响应
　　　//(第一个 DWORD 为 1 意味着我们可以继续)
　　　// 同步存取该缓冲区
　　　KeWaitForSingleObject(&event,Executive,KernelMode,0,0);
　　　// 把缓冲区的前两个 DWORD 置为 0，
　　　// 把字符串复制到该缓冲区中，并循环下去，直到用户把每一个
　　　//DWORD 置为 1.
　　　// 第二个 DWORD 的值指明用户的响应
　　strcpy(&output[8],buff);
　　RtlFreeAnsiString(&str);
　　a=1;
　　memmove(&output[0],&a,4);
　　while(1)
　　{
　　　KeDelayExecutionThread(KernelMode,0,&li);
　　　memmove(&a,&output[0],4);
　　　if(!a)break;
　　}
　　memmove(&a,&output[4],4);
　　KeSetEvent(&event,0,0);
　　return a;
　}
　// 仅保存执行上下文并调用 check() 
　_declspec(naked) Proxy()
　{
　　_asm{
　　　// 保存执行上下文并调用 check()
　　　//- 后面的依赖于 check() 所返回的值
　　　// 如果返回值是 1，继续实际的调用。
　　　// 否则，返回 STATUS_ACCESS_DENIED
　　　pushfd
　　　pushad
　　　mov ebx,esp
　　　add ebx,40
　　　push ebx
　　　call check
　　　cmp eax,1
　　　jne block
　　　// 继续实际的调用
　　　popad
　　　popfd
　　　jmp RealCallee
　　　// 返回 STATUS_ACCESS_DENIED
　　　block:popad
　　　mov ebx, dword ptr[esp+8]
　　　mov dword ptr[ebx],0
　　　mov eax,0xC0000022L
　　　popfd
　　　ret 32
　　}
　}
```

　　Proxy() 保存寄存器和标志，把一个指向服务参数的指针压入栈中并调用 check()。其它的依赖于 check() 所返回的值。如果 check() 返回 TRUE(也就是，我们想要继续请求)，那么，Proxy() 将恢复寄存器和标志，并且把控制权交给服务实现部分。否则，Proxy() 将把 STATUS_ACCESS_DENIED 写入 EAX，恢复 ESP 并返回 - 从调用者的观点来看，这就象对 NtCreateSection() 的调用失败一样 - 以错误状态 STATUS_ACCESS_DENIED 返回。

　check() 函数是怎样做出决定的？一旦它收到一个指向服务参数的指针参数，它就可以检查这些参数。首先，它检查标志和属性 - 如果有一部分没有被要求作为一个可执行映像映射，或如果要求的页面保护不允许执行，那么我们可以确定 NtCreateSection() 调用与进程创建毫无关系。在这种情况下，check() 直接返回 TRUE。否则，它将检查该潜在文件的扩展 - 毕竟，SEC_IMAGE 属性和允许执行的页面保护可能被要求来映射某个 DLL 文件。如果该潜在文件不是一个. exe 文件，那么，check() 将返回 TRUE。否则，它给用户模式代码一个作出决定的机会。因此，它仅把文件名和路径写到交换缓冲区，并且对它循环查询，直到它得到响应为止。

　　在打开我们的驱动程序前，我们的应用程序创建一个运行下面函数的线程：

``` cpp
void thread()
{
　DWORD a,x; char msgbuff[512];
　while(1)
　{
　　memmove(&a,&outputbuff[0],4);
　　// 如果什么也没有，Sleep() 10 毫秒并再检查
　　if(!a){Sleep(10);continue;}
　　// 看起来象我们的权限被询问。 
　　// 如果被怀疑的文件已经存在于空白列表中，
　　// 则给出一个积极的响应。
　　char*name=(char*)&outputbuff[8];
　　for(x=0;x<stringcount;x++)
　　{
　　　if(!stricmp(name,strings[x])){a=1;goto skip;}
　　}
　　// 要求用户允许运行该程序
　　strcpy(msgbuff, "Do you want to run");
　　strcat(msgbuff,&outputbuff[8]);
　　// 如果用户的答复是积极的，那么把这个程序添加到空白列表中 
　　if(IDYES==MessageBox(0, msgbuff,"WARNING",MB_YESNO|MB_ICONQUESTION|0x00200000L))
　　　{a=1; strings[stringcount]=_strdup(name);stringcount++;}
　　else a=0;
　　// 把响应写入缓冲区中，而由驱动程序之后取回它
　　skip:memmove(&outputbuff[4],&a,4);
　　// 告诉驱动程序继续
　　a=0;
　　memmove(&outputbuff[0],&a,4);
　}
}
```


　　这段代码是显然的 - 我们的线程每 10 毫秒查询交换缓冲区。如果它发现我们的驱动程序已经把它的请求寄到了该缓冲区中，它就检查被允许在本机上运行的程序列表中的文件的文件名和路径。如果发现匹配，它直接给出一个 OK 响应。否则，它显示一个消息窗口，询问用户是否允许有问题的程序执行。如果响应是积极的，我们就把有问题的程序添加到允许在本机上运行的软件列表中。最后，我们把用户响应写入缓冲区，也就是说，把它传递到我们的驱动程序。因此，该用户就能完全控制它的 PC 上的进程的创建 - 只要我们的程序运行，在没有用户所给予权限的情况下，绝对没有办法来启动该 PC 上的任何进程。

　　正如你所见，我们让内核方式代码等待用户反应。这是否是一种聪明的举措呢？为了回答这个问题，你必须问你自己你是否正在堵住任何关键的系统资源 - 一切都依赖于具体的情况。在我们的情况下，一切发生在 IRQLPASSIVE_LEVEL 级上，并没有包含对 IRPs 的处理，并且必须等待用户响应的线程并不十分重要。因此，在我们的情况下，一切工作正常。然而，本例仅为演示之目的而编写。为了实际地使用它，以一个自动启动的服务的方式来重写我们的应用程序是很重要的。在这种情况下，我建议我们解除 LocalSystem 帐户，并且，在 NtCreateSection() 被用 LocalSystem 帐户特权在一个线程的上下文中调用的情况下，可以继续实际的服务实现而不施行任何检查 - 不管怎么说，LocalSystem 帐户仅运行那些在注册表中指定的可执行程序。因此，这样的一种解除不会是与我们的安全相妥协的。

　　四、 结论

　　最后，我必须指出，钩住本机 API 很明显是现已存在的最强有力的编程技术之一。本文通过一个例子向你展示通过钩住本机 API 可以实现的能力 - 正如你所见，我们已设法防止未被授权的程序的执行 - 这可以通过钩住单一的本机 API 函数来实现。你可以进一步扩展这个方法，并且获得对硬件设备、文件 IO 操作、网络流量等等的完全控制。然而，我们现在的解决方案并不是准备为内核模式 API 调用者所用 - 一旦内核模式代码被允许直接调用 ntoskrnl.exe 的输出，则这些调用就不需要经由系统服务发送者进行了。

　　本文源码在运行 Windows XP SP2 的若干机器上成功地测试过。尽管我还没在任何另外的环境下面测试它，我相信它应该到处工作正常 - 不管怎么说，它从未使用任何系统特定的结构。为了运行这个示例，所有你要做的是放置 protector.exe 和 protector.sys 到相同的目录下，并且运行 protector.exe。直到 protector.exe 的应用程序窗口被关闭为止；否则，每次你都会被提示你试图运行任何可执行程序。