#ifndef _PNG_DEC_H_
#define _PNG_DEC_H_

#pragma pack(1)
// �ļ�ͷ
struct tPngHeader
{
	char signature[8];// Png�ļ����
};
// �ļ�ͷ���ݿ�
struct tPng_IHDR
{
	int Width;					// ͼ���ȣ�������Ϊ��λ
	int Height;					// ͼ��߶ȣ�������Ϊ��λ
	char BitDepth;				// ͼ����ȣ�������ɫͼ��1��2��4��8   �Ҷ�ͼ��1��2��4��8��16   ���ɫͼ��8��16 
	char ColorType;				// ��ɫ���ͣ�0���Ҷ�ͼ��, 1��2��4��8��16   2�����ɫͼ��8��16   3��������ɫͼ��1��2��4��8
								// 4������ͨ�����ݵĻҶ�ͼ��8��16   6������ͨ�����ݵ����ɫͼ��8��16 
	char CompressionMethod;		// ѹ������(LZ77�����㷨) 
	char FilterMethod;			// �˲�������
	char InterlaceMethod;		// ����ɨ�跽����0���Ǹ���ɨ��   1�� Adam7(��Adam M. Costello������7�����ɨ�跽��)
};

class CDymPng
{
	tPngHeader m_tPngHeader;	// �ļ�ͷ
	tPng_IHDR m_tIHDR;			// �ļ�ͷ���ݿ�
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