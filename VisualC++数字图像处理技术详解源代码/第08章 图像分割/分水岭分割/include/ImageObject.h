// ImageObject.h: interface for the CImageObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEOBJECT_H__5C19DEAE_E21C_4E61_B021_E3A699B4717B__INCLUDED_)
#define AFX_IMAGEOBJECT_H__5C19DEAE_E21C_4E61_B021_E3A699B4717B__INCLUDED_

#include "ImageErrors.h" 


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

 
#define JGPT_POPULARITY_PALETTE 0
#define JGPT_MEDIAN_CUT_PALETTE 1
#define JGPT_FIXED_PALETTE      2

#define GET_RGB_16( a, b, c, d ) { WORD *wData = (WORD *) d; a = (unsigned char) ( ( (*wData) & 0x7c00 ) >> 7 ); b = (unsigned char) ( ( (*wData) & 0x03e0 ) >> 2 ); c = (unsigned char) ( ( (*wData) & 0x001f ) << 3 ); }
//#define GET_RGB_565( a, b, c, d ) { WORD *wData = (WORD *) d; a = (unsigned char) ( ( (*wData) & 0xf800 ) >> 8 ); b = (unsigned char) ( ( (*wData) & 0x07e0 ) >> 3 ); c = (unsigned char) ( ( (*wData) & 0x001f ) << 3 ); }
#define GET_RGB_32( a, b, c, d ) { DWORD *dwData = (DWORD *) d; a = (unsigned char) ( (*dwData) >> 16 ); b = (unsigned char) ( ( (*dwData) & 0x0000ff00 ) >> 8 ); c = (unsigned char) ( (*dwData) & 0x000000ff ); }
#define PUT_RGB_16( a, b, c, d ) { WORD *wData = (WORD *) d; *wData = ( ( ( (WORD) a & 0x00f8 ) << 7 ) | ( ( (WORD) b & 0x00f8 ) << 2 ) | ( (WORD) c >> 3 ) ); }
//#define PUT_RGB_565( a, b, c, d ) { WORD *wData = (WORD *) d; *wData = ( ( ( (WORD) a & 0x00f8 ) << 8 ) | ( ( (WORD) b & 0x00fc ) << 3 ) | ( (WORD) c >> 3 ) ); }
#define PUT_RGB_32( a, b, c, d ) { DWORD *dwData = (DWORD *) d; *dwData = ( (DWORD) a << 16 ) | ( (DWORD) b << 8 ) | (DWORD) c; }

#define _RGB( r, g, b ) ( WORD )( ( ( b ) & ~7 ) << 7 ) | ( ( ( g ) & ~7 ) << 2 ) | ( ( r ) >> 3 )

struct ImageINFO
{
	int nWidth;
	int nHeight;
	int nPlanes;
	int nColorDepth;
	int nColorNumber;
};

class CImageObject : public CObject
{
  DECLARE_DYNCREATE( CImageObject ) 

public:
  // Constructors and destructor
	CImageObject();
	CImageObject( const char *pszFileName, CDC *pDC = NULL, int nX = -1, int nY = -1 );
	~CImageObject();
	void operator=( const CImageObject &ImageObject );
  
	// Error handler
	int GetLastError( void );
	
	// Load and save member functions
//	bool GetImageInfo( const char *pszFileName, int *pnWidth = NULL, int *pnHeight = NULL, int *pnPlanes = NULL, int *pnBitsPerPixel = NULL, int *pnNumColors = NULL );
	bool GetImageInfo( ImageINFO *gi );
	ImageINFO *GetImageInfo( void );
	bool LoadFromFile( const char *pszFileName, CDC *pDC = NULL, int nX = -1, int nY = -1 );
    bool SaveToFile( const char *pszFileName, int nType = -1 );
	bool IsLoaded( void );

//-----------------------------------------------------------------------------------------
// The following 10 functions were designed by maple 
	
	// LoadDIB to buf and Update the DIB                       
	bool LoadDIBToBuf(BYTE * buf);                           
	bool UpdateDIB(int nNewWidth,int nNewHeight,BYTE * buf);   

   	// Get | Set pixel color
	RGBQUAD GetPixelColor(int x,int y);                        
	void SetPixelColor(int x,int y,RGBQUAD color);	
    
	// create a new image by color bits,create a 24 bits image  
	bool CreateDIBFromBits(int nWidth,int nHeight,BYTE * buf);  
    
	// Convert a standard hDIB to the DIB used in this class
	bool CreateFromHANDLE(HANDLE handle);                      
                                                            

