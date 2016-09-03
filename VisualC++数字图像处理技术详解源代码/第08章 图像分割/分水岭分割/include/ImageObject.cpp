// ImageObject.cpp: implementation of the CImageObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageObject.h"
#include "ImageLoad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE( CImageObject, CObject ) 

char *CImageObject::pszExtensions[] = { ".bmp", ".gif", ".pcx", ".tga", ".jpg", ".tif", "" };

//////////////////////////////////////////////////
// Constructor and destructor
//
CImageObject::CImageObject()
{
	m_nWidth = m_nHeight = m_nBits = m_nColors = m_nImageType = 0;
	m_nX = m_nY = 0;
	m_nLastError = 0;
	m_hDib = NULL;
	m_nPaletteCreationType = JGPT_FIXED_PALETTE;
    m_pszFileName = NULL;
    m_pLogPalette = NULL;
	m_nQuality = 80;
	m_pDib = NULL;
	m_lBufSize = 0;
}

CImageObject::CImageObject( const char *pszFileName, CDC *pDC, int nX, int nY )
{
	m_nWidth = m_nHeight = m_nBits = m_nColors = m_nImageType = 0;
	m_nX = m_nY = 0;
	m_nLastError = 0;
	m_hDib = NULL;
	m_nPaletteCreationType = JGPT_FIXED_PALETTE;
	m_pszFileName = NULL;
	m_pLogPalette = NULL;
	m_nQuality = 80;
	m_pDib = NULL;
	m_lBufSize = 0;
	LoadFromFile( pszFileName, pDC, nX, nY );
}

void CImageObject::operator=( const CImageObject &ImageObject )
{
	DestroyImage();

	m_nLastError = ImageObject.m_nLastError;

    m_nWidth = ImageObject.m_nWidth;
	m_nHeight = ImageObject.m_nHeight;
	m_nPlanes = ImageObject.m_nPlanes;
	m_nBits = ImageObject.m_nBits;
	m_nColors = ImageObject.m_nColors;
	m_nImageType = ImageObject.m_nImageType;
	
	m_nX = ImageObject.m_nX;
	m_nY = ImageObject.m_nY;

	m_nScreenPlanes = ImageObject.m_nScreenPlanes;
	m_nScreenBits = ImageObject.m_nScreenBits;
	m_nPaletteInBytes = ImageObject.m_nPaletteInBytes;
	m_nQuality = ImageObject.m_nQuality;
	m_nPaletteCreationType = ImageObject.m_nPaletteCreationType;
	
	int nNumColors = m_nColors;
	int nWidthBytes = GetWidthInBytes( m_nBits, m_nWidth );
	
	if( ImageObject.m_hDib != NULL )
	{
		DWORD dwSize = ::GlobalSize( ImageObject.m_hDib );
		char *pData = ( char * )::GlobalLock( ImageObject.m_hDib );
		if( pData != NULL )
		{
			HGLOBAL hGlobal = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );
			if( hGlobal != NULL )
			{
				char *pDestData = ( char * )::GlobalLock( hGlobal );
				if( pDestData != NULL )
				{
					memcpy( pDestData, pData, dwSize );
					if( nNumColors != 0 )
						CreatePaletteFromDIB( ( RGBQUAD * )&pData[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)], nNumColors );
					else if( ImageObject.m_pLogPalette != NULL )
					{
						m_pLogPalette = (LOGPALETTE *) new char[ sizeof( LOGPALETTE ) + ImageObject.m_pLogPalette->palNumEntries * sizeof( PALETTEENTRY ) ];
						if( m_pLogPalette != NULL )
						{
							for( int i=0; i<ImageObject.m_pLogPalette->palNumEntries; i++ )
								m_pLogPalette[i] = ImageObject.m_pLogPalette[i];
							m_Palette.CreatePalette( m_pLogPalette );
						}
					}
					::GlobalUnlock( hGlobal );
					m_hDib = hGlobal;
				}
				else ::GlobalFree( hGlobal );
			}
			::GlobalUnlock( ImageObject.m_hDib );
		}
	}
	if( ImageObject.m_pszFileName != NULL )
	{
		m_pszFileName = new char [strlen( ImageObject.m_pszFileName ) + 1 ];
		strcpy( m_pszFileName, ImageObject.m_pszFileName );
	}
}

CImageObject::~CImageObject()
{
	if(m_pDib)
		::GlobalUnlock(m_hDib);
	if(m_hDib)
		::GlobalFree( m_hDib );
	if(m_pszFileName != NULL )
	{
		delete[] m_pszFileName;
		m_pszFileName = NULL;
	}
}

//////////////////////////////////////////
// Get/set member functions
//
int CImageObject::GetExtensionIndex( const char *pszFileName )
{
	int Index = 0;
	char *pszExt;
	pszExt = ( char * )&pszFileName[ strlen( pszFileName ) - 4 ];
	while( pszExtensions[ Index ][ 0 ] )
	{
		if( !stricmp( pszExt, pszExtensions[ Index ] ) ) 
			return( Index + 1 );
		Index++;
	}
	return( -1 );
}

int CImageObject::GetLastError( void )
{
	return( m_nLastError );
}

bool CImageObject::IsLoaded( void )
{
	return( m_hDib != NULL );
}

int CImageObject::GetWidth( void )
{
	return( m_nWidth );
}

int CImageObject::GetHeight( void )
{
	return( m_nHeight );
}

int CImageObject::GetBits( void )
{
	return( m_nBits );
}

int CImageObject::GetColors( void )
{
	return( m_nColors );
}

HGLOBAL CImageObject::GetDib( void )
{
	return( m_hDib );
}

void CImageObject::SetDib( HGLOBAL hDib )
{
	m_hDib = hDib;
}


long CImageObject::GetSize()
{
   return (long) sizeof(BITMAPFILEHEADER)
        +sizeof(BITMAPINFOHEADER)
		+m_nColors*sizeof(RGBQUAD)
		+GetWidthInBytes(m_nBits,m_nWidth)*m_nHeight;
}

int CImageObject::GetDestX()
{
	return m_nX;
}

int CImageObject::GetDestY()
{
	return m_nY;
}

BYTE * CImageObject::GetDibPoint(void)
{
	return(m_pDib);
}
// Palette
int CImageObject::GetPaletteInBytes( void )
{
	return( m_nPaletteInBytes );
}

CPalette *CImageObject::GetPalette( void )
{
	return( &m_Palette );
}

LOGPALETTE *CImageObject::GetLogPalette( void )
{
	return( m_pLogPalette );
}

void CImageObject::SetLogPalette( LOGPALETTE *pLogPalette )
{
	m_pLogPalette = pLogPalette;
}

void CImageObject::SetPaletteCreationType( int nType )
{
	m_nPaletteCreationType = nType;
}

int CImageObject::GetPaletteCreationType( void )
{
	return( m_nPaletteCreationType );
}

void CImageObject::SetQuality( int nQuality )
{
	m_nQuality = nQuality;
}

/*
int CImageObject::GetImageType( const char *pszFileName )
{
	return( ::FileType( pszFileName ) );
}
*/
char *CImageObject::GetImageName()
{
  return( m_pszFileName );
}

int CImageObject::GetImageType( void )
{
	return( m_nImageType );
}

//////////////////////////////////////////
// Get/set member functions
//
/*
bool CImageObject::GetImageInfo( const char *pszFileName, int *pnWidth,
	int *pnHeight, int *pnPlanes, int *pnBitsPerPixel, int *pnNumColors )
{
	int nImageType;
	nImageType = ::FileType( pszFileName );
	if( nImageType == 0 )
	{
		m_nLastError = GL_UNSUPPORTED_FILETYPE;
		return( false );
	}
	switch( nImageType )
	{
		case GT_BMP:
			if( ::GetBMPInfo( pszFileName, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) )
				return( true );
		  break;
		case GT_GIF:
			if( ::GetGIFInfo( pszFileName, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) )
				return( true );
		  break;
		case GT_JPG:
			if( ::GetJPGInfo( pszFileName, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) )
				return( true );
		  break;
		case GT_PCX:
			if( ::GetPCXInfo( pszFileName, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) )
				return( true );
		  break;
		case GT_TGA:
			if( ::GetTGAInfo( pszFileName, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) )
				return( true );
		  break;
		case GT_TIF:
			if( ::GetTIFInfo( pszFileName, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) )
				return( true );
		  break;
	}
	return( false );
}
*/
bool CImageObject::GetImageInfo( ImageINFO *gi )
{
	int nWidth, nHeight, nPlanes, nColorDepth, nColorNumber;

	if( m_nImageType == 0 )
	{
		m_nLastError = GL_UNSUPPORTED_FILETYPE;
		return( false );
	}
	switch( m_nImageType )
	{
		case GT_BMP:
			if( ::GetBMPInfo( m_pszFileName, &nWidth, &nHeight, &nPlanes, &nColorDepth, &nColorNumber ) )
			{
			  gi->nWidth = nWidth;
			  gi->nHeight = nHeight;
			  gi->nPlanes = nPlanes;
			  gi->nColorDepth = nColorDepth;
			  gi->nColorNumber = nColorNumber;
				return( true );
			}
		  break;
		case GT_GIF:
			if( ::GetGIFInfo( m_pszFileName, &nWidth, &nHeight, &nPlanes, &nColorDepth, &nColorNumber ) )
			{
			  gi->nWidth = nWidth;
			  gi->nHeight = nHeight;
			  gi->nPlanes = nPlanes;
			  gi->nColorDepth = nColorDepth;
			  gi->nColorNumber = nColorNumber;
				return( true );
			}
		  break;
		case GT_JPG:
			if( ::GetJPGInfo( m_pszFileName, &nWidth, &nHeight, &nPlanes, &nColorDepth, &nColorNumber ) )
			{
			  gi->nWidth = nWidth;
			  gi->nHeight = nHeight;
			  gi->nPlanes = nPlanes;
			  gi->nColorDepth = nColorDepth;
			  gi->nColorNumber = nColorNumber;
				return( true );
			}
		  break;
		case GT_PCX:
			if( ::GetPCXInfo( m_pszFileName, &nWidth, &nHeight, &nPlanes, &nColorDepth, &nColorNumber ) )
			{
			  gi->nWidth = nWidth;
			  gi->nHeight = nHeight;
			  gi->nPlanes = nPlanes;
			  gi->nColorDepth = nColorDepth;
			  gi->nColorNumber = nColorNumber;
				return( true );
			}
		  break;
		case GT_TGA:
			if( ::GetTGAInfo( m_pszFileName, &nWidth, &nHeight, &nPlanes, &nColorDepth, &nColorNumber ) )
			{
			  gi->nWidth = nWidth;
			  gi->nHeight = nHeight;
			  gi->nPlanes = nPlanes;
			  gi->nColorDepth = nColorDepth;
			  gi->nColorNumber = nColorNumber;
				return( true );
			}
		  break;
		case GT_TIF:
			if( ::GetTIFInfo( m_pszFileName, &nWidth, &nHeight, &nPlanes, &nColorDepth, &nColorNumber ) )
			{
			  gi->nWidth = nWidth;
			  gi->nHeight = nHeight;
			  gi->nPlanes = nPlanes;
			  gi->nColorDepth = nColorDepth;
			  gi->nColorNumber = nColorNumber;
				return( true );
			}
		  break;
	}
	return( false );
}

