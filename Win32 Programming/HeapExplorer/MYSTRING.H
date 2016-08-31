class CMyData {
    public:
        CMyData();
        ~CMyData();
        void setString(LPCTSTR s);
        LPVOID operator new(size_t size);
        LPVOID operator new(size_t size, LPCSTR filename, int line);
        void operator delete(LPVOID s);
        LPCTSTR getString();
        // These are for debugging and demo purposes
        static HANDLE getHeapHandle();
        static BOOL setAutoDeallocate(BOOL automode);
        static BOOL getAutoDeallocate();
    private:
        static DWORD count;
        static HANDLE heap;
        static BOOL auto_deallocate;
        LPCTSTR s;
                                 };
