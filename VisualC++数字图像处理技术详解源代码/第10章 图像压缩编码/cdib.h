// cdib.h 对CDib类中数据和函数进行进行声明

#ifndef _INSIDE_VISUAL_CPP_CDIB
#define _INSIDE_VISUAL_CPP_CDIB

class CDib : public CObject
{
public:
	enum Alloc {noAlloc, crtAlloc, heapAlloc};	// 枚举类型，表示内存分配的状况
	DECLARE_SERIAL(CDib)
public:
	LPVOID m_lpvColorTable;				// 调色板指针
	HBITMAP m_hBitmap;				// BITMAP结构指针
	LPBYTE m_lpImage;				// DIB位图数据块地址
	LPBITMAPINFOHEADER m_lpBMIH;			// DIB信息头指针
	HGLOBAL m_hGlobal;				// 全局的句柄，用于内存映射文件中	                   
	Alloc m_nBmihAlloc;				// 表示信息头内存分配的状况
	Alloc m_nImageAlloc;				// 表示位图数据分配的状况
	DWORD m_dwSizeImage;				// DIB位图中的字节数（信息头和调色板数据除外）
	int m_nColorTableEntries;			// 调色板表项数
	
	HANDLE m_hFile;					// 文件句柄
	HANDLE m_hMap;					// 内存映射文件句柄
	LPVOID m_lpvFile;				// 文件句柄
	HPALETTE m_hPalette;				// 调色板句柄
public:
	RGBQUAD GetPixel(int x, int y);			// 获取象素真实的颜色值

	LONG GetPixelOffset(int  x, int y);		// 获取象素在图象数据块中的位置

	CSize GetDibSaveDim();				// 获取DIB位图数据块的存储尺寸

	BOOL IsEmpty();					// 判断DIB是否为空

	WORD PaletteSize();				// 计算调色板的表项数

	CDib();						// 构造函数

	CDib(CSize size, int nBitCount);		// 根据指定的位图尺寸和象素位数来构造CDib实例

	~CDib();					// 析构函数

	int GetSizeImage() {return m_dwSizeImage;}	// 获取DIB位图中数据的字节数
	
	int GetSizeHeader()				// 获取信息头和调色板的尺寸
		{return sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;}

	CSize GetDimensions();				// 获取以象素表示的DIB的宽度和高度

	// 以读模式打开内存映射文件，并将其与CDib对象进行关联]
	BOOL AttachMapFile(const char* strPathname, BOOL bShare = FALSE);	
	
	BOOL CopyToMapFile(const char* strPathname);	//创建一个新的内存映射文件，并进行数据的复制

	// 用内存中的DIB与已有的CDib对象关联
	BOOL AttachMemory(LPVOID lpvMem, BOOL bMustDelete = FALSE, HGLOBAL hGlobal = NULL);

	// 将CDib对象按照指定的尺寸输出到显示器（或者打印机）
	BOOL Draw(CDC* pDC, CPoint origin, CSize size);  // until we implemnt CreateDibSection

	// 创建一个DIB短，图象内存将不被初始化
	HBITMAP CreateSection(CDC* pDC = NULL);

	// 将CDib对象的逻辑调色板选入设备上下文，然后实现该调色板
	UINT UsePalette(CDC* pDC, BOOL bBackground = FALSE);

	// 如果调色板存在的话，读取调色板，并创建一个Windows调色板
	BOOL MakePalette();

	// 如果16bpp、24bpp或32bppDIB不举杯调色板，则该函数可以为CDib对象创建一个逻辑调色板，
	// 它与由CreatehalftonePalette函数返回的调色板相匹配。如果程序在256色调色板显示器上
	// 运行，而你又没有调用SetSystemPalette，那么，你将不具有任何调色板，只有20中标准的
	// Windows颜色出现在DIB中
 	BOOL SetSystemPalette(CDC* pDC);

	// 将DIB重新生成为压缩的或者不压缩的DIB。
	BOOL Compress(CDC* pDC, BOOL bCompress = TRUE); 

	// 从以后的DIB中创建DDB位图，实现DIB到DDB的转换
	HBITMAP CreateBitmap(CDC* pDC);

	// 从DDB中创建DIB，实现DDB到DIB的转换
	BOOL ConvertFromDDB(HBITMAP hBitmap, HPALETTE hPal);

	// 从文件中读取数据，并填充文件头、信息头、调色板和位图数据
	BOOL Read(CFile* pFile);

	// 从BMP文件中读取信息头，调用CreateDIBSection来分配位图数据内存，然后将位图从该文件读入刚才分配的内存
	BOOL ReadSection(CFile* pFile, CDC* pDC = NULL);

	// 将DIB从CDib对象写入文件
	BOOL Write(CFile* pFile);

	// 串行化过程
	void Serialize(CArchive& ar);

	// 清空DIB，释放已经分配的内存，并且关闭映射文件
	void Empty();

	// 计算调色板的尺寸
	void ComputePaletteSize(int nBitCount);
private:
	// 断开映射文件的关联
	void DetachMapFile();
	
	// 计算调色板和位图尺寸
	void ComputeMetrics();
};
#endif // _INSIDE_VISUAL_CPP_CDIB
