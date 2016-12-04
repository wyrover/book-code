#include "linker.h"
#include <stdio.h>
#include <process.h>
#include <string.h>
#include <memory.h>
#include <malloc.h>

void fatalError(char * msg)
{
	printf("%s\n", msg);
	exit(1);
}


void initStrTb(struct LinkInfo * plinkInfo)
{
	int symbolTbSize = plinkInfo->fileHdr->ulNumSymbol * sizeof(struct COFF_Symbol);
	//串表在符号表后面，所以计算符号表结束的偏移
	plinkInfo->strTable = plinkInfo->fileContent + plinkInfo->fileHdr->ulSymbolOffset + symbolTbSize;	
}

void initSymbolTb(struct LinkInfo * plinkInfo)
{
	plinkInfo->symbolTable = (struct COFF_Symbol *)(plinkInfo->fileContent + plinkInfo->fileHdr->ulSymbolOffset);
}


void initSectionHdrs(struct LinkInfo * plinkInfo)
{
	plinkInfo->sectionHdrs = (struct COFF_SectionHdr *)(plinkInfo->fileContent + sizeof(COFF_FileHdr));
	if (plinkInfo->fileHdr->usOptHdrSZ != 0)
		plinkInfo->sectionHdrs = (struct COFF_SectionHdr *)((char *)(plinkInfo->sectionHdrs) + plinkInfo->fileHdr->usOptHdrSZ);
}

#define GET_STR_ENTRY(plinkInfo, offset) plinkInfo->strTable + offset

#define GET_SECTION_ADDRESS(plinkInfo, sectionIndex) plinkInfo->fileContent + plinkInfo->sectionHdrs[sectionIndex].ulSecOffset

bool compareNameInStrTb(struct LinkInfo * plinkInfo,  char * name, int offsetInStrTb)
{
	return strcmp(GET_STR_ENTRY(plinkInfo, offsetInStrTb), name) == 0;
}

bool getStringFromTb(struct LinkInfo * plinkInfo,  char * name, int offset)
{
	strcpy(name, GET_STR_ENTRY(plinkInfo, offset));
	return true;
}

bool comparePureName(char * pureName, char * symbolName, bool isShort)
{
	if (* symbolName == 0)
		return false;
	bool beginCheck = false;
	//int pureLen = strlen(pureName);
	int limit = isShort ? 8 : 0x7fffffff;
	int count = 0;
	while((* symbolName != 0) && (* pureName != 0) && (count < limit))
	{
		count++;
		if (!beginCheck && (* symbolName == '?'))
		{
			beginCheck = true;
			symbolName++;
			continue;
		}
		if (!beginCheck)
		{
			symbolName++;
			continue;
		}
		if (* symbolName != * pureName)
		{
			return false;			
		}
		symbolName++;
		pureName++;
		if ((* pureName == 0) && *symbolName == '@')
			return true;
	}
	return false;
}

bool getSymbolByPureName(struct LinkInfo * plinkInfo, char * symbolPureName, struct COFF_Symbol * pSymbol)
{	
	for (int i = 0; i < plinkInfo->fileHdr->ulNumSymbol; i++)
	{
		* pSymbol = plinkInfo->symbolTable[i];
		char * symbolName;
		bool isShort = false;
		if (pSymbol->name.longName.ulZero == 0)
			symbolName = GET_STR_ENTRY(plinkInfo, pSymbol->name.longName.ulOffset);
		else
		{
			symbolName = pSymbol->name.cName;
			isShort = true;
		}
		if (comparePureName(symbolPureName, symbolName, isShort))
			return true;
	}
	return false;
}