    // 用于专门处理256色灰度图象  date:2004.3.29
	bool LoadDIBToBuf2(BYTE * buf);
    bool UpdateDIB2(int nNewWidth,int nNewHeight,BYTE * buf);
    BYTE GetPixelColor2(int x,int y);
    void SetPixelColor2(int x,int y,BYTE color);


//-----------------------------------------------------------------------------------------

	// Draw member function
	virtual bool Draw( CDC *pDC, int nX = -1, int nY = -1 );
    virtual bool DrawTo(CDC *pDC,int nDestX = -1,int nDestY = -1,int nDestWidth=0,int nDestHeight=0);
    bool DrawToHdc(HDC pHdc, int nDestX ,int nDestY ,int nDestWidth,int nDestHeight);

	// Palette member functions
	bool GetPaletteData( RGBQUAD *pRGBPalette );
	RGBQUAD *GetPaletteData( void );
	bool SetPalette( CDC *pDC );
	void SetPaletteCreationType( int nType );
	int GetPaletteCreationType( void );

  // Get/set member functions
    char *GetImageName();
//	int GetImageType( const char *pszFileName );
	int GetImageType( void );
	int GetWidth( void );
	int GetHeight( void );
	int GetBits( void );
	int GetColors( void );
	BYTE * GetDibPoint(void);
	long GetSize();
	int GetDestX();
	int GetDestY();

	HGLOBAL GetDib( void );
	void SetDib( HGLOBAL hDib );
	void SetQuality( int nQuality );

	bool SetColorDepth( int nColorDepth );

// Diagnostics and dump member functions, overrided
#ifdef _DEBUG
	virtual void Dump( CDumpContext& dc ) const;
    virtual void AssertValid() const;
#endif

//protected:
private:
	// LoadFromFile invokes these two functions, and SetResolution only the former.
	void LoadImageHeader( void );
	void LoadPalette( void );
	void DestroyImage( void );

	LOGPALETTE *GetLogPalette( void );
	void SetLogPalette( LOGPALETTE *pLogPalette );
	LOGPALETTE *CreateLogPalette( RGBQUAD *pPalette, int nNumColors );
	CPalette *GetPalette( void );
	int GetPaletteInBytes( void );
	void *GetDIBPointer( int *nWidthBytes = NULL, int nNewBits = 0, int *nNewWidthBytes = NULL, int nNewWidth = -1 );
	void CreatePaletteFromDIB( RGBQUAD *pRGBPalette, int nColors );
	LOGPALETTE *CreatePaletteFromBitmap( int nColors, unsigned char *pBits, int nBits, int nWidth, int nHeight );
	RGBQUAD *GeneratePopularityPalette( int nColors, unsigned char *pBits, int nBits, int nWidth, int nHeight );
	RGBQUAD *GenerateMedianCutPalette( int nColors, unsigned char *pBits, int nBits, int nWidth, int nHeight );
	RGBQUAD *GenerateFixedPalette( int nColors );

private:
	int GetWidthInBytes( int nBits, int nWidth );
	int GetExtensionIndex( const char *pszFileName );
    int GetNearestIndex( unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, RGBQUAD *pRGBPalette, int nNumColors );

	// the following 4 functions was designed to  get/set pixel color
	RGBQUAD GetPaletteColor(BYTE idx);
	BYTE GetPixelIndex(int x,int y);
	void SetPixelIndex(int x,int y,BYTE index);
	BYTE GetNearestIndex(RGBQUAD color);

	BOOL SaveBMP(const char *, HGLOBAL );





public:
	static char *pszExtensions[7];  // Image file filter

private:
	int m_nScreenPlanes;
	int m_nScreenBits;
	int m_nPaletteInBytes;
	int m_nLastError;  // Store the last error number

protected:
	char *m_pszFileName;  // Image file name
	int m_nImageType;     // Image type
	HGLOBAL m_hDib;       // DIB handler of the Image 
	BYTE * m_pDib;        // buf for DIB
	CPalette m_Palette;   // Palette used by the Image
	LOGPALETTE *m_pLogPalette;  // Logical Palette used by the Image
	int m_nPaletteCreationType;  // Palette creation type: 0 for JGPT_POPULARITY_PALETTE, 1 for JGPT_MEDIAN_CUT_PALETTE, 2 for JGPT_FIXED_PALETTE
	int m_nWidth, m_nHeight, m_nPlanes, m_nBits, m_nColors;  // Information of the Image
	int m_nX, m_nY;  // Position when drawing the Image
	int m_nDestWidth,m_nDestHeight;  // The destination image size of the image
	int m_nQuality;  // Only for JPG Images
    long m_lBufSize;

};




#endif // !defined(AFX_IMAGEOBJECT_H__5C19DEAE_E21C_4E61_B021_E3A699B4717B__INCLUDED_)
