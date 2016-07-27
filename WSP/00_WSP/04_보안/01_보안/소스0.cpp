//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
//  [ TIP ] 파티션 정보 얻기 
//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓


#include <windows.h>

#include <stdio.h>

 

// 파티션 table 모양

typedef struct _PARTITION

{

        char active;  // 부팅 가능한가

        char begin[3];// CHS 모드 에서만 사용

        char type;    // 파티션 종류

        char end[3];  // CHS 모드에서 사용

 

        int start;    // LBA 모드에서의 파티션이 시작되는 sector 번호

        int size;     // LBA 모드에서 파티션의 크기

} PARTITION;

 

void print(PARTITION *p)

{
        static int n = 1;

 

        printf("[PARTITION %d]\n", n++);

        printf("Start : %d\n", p->start);

        printf("Size : %d MB\n", p->size / 1024*512 / 1024);

        printf("Bootable : %x\n", p->active & 0x00ff); 

                                              // 0x80 : 부팅 가능

                                              // 0x00 : 부팅 불가능

        printf("----------------\n");
}

 

 

//-----------------------------------------------------------------

void main()

{

        // 하드디스크의 1번째 섹터를 물리적으로 읽어온다

        HANDLE hFile = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_READ,

                                    FILE_SHARE_READ | FILE_SHARE_WRITE,

                                    0, OPEN_EXISTING,

                                    FILE_ATTRIBUTE_NORMAL, 0);

 

        if(hFile == INVALID_HANDLE_VALUE)

        {

                printf("실패\n");

                return ;

        }

        //-------------------------------------------------------------

        char buf[512] = {0};

        DWORD len;

 

        BOOL b = ReadFile(hFile, buf, 512, &len, 0);

 

        PARTITION *p = (PARTITION*)(buf + 0x1BE);  // 파티션 table이 있는

                                                   // 곳으로 이동

 

        // Table의 내용을 출력해준다.

        for(; p->size != 0; ++p)

        {

                print(p);

        }

}
