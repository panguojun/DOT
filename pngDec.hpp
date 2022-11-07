#ifndef _PNG_DEC_H_
#define _PNG_DEC_H_

#pragma pack(1)
// 文件头
struct tPngHeader
{
	char signature[8];// Png文件标记
};
// 文件头数据块
struct tPng_IHDR
{
	int Width;					// 图像宽度，以像素为单位
	int Height;					// 图像高度，以像素为单位
	char BitDepth;				// 图像深度：索引彩色图像：1，2，4或8   灰度图像：1，2，4，8或16   真彩色图像：8或16 
	char ColorType;				// 颜色类型：0：灰度图像, 1，2，4，8或16   2：真彩色图像，8或16   3：索引彩色图像，1，2，4或8
								// 4：带α通道数据的灰度图像，8或16   6：带α通道数据的真彩色图像，8或16 
	char CompressionMethod;		// 压缩方法(LZ77派生算法) 
	char FilterMethod;			// 滤波器方法
	char InterlaceMethod;		// 隔行扫描方法：0：非隔行扫描   1： Adam7(由Adam M. Costello开发的7遍隔行扫描方法)
};

class CDymPng
{
	tPngHeader m_tPngHeader;	// 文件头
	tPng_IHDR m_tIHDR;			// 文件头数据块
	unsigned int m_dwPaletteColor[ 256 ];
	unsigned int *m_pPixelData;
	int m_nOutputLocate;
	int m_nWritePixel;
	int m_nScanline;
	unsigned char *m_pDataBuffer;
	
	int m_nDataBufferSize;
	unsigned char *m_pDataBuffer2;
	int m_nDataBufferSize2;

	unsigned int ReadInt( const void *pData , int &seek );
	unsigned char ReadChar( const void *pData , int &seek );
	void _FilterRow( unsigned char *pData , int start );
	void _DecompressZipOutDataStream2( void *pData , int size );
public:
	CDymPng();
	~CDymPng();
	void _DecompressZipOutDataStream( void *pData , int size );

	int LoadFromFileA( const char *strFileName );
	int LoadFromMemory( const void *pData , unsigned int datasize );
	int Width();
	int Height();
	unsigned int gifGetPixel( int x , int y , int frame = 0 );
	static bool isPng( const void *pData , unsigned int datasize );
};

#pragma pack()
#endif // _PNG_DEC_H_