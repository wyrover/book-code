#ifndef IATCALL_INFO_H
#define IATCALL_INFO_H

#include <windows.h>
#include "../../../nCom/avltree.h"
enum IATCall_Type{
    iatcall_call,       //call xxx调用类型
    iatcall_jmp,        //jmp xxx调用类型
    iatcall_callmem,    //call [xxx]调用类型
    iatcall_jmpmem,     //jmp [xxx]调用类型
    iatcall_pushret,    //push xxx,ret调用类型
    iatcall_memaddr     //其它地址引用型
};

#define IATCALL_FLAG_TRACEFAILD     0x1
#define IATCALL_FLAG_REACHLIMIT     0x2
#define IATCALL_FLAG_MARKOKAY       0x4
#define IATCALL_FLAG_MARKINVAILD    0x8
#define IATCALL_FLAG_TRACEDOUT      0x10
#define IATCALL_FLAG_SUSPECT        0x20

#define IATCALL_FLAG_KEEPMARK_MASK  (IATCALL_FLAG_MARKOKAY | IATCALL_FLAG_MARKINVAILD)
typedef struct iatcall_data{
    ULONG_PTR       cmdaddr;    //调用代码地址
    ULONG_PTR       callto;     //目标地址
    BYTE            opcode[16]; //调用代码opcode
    int             szop;       //调用代码opcode大小
    IATCall_Type    calltype;   //调用代码类型
    int             memoft;     //如果是内存引用型调用那么保存内存地址偏移
    BYTE            ophead;     //调用指令前一个字节
    BYTE            opsuffix;

    ULONG_PTR       retaddr;    //追踪结果返回地址
    BOOL            hasHead;    //是否有调用代码头，即push xxx,call xxx这样的垃圾指令
    long            rtflags;    //用于描述一些特别的追踪结果标记
    long            modhash;
    long            funchash;
}iatcall_data;

struct iat_module;
typedef struct iat_func{
    long            nameHash;
    std::wstring    name;
    iat_module*     module;
    long            modHash;
    long            IATRVA;
}iat_func;

typedef index_map<long,iat_func>    iat_modfuncs;
typedef struct iat_module{
    long            nameHash;
    std::wstring    name;
    iat_modfuncs    funcs;
}iat_module;

typedef index_map<long,iat_module>  iat_modules;

enum iatcall_vids{
    iatcall_vid_module = 0,
    iatcall_vid_record,
    iatcall_vid_hash,
    iatcall_vid_name,
    iatcall_vid_node,
    iatcall_vid_cmdaddr,
    iatcall_vid_callto,
    iatcall_vid_calltype,
    iatcall_vid_memoft,
    iatcall_vid_ophead,
    iatcall_vid_retaddr,
    iatcall_vid_hashead,
    iatcall_vid_rtflags,
    iatcall_vid_opcode,
    iatcall_vid_modhash,
    iatcall_vid_apihash,
    iatcall_vid_iatrva,
    iatcall_vid_opsuffix
};



#endif // IATCALL_INFO_H
