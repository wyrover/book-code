#ifndef LINKER_XXXX 
#define LINKER_XXXX 

#define COFF_SYMBOL_TYPE_FUNC 0x20

#define COFF_BSS_TYPE 0x00000080

#define IMAGE_SYM_CLASS_EXTERNAL 0x02

#define IMAGE_REL_I386_DIR32 0x0006
#define IMAGE_REL_I386_REL32 0x0014

#define IMAGE_SCN_CNT_CODE 0X20

#pragma pack(push, 1)

struct COFF_FileHdr{
	unsigned short usMagic;  // 魔法数字
	unsigned short usNumSec;  // 节（Section）数
	unsigned long  ulTime;  // 时间戳
	unsigned long  ulSymbolOffset;  // 符号表偏移
	unsigned long  ulNumSymbol;  // 符号数
	unsigned short usOptHdrSZ;  // 可选头长度
	unsigned short usFlags;  // 文件标记
};

struct COFF_SectionHdr{
	char           cName[8];  // 节名
	unsigned long  ulVSize;  // 虚拟大小
	unsigned long  ulVAddr;  // 虚拟地址
	unsigned long  ulSize;  // 节长度
	unsigned long  ulSecOffset;  // 节数据偏移
	unsigned long  ulRelOffset;  // 节重定位表偏移
	unsigned long  ulLNOffset;  // 行号表偏移
	unsigned short ulNumRel;  // 重定位表元素个数
	unsigned short ulNumLN;  // 行号表长度
	unsigned long  ulFlags;  // 节标识
};

struct COFF_Relocation{
	unsigned long  ulAddr;  // 定位偏移
	unsigned long  ulSymbol;  // 符号
	unsigned short usType;  // 定位类型
};

struct COFF_Symbol{
	union {
		char cName[8];            // 符号名称
		struct {
			unsigned long ulZero;   // 字符串表标识
			unsigned long ulOffset; // 字符串偏移
		} longName;
	}  name;
	unsigned long ulValue;     // 符号值
	short iSection;            // 符号所在节
	unsigned short usType;     // 符号类型
	unsigned char usClass;     // 符号存储类型
	unsigned char usNumAux;    // 符号附加记录数
};

#pragma pack(pop)

struct LinkInfo
{
	COFF_FileHdr * fileHdr;
	char * strTable;
	struct COFF_Symbol * symbolTable;
	struct COFF_SectionHdr * sectionHdrs;
	char * fileContent;	
};

struct Program
{
	char * entry;
	char * wholeProgram;	
};
//本函数只链接一个obj文件内的代码，而不处理外部链接
struct Program * linkFunction(char * funcName, char * objFileName);

void freeProgram(struct Program * );

#endif