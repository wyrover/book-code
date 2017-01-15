#include <iostream>
#include <Windows.h>
#include <sstream>
#include <conio.h>

bool DumpPEFileInfo(const char* filename);
void ShowStructImageDosHeader(FILE* pFile , const _IMAGE_DOS_HEADER& header);
void ShowStructImageNTHeader(FILE* pFile , const IMAGE_NT_HEADERS32& header);
void ShowStructImageFileHeader(FILE* pFile ,  const IMAGE_FILE_HEADER& header);
void ShowStructImageOptionalHeader(FILE* pFile , const IMAGE_OPTIONAL_HEADER& header);
void ShowStructImageSectionHeader(FILE* pFile , const IMAGE_SECTION_HEADER& header) ;
std::string AnalysisFileHeaderMachineFlag(const unsigned int flag);
std::string AnalysisFileHeaderCharacteristics(const unsigned int flag);
std::string AnalysisSectionHeaderCharacteristics(const unsigned int flag);
std::string AnalysisImageOptionalHeaderSubSystem(const unsigned int subSys);
std::string AnalysisImageOptionalHeaderDllCharacteristics(const unsigned int flag);

//int WINAPI WinMain(HINSTANCE, HINSTANCE, char *, int cmdShow)
int main()
{
    WIN32_FIND_DATAA win32FindData;
    memset(&win32FindData , 0 , sizeof(win32FindData));
    HANDLE hFind = FindFirstFileA("*.*", &win32FindData);

    if (INVALID_HANDLE_VALUE  != hFind) {
        do {
            if (win32FindData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
                if (true == DumpPEFileInfo(win32FindData.cFileName)) {
                    std::cout << "成功导出\"" << win32FindData.cFileName << "\"的PE信息..." << std::endl;
                }
            }
        } while (TRUE == FindNextFileA(hFind , &win32FindData));

        FindClose(hFind);
        hFind = NULL;
    }

    std::cout << "按任意键退出..." << std::endl;
    _getch();
    return 0;
}

bool DumpPEFileInfo(const char* filename)
{
    IMAGE_DOS_HEADER  imageDosHeader; //待读取的ImageDos头
    IMAGE_NT_HEADERS imageNTHeader;    //待读取的ImageNT头
    memset(&imageDosHeader , 0 , sizeof(imageDosHeader));
    memset(&imageNTHeader , 0 , sizeof(imageNTHeader));
    //尝试打开PE文件
    FILE* pPEFile = NULL;
    errno_t err = fopen_s(&pPEFile , filename , "rb");

    if (0 != err) {
        //std::cout<<"文件\""<<filename<<"\"不存在!"<<std::endl;
        return false;
    }

    size_t bytesRead = fread_s(&imageDosHeader , sizeof(imageDosHeader) , 1 , sizeof(imageDosHeader) , pPEFile);

    if (bytesRead != sizeof(imageDosHeader) ||
        imageDosHeader.e_magic  != 0x5A4D) {
        //std::cout<<"文件 \""<<filename<<"\" 中的 IMAGE_DOS_HEADER 结构破损!"<<std::endl;
        fclose(pPEFile);
        pPEFile = NULL;
        return 0;
    }

    //定位到NT头所在位置
    int iRes = fseek(pPEFile , imageDosHeader.e_lfanew , SEEK_SET);

    if (0 != iRes) {
        //std::cout<<"文件破损!无法定位到 IMAGE_NT_HEADERS32 结构体!"<<std::endl;
        fclose(pPEFile);
        pPEFile = NULL;
        return false;
    }

    bytesRead =
        fread_s(&imageNTHeader , sizeof(imageNTHeader) , 1 , sizeof(imageNTHeader) , pPEFile);
    std::ostringstream strBumpFileName;
    strBumpFileName << filename << "_" << "PE_Info_Dump" << ".txt";
    FILE* pDumpFile = NULL;
    err = fopen_s(&pDumpFile , strBumpFileName.str().c_str() , "wt");

    if (0 != err) {
        //std::cout<<"文件\""<<strBumpFileName.str().c_str()<<"\"无法打开!"<<std::endl;
        fclose(pPEFile);
        pPEFile = NULL;
        return false;
    }

    //展示_IMAGE_DOS_HEADER结构体
    ShowStructImageDosHeader(pDumpFile ,  imageDosHeader);
    ShowStructImageNTHeader(pDumpFile , imageNTHeader);
    const int iSectionNum = imageNTHeader.FileHeader.NumberOfSections;

    for (int iSec = 0 ; iSec < iSectionNum ; iSec++) {
        IMAGE_SECTION_HEADER imageSectionHeader;
        memset(&imageSectionHeader , 0 , sizeof(imageSectionHeader));
        bytesRead =
            fread_s(&imageSectionHeader , sizeof(imageSectionHeader) , 1 , sizeof(imageSectionHeader) , pPEFile);
        fprintf_s(pDumpFile  , "\n\n[Section %d]\n" , iSec);
        ShowStructImageSectionHeader(pDumpFile ,  imageSectionHeader);
    }

    //关闭PE文件
    fclose(pPEFile);
    pPEFile = NULL;
    fclose(pDumpFile);
    pDumpFile = NULL;
    return true;
}