ImageINFO *CImageObject::GetImageInfo( void )
{
	int nWidth, nHeight, nPlanes, nColorDepth, nColorNumber;

	if( m_nImageType == 0 )
	{
		m_nLastError = GL_UNSUPPORTED_FILETYPE;
		return( false );
	}
	switch( m_nImageType )
	{
		case GT_BMP:
			if( ::GetBMPInfo( m_pszFileName, &nWidth, &nHeight, &nPlanes, &nColorDepth, &nColorNumber ) )
			{
      	ImageINFO *gi = new ImageINFO;
			  gi->nWidth = nWidth;
			  gi->nHeight = nHeight;
			  gi->nPlanes = nPlanes;
			  gi->nColorDepth = nColorDepth;
			  gi->nColorNumber = nColorNumber;
				return( gi );
			}
		  break;
		case GT_GIF:
			if( ::GetGIFInfo( m_pszFileName, &nWidth, &nHeight, &nPlanes, &nColorDepth, &nColorNumber ) )
			{
      	ImageINFO *gi = new ImageINFO;
			  gi->nWidth = nWidth;
			  gi->nHeight = nHeight;
			  gi->nPlanes = nPlanes;
			  gi->nColorDepth = nColorDepth;
			  gi->nColorNumber = nColorNumber;
				return( gi );
			}
		  break;
		case GT_JPG:
			if( ::GetJPGInfo( m_pszFileName, &nWidth, &nHeight, &nPlanes, &nColorDepth, &nColorNumber ) )
			{
      	ImageINFO *gi = new ImageINFO;
			  gi->nWidth = nWidth;
			  gi->nHeight = nHeight;
			  gi->nPlanes = nPlanes;
			  gi->nColorDepth = nColorDepth;
			  gi->nColorNumber = nColorNumber;
				return( gi );
			}
		  break;
		case GT_PCX:
			if( ::GetPCXInfo( m_pszFileName, &nWidth, &nHeight, &nPlanes, &nColorDepth, &nColorNumber ) )
			{
      	ImageINFO *gi = new ImageINFO;
			  gi->nWidth = nWidth;
			  gi->nHeight = nHeight;
			  gi->nPlanes = nPlanes;
			  gi->nColorDepth = nColorDepth;
			  gi->nColorNumber = nColorNumber;
				return( gi );
			}
		  break;
		case GT_TGA:
			if( ::GetTGAInfo( m_pszFileName, &nWidth, &nHeight, &nPlanes, &nColorDepth, &nColorNumber ) )
			{
      	ImageINFO *gi = new ImageINFO;
			  gi->nWidth = nWidth;
			  gi->nHeight = nHeight;
			  gi->nPlanes = nPlanes;
			  gi->nColorDepth = nColorDepth;
			  gi->nColorNumber = nColorNumber;
				return( gi );
			}
		  break;
		case GT_TIF:
			if( ::GetTIFInfo( m_pszFileName, &nWidth, &nHeight, &nPlanes, &nColorDepth, &nColorNumber ) )
			{
      	ImageINFO *gi = new ImageINFO;
			  gi->nWidth = nWidth;
			  gi->nHeight = nHeight;
			  gi->nPlanes = nPlanes;
			  gi->nColorDepth = nColorDepth;
			  gi->nColorNumber = nColorNumber;
				return( gi );
			}
		  break;
	}
	return( NULL );
}

// Change color depth of the Image
bool CImageObject::SetColorDepth( int nColorDepth )
{
	m_nLastError = GL_SUCCESS;
	if( nColorDepth == m_nBits )
		return( true );
	int nOldWidthInBytes, nNewWidthInBytes;
	char *pBuffer = ( char * )GetDIBPointer( &nOldWidthInBytes, nColorDepth, &nNewWidthInBytes );
	if( pBuffer == NULL )
		return( false );

	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	RGBQUAD *pOldRGBPalette, *pNewRGBPalette;
	unsigned char *pOldBits, *pNewBits;
	int nOldColors, nNewColors;
	pOldBFH = ( BITMAPFILEHEADER * )pBuffer;
	pOldBIH = ( BITMAPINFOHEADER * )&pBuffer[ sizeof( BITMAPFILEHEADER ) ];
	pOldRGBPalette = ( RGBQUAD * )&pBuffer[ sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) ];
	nOldColors = m_nColors;
	nNewColors = 1 << nColorDepth;
	if( nColorDepth > 8 )
		nNewColors = 0;
	pOldBits = ( unsigned char * )&pBuffer[ sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER) + nOldColors * sizeof( RGBQUAD ) ];
	if( m_nBits >= 16 && nColorDepth < 16 )
	{
		if( m_pLogPalette != NULL )
			delete[] m_pLogPalette;
		m_pLogPalette = CreatePaletteFromBitmap( nNewColors, pOldBits, m_nBits, m_nWidth, m_nHeight );
	}

	HGLOBAL hGlobal;
	DWORD dwSize;
	dwSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + nNewColors * sizeof( RGBQUAD ) + m_nHeight * nNewWidthInBytes;
	hGlobal = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );
	if( hGlobal == NULL )
	{
		::GlobalUnlock( m_hDib );
		return( false );
	}
	pBuffer = (char *) ::GlobalLock( hGlobal );
	if( pBuffer == NULL )
	{
		::GlobalFree( hGlobal );
		::GlobalUnlock( m_hDib );
		return( false );
	}

	pNewBFH = ( BITMAPFILEHEADER * )pBuffer;
	pNewBIH = ( BITMAPINFOHEADER * )&pBuffer[ sizeof( BITMAPFILEHEADER ) ];
	pNewRGBPalette = ( RGBQUAD * )&pBuffer[ sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) ];
	*pNewBFH = *pOldBFH;
	*pNewBIH = *pOldBIH;
	int i, j = nNewColors;
	if( m_nBits < 16 && nColorDepth < 16 )
	{
		for( i=0; i<j; i++ )
			pNewRGBPalette[i] = pOldRGBPalette[i];
	}
	else if( m_nBits >= 16 )
	{
 		for( i=0; i<j; i++ )
		{
			pNewRGBPalette[ i ].rgbRed = m_pLogPalette->palPalEntry[ i ].peRed;
			pNewRGBPalette[ i ].rgbGreen = m_pLogPalette->palPalEntry[ i ].peGreen;
			pNewRGBPalette[ i ].rgbBlue = m_pLogPalette->palPalEntry[ i ].peBlue;
		}
	}

	pNewBIH->biBitCount = nColorDepth;
	pNewBIH->biSizeImage = nNewWidthInBytes * m_nHeight;
	pNewBIH->biClrUsed = nNewColors;
	pNewBFH->bfSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + nNewColors * sizeof( RGBQUAD ) + pNewBIH->biSizeImage;
	pNewBFH->bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + nNewColors * sizeof( RGBQUAD );
	pNewBits = ( unsigned char * )&pBuffer[ sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + nNewColors * sizeof( RGBQUAD ) ];
	m_nPaletteInBytes = nNewColors * sizeof( RGBQUAD );
	for( int y=0; y<m_nHeight; y++ )
	{
		unsigned char ucRed, ucGreen, ucBlue;
		unsigned char *pSrc, *pDest;
		pSrc = pOldBits;
		pSrc += ( nOldWidthInBytes * y );
		pDest = pNewBits;
		pDest += ( nNewWidthInBytes * y );
		for( int x=0; x<m_nWidth; x++ )
		{
			switch( m_nBits )
			{
				case 1:
					if( pSrc[ x/8 ] & ( 0x80 >> ( x & 7 ) ) )
						ucRed = ucGreen = ucBlue = 0xff;
					else
						ucRed = ucGreen = ucBlue = 0x00;
					break;
				case 4:
					if( !( x & 1 ) )
					{
						ucRed = pOldRGBPalette[ pSrc[ x/2 ] >> 4 ].rgbRed;
						ucGreen = pOldRGBPalette[ pSrc[ x/2 ] >> 4 ].rgbGreen;
						ucBlue = pOldRGBPalette[ pSrc[ x/2 ] >> 4 ].rgbBlue;
					}
					else
					{
						ucRed = pOldRGBPalette[ pSrc[ x/2 ] & 15 ].rgbRed;
						ucGreen = pOldRGBPalette[ pSrc[ x/2 ] & 15 ].rgbGreen;
						ucBlue = pOldRGBPalette[ pSrc[ x/2 ] & 15 ].rgbBlue;
					}
					break;
				case 8:
					ucRed = pOldRGBPalette[ pSrc[ x ] ].rgbRed;
					ucGreen = pOldRGBPalette[ pSrc[ x ] ].rgbGreen;
					ucBlue = pOldRGBPalette[ pSrc[ x ] ].rgbBlue;
					break;
				case 16:
					GET_RGB_16( ucRed, ucGreen, ucBlue, &pSrc[ 2*x ] );
					break;
				case 24:
					ucRed = pSrc[ 3*x + 2 ];
					ucGreen = pSrc[ 3*x + 1 ];
					ucBlue = pSrc[ 3*x ];
					break;
				case 32:
					GET_RGB_32( ucRed, ucGreen, ucBlue, &pSrc[ 4*x ] );
					break;
			}
			switch( nColorDepth )
			{
				case 1:
					if( !( x & 7 ) )
						pDest[ x/8 ] = 0;
					pDest[ x/8 ] |= ( unsigned char )GetNearestIndex( ucRed, ucGreen, ucBlue, pNewRGBPalette, nNewColors ) << ( x & 7 );
					break;
				case 4:
					if( !( x & 1 ) )
						pDest[ x/2 ] = ( unsigned char )GetNearestIndex( ucRed, ucGreen, ucBlue, pNewRGBPalette, nNewColors ) << 4;
					else
						pDest[ x/2 ] |= ( unsigned char )GetNearestIndex( ucRed, ucGreen, ucBlue, pNewRGBPalette, nNewColors );
					break;
				case 8:
					pDest[ x ] = ( unsigned char )GetNearestIndex( ucRed, ucGreen, ucBlue, pNewRGBPalette, nNewColors );
					break;
				case 16:
					PUT_RGB_16( ucRed, ucGreen, ucBlue, &pDest[ 2*x ] );
					break;
				case 24:
					pDest[ 3*x + 2 ] = ucRed;
					pDest[ 3*x + 1 ] = ucGreen;
					pDest[ 3*x ] = ucBlue;
					break;
				case 32:
					PUT_RGB_32( ucRed, ucGreen, ucBlue, &pDest[ 4*x ] );
					break;
			}
		}
	}

	::GlobalUnlock( m_hDib );
	::GlobalFree( m_hDib );
	::GlobalUnlock( hGlobal );
	m_hDib = hGlobal;
	LoadImageHeader();
	return( true );
}

