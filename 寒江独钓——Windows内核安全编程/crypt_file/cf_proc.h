///
/// @file         cf_proc.c
/// @author    crazy_chu
/// @date       2009-1-30
/// @brief      获得当前进程的名字。 
/// 
/// 授权协议
/// 本代码从属于工程crypt_file.是crazy_chu为《寒江独钓——
/// Windows内核编程与信息安全》所编写的文件透明加密示例。
/// 本工程仅仅支持WindowsXP下，FastFat文件系统下记事本的
/// 加密。未测试与杀毒软件或者其他文件过滤驱动并存的情况。
/// 本代码全部权利为作者保留，仅供读者学习和阅读使用。阅
/// 读者须承诺：不直接复制、或者基于此代码进行修改、利用
/// 此代码提供的全部或者部分技术用于商业的软件开发、或者
/// 其他的获利行为。如有违反，同意付出获利十倍之赔偿。
/// 阅读此代码，则自动视为接受以上授权协议。如不接受此协
/// 议者，请不要阅读此代码。

#ifndef _CF_PROC_HEADER_
#define _CF_PROC_HEADER_

void cfCurProcNameInit();

// 以下函数可以获得进程名。返回获得的长度。
ULONG cfCurProcName(PUNICODE_STRING name);

// 判断当前进程是不是notepad.exe
BOOLEAN cfIsCurProcSec(void);

#endif // _CF_PROC_HEADER_