#pragma pack(2)
typedef struct {
    DWORD   key;
    WORD  hmf;          // Unused "HANDLE" in Win16 is 16 bit value
    SMALL_RECT    bbox;
    WORD    inch;
    DWORD   reserved;
    WORD    checksum;
} METAFILEHEADER;
#pragma pack()