void initLinkInfo(struct LinkInfo * plinkInfo, char * objFileName)
{
	memset(plinkInfo, 0, sizeof(struct LinkInfo));
	FILE *fp = fopen(objFileName, "rb");
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	plinkInfo->fileContent = (char *)malloc(size);
	fseek(fp, 0, SEEK_SET);
	char * buf = plinkInfo->fileContent;
	int count = 0;
	while (!feof(fp))
	{
		if (fread(buf + count, 1, 1, fp) != 1)
		{
			if (count == size)
				break;
			else
				fatalError("cannot read whole file\n");
		}
		count++;
	}
	plinkInfo->fileHdr = (struct COFF_FileHdr *)plinkInfo->fileContent;
	initStrTb(plinkInfo);
	initSymbolTb(plinkInfo);
	initSectionHdrs(plinkInfo);
	fclose(fp);
}

void freeProgram(struct Program * prg)
{
	free(prg->wholeProgram);
	free(prg);
}

void relocateSection(struct LinkInfo * plinkInfo, struct COFF_SectionHdr * sectionHdr)
{
	int relocationCount = sectionHdr->ulNumRel;
	char * sectionStart = plinkInfo->fileContent + sectionHdr->ulSecOffset;
	if (relocationCount > 0)//if has relocation
	{
		struct COFF_Relocation * relocationTb = (struct COFF_Relocation *)(plinkInfo->fileContent + sectionHdr->ulRelOffset);
		for(int i = 0; i < relocationCount; i++)
		{
			char * symbolAddress;
			char * relocationAddress = sectionStart + relocationTb[i].ulAddr;
			int symbolIndex = relocationTb[i].ulSymbol;
			struct COFF_Symbol * relSymbol = plinkInfo->symbolTable + symbolIndex;
			if (relSymbol->usClass != IMAGE_SYM_CLASS_EXTERNAL)
				fatalError("i do not know how to relocate non IMAGE_SYM_CLASS_EXTERNAL symbol\n");
			char * sectionOfRel = GET_SECTION_ADDRESS(plinkInfo, relSymbol->iSection - 1);
			symbolAddress = sectionOfRel +  relSymbol->ulValue;
			if (relSymbol->usType == COFF_SYMBOL_TYPE_FUNC)
			{							
				if (relocationTb[i].usType == IMAGE_REL_I386_REL32)
					symbolAddress = (char *)(symbolAddress - (relocationAddress + 4));//the offset of  call instruct.				
			}
			else
			{				
				if (relocationTb[i].usType != IMAGE_REL_I386_DIR32)
					fatalError("i do not know how to relative relocate a symbol not function\n");
			}
			*((void **)relocationAddress) = symbolAddress;//relocate the address
		}		
	}	
}

void relocateTextSections(struct LinkInfo * plinkInfo)
{
	int n = plinkInfo->fileHdr->usNumSec;
	for (int i = 0; i < n; i++)
	{
		if ((plinkInfo->sectionHdrs[i].ulFlags & IMAGE_SCN_CNT_CODE) == IMAGE_SCN_CNT_CODE)
		{
			relocateSection(plinkInfo, &plinkInfo->sectionHdrs[i]);
		}
	}
}

struct Program *  linkFunction(char * funcName, char * objFileName)
{
	struct LinkInfo info;	
	initLinkInfo(&info, objFileName);	
	struct COFF_Symbol symbol;
	if (!getSymbolByPureName(&info, funcName, &symbol))
		fatalError("cannot get function\n");
	if (symbol.usType != COFF_SYMBOL_TYPE_FUNC)
		fatalError("this symbol is not function\n");	
	if (symbol.usClass != IMAGE_SYM_CLASS_EXTERNAL)
		fatalError("i do not know how to relocate non IMAGE_SYM_CLASS_EXTERNAL symbol\n");
	char * codeStart = GET_SECTION_ADDRESS((&info), symbol.iSection - 1) + symbol.ulValue;
	relocateTextSections(&info);
	struct Program * prg = (struct Program *)malloc(sizeof(struct Program));
	prg->entry = codeStart;
	prg->wholeProgram = info.fileContent;	
	return prg;
}