//////////////////////////////////////////
// Palette functions
//
bool CImageObject::SetPalette( CDC *pDC )
{
	m_nLastError = GL_HDIB_NULL;
	if( m_hDib == NULL )
		return( false );
	pDC->SelectPalette( &m_Palette, false );
	pDC->RealizePalette();
	m_nLastError = GL_SUCCESS;
	return( true );
}

bool CImageObject::GetPaletteData( RGBQUAD *pRGBPalette )
{
	m_nLastError = GL_HDIB_NULL;
	if( m_hDib == NULL )
		return( false );
	char *pTemp;
	pTemp = ( char * )::GlobalLock( m_hDib );
	m_nLastError = GL_MEMORY_LOCK_ERROR;
	if( pTemp == NULL )
		return( false );
	memcpy( pRGBPalette, &pTemp[ sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) ], m_nPaletteInBytes );
	m_nLastError = GL_SUCCESS;
	::GlobalUnlock( m_hDib );
	return( true );
}

RGBQUAD *CImageObject::GetPaletteData( void )
{
	m_nLastError = GL_HDIB_NULL;
	if( m_hDib == NULL )
		return( NULL );
	m_nLastError = GL_NO_PALETTE_FOR_HIGH_COLOR;
	if( m_nColors <= 0 || m_nColors > 256 )
		return( NULL );
	RGBQUAD *pRGBPalette;
	pRGBPalette = new RGBQUAD[ m_nColors ];
	if( pRGBPalette == NULL )
	{
		m_nLastError = GL_MEMORY_ALLOCATION_ERROR;
		return( NULL );
	}
	char *pTemp;
	pTemp = ( char * )::GlobalLock( m_hDib );
	m_nLastError = GL_MEMORY_LOCK_ERROR;
	if( pTemp == NULL )
	{
		delete[] pRGBPalette;
		return( NULL );
	}
	memcpy( pRGBPalette, &pTemp[ sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) ], m_nColors * sizeof( RGBQUAD ) );
	m_nLastError = GL_SUCCESS;
	::GlobalUnlock( m_hDib );
	return( pRGBPalette );
}

//////////////////////////////////////////
// Load and Save functions
//
bool CImageObject::LoadFromFile( const char *pszFileName, CDC *pDC, int nX, int nY )
{
	m_nImageType = ::FileType( pszFileName );
	if( m_nImageType == 0 )
	{
		m_nLastError = GL_UNSUPPORTED_FILETYPE;
		return( false );
	}
	DestroyImage();
	m_pszFileName = new char[ strlen( pszFileName ) + 1 ];
	if( m_pszFileName != NULL )
		strcpy( m_pszFileName, pszFileName );
	switch( m_nImageType )
	{
		case GT_BMP:
			m_hDib = ::LoadBMP( pszFileName );
			if( m_hDib == NULL )
			{
				m_nLastError = ::GetLastPicLibError();
				return( false );
			}
			break;
		case GT_GIF:
			m_hDib = ::LoadGIF( pszFileName );
			if( m_hDib == NULL )
			{
				m_nLastError = ::GetLastPicLibError();
				return( false );
			}
			break;
		case GT_JPG:
			m_hDib = ::LoadJPG( pszFileName );
			if( m_hDib == NULL )
			{
				m_nLastError = ::GetLastPicLibError();
				return( false );
			}
			break;
		case GT_PCX:
			m_hDib = ::LoadPCX( pszFileName );
			if( m_hDib == NULL )
			{
				m_nLastError = ::GetLastPicLibError();
				return( false );
			}
			break;
		case GT_TGA:
			m_hDib = ::LoadTGA( pszFileName );
			if( m_hDib == NULL )
			{
				m_nLastError = ::GetLastPicLibError();
				return( false );
			}
			break;
		case GT_TIF:
			m_hDib = ::LoadTIF( pszFileName );
			if( m_hDib == NULL )
			{
				m_nLastError = ::GetLastPicLibError();
				return( false );
			}
			break;
		}
	LoadImageHeader();
	LoadPalette();
	
    //----------------------------------------------------------------------
	// added by maple to support GetPixelColor() and SetPixelColor()  
	// 2004.3.26
	if(m_pDib!=NULL)
		::GlobalUnlock(m_hDib);
	m_pDib = (BYTE *)::GlobalLock(m_hDib);

	//----------------------------------------------------------------------
	if( pDC != NULL )
		Draw( pDC, nX, nY );
	return( true );
}

bool CImageObject::SaveToFile( const char *pszFileName, int nType )
{
	if( nType == -1 )
		nType = GetExtensionIndex( pszFileName );
	if( nType < GT_FIRSTTYPE || nType > GT_LASTTYPE )
		return( false );
	m_nImageType = nType;
	delete[] m_pszFileName;
	m_pszFileName = new char [ strlen( pszFileName ) + 1 ];
	if( m_pszFileName != NULL )
		strcpy( m_pszFileName, pszFileName );
	switch( m_nImageType )
	{
		case GT_BMP:
			if( SaveBMP( pszFileName, m_hDib ) )
				return( true );
		  break;
		case GT_GIF:
			if( ::SaveGIF( pszFileName, m_hDib ) )
				return( true );
		  break;
		case GT_JPG:
			if( ::SaveJPG( pszFileName, m_hDib, m_nQuality ) )
				return( true );
		  break;
		case GT_PCX:
			if( ::SavePCX( pszFileName, m_hDib ) )
				return( true );
		  break;
		case GT_TGA:
			if( ::SaveTGA( pszFileName, m_hDib ) )
				return( true );
		  break;
		case GT_TIF:
			if( ::SaveTIF( pszFileName, m_hDib ) )
				return( true );
		  break;
	}
	return( false );  // true?
}

//////////////////////////////////////////
// Draw functions
//
bool CImageObject::Draw( CDC *pDC, int nX, int nY )
{
	if( nX != -1 )
		m_nX = nX;
	if( nY != -1 )
		m_nY = nY;
	m_nLastError = GL_HDIB_NULL;
	if( m_hDib == NULL )
		return( false );
	char *pTemp;
	pTemp = ( char * )::GlobalLock( m_hDib );
	m_nLastError = GL_MEMORY_LOCK_ERROR;
	if( pTemp == NULL )
		return( NULL );
	BITMAPINFOHEADER *pBIH;
	pBIH = ( BITMAPINFOHEADER * )&pTemp[ sizeof( BITMAPFILEHEADER ) ];
	::SetStretchBltMode(pDC->GetSafeHdc(),COLORONCOLOR);
	int nRet = ::StretchDIBits( pDC->m_hDC, m_nX, m_nY, m_nWidth, m_nHeight,
		                        0, 0, m_nWidth, m_nHeight,
		                        (const void FAR *) &pTemp[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+m_nPaletteInBytes], 
		                        (BITMAPINFO *) pBIH,
								DIB_RGB_COLORS, 
								SRCCOPY );
	::GlobalUnlock( m_hDib );
	m_nLastError = GL_STRETCHDIBITS_ERROR;
	if( nRet != m_nHeight )
		return( false );
	m_nLastError = GL_SUCCESS;
	return( true );
}