void ShowStructImageDosHeader(FILE* pFile , const _IMAGE_DOS_HEADER& header)
{
    fprintf_s(pFile , "\n\n[Image_Dos_Header]\n");
    fprintf_s(pFile , "e_magic = 0x%.4x\n" , (unsigned int)header.e_magic);
    fprintf_s(pFile , "e_cblp = %d\n" , (unsigned int)header.e_cblp);
    fprintf_s(pFile , "e_cp = %d\n" , (unsigned int)header.e_cp);
    fprintf_s(pFile , "e_crlc = %d\n" , (unsigned int)header.e_crlc);
    fprintf_s(pFile , "e_cparhdr = %d\n" , (unsigned int)header.e_cparhdr);
    fprintf_s(pFile , "e_minalloc = %d\n" , (unsigned int)header.e_minalloc);
    fprintf_s(pFile , "e_maxalloc = %d\n" , (unsigned int)header.e_maxalloc);
    fprintf_s(pFile , "e_ss =  0x%.4x\n" , (unsigned int)header.e_ss);
    fprintf_s(pFile , "e_sp =  0x%.4x\n" , (unsigned int)header.e_sp);
    fprintf_s(pFile , "e_csum =  0x%.4x\n" , (unsigned int)header.e_csum);
    fprintf_s(pFile , "e_ip = 0x%.4x\n" , (unsigned int)header.e_ip);
    fprintf_s(pFile , "e_cs = 0x%.4x\n" , (unsigned int)header.e_cs);
    fprintf_s(pFile , "e_lfarlc = 0x%.4x\n" , (unsigned int)header.e_lfarlc);
    fprintf_s(pFile , "e_ovno = 0x%.4x\n" , (unsigned int)header.e_ovno);
    fprintf_s(pFile , "e_lfarlc = %d\n" , (unsigned int)header.e_lfarlc);
    fprintf_s(pFile , "e_lfanew = %d\n" , (unsigned int)header.e_lfanew);
};

void ShowStructImageNTHeader(FILE* pFile , const IMAGE_NT_HEADERS& header)
{
    fprintf_s(pFile , "\n\n[Image_NT_Header]\n");
    fprintf_s(pFile , "Signature = 0x%.8x\n" , (unsigned int)header.Signature);
    ShowStructImageFileHeader(pFile , header.FileHeader);
    ShowStructImageOptionalHeader(pFile , header.OptionalHeader);
}

void ShowStructImageFileHeader(FILE* pFile , const IMAGE_FILE_HEADER& header)
{
    fprintf_s(pFile , "\n\n[Image_File_Header]\n");
    fprintf_s(pFile , "Machine = %s\n" , AnalysisFileHeaderMachineFlag(header.Machine).c_str());
    fprintf_s(pFile , "NumberOfSections = %d\n" , (unsigned int)header.NumberOfSections);
    fprintf_s(pFile , "TimeDateStamp = %d\n" , (unsigned int)header.TimeDateStamp);
    fprintf_s(pFile , "PointerToSymbolTable = %d\n" , (unsigned int)header.PointerToSymbolTable);
    fprintf_s(pFile , "NumberOfSymbols = %d\n" , (unsigned int)header.NumberOfSymbols);
    fprintf_s(pFile , "SizeOfOptionalHeader = %d\n" , (unsigned int)header.SizeOfOptionalHeader);
    fprintf_s(pFile , "Characteristics =  %s\n" , AnalysisFileHeaderCharacteristics(header.Characteristics).c_str());
}

