///
/// @file         cf_list.c
/// @author    crazy_chu
/// @date       2009-1-29
/// @brief      实现一个链表，保存所有正在加密打开着的文件。
///                请注意只支持WindowsXP下的FastFat文件系统。
///
/// 免责声明
/// 本代码为示例代码。未经详尽测试，不保证可靠性。作者对
/// 任何人使用此代码导致的直接和间接损失不负责任。
/// 
/// 授权协议
/// 本代码从属于工程crypt_file.是楚狂人与wowocock为《寒江独
/// 钓——Windows内核编程与信息安全》所编写的文件透明加密
/// 示例。本工程仅仅支持WindowsXP下，FastFat文件系统下记事
/// 本的加密。未测试与杀毒软件或者其他文件过滤驱动并存的
/// 情况。本代码全部权利为作者保留，仅供读者学习和阅读使
/// 用。未经两位作者书面授权，不得直接复制、或者基于此代
/// 码进行修改、利用此代码提供的全部或者部分技术用于商业
/// 的软件开发、或者其他的获利行为。如有违反，作者保留起
/// 诉和获取赔偿之权力。阅读此代码，则自动视为接受以上授
/// 权协议。如不接受此协议者，请不要阅读此代码。
///

#include <ntifs.h>
#include "cf_file_irp.h"
#include "fat_headers/fat.h"
#include "fat_headers/nodetype.h"
#include "fat_headers/fatstruc.h"

#define CF_MEM_TAG 'cfls'
#define CF_FILE_HEADER_SIZE (1024*4)

typedef struct {
    LIST_ENTRY list_entry;
    FCB *fcb;
} CF_NODE,*PCF_NODE;

static LIST_ENTRY s_cf_list;
static KSPIN_LOCK s_cf_list_lock;
static KIRQL s_cf_list_lock_irql;
static BOOLEAN s_cf_list_inited = FALSE;
BOOLEAN cfListInited()
{
    return s_cf_list_inited;
}
 
void cfListLock()
{
    ASSERT(s_cf_list_inited);
    KeAcquireSpinLock(&s_cf_list_lock,&s_cf_list_lock_irql);
}

void cfListUnlock()
{
    ASSERT(s_cf_list_inited);
    KeReleaseSpinLock(&s_cf_list_lock,s_cf_list_lock_irql);
}

void cfListInit()
{
    InitializeListHead(&s_cf_list);
    KeInitializeSpinLock(&s_cf_list_lock);
    s_cf_list_inited = TRUE;
}

// 任意给定一个文件，判断是否在加密链表中。这个函数没加锁。
BOOLEAN cfIsFileCrypting(PFILE_OBJECT file)
{
    PLIST_ENTRY p;
    PCF_NODE node;
   for(p = s_cf_list.Flink; p != &s_cf_list; p = p->Flink)
    {
	    node = (PCF_NODE)p;
        if(node->fcb == file->FsContext)
        {
            //KdPrint(("cfIsFileCrypting: file %wZ is crypting. fcb = %x \r\n",&file->FileName,file->FsContext));
            return TRUE;
        }
    } 
    return FALSE;
}

// 追加一个正在使用的机密文件。这个函数有加锁来保证只插入一
// 个，不会重复插入。
BOOLEAN cfFileCryptAppendLk(PFILE_OBJECT file)
{
    // 先分配空间
    PCF_NODE node = (PCF_NODE)
        ExAllocatePoolWithTag(NonPagedPool,sizeof(CF_NODE),CF_MEM_TAG);
    node->fcb = (PFCB)file->FsContext;

    cfFileCacheClear(file);

    // 加锁并查找，如果已经有了，这是一个致命的错误。直接报错即可。
    cfListLock();
    if(cfIsFileCrypting(file))
    {
        ASSERT(FALSE);
        return TRUE;
    }
    else if(node->fcb->UncleanCount > 1)
    {
        // 要成功的加入，必须要符合一个条件。就是FCB->UncleanCount <= 1.
        // 这样的话说明没有其他程序打开着这个文件。否则的话可能是一个普
        // 通进程打开着它。此时不能加密。返回拒绝打开。
        cfListUnlock();
        // 释放掉。
        ExFreePool(node);
        return FALSE;
    }

    // 否则的话，在这里插入到链表里。
    InsertHeadList(&s_cf_list, (PLIST_ENTRY)node);
    cfListUnlock();

    //cfFileCacheClear(file);
    return TRUE;
}


// 当有文件被clean up的时候调用此函数。如果检查发现
// FileObject->FsContext在列表中
BOOLEAN cfCryptFileCleanupComplete(PFILE_OBJECT file)
{
    PLIST_ENTRY p;
    PCF_NODE node;
    FCB *fcb = (FCB *)file->FsContext;

    KdPrint(("cfCryptFileCleanupComplete: file name = %wZ, fcb->UncleanCount = %d\r\n",
        &file->FileName,fcb->UncleanCount));

    // 必须首先清文件缓冲。然后再从链表中移除。否则的话，清缓
    // 冲时的写操作就不会加密了。
    if(fcb->UncleanCount <= 1 || (fcb->FcbState & FCB_STATE_DELETE_ON_CLOSE) )
        cfFileCacheClear(file);
    else
        return FALSE;

    cfListLock();
   for(p = s_cf_list.Flink; p != &s_cf_list; p = p->Flink)
   {
	    node = (PCF_NODE)p;
        if(node->fcb == file->FsContext && 
            (node->fcb->UncleanCount == 0 ||
            (fcb->FcbState & FCB_STATE_DELETE_ON_CLOSE)))
        {
            // 从链表中移除。
            RemoveEntryList((PLIST_ENTRY)node);
            cfListUnlock();
            //  释放内存。
            ExFreePool(node);
            return TRUE;
        }
    } 
    cfListUnlock();
   return FALSE;
}