/***********************************************************************
*  Description: this function was added by maple,2004.3.21
*
*  函数名称：DrawTo
* 
*  参数：　　CDC *pDC        -- 绘图设备环境
*            int nDestX      -- 绘制起点横坐标
*            int nDestY      -- 绘制七点纵坐标
*            int nDestWidth  -- 绘制目标区域宽度
*            int nDestHeight -- 绘制目标区域高度
* 
*  返回值：  bool 
*
*  说明：    该函数用于将一图象绘制到指定区域。          
************************************************************************/

bool CImageObject::DrawTo(CDC *pDC,int nDestX ,int nDestY ,int nDestWidth,int nDestHeight)
{
	if( nDestX != -1 )
		m_nX = nDestX;
	if( nDestY != -1 )
		m_nY = nDestY;
	if(nDestWidth != 0)
		m_nDestWidth = nDestWidth;
	if(nDestHeight != 0)
		m_nDestHeight = nDestHeight;
	m_nLastError = GL_HDIB_NULL;
	if( m_hDib == NULL )
		return( false );
	char *pTemp;
	pTemp = ( char * )::GlobalLock( m_hDib );
	m_nLastError = GL_MEMORY_LOCK_ERROR;
	if( pTemp == NULL )
		return( NULL );
	BITMAPINFOHEADER *pBIH;
	pBIH = ( BITMAPINFOHEADER * )&pTemp[ sizeof( BITMAPFILEHEADER ) ];
	::SetStretchBltMode(pDC->GetSafeHdc(),COLORONCOLOR);
	int nRet = ::StretchDIBits( pDC->m_hDC, m_nX, m_nY, m_nDestWidth, m_nDestHeight,
		                        0, 0, m_nWidth, m_nHeight,	
		                        (const void FAR *) &pTemp[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+m_nPaletteInBytes], 
		                        (BITMAPINFO *) pBIH,
								DIB_RGB_COLORS, 
								SRCCOPY );
	::GlobalUnlock( m_hDib );
	m_nLastError = GL_STRETCHDIBITS_ERROR;
	if( nRet != m_nHeight )
		return( false );
	m_nLastError = GL_SUCCESS;
	return( true );
}

bool CImageObject::DrawToHdc(HDC pHdc, int nDestX ,int nDestY ,int nDestWidth,int nDestHeight)
{
	if( nDestX != -1 )
		m_nX = nDestX;
	if( nDestY != -1 )
		m_nY = nDestY;
	if(nDestWidth != 0)
		m_nDestWidth = nDestWidth;
	if(nDestHeight != 0)
		m_nDestHeight = nDestHeight;
	m_nLastError = GL_HDIB_NULL;
	if( m_hDib == NULL )
		return( false );
	char *pTemp;
	pTemp = ( char * )::GlobalLock( m_hDib );
	m_nLastError = GL_MEMORY_LOCK_ERROR;
	if( pTemp == NULL )
		return( NULL );
	BITMAPINFOHEADER *pBIH;
	pBIH = ( BITMAPINFOHEADER * )&pTemp[ sizeof( BITMAPFILEHEADER ) ];
	::SetStretchBltMode(pHdc, COLORONCOLOR);
	int nRet = ::StretchDIBits( pHdc, m_nX, m_nY, m_nDestWidth, m_nDestHeight,
		                        0, 0, m_nWidth, m_nHeight,	
		                        (const void FAR *) &pTemp[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+m_nPaletteInBytes], 
		                        (BITMAPINFO *) pBIH,
								DIB_RGB_COLORS, 
								SRCCOPY );
	::GlobalUnlock( m_hDib );
	m_nLastError = GL_STRETCHDIBITS_ERROR;
	if( nRet != m_nHeight )
		return( false );
	m_nLastError = GL_SUCCESS;
	return( true );
}
//////////////////////////////////////////
// Basic private member functions
//
void CImageObject::LoadImageHeader( void )
{
	m_nLastError = GL_HDIB_NULL;
	if( m_hDib == NULL )
		return;
	char *pTemp;
	BITMAPINFOHEADER *pBIH;
	pTemp = ( char * )::GlobalLock( m_hDib );
	m_nLastError = GL_MEMORY_LOCK_ERROR;
	if( pTemp == NULL )
		return;

	BITMAPFILEHEADER * pBFH;
	pBFH = (BITMAPFILEHEADER *) pTemp;
	WORD bfType = pBFH->bfType;

	pBIH = ( BITMAPINFOHEADER * )&pTemp[ sizeof( BITMAPFILEHEADER ) ];
	m_nWidth = pBIH->biWidth;
	m_nHeight = pBIH->biHeight;
	m_nPlanes = pBIH->biPlanes;
	m_nBits = pBIH->biBitCount;
	m_nColors = 1 << m_nBits;

//	DWORD tmp = pBIH->biClrUsed;
//
//    LONG PIX = pBIH->biXPelsPerMeter;
//	LONG PIY = pBIH->biYPelsPerMeter;


	if( m_nPlanes > 1 )
		m_nColors <<= ( m_nPlanes - 1 );
	if( m_nBits >= 16 )
		m_nColors = 0;
	::GlobalUnlock( m_hDib );
	m_nLastError = GL_SUCCESS;
}

void CImageObject::LoadPalette( void )
{
	m_nLastError = GL_HDIB_NULL;
	if( m_hDib == NULL )
		return;
	CWindowDC WindowDC( NULL );
	m_nScreenPlanes = WindowDC.GetDeviceCaps( PLANES );
	m_nScreenBits = WindowDC.GetDeviceCaps( BITSPIXEL );
	m_nPaletteInBytes = 0;
	m_Palette.DeleteObject();
	if( m_nBits <= 8 )
		m_nPaletteInBytes = m_nColors * sizeof( RGBQUAD );
	if( m_nScreenBits >= 16 )
		return;
	char *pTemp;
	pTemp = ( char * )::GlobalLock( m_hDib );
	m_nLastError = GL_MEMORY_LOCK_ERROR;
	if( pTemp == NULL )
		return;
	if( m_nBits <= 8 )
	{
		RGBQUAD *pRGBPalette;
		pRGBPalette = ( RGBQUAD * )&pTemp[ sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) ];
		LOGPALETTE *pPalette;
		pPalette = CreateLogPalette( pRGBPalette, m_nColors );
		if( pPalette == NULL )
		{
			m_nLastError = GL_LOGICAL_PALETTE_CREATION_ERROR;
			// These statements are inserted by June.
			::GlobalUnlock( m_hDib );
			return;
			//goto LoadPaletteExit;
		}
		m_Palette.CreatePalette( pPalette );
		delete[] pPalette;
	}
	m_nLastError = GL_SUCCESS; 
//LoadPaletteExit:
	::GlobalUnlock( m_hDib );
}

void CImageObject::DestroyImage( void )
{
// add by maple
	if( m_pDib)
		::GlobalUnlock(m_hDib);
	m_pDib = NULL;
//---------------------------------------
	if( m_hDib )
		::GlobalFree( m_hDib );
	m_hDib = NULL;

	if( m_pLogPalette != NULL )
		delete[] m_pLogPalette;
	m_pLogPalette = NULL;
	if( m_pszFileName != NULL )
		delete[] m_pszFileName;
	m_pszFileName = NULL;
	m_Palette.DeleteObject();
}

LOGPALETTE *CImageObject::CreateLogPalette( RGBQUAD *pPalette, int nNumColors )
{
	LOGPALETTE *pLogPalette;
	int i;
	if( pPalette == NULL )
		return( NULL );
	pLogPalette = ( LOGPALETTE * )new char[ sizeof( LOGPALETTE ) + nNumColors * sizeof( PALETTEENTRY ) ];
	if( pLogPalette == NULL )
		return( NULL );
	pLogPalette->palVersion = 0x300;
	pLogPalette->palNumEntries = ( unsigned short )nNumColors;
	for( i=0; i<nNumColors; i++ )
	{
		pLogPalette->palPalEntry[ i ].peRed = pPalette[ i ].rgbRed;
		pLogPalette->palPalEntry[ i ].peGreen = pPalette[ i ].rgbGreen;
		pLogPalette->palPalEntry[ i ].peBlue = pPalette[ i ].rgbBlue;
		pLogPalette->palPalEntry[ i ].peFlags = 0;
	}
	return( pLogPalette );
}

void *CImageObject::GetDIBPointer( int *nWidthBytes, int nNewBits, int *nNewWidthBytes, int nNewWidth )
{
	m_nLastError = GL_HDIB_NULL;
	if( m_hDib == NULL )
		return( NULL );
	void *pTemp;
	pTemp = ::GlobalLock( m_hDib );
	m_nLastError = GL_MEMORY_LOCK_ERROR;
	if( pTemp == NULL )
		return( NULL );
	if( nWidthBytes != NULL )
		*nWidthBytes = GetWidthInBytes( m_nBits, m_nWidth );
	if( nNewWidthBytes != NULL )
	{
		if( nNewWidth == -1 )
			nNewWidth = m_nWidth;
		*nNewWidthBytes = GetWidthInBytes( nNewBits, nNewWidth );
	}
	return( pTemp );
}

int CImageObject::GetWidthInBytes( int nBits, int nWidth )
{
	int nWidthBytes;
	nWidthBytes = nWidth;
	if( nBits == 1 )
		nWidthBytes = ( nWidth + 7 ) / 8;
	else if( nBits == 4 )
		nWidthBytes = ( nWidth + 1 ) / 2;
	else if( nBits == 16 )
		nWidthBytes = nWidth * 2;
	else if( nBits == 24 )
		nWidthBytes = nWidth * 3;
	else if( nBits == 32 )
		nWidthBytes = nWidth * 4;
	while( ( nWidthBytes & 3 ) != 0 )
		nWidthBytes++;
	return( nWidthBytes );
}