void ShowStructImageOptionalHeader(FILE* pFile , const IMAGE_OPTIONAL_HEADER& header)
{
    fprintf_s(pFile , "\n\n[Image_Optional_Header]\n");
    fprintf_s(pFile , "Magic = 0x%.4x\n" , (unsigned int)header.Magic);
    fprintf_s(pFile , "MajorLinkerVersion = %d\n" , (unsigned int)header.MajorLinkerVersion);
    fprintf_s(pFile , "MinorLinkerVersion = %d\n" , (unsigned int)header.MinorLinkerVersion);
    fprintf_s(pFile , "SizeOfCode = %d\n" , (unsigned int)header.SizeOfCode);
    fprintf_s(pFile , "SizeOfInitializedData = %d\n" , (unsigned int)header.SizeOfInitializedData);
    fprintf_s(pFile , "SizeOfUninitializedData = %d\n" , (unsigned int)header.SizeOfUninitializedData);
    fprintf_s(pFile , "AddressOfEntryPoint = 0x%.8x\n" , (unsigned int)header.AddressOfEntryPoint);
    fprintf_s(pFile , "BaseOfCode = 0x%.8x\n" , (unsigned int)header.BaseOfCode);
    fprintf_s(pFile , "BaseOfData = 0x%.8x\n" , (unsigned int)header.BaseOfData);
    fprintf_s(pFile , "ImageBase = 0x%.8x\n" , (unsigned int)header.ImageBase);
    fprintf_s(pFile , "SectionAlignment = %d\n" , (unsigned int)header.SectionAlignment);
    fprintf_s(pFile , "FileAlignment = %d\n" , (unsigned int)header.FileAlignment);
    fprintf_s(pFile , "MajorOperatingSystemVersion = %d\n" , (unsigned int)header.MajorOperatingSystemVersion);
    fprintf_s(pFile , "MinorOperatingSystemVersion = %d\n" , (unsigned int)header.MinorOperatingSystemVersion);
    fprintf_s(pFile , "MajorImageVersion = %d\n" , (unsigned int)header.MajorImageVersion);
    fprintf_s(pFile , "MinorImageVersion = %d\n" , (unsigned int)header.MinorImageVersion);
    fprintf_s(pFile , "MajorSubsystemVersion = %d\n" , (unsigned int)header.MajorSubsystemVersion);
    fprintf_s(pFile , "MinorSubsystemVersion = %d\n" , (unsigned int)header.MinorSubsystemVersion);
    fprintf_s(pFile , "Win32VersionValue = %d\n" , (unsigned int)header.Win32VersionValue);
    fprintf_s(pFile , "SizeOfImage = %d\n" , (unsigned int)header.SizeOfImage);
    fprintf_s(pFile , "SizeOfHeaders = %d\n" , (unsigned int)header.SizeOfHeaders);
    fprintf_s(pFile , "CheckSum = %d\n" , (unsigned int)header.CheckSum);
    fprintf_s(pFile , "Subsystem = %s\n" ,  AnalysisImageOptionalHeaderSubSystem((unsigned int)header.Subsystem).c_str());
    fprintf_s(pFile , "DllCharacteristics = %s\n" ,  AnalysisImageOptionalHeaderDllCharacteristics((unsigned int)header.DllCharacteristics).c_str());
    fprintf_s(pFile , "SizeOfStackReserve = %d\n" , (unsigned int)header.SizeOfStackReserve);
    fprintf_s(pFile , "SizeOfStackCommit = %d\n" , (unsigned int)header.SizeOfStackCommit);
    fprintf_s(pFile , "SizeOfHeapReserve = %d\n" , (unsigned int)header.SizeOfHeapReserve);
    fprintf_s(pFile , "SizeOfHeapCommit = %d\n" , (unsigned int)header.SizeOfHeapCommit);
    fprintf_s(pFile , "LoaderFlags = %d\n" , (unsigned int)header.LoaderFlags);
    fprintf_s(pFile , "NumberOfRvaAndSizes = %d\n" , (unsigned int)header.NumberOfRvaAndSizes);

    for (int i = 0 ; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES ; i++) {
        fprintf_s(pFile , "\t\t [Directory %d]:\n" , i);
        fprintf_s(pFile , "\t\t\t VirtualAddress = 0x%.8x\n" , header.DataDirectory[i].VirtualAddress);
        fprintf_s(pFile , "\t\t\t Size = %d\n" , header.DataDirectory[i].Size);
    }
}

