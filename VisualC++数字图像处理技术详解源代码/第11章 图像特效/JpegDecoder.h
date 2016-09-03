#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE unsigned char //长度1个字节（=8比特）
#define WORD unsigned short int //长度2个字节
#define DWORD unsigned int //长度4个字节
#define SDWORD signed int //长度4个字节
#define SBYTE signed char //长度1个字节
#define SWORD signed short int //长度2个字节


typedef struct {
	// k的取值范围是1到16
	// Length[k] 表示长度为k的霍夫曼编码的数量
	BYTE Length[17];	
	//表示长度为k的最小霍夫曼编码值 
	WORD minor_code[17];
	//表示长度为k的最高霍夫曼编码值 
	WORD major_code[17];	
	// V[k][j] 表示第j个长度为 k的霍夫曼编码
	BYTE V[65536];				
} Huffman_table;


class JpegDecoder
{
public:
	//带参数的构造函数，
	JpegDecoder(FILE *f);
	//析造函数
	~JpegDecoder(void);
	//返回解码后的像素数组
	int GetJPEGBuffer(WORD X_image,WORD Y_image, BYTE **address_dest_buffer);
	//返回图像的宽度
	int GetWidth() const { return width; }
	//返回图像的高度
	int GetHeight() const { return height; }

private:
	//成员变量
	//图像的宽度
	DWORD	width;
	//图像的高度
	DWORD	height;
	//存储JPEG文件的指针
	BYTE*	buf;	
	//当前字节
	BYTE	bp;
	//当前word
	WORD	wp;
	//当前字节的位置
	DWORD	byte_pos;

	WORD	X_round;	
	WORD	Y_round;
	//按RGBA形式排列的像素数组，存放最终的解码结果
	BYTE*	im_buffer;
	//图像一行的字节数
	DWORD	X_image_bytes;

	DWORD	y_inc_value;
	//Y,Cb,Cr各自在水平和垂直方向上的采样
	BYTE	YH, YV, CbH, CbV, CrH, CrV;
	WORD	Hmax,Vmax;
	//量化表，最多4个
	float*	QT[4];
	//DC霍夫曼表
	Huffman_table HTDC[4]; 
	//AC霍夫曼表
	Huffman_table HTAC[4]; 
	//Y, Cb, Cr量化表的数量
	BYTE YQ_nr,CbQ_nr,CrQ_nr; 
	//Y, Cb, Cr对应的DC霍夫曼表的数量
	BYTE YDC_nr,CbDC_nr,CrDC_nr;
	//Y, Cb, Cr对应的AC霍夫曼表的数量
	BYTE YAC_nr,CbAC_nr,CrAC_nr;
	//重新开始标记，标记为0时表示没有重新开始标记
	BYTE Restart_markers; 
	//MCU重开始标记，它位于每个MCU块的前部
	WORD MCU_restart;
	SWORD DCY, DCCb, DCCr; // Coeficientii DC pentru Y,Cb,Cr
	//离散余弦变换系数
	SWORD DCT_coeff[64];
	//当前8x8子块的Y, Cb, Cr
	BYTE Y[64],Cb[64],Cr[64]; 

	BYTE Y_1[64],Y_2[64],Y_3[64],Y_4[64];
	BYTE tab_1[64],tab_2[64],tab_3[64],tab_4[64]; 
	SWORD Cr_tab[256], Cb_tab[256]; // Precalculated Cr, Cb tables
	SWORD Cr_Cb_green_tab[65536];

	//内存中的位移量, 它与w1的偏移量相关, 取值总小于16
	BYTE d_k;
	// w1表示内存中的第一个word ; w2 表示内存中的第二个word
	WORD w1,w2; 
	DWORD mask[17];

	int		shift_temp;

	BYTE*	rlimit_table;
	BYTE*	tempTable;
	char	error_string[90];
	void PrepareRangeLimitTable(void);
	void SkipKbits(BYTE k);
	SWORD GetKbits(BYTE k);
	void CalculateMask(void);
	//初始化量化表
	void InitQT();
	//载入量化表
	void LoadQuantTable(float *quant_table);
	//载入霍夫曼表
	void LoadHuffmanTable(Huffman_table *HT);
	//处理霍夫曼数据单元
	void ProcessHuffmanDataUnit(BYTE DC_nr, BYTE AC_nr,SWORD *previous_DC);
	//离散反余弦变换
	void IDCTTransform(SWORD *incoeff,BYTE *outcoeff,BYTE Q_nr);
	void Precalculate_Cr_Cb_Tables();
	//色彩空间的转换YCbCr->RGB
	void Convert8x8YCbCrToRGB(BYTE *Y, BYTE *Cb, BYTE *Cr, DWORD im_loc, DWORD X_image_bytes, BYTE *im_buffer);
	void Convert8x8YCbCrToRGBTab(BYTE *Y, BYTE *Cb, BYTE *Cr, BYTE *tab, DWORD im_loc, DWORD X_image_bytes, BYTE *im_buffer);
	void CalculateTabs();
	//JPEG解码函数的初始化工作
	int InitJPGDecoding();
	//返回JPEG文件的长度
	DWORD GetFileSize(FILE *fp);
	void Resync();
	//解码1x1的MCU单元，下同
	void DecodeMCU1x1(DWORD im_loc);
	void DecodeMCU2x1(DWORD im_loc);
	void DecodeMCU2x2(DWORD im_loc);
	void DecodeMCU1x2(DWORD im_loc);
	//载入JPEG头
	bool LoadJPEGHeader(FILE* fp);
	//JPEG解码函数
	void DecodeJPEGImage();
};