int CImageObject::GetNearestIndex( unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, RGBQUAD *pRGBPalette, int nColors )
{
	int i, Index = 0;
	DWORD NewDiff, Diff = 100000L;
	DWORD dwRed, dwGreen, dwBlue;
	for( i=0; i<nColors; i++ )
	{
		if( ucRed > pRGBPalette[ i ].rgbRed )
			dwRed = ( DWORD )( pRGBPalette[ i ].rgbRed - ucRed );
		else
			dwRed = ( DWORD )( ucRed - pRGBPalette[ i ].rgbRed );
		if( ucGreen > pRGBPalette[ i ].rgbGreen )
			dwGreen = ( DWORD )( pRGBPalette[ i ].rgbGreen - ucGreen );
		else
			dwGreen = ( DWORD )( ucGreen - pRGBPalette[ i ].rgbGreen );
		if( ucBlue > pRGBPalette[ i ].rgbBlue )
			dwBlue = ( DWORD )( pRGBPalette[ i ].rgbBlue - ucBlue );
		else
			dwBlue = ( DWORD )( ucBlue - pRGBPalette[ i ].rgbBlue );
		NewDiff = ( dwRed * dwRed ) + ( dwGreen * dwGreen ) + ( dwBlue * dwBlue );
		if( NewDiff < Diff )
		{
			if( NewDiff <= 1 ) 
				return( i );
			Diff = NewDiff;
			Index = i;
		}
	}
	return( Index );
}

void CImageObject::CreatePaletteFromDIB( RGBQUAD *pRGBPalette, int nColors )
{
	if( pRGBPalette != NULL )
	{
		LOGPALETTE *pPalette;
		pPalette = CreateLogPalette( pRGBPalette, nColors );
		if( pPalette != NULL )
		{
			m_Palette.CreatePalette( pPalette );
			delete[] pPalette;
		}
		else
			m_nLastError = GL_LOGICAL_PALETTE_CREATION_ERROR;
	}
}

LOGPALETTE *CImageObject::CreatePaletteFromBitmap( int nColors, unsigned char *pBits, int nBits, int nWidth, int nHeight )
{
	RGBQUAD *pRGBPalette;
	if( nBits != 8 && m_nPaletteCreationType == JGPT_POPULARITY_PALETTE )
		pRGBPalette = GeneratePopularityPalette( nColors, pBits, nBits, nWidth, nHeight );
	else if( nBits != 8 && m_nPaletteCreationType == JGPT_MEDIAN_CUT_PALETTE )
		pRGBPalette = GenerateMedianCutPalette( nColors, pBits, nBits, nWidth, nHeight );
	else if( m_nPaletteCreationType == JGPT_FIXED_PALETTE )
		pRGBPalette = GenerateFixedPalette( nColors );
	if( pRGBPalette == NULL )
		return( NULL );
	LOGPALETTE *pLogPalette = CreateLogPalette( pRGBPalette, nColors );
	delete[] pRGBPalette;
	return( pLogPalette );
}

RGBQUAD *CImageObject::GeneratePopularityPalette( int nColors, unsigned char *pBits, int nBits, int nWidth, int nHeight )
{
	RGBQUAD *pRGBQuad = new RGBQUAD[ nColors ];
	if( pRGBQuad == NULL )
		return( GenerateFixedPalette( nColors ) );
	memset( pRGBQuad, 0, nColors * sizeof( RGBQUAD ) );
	BYTE bzColorMap[ 256 ][ 3 ];
	if( !::Popularity( pBits, nBits, nWidth, nHeight, bzColorMap ) )
	{
		delete[] pRGBQuad;
		return( GenerateFixedPalette( nColors ) );
	}
	for( int i=0; i<nColors; i++ )
	{
		pRGBQuad[ i ].rgbRed = bzColorMap[ i ][ 0 ];
		pRGBQuad[ i ].rgbGreen = bzColorMap[ i ][ 1 ];
		pRGBQuad[ i ].rgbBlue = bzColorMap[ i ][ 2 ];
	}
	return( pRGBQuad );
}

RGBQUAD *CImageObject::GenerateMedianCutPalette( int nColors, unsigned char *pBits, int nBits, int nWidth, int nHeight )
{
	RGBQUAD *pRGBQuad = new RGBQUAD[ nColors ];
	if( pRGBQuad == NULL )
		return( GenerateFixedPalette( nColors ) );
	memset( pRGBQuad, 0, nColors * sizeof( RGBQUAD ) );
	BYTE bzColorMap[ 256 ][ 3 ];
	WORD *Hist = new WORD[ 32768 ];
	if( Hist == NULL )
	{
		delete [] pRGBQuad;
		return( GenerateFixedPalette( nColors ) );
	}
	memset( Hist, 0, 32768 * sizeof( WORD ) );
	int nWidthBytes = GetWidthInBytes( nBits, nWidth );

	for( int y=0; y<nHeight; y++ )
	{
		unsigned char *pData = pBits;
		unsigned char ucRed, ucGreen, ucBlue;
		WORD wColor;
		pData += ( y * nWidthBytes );
		for( int x=0; x<nWidth; x++ )
		{
			switch( nBits )
			{
				case 16:
					GET_RGB_16( ucRed, ucGreen, ucBlue, &pData[ 2*x ] );
					break;
				case 24:
					ucRed = pData[ 3*x + 2 ];
					ucGreen = pData[3*x + 1 ];
					ucBlue = pData[ 3*x ];
					break;
				case 32:
					GET_RGB_32( ucRed, ucGreen, ucBlue, &pData[ 4*x ] );
					break;
			}
			wColor = _RGB( ucRed, ucGreen, ucBlue );
			if( Hist[ wColor ] < 65535 )
				Hist[ wColor ]++;
		}
	}
	
	::MedianCut( Hist, bzColorMap, ( int )256 );
	for( int i=0; i<nColors; i++ )
	{
		pRGBQuad[ i ].rgbRed = bzColorMap[ i ][ 0 ];
		pRGBQuad[ i ].rgbGreen = bzColorMap[ i ][ 1 ];
		pRGBQuad[ i ].rgbBlue = bzColorMap[ i ][ 2 ];
	}
	delete[] Hist;
	return( pRGBQuad );
}

RGBQUAD *CImageObject::GenerateFixedPalette( int nColors )
{
	RGBQUAD *pRGBQuad = new RGBQUAD[ nColors ];
	if( pRGBQuad == NULL )
		return( NULL );
	static int nzColors[] = {	255, 255, 255,   0,   0,   0, 255,   0,   0,   0,   0, 255,
		                          0, 255,   0, 150, 150, 150, 255, 255,		0,   0, 150, 150,
								  					150,   0, 150, 150, 150,   0,   0, 255, 255, 255,   0, 255,
									  				255, 120, 120, 120, 255, 120,	120, 120, 255,	90,  90,  90 };
	int nSteps = ( ( nColors + 15 ) / 16 );
	for( int i=0; i<nSteps; i++ )
	{
		for( int j=0; j<16; j++ )
		{
			if( i * 16 + j < nColors )
			{
				int r, g, b;
				r = nzColors[ 3*j ];
				g = nzColors[ 3*j + 1 ];
				b = nzColors[ 3*j + 2 ];
				r = ( ( nSteps - i ) * r ) / nSteps;
				g = ( ( nSteps - i ) * g ) / nSteps;
				b = ( ( nSteps - i ) * b ) / nSteps;
				pRGBQuad[ 16*i + j ].rgbRed = ( unsigned char )r;
				pRGBQuad[ 16*i + j ].rgbGreen = ( unsigned char )g;
				pRGBQuad[ 16*i + j ].rgbBlue = ( unsigned char )b;
			}
		}
	}
	return( pRGBQuad );
}

// Diagnostics and dump member functions, overrided
#ifdef _DEBUG
void CImageObject::Dump( CDumpContext& dc ) const
{
  // call base class function first
  CObject::Dump( dc );
  // now do the stuff for our specific class
//  dc << "last name: " << m_lastName << "\n" << "first name: " << m_firstName << "\n";
}

void CImageObject::AssertValid() const
{ 
	// call inherited AssertValid first
  CObject::AssertValid();
	// check CImageObject members...
//  ASSERT( !m_strName.IsEmpty()); // Must have a name
//  ASSERT( m_salary > 0 ); // Must have an income
} 

#endif

/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  函数名称：LoadDIBTOBuf
* 
*  参数：　　BYTE * buf   --  传入的用于放置象素值的区域的指针 
*   
*  返回值：  bool
*
*  说明：    该函数将图象DIB中对应的象素值转移到m_buf中，变成同一格式，
*            便于处理。        
************************************************************************/

bool CImageObject::LoadDIBToBuf(BYTE * buf)
{
	if(m_hDib == NULL)
		return FALSE;

  	RGBQUAD color ;
	COLORREF colorref;

	if(m_pDib)
		m_pDib = (BYTE *)::GlobalUnlock(m_hDib);
	m_pDib = (BYTE *)::GlobalLock(m_hDib);

	if(m_pDib!=NULL)
	{
		for( int j=0; j<m_nHeight; j++)
			for(int i=0; i<m_nWidth; i++) 
			{
				color = GetPixelColor( i, j );
				colorref =RGB(color.rgbRed, color.rgbGreen, color.rgbBlue);
				unsigned long temp=(long)j*(long)m_nWidth*3+(long)i*3;
			  	    *(buf+temp)=GetBValue(colorref);
			        *(buf+temp+1)=GetGValue(colorref);
				    *(buf+temp+2)=GetRValue(colorref);
			}
	}

	::GlobalUnlock(m_hDib);
 
	return TRUE;	
}

