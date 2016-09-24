///
/// @file         cf_list.h
/// @author    crazy_chu
/// @date       2009-1-29
///

#ifndef _CF_LIST_HEADER_
#define _CF_LIST_HEADER_

void cfListInit();
BOOLEAN cfListInited();
void cfListLock();
void cfListUnlock();
// 任意给定一个文件，判断是否在加密链表中。
BOOLEAN cfIsFileCrypting(PFILE_OBJECT file);
BOOLEAN cfFileCryptAppendLk(PFILE_OBJECT file);
BOOLEAN cfIsFileNeedCrypt(
    PFILE_OBJECT file,
    PDEVICE_OBJECT next_dev,
    ULONG desired_access,
    BOOLEAN *need_write_header);
// 当有文件被clean up的时候调用此函数。如果检查发现
// FileObject->FsContext在列表中
BOOLEAN cfCryptFileCleanupComplete(PFILE_OBJECT file);
NTSTATUS cfWriteAHeader(PFILE_OBJECT file,PDEVICE_OBJECT next_dev);

#endif // _CF_LIST_HEADER_