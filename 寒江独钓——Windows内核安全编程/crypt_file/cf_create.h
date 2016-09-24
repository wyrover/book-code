///
/// @file         cf_create.c
/// @author    crazy_chu
/// @date       2009-2-4
/// @brief      实现对create irp的处理。 
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

#ifndef _CF_CREATE_HEADER_
#define _CF_CREATE_HEADER_

// 打开预处理。请注意，只有当前进程为加密进程，才需要调
// 用这个预处理来处理。
ULONG cfIrpCreatePre(
    PIRP irp,
    PIO_STACK_LOCATION irpsp,
    PFILE_OBJECT file,
    PDEVICE_OBJECT next_dev);

#endif // _CF_CREATE_HEADER_