/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  函数名称：UpdateDIB
* 
*  参数：　　int nNewWidth    -- 图象经过处理后新的宽度   
*            int nNewHeight   -- 图象经过处理后新的高度
* 
*  返回值：  bool
*
*  说明：    该函数用处理后的buf中的象素值来更新DIB，
*            并根据传入的新的高度和宽度来决定是否新建一DIB。            
************************************************************************/

bool CImageObject::UpdateDIB(int nNewWidth,int nNewHeight,BYTE * buf)
{
    // if the size of the image has no change,
	// we only need to update the old DIB.

	if(nNewWidth == m_nWidth && nNewHeight == m_nHeight)
	{
    	RGBQUAD color ;

 	    if(m_pDib!=NULL)
		    ::GlobalUnlock(m_hDib);
	    m_pDib = (BYTE *)::GlobalLock(m_hDib);

	    if(m_pDib!=NULL)
		{
    	    for( int j=0; j<m_nHeight; j++)
		        for( int i=0; i<m_nWidth; i++)
				{   
			       unsigned long temp=(long)j*(long)m_nWidth*3+(long)i*3;
              	   BYTE r,g,b;
             	   b=*(buf+temp);
               	   g=*(buf+temp+1);
               	   r=*(buf+temp+2);

			       color.rgbRed   = r ;
			       color.rgbGreen = g;
			       color.rgbBlue  = b;
		    	   SetPixelColor( i, j, color ) ;
				} 
		    ::GlobalUnlock(m_hDib);
			return(true);
		}
		else
		{
		    ::GlobalUnlock(m_hDib);
			return(false);
		}


	}
	// the size of the image has been changed
	// we must create a new DIb to replace the old one.
	else   
	{
		RGBQUAD color ;
		HGLOBAL hNewDib;
		DWORD dwNewSize;
		BYTE * pNewBuf;
		BYTE * pNewBits;
		BITMAPFILEHEADER *pOldBFH,*pNewBFH;
		BITMAPINFOHEADER *pOldBIH,*pNewBIH;
		RGBQUAD *pOldPalette,*pNewPalette;

	    if(m_pDib!=NULL)
		    ::GlobalUnlock(m_hDib);
	    m_pDib = (BYTE *)::GlobalLock(m_hDib);
		if(m_pDib==NULL)
		{
			::GlobalUnlock(m_hDib);
			return(false);
		}

		dwNewSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
			     +m_nColors*sizeof(RGBQUAD)+GetWidthInBytes(m_nBits,nNewWidth)*nNewHeight;
		hNewDib = :: GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,dwNewSize);
		if(hNewDib==NULL)
		{
			::GlobalUnlock(m_hDib);
			return (false);
		}

		pNewBuf = (BYTE *)::GlobalLock(hNewDib);
		if(pNewBuf==NULL)
		{
            ::GlobalFree(hNewDib);
			::GlobalUnlock(m_hDib);
			return (false);
		}
   
		pOldBFH = (BITMAPFILEHEADER *)m_pDib;
		pOldBIH = (BITMAPINFOHEADER *)(m_pDib+sizeof(BITMAPFILEHEADER));
		pOldPalette = (RGBQUAD *)(m_pDib+sizeof(BITMAPFILEHEADER)+
			                   sizeof(BITMAPINFOHEADER));
		
		pNewBFH = (BITMAPFILEHEADER *) pNewBuf;
		pNewBIH = (BITMAPINFOHEADER *) (pNewBuf+sizeof(BITMAPFILEHEADER));
		pNewPalette = (RGBQUAD *)(pNewBuf+sizeof(BITMAPFILEHEADER)+
			                   sizeof(BITMAPINFOHEADER));
		pNewBits = pNewBuf+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
			       +m_nColors*sizeof(RGBQUAD);

		*pNewBFH = *pOldBFH;
		*pNewBIH = *pOldBIH; 
        pNewBFH->bfSize = dwNewSize;
		pNewBIH->biWidth = (LONG)nNewWidth;
		pNewBIH->biHeight = (LONG)nNewHeight;
		pNewBIH->biSizeImage = (DWORD)GetWidthInBytes(m_nBits,nNewWidth)*nNewHeight;
  
		for(int k=0;k<m_nColors;k++)
            pNewPalette[k] = pOldPalette[k];
 
		// very important!!
        // if we miss the following two lines,the display of the image
  		m_nWidth = nNewWidth;         
		m_nHeight = nNewHeight; 
		
        m_nPlanes = pNewBIH->biPlanes;      
		m_nBits = pNewBIH->biBitCount;
		m_nColors = pNewBIH->biClrUsed;
		m_nPaletteInBytes = m_nColors*sizeof(RGBQUAD);
		
		
		// will be wrong!
		::GlobalUnlock(m_hDib);
		::GlobalFree(m_hDib);
		::GlobalUnlock(hNewDib);
		SetDib(hNewDib);
       		
	    if(m_pDib)
		    m_pDib = (BYTE *)::GlobalUnlock(m_hDib);
	    m_pDib = (BYTE *)::GlobalLock(m_hDib);

	    if(m_pDib!=NULL)
		{
    	    for( int j=0; j<m_nHeight; j++)
		        for( int i=0; i<m_nWidth; i++)
				{   
			       unsigned long temp=(long)j*(long)m_nWidth*3+(long)i*3;
              	   BYTE r,g,b;
             	   b=*(buf+temp);
               	   g=*(buf+temp+1);
               	   r=*(buf+temp+2);

			       color.rgbRed   = r ;
			       color.rgbGreen = g;
			       color.rgbBlue  = b;
		    	   SetPixelColor( i, j, color ) ;
				} 

	        ::GlobalUnlock(m_hDib);
     	    return TRUE;
		}
		else
		{
			::GlobalUnlock(m_hDib);
			return(false);
		}
	}

}



/***********************************************************************
*  Description: this function was added by maple,2004.3.22
*
*  函数名称：CreatDIBFromBits
* 
*  参数：　　int nWidth    -- 要新建的图象的宽度   
*            int nHeight   -- 要新建的图象的高度
*            BYTE * buf    -- 存放象素值的内存区域指针
* 
*  返回值：  bool
*
*  说明：    该函数利用一块存放象素值的内存区域来创建一个DIB，所
*            创建的图象是24位位图。            
************************************************************************/


bool CImageObject::CreateDIBFromBits(int nWidth,int nHeight,BYTE * buf)
{
    RGBQUAD color;
	HGLOBAL hDib;
	DWORD dwSize;
	BYTE * pBuf;
	BITMAPFILEHEADER *pBFH;
	BITMAPINFOHEADER *pBIH;

	dwSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
			 +GetWidthInBytes(24,nWidth)*nHeight;
	hDib = :: GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,dwSize);
	if(hDib==NULL)
	{
		return (false);
	}
	
	pBuf = (BYTE *)::GlobalLock(hDib);
	if(pBuf==NULL)
	{
		::GlobalFree(hDib);
		return (false);
	}
	pBFH = (BITMAPFILEHEADER *) pBuf;
	pBIH = (BITMAPINFOHEADER *) (pBuf+sizeof(BITMAPFILEHEADER));
    
	pBFH->bfType = (WORD)19778;
	pBFH->bfSize = (DWORD)dwSize;
	pBFH->bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	
	pBIH->biSize = (DWORD)40;
	m_nWidth = pBIH->biWidth = (LONG)nWidth;
	m_nHeight = pBIH->biHeight = (LONG)nHeight;
	m_nPlanes = pBIH->biPlanes = (WORD)1;
	m_nBits = pBIH->biBitCount = (WORD)24;
	pBIH->biCompression = BI_RGB;
	pBIH->biSizeImage = (DWORD)GetWidthInBytes(24,nWidth)*nHeight;
//	pBIH->biXPelsPerMeter = (LONG)4724;
//	pBIH->biYPelsPerMeter = (LONG)4724; 
    m_nColors = pBIH->biClrUsed = (DWORD)0;
	pBIH->biClrImportant = (DWORD)0;

	m_nPaletteInBytes = 0;     // used in Draw() 

	::GlobalUnlock(hDib);
	SetDib(hDib);
	
	if(m_pDib)
		m_pDib = (BYTE *)::GlobalUnlock(m_hDib);
	m_pDib = (BYTE *)::GlobalLock(m_hDib);
	
	if(m_pDib!=NULL)     
	{
		BYTE * pBits = m_pDib+sizeof(BITMAPFILEHEADER)
			+sizeof(BITMAPINFOHEADER);

		for( int k=0; k<GetWidthInBytes(24,nWidth)*nHeight; k++)
			pBits[k] = 0;

		for( int j=0; j<m_nHeight; j++)
			for( int i=0; i<m_nWidth; i++)
			{   
 				unsigned long temp=(long)j*(long)m_nWidth*3+(long)i*3;				
				BYTE r,g,b;
				b=*(buf+temp);
				g=*(buf+temp+1);
				r=*(buf+temp+2);
				
				color.rgbRed   = r ;
				color.rgbGreen = g;
				color.rgbBlue  = b;
				SetPixelColor( i, j, color ) ;
			} 
			
			::GlobalUnlock(m_hDib);
			return TRUE;
	}
	else
	{
		::GlobalUnlock(m_hDib);
		return(false);
	}
}


/***********************************************************************
*  Description: this function was added by maple,2004.3.22
*
*  函数名称：CreatFromHANDLE
* 
*  参数：　　HANDLE handle    -- 要新建的图象的宽度   
*
*  返回值：  bool
*
*  说明：    该函数利用从剪贴板获得的标准DIB句柄创建一个用于
*            该ImageObject的包含BITMAPFILEHEADER的DIB。            
************************************************************************/

bool CImageObject::CreateFromHANDLE(HANDLE handle)
{

	HANDLE hDib;
	BYTE * pBuf,*pTemp;
	BITMAPFILEHEADER *pBFH;
	BITMAPINFOHEADER *pBIH;
	DWORD size = ::GlobalSize(handle);
	DWORD dwSize = ::GlobalSize(handle)+sizeof(BITMAPFILEHEADER);
	hDib = :: GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,dwSize);
	if(hDib==NULL)
	{
		return (false);
	}
	pBuf = (BYTE *)::GlobalLock(hDib);
	if(pBuf==NULL)
	{
		::GlobalFree(hDib);
		return(false);
	}
	pTemp = (BYTE *)::GlobalLock(handle);
	if(pTemp==NULL)
	{
		return (false);
	}

    pBFH = (BITMAPFILEHEADER *)pBuf;
	pBFH->bfType = (WORD)19778;
	pBFH->bfSize = (DWORD)dwSize;
	pBFH->bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);


	memcpy(pBuf+sizeof(BITMAPFILEHEADER),pTemp,size);

	pBIH = (BITMAPINFOHEADER *)(pBuf+sizeof(BITMAPFILEHEADER));
	m_nWidth = pBIH->biWidth;
	m_nHeight = pBIH->biHeight;
	m_nPlanes = pBIH->biPlanes;
	m_nBits = pBIH->biBitCount;
	m_nColors = pBIH->biClrUsed;
	m_nPaletteInBytes = m_nColors*sizeof(RGBQUAD);
    
	::GlobalUnlock(handle);
    ::GlobalUnlock(hDib); 
	SetDib(hDib);

	return(TRUE);
}


/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  函数名称：GetPixelColor
* 
*  参数：　　int x    -- 图象的横坐标   
*            int y    -- 图象的纵坐标
* 
*  返回值：  RGBQUAD  -- 点(x,y)处对应的RGBQUAD结构的象素值 
*
*  说明：    该函数用于获取(x,y)处的象素值，
*            被LoadDIBToBuf()所调用或者单独使用          
************************************************************************/

RGBQUAD CImageObject::GetPixelColor(int x,int y)
{
	RGBQUAD rgb;
	rgb.rgbBlue = rgb.rgbGreen = rgb.rgbRed = rgb.rgbReserved = 0;
    if(m_pDib == NULL)
		return rgb;

    y = m_nHeight -1 - y;  // add by maple,2004.5.14
	
	BYTE * pBits = m_pDib + sizeof(BITMAPFILEHEADER)
		+ sizeof(BITMAPINFOHEADER) + m_nColors*sizeof(RGBQUAD);
    
	if (m_nColors)
	{   
		BYTE tmp;
		tmp = GetPixelIndex(x,y);
	    rgb = GetPaletteColor(tmp);
	} 
	else
	{
		BYTE* iDst  = pBits + y*GetWidthInBytes(m_nBits,m_nWidth) + x*3;
		rgb.rgbBlue = *iDst++;
		rgb.rgbGreen= *iDst++;
		rgb.rgbRed  = *iDst;
    	rgb.rgbReserved =(BYTE) 0;
	}

	return rgb;
}



/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  函数名称：GetPaletteColor
* 
*  参数：　　BYTE idx -- 调色板中对应的下标 值
* 
*  返回值：  RGBQUAD  -- 点(x,y)处对应的RGBQUAD结构的象素值 
*
*  说明：    该函数用于调色板中下标 idx 对应的RGBQUAD,被 GetPixelColor()调用        
************************************************************************/

RGBQUAD CImageObject::GetPaletteColor(BYTE idx)
{
	RGBQUAD rgb = {0,0,0,0};
	
	if ((m_pDib)&&(m_nColors))
	{
		BYTE* iDst = (BYTE*)(m_pDib +sizeof(BITMAPFILEHEADER)
			         +sizeof(BITMAPINFOHEADER));
		if (idx<m_nColors)
		{
			long ldx=idx*sizeof(RGBQUAD);
			rgb.rgbBlue = iDst[ldx++];
			rgb.rgbGreen=iDst[ldx++];
			rgb.rgbRed =iDst[ldx++];
			rgb.rgbReserved = iDst[ldx];
		}
	}

	return rgb;
}

/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  函数名称：GetPixelIndex
* 
*  参数：　　int x    -- 横坐标
*            int y    -- 纵坐标
* 
*  返回值：  BYTE     -- 点(x,y)的象素值在调色板中的下标
*
*  说明：    该函数用于获取(x,y)的象素值在调色板中的下标。
*            被函数 GetPaletteColor()所调用       
************************************************************************/

BYTE CImageObject::GetPixelIndex(int x,int y)
{

    if ((m_pDib==NULL)||(m_nColors==0)) return 0;

	BYTE * pBits = m_pDib + sizeof(BITMAPFILEHEADER)
		+ sizeof(BITMAPINFOHEADER) + m_nColors*sizeof(RGBQUAD);
	   
	if (m_nBits==8)
	{
		return (BYTE)*(pBits+y*GetWidthInBytes(m_nBits,m_nWidth)+x);
	}
	else 
	{
		BYTE pos;
		BYTE iDst= (BYTE)*(pBits+y*GetWidthInBytes(m_nBits,m_nWidth)+(x*m_nBits >> 3));
		if (m_nBits==4)
		{
			pos = (BYTE)(4*(1-x%2));
			iDst &= (0x0F<<pos);
			return (BYTE)(iDst >> pos);
		} 
		else if (m_nBits==1)
		{
			pos = (BYTE)(7-x%8);
			iDst &= (0x01<<pos);
			
			return (BYTE)(iDst >> pos);
		}
	}
	   
	return 0;
}


/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  函数名称：SetPixelColor
* 
*  参数：　　int x            -- 横坐标
*            int y            -- 纵坐标
*            RGBQUAD  color   -- RGBQUAD结构的调色板象素值
* 
*  返回值：  void     
*
*  说明：    该函数用color中对应的象素值代替(x,y)对应的象素值 
*            被函数UpdateDIB(所调用
************************************************************************/

void CImageObject:: SetPixelColor(int x,int y,RGBQUAD color)
{
    if ((m_pDib==NULL)||(x<0)||(y<0)||
		  (x>=m_nWidth)||(y>=m_nHeight)) 
	return;

	y = m_nHeight -1 - y;   // add by maple, 2004.5.14
	
	BYTE * pBits = m_pDib + sizeof(BITMAPFILEHEADER)
		+ sizeof(BITMAPINFOHEADER) + m_nColors*sizeof(RGBQUAD);

	if (m_nColors)
		SetPixelIndex(x,y,GetNearestIndex(color));
	else 
	{
		BYTE* iDst = pBits + y*GetWidthInBytes(m_nBits,m_nWidth) + x*3;
		*iDst++ = color.rgbBlue;
		*iDst++ = color.rgbGreen;
		*iDst   = color.rgbRed;
	}
}



/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  函数名称：SetPixelIndex
* 
*  参数：　　int x            -- 横坐标
*            int y            -- 纵坐标
*            BYTE index       -- (x,y)中象素值所对应的在调色板中的下标
* 
*  返回值：  void     
*
*  说明：    该函数用下标index来替换原DIB中的索引
*            被函数SetPixelColor()所调用
************************************************************************/

void CImageObject::SetPixelIndex(int x,int y,BYTE index)
{

	if ((m_pDib==NULL)||(m_nColors==0)||
		(x<0)||(y<0)||(x>=m_nWidth)||(y>=m_nHeight)) return ;

	BYTE * pBits = m_pDib + sizeof(BITMAPFILEHEADER)
		+ sizeof(BITMAPINFOHEADER) + m_nColors*sizeof(RGBQUAD);
	
	if (m_nBits==8)
	{
	    *(pBits+y*GetWidthInBytes(m_nBits,m_nWidth) + x)=index;
		return;
	} 
	else 
	{
		BYTE pos;
		BYTE* iDst= pBits + y*GetWidthInBytes(m_nBits,m_nWidth) + (x*m_nBits >> 3);
		if (m_nBits==4)
		{
			pos = (BYTE)(4*(1-x%2));
			*iDst &= ~(0x0F<<pos);
			*iDst |= ((index & 0x0F)<<pos);
			return;
		} 
		else if (m_nBits==1)
		{
			pos = (BYTE)(7-x%8);
			*iDst &= ~(0x01<<pos);
			*iDst |= ((index & 0x01)<<pos);
			return;
		}
	}	
}


/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  函数名称：GetNearestIndex
* 
*  参数：　　RGBQUAD color  -- 调色板的一个单元结构
*
*  返回值：  BYTE           -- 输入调色板单元所对应的索引
*
*  说明：    该函数用来得到输入调色板单元所对应的索引，
*            它作为函数 SetPixelIndex()的一个参数被调用。
************************************************************************/

BYTE CImageObject::GetNearestIndex(RGBQUAD color)
{

	if ((m_pDib==NULL)||(m_nColors==0)) return 0;

	BYTE* iDst = m_pDib+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	long distance=200000;
	int i,j=0;
	long k,l;

	for(i=0,l=0;i<m_nColors;i++,l+=sizeof(RGBQUAD))
	{
		k = (iDst[l]-color.rgbBlue)*(iDst[l]-color.rgbBlue)+
			(iDst[l+1]-color.rgbGreen)*(iDst[l+1]-color.rgbGreen)+
			(iDst[l+2]-color.rgbRed)*(iDst[l+2]-color.rgbRed);
//		k = abs(iDst[l]-c.rgbBlue)+abs(iDst[l+1]-c.rgbGreen)+abs(iDst[l+2]-c.rgbRed);
		if (k==0)
		{
			j=i;
			break;
		}
		if (k<distance){
			distance=k;
			j=i;
		}
	} 
	
	return (BYTE)j;
}

////////////////////////////////////////////////////////////////////////
//  以下四个函数专用于256色灰度图象
////////////////////////////////////////////////////////////////////////

/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  函数名称：LoadDIBTOBuf2
* 
*  参数：　　BYTE * buf   --  传入的用于放置象素值的区域的指针 
*   
*  返回值：  bool
*
*  说明：    该函数专用于256色灰度图象，DIB中对应的象素值转移到m_buf中，
*            每个点的象素值占一个字节。        
************************************************************************/

bool CImageObject::LoadDIBToBuf2(BYTE * buf)
{
	if(m_hDib == NULL)
		return false;
	if(m_pDib)
		m_pDib = (BYTE *)::GlobalUnlock(m_hDib);
	m_pDib = (BYTE *)::GlobalLock(m_hDib);

	if(m_pDib!=NULL)
	{
		int nWidthInBytes = GetWidthInBytes(m_nBits,m_nWidth);
		for( int j=0; j<m_nHeight; j++)
			for(int i=0; i<m_nWidth; i++) 
			{
			   *(buf+j*m_nWidth+i) = GetPixelColor2(i,j);
			}
	}

	::GlobalUnlock(m_hDib);
 
	return true;		
}



/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  函数名称：UpdateDIB2
* 
*  参数：　　int nNewWidth    -- 图象经过处理后新的宽度   
*            int nNewHeight   -- 图象经过处理后新的高度
* 
*  返回值：  bool
*
*  说明：    更新256灰度图象的DIB 
*            该函数用处理后的buf中的象素值来更新DIB，
*            并根据传入的新的高度和宽度来决定是否新建一DIB。            
************************************************************************/

bool CImageObject::UpdateDIB2(int nNewWidth,int nNewHeight,BYTE * buf)
{
    // if the size of the image has no change,
	// we only need to update the old DIB.

	if(nNewWidth == m_nWidth && nNewHeight == m_nHeight)
	{
 	    if(m_pDib!=NULL)
		    ::GlobalUnlock(m_hDib);
	    m_pDib = (BYTE *)::GlobalLock(m_hDib);

	    if(m_pDib!=NULL)
		{
			BYTE color;
    	    for( int j=0; j<m_nHeight; j++)
		        for( int i=0; i<m_nWidth; i++)
				{   
                   color = *(buf+j*m_nWidth+i);
		    	   SetPixelColor2( i, j, color ) ;
				} 
		    ::GlobalUnlock(m_hDib);
			return(true);
		}
		else
		{
		    ::GlobalUnlock(m_hDib);
			return(false);
		}


	}
	// the size of the image has been changed
	// we must create a new DIb to replace the old one.
	else   
	{
		HGLOBAL hNewDib;
		DWORD dwNewSize;
		BYTE * pNewBuf;
		BYTE * pNewBits;
		BITMAPFILEHEADER *pOldBFH,*pNewBFH;
		BITMAPINFOHEADER *pOldBIH,*pNewBIH;
		RGBQUAD *pOldPalette,*pNewPalette;

	    if(m_pDib!=NULL)
		    ::GlobalUnlock(m_hDib);
	    m_pDib = (BYTE *)::GlobalLock(m_hDib);
		if(m_pDib==NULL)
		{
			::GlobalUnlock(m_hDib);
			return(false);
		}

		dwNewSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
			     +m_nColors*sizeof(RGBQUAD)+GetWidthInBytes(m_nBits,nNewWidth)*nNewHeight;
		hNewDib = :: GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,dwNewSize);
		if(hNewDib==NULL)
		{
			::GlobalUnlock(m_hDib);
			return (false);
		}

		pNewBuf = (BYTE *)::GlobalLock(hNewDib);
		if(pNewBuf==NULL)
		{
            ::GlobalFree(hNewDib);
			::GlobalUnlock(m_hDib);
			return (false);
		}
   
		pOldBFH = (BITMAPFILEHEADER *)m_pDib;
		pOldBIH = (BITMAPINFOHEADER *)(m_pDib+sizeof(BITMAPFILEHEADER));
		pOldPalette = (RGBQUAD *)(m_pDib+sizeof(BITMAPFILEHEADER)+
			                   sizeof(BITMAPINFOHEADER));
		
		pNewBFH = (BITMAPFILEHEADER *) pNewBuf;
		pNewBIH = (BITMAPINFOHEADER *) (pNewBuf+sizeof(BITMAPFILEHEADER));
		pNewPalette = (RGBQUAD *)(pNewBuf+sizeof(BITMAPFILEHEADER)+
			                   sizeof(BITMAPINFOHEADER));
		pNewBits = pNewBuf+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
			       +m_nColors*sizeof(RGBQUAD);

		*pNewBFH = *pOldBFH;
		*pNewBIH = *pOldBIH; 
        pNewBFH->bfSize = dwNewSize;
		pNewBIH->biWidth = (LONG)nNewWidth;
		pNewBIH->biHeight = (LONG)nNewHeight;
		pNewBIH->biSizeImage = (DWORD)GetWidthInBytes(m_nBits,nNewWidth)*nNewHeight;
  
		for(int k=0;k<m_nColors;k++)
            pNewPalette[k] = pOldPalette[k];
 
		// very important!!
        // if we miss the following two lines,the display of the image
  		m_nWidth = nNewWidth;         
		m_nHeight = nNewHeight; 
		
        m_nPlanes = pNewBIH->biPlanes;      
		m_nBits = pNewBIH->biBitCount;
		m_nColors = pNewBIH->biClrUsed;
		m_nPaletteInBytes = m_nColors*sizeof(RGBQUAD);
		
		
		// will be wrong!
		::GlobalUnlock(m_hDib);
		::GlobalFree(m_hDib);
		::GlobalUnlock(hNewDib);
		SetDib(hNewDib);
       		
	    if(m_pDib)
		    m_pDib = (BYTE *)::GlobalUnlock(m_hDib);
	    m_pDib = (BYTE *)::GlobalLock(m_hDib);

	    if(m_pDib!=NULL)
		{
    	    BYTE color;
			for( int j=0; j<m_nHeight; j++)
		        for( int i=0; i<m_nWidth; i++)
				{  
					color = *(buf+j*m_nWidth+i);
		    	    SetPixelColor2( i, j, color ) ;
				} 
	        ::GlobalUnlock(m_hDib);
     	    return TRUE;
		}
		else
		{
			::GlobalUnlock(m_hDib);
			return(false);
		}
	}

}

/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  函数名称：GetPixelColor2
* 
*  参数：　　int x    -- 图象的横坐标   
*            int y    -- 图象的纵坐标
* 
*  返回值：  RGBQUAD  -- 点(x,y)处对应的RGBQUAD结构的象素值 
*
*  说明： 此函数专用于256色灰度图象，用于获取(x,y)处的象素值，   
*         被LoadDIBToBuf()所调用或者单独使用   
************************************************************************/
BYTE CImageObject::GetPixelColor2(int x,int y)
{
    if(m_pDib == NULL)
		return (BYTE)0;
   
	y = m_nHeight -1 - y;   // add by maple, 2004.5.14
	
	BYTE * pBits = m_pDib + sizeof(BITMAPFILEHEADER)
		+ sizeof(BITMAPINFOHEADER) + m_nColors*sizeof(RGBQUAD);
	BYTE tmpColor;
	    tmpColor = * (pBits + y*GetWidthInBytes(m_nBits,m_nWidth) + x);
	return tmpColor;

}


/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  函数名称：SetPixelColor2
* 
*  参数：　　int x            -- 横坐标
*            int y            -- 纵坐标
*            BYTE  color      -- 指定坐标对应的象素值  
* 
*  返回值：  void     
*
*  说明：    该函数专用于256色灰度图象， 
*            用color中对应的象素值代替(x,y)对应的象素值， 
*            被函数UpdateDIB(所调用或者单独使用。
************************************************************************/

void CImageObject:: SetPixelColor2(int x,int y,BYTE color)
{
    if ((m_pDib==NULL)||(x<0)||(y<0)||
		  (x>=m_nWidth)||(y>=m_nHeight)) 
	return;
    
    y = m_nHeight -1 - y;   // add by maple, 2004.5.14

	BYTE * pBits = m_pDib + sizeof(BITMAPFILEHEADER)
		+ sizeof(BITMAPINFOHEADER) + m_nColors*sizeof(RGBQUAD);
 
	*(pBits+y*GetWidthInBytes(m_nBits,m_nWidth) + x) = color;		
}

/***********************************************************************
*  Description: this function was added by maple,2004.4.16
*
*  函数名称：SaveBMP()
*
*  参数：    const char * strPathName   -- 文件保存名
*            HGLOBAL hDib               -- 位图句柄
*
*  返回值：  BOOL
*
*  说明：　  该函数用于代替 imageload.dll中的保存BMP的函数
************************************************************************/

BOOL CImageObject::SaveBMP(const char * strPathName, HGLOBAL hDib )
{
    if(hDib == NULL)
		return FALSE;
	BYTE * pDib;
	pDib = (BYTE *)::GlobalLock(hDib);
	if(pDib == NULL)
		return FALSE;

	// 得到文件长度
	long filelength = (long)::GlobalSize(hDib);
	CFile file(strPathName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
   
	// 往文件里写图象数据
	file.Write(pDib,filelength);
	file.Close();

	::GlobalUnlock(hDib);
  
	return TRUE;	
}