void ShowStructImageSectionHeader(FILE* pFile , const IMAGE_SECTION_HEADER& header)
{
    fprintf_s(pFile , "Name = \"%s\"\n" , header.Name);
    fprintf_s(pFile , "Misc.VirtualSize = %d\n" , header.Misc.VirtualSize);
    fprintf_s(pFile , "VirtualAddress = 0x%.8x\n" , header.VirtualAddress);
    fprintf_s(pFile , "SizeOfRawData = %d\n" , header.SizeOfRawData);
    fprintf_s(pFile , "PointerToRawData = %d\n" , header.PointerToRawData);
    fprintf_s(pFile , "PointerToRelocations = %d\n" , header.PointerToRelocations);
    fprintf_s(pFile , "NumberOfRelocations = %d\n" , header.NumberOfRelocations);
    fprintf_s(pFile , "NumberOfLinenumbers = %d\n" , header.NumberOfLinenumbers);
    fprintf_s(pFile , "Characteristics = %s\n" , AnalysisSectionHeaderCharacteristics(header.Characteristics).c_str());
}

std::string AnalysisFileHeaderMachineFlag(const unsigned int flag)
{
    if (IMAGE_FILE_MACHINE_I386 == flag) {
        return "I386";
    } else if (IMAGE_FILE_MACHINE_IA64 == flag) {
        return "IA64";
    } else if (IMAGE_FILE_MACHINE_AMD64 == flag) {
        return "AMD64";
    }

    return "UNKNOWN";
}

std::string AnalysisFileHeaderCharacteristics(const unsigned int flag)
{
    std::string strFinalFlags;

    if (flag & IMAGE_FILE_RELOCS_STRIPPED) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_RELOCS_STRIPPED";
    }

    if (flag & IMAGE_FILE_EXECUTABLE_IMAGE) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_EXECUTABLE_IMAGE";
    }

    if (flag & IMAGE_FILE_LINE_NUMS_STRIPPED) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_LINE_NUMS_STRIPPED";
    }

    if (flag & IMAGE_FILE_LOCAL_SYMS_STRIPPED) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_LOCAL_SYMS_STRIPPED";
    }

    if (flag & IMAGE_FILE_AGGRESIVE_WS_TRIM) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_AGGRESIVE_WS_TRIM";
    }

    if (flag & IMAGE_FILE_LARGE_ADDRESS_AWARE) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_LARGE_ADDRESS_AWARE";
    }

    if (flag & IMAGE_FILE_BYTES_REVERSED_LO) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_BYTES_REVERSED_LO";
    }

    if (flag & IMAGE_FILE_32BIT_MACHINE) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_32BIT_MACHINE";
    }

    if (flag & IMAGE_FILE_DEBUG_STRIPPED) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_DEBUG_STRIPPED";
    }

    if (flag & IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP";
    }

    if (flag & IMAGE_FILE_NET_RUN_FROM_SWAP) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_NET_RUN_FROM_SWAP";
    }

    if (flag & IMAGE_FILE_SYSTEM) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_SYSTEM";
    }

    if (flag & IMAGE_FILE_DLL) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_DLL";
    }

    if (flag & IMAGE_FILE_UP_SYSTEM_ONLY) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_UP_SYSTEM_ONLY";
    }

    if (flag & IMAGE_FILE_BYTES_REVERSED_HI) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_FILE_BYTES_REVERSED_HI";
    }

    if (strFinalFlags.empty()) {
        return "0";
    }

    return strFinalFlags;
}

std::string AnalysisSectionHeaderCharacteristics(const unsigned int flag)
{
    std::string strFinalFlags;

    if (flag & IMAGE_SCN_TYPE_NO_PAD) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_TYPE_NO_PAD";
    }

    if (flag & IMAGE_SCN_CNT_CODE) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_CNT_CODE";
    }

    if (flag & IMAGE_SCN_CNT_INITIALIZED_DATA) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_CNT_INITIALIZED_DATA";
    }

    if (flag & IMAGE_SCN_CNT_UNINITIALIZED_DATA) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_CNT_UNINITIALIZED_DATA";
    }

    if (flag & IMAGE_SCN_LNK_OTHER) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_LNK_OTHER";
    }

    if (flag & IMAGE_SCN_LNK_INFO) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_LNK_INFO";
    }

    if (flag & IMAGE_SCN_LNK_REMOVE) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_LNK_REMOVE";
    }

    if (flag & IMAGE_SCN_LNK_COMDAT) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_LNK_COMDAT";
    }

    if (flag & IMAGE_SCN_NO_DEFER_SPEC_EXC) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_NO_DEFER_SPEC_EXC";
    }

    if (flag & IMAGE_SCN_GPREL) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_GPREL";
    }

    if (flag & IMAGE_SCN_MEM_PURGEABLE) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_MEM_PURGEABLE";
    }

    if (flag & IMAGE_SCN_MEM_LOCKED) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_MEM_LOCKED";
    }

    if (flag & IMAGE_SCN_MEM_PRELOAD) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_MEM_PRELOAD";
    }

    if (flag & IMAGE_SCN_ALIGN_1BYTES) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_ALIGN_1BYTES";
    }

    if (flag & IMAGE_SCN_ALIGN_2BYTES) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_ALIGN_2BYTES";
    }

    if (flag & IMAGE_SCN_ALIGN_4BYTES) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_ALIGN_4BYTES";
    }

    if (flag & IMAGE_SCN_ALIGN_8BYTES) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_ALIGN_8BYTES";
    }

    if (flag & IMAGE_SCN_ALIGN_16BYTES) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_ALIGN_16BYTES";
    }

    if (flag & IMAGE_SCN_ALIGN_32BYTES) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_ALIGN_32BYTES";
    }

    if (flag & IMAGE_SCN_ALIGN_64BYTES) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_ALIGN_64BYTES";
    }

    if (flag & IMAGE_SCN_ALIGN_128BYTES) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_ALIGN_128BYTES";
    }

    if (flag & IMAGE_SCN_ALIGN_256BYTES) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_ALIGN_256BYTES";
    }

    if (flag & IMAGE_SCN_ALIGN_512BYTES) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_ALIGN_512BYTES";
    }

    if (flag & IMAGE_SCN_ALIGN_1024BYTES) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_ALIGN_1024BYTES";
    }

    if (flag & IMAGE_SCN_ALIGN_2048BYTES) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_ALIGN_2048BYTES";
    }

    if (flag & IMAGE_SCN_ALIGN_4096BYTES) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_ALIGN_4096BYTES";
    }

    if (flag & IMAGE_SCN_ALIGN_8192BYTES) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_ALIGN_8192BYTES";
    }

    if (flag & IMAGE_SCN_LNK_NRELOC_OVFL) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_LNK_NRELOC_OVFL";
    }

    if (flag & IMAGE_SCN_MEM_DISCARDABLE) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_MEM_DISCARDABLE";
    }

    if (flag & IMAGE_SCN_MEM_NOT_CACHED) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_MEM_NOT_CACHED";
    }

    if (flag & IMAGE_SCN_MEM_NOT_PAGED) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_MEM_NOT_PAGED";
    }

    if (flag & IMAGE_SCN_MEM_SHARED) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_MEM_SHARED";
    }

    if (flag & IMAGE_SCN_MEM_EXECUTE) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_MEM_EXECUTE";
    }

    if (flag & IMAGE_SCN_MEM_READ) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_MEM_READ";
    }

    if (flag & IMAGE_SCN_MEM_WRITE) {
        if (!strFinalFlags.empty()) {
            strFinalFlags += "|";
        }

        strFinalFlags += "IMAGE_SCN_MEM_WRITE";
    }

    if (strFinalFlags.empty()) {
        return "0";
    }

    return strFinalFlags;
}

std::string AnalysisImageOptionalHeaderSubSystem(const unsigned int subSys)
{
    std::string strSubSys = "UNKNOWN";

    switch (subSys) {
    case IMAGE_SUBSYSTEM_UNKNOWN:
        break;

    case IMAGE_SUBSYSTEM_NATIVE:
        strSubSys = "Native";
        break;

    case IMAGE_SUBSYSTEM_WINDOWS_GUI:
        strSubSys = "WinGUI";
        break;

    case IMAGE_SUBSYSTEM_WINDOWS_CUI:
        strSubSys = "WinCUI";
        break;

    case IMAGE_SUBSYSTEM_OS2_CUI:
        strSubSys = "OS2CUI";
        break;

    case IMAGE_SUBSYSTEM_POSIX_CUI:
        strSubSys = "POSIX CUI";
        break;

    case IMAGE_SUBSYSTEM_NATIVE_WINDOWS:
        strSubSys = "Native Windows";
        break;

    case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
        strSubSys = "WinCE GUI";
        break;

    case IMAGE_SUBSYSTEM_EFI_APPLICATION:
        strSubSys = "EFI APP";
        break;

    case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
        strSubSys = "EFI Boot Service Driver";
        break;

    case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER:
        strSubSys = "EFI Runtime Driver";
        break;

    case IMAGE_SUBSYSTEM_EFI_ROM:
        strSubSys = "EFI Rom";
        break;

    case IMAGE_SUBSYSTEM_XBOX:
        strSubSys = "XBox";
        break;

    case IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION:
        strSubSys = "Win Boot App";
        break;

    default:
        break;
    }

    return strSubSys;
}

std::string AnalysisImageOptionalHeaderDllCharacteristics(const unsigned int flag)
{
    std::string strDllFlag;

    if (flag & IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE) {
        if (!strDllFlag.empty()) {
            strDllFlag += "|";
        }

        strDllFlag += "IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE";
    }

    if (flag & IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY) {
        if (!strDllFlag.empty()) {
            strDllFlag += "|";
        }

        strDllFlag += "IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY";
    }

    if (flag & IMAGE_DLLCHARACTERISTICS_NX_COMPAT) {
        if (!strDllFlag.empty()) {
            strDllFlag += "|";
        }

        strDllFlag += "IMAGE_DLLCHARACTERISTICS_NX_COMPAT";
    }

    if (flag & IMAGE_DLLCHARACTERISTICS_NO_ISOLATION) {
        if (!strDllFlag.empty()) {
            strDllFlag += "|";
        }

        strDllFlag += "IMAGE_DLLCHARACTERISTICS_NO_ISOLATION";
    }

    if (flag & IMAGE_DLLCHARACTERISTICS_NO_SEH) {
        if (!strDllFlag.empty()) {
            strDllFlag += "|";
        }

        strDllFlag += "IMAGE_DLLCHARACTERISTICS_NO_SEH";
    }

    if (flag & IMAGE_DLLCHARACTERISTICS_NO_BIND) {
        if (!strDllFlag.empty()) {
            strDllFlag += "|";
        }

        strDllFlag += "IMAGE_DLLCHARACTERISTICS_NO_BIND";
    }

    if (flag & IMAGE_DLLCHARACTERISTICS_WDM_DRIVER) {
        if (!strDllFlag.empty()) {
            strDllFlag += "|";
        }

        strDllFlag += "IMAGE_DLLCHARACTERISTICS_WDM_DRIVER";
    }

    if (flag & IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE) {
        if (!strDllFlag.empty()) {
            strDllFlag += "|";
        }

        strDllFlag += "IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE";
    }

    if (strDllFlag.empty()) {
        strDllFlag = "0";
    }

    return strDllFlag;
}



