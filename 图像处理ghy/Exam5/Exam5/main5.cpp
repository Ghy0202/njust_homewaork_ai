#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include<string>
#include<iostream>
#include<string.h>
#include <intrin.h> 
#include "bmpFile.h"
#include "RmwALG_EdgeSharpen.h"
//#include "RmwALG_EdgeDetect.h"
#include "RmwALG_2DHistogram.h"
#include "RmwLocateMark.h"


using namespace std;
/*���в���4.6*/
void testEdgeSharpen()
{
	unsigned char* pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("./data/H0401Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE* pResImg = new BYTE[width * height];
	// step.2-------------����ͼ��--------------------------//
	//0.5
	RmwEdgeShapen(pGryImg, width, height, 0.5, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "./res_data/shapen_0_5_H0401Gry.bmp");
	//2
	RmwEdgeShapen(pGryImg, width, height, 2, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "./res_data/shapen_2_H0401Gry.bmp");
	//4
	RmwEdgeShapen(pGryImg, width, height, 4, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "./res_data/shapen_4_H0401Gry.bmp");
	//8
	RmwEdgeShapen(pGryImg, width, height, 8, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "./res_data/shapen_8_H0401Gry.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}
/*Soble��һ��*/
/*
void testSobelRice()
{
	unsigned char* pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("./data/H0401Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE* pResImg = new BYTE[width * height];
	// step.2-------------����ͼ��--------------------------//
	RmwSobelGryImg(pGryImg, width, height, pResImg);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "./res_data/Fig0431Gry_1_Sobel.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}
*/
/*�����ӣ�����*/
/*
void testShenJunRice()
{
	unsigned char* pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("./data/H0401Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE* pResImg = new BYTE[width * height];
	BYTE* pTmpImg = new BYTE[width * height];
	// step.2-------------����ͼ��--------------------------//
	RmwShenJunGryImg(pGryImg, pTmpImg, width, height, 0.5, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "./res_data/H0401_shen.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	delete pTmpImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}
*/
/*�����ı�Ե����㷨��һ�ס������Լ���������ٶ�*/
/*
* void testExtractRiceEdge()
{
	unsigned char* pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("./data/H0401Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE* pResImg = new BYTE[width * height];
	BYTE* pTmpImg = new BYTE[width * height];
	// step.2-------------����ͼ��--------------------------//
	RmwExtractRiceEdge(pGryImg, pTmpImg, width, height, 0.5, 32 * 2, pResImg);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "./res_data/H0401Gry_3.bmp");
	//�������ɫ
	for (int i = 0; i < width * height; i++)
	{
		*(pGryImg + i) = max(pGryImg[i], pResImg[i]);
	}
	Rmw_Write8BitImg2BmpFileMark(pGryImg, width, height, "./res_data/H0401Gry_4_Overlay.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	delete pTmpImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}
*/
/*4.7 */

/*5-2 ֱ��ͼ��Otus��ֵ����ֵ��*/
void Exam5_2() {
	unsigned char* pGryImg;
	unsigned char* pImg;//ͼ��ָ��
	int width, height;
	int thre, num=0;
	int histogram[256];
	memset(histogram,0, sizeof(histogram));
	
	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("./data/H0501Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE* pResImg = new BYTE[width * height];
	//ֱ��ͼ��������:���ǵ�����ԭͼ
	pImg = pGryImg;
	int gmin=255, gmax=0;
	for (; pImg < pGryImg + height * width; pImg++) {
		histogram[*pImg]++;
		if (*pImg > gmax) {
			gmax = *pImg;
		}
		if (*pImg < gmin) {
			gmin = *pImg;
		}
		num++;
	}
	//num = RmwHistogramBy2D(pGryImg, pAvrImg, width, height, 4 * 2, histogram);
	//����ֱ��ͼ
	RmwDrawHistogram2Img(histogram, 256, 1.0, pResImg, width, height);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "./res_data/histogram.bmp");
	// step.3-------------��ֵ��----------------------------//
	//�õ���ֵ
	thre = my_RmwGetOtsuThreshold(histogram, 256, gmin, gmax);
	printf("\nthre1=%d\n",  thre);
	//��ֵͼ��
	RmwThreshold(pGryImg, width, height, thre);
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "./res_data/H0401Gry_Bin.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;

}
/*5-3�˲����и�*/
void Exam5_3() {
	unsigned char* pGryImg;
	unsigned char* pImg;//ͼ��ָ��
	int width, height;
	int thre1,thre2, num = 0;
	int histogram[256];
	int reshistogram[256];
	memset(histogram, 0, sizeof(histogram));
	memset(reshistogram, 0, sizeof(reshistogram));
	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("./data/H0501Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE* pResImg = new BYTE[width * height];
	//ֱ��ͼ��������:ԭͼ��ֱ��ͼ
	pImg = pGryImg;
	int gmin = 255, gmax = 0;
	for (; pImg < pGryImg + height * width; pImg++) {
		histogram[*pImg]++;
		if (*pImg > gmax) {
			gmax = *pImg;
		}
		if (*pImg < gmin) {
			gmin = *pImg;
		}
		num++;
	}
	//ƽ����õ��µ�ֱ��ͼ
	int sum = histogram[0] + histogram[1] + histogram[2] + histogram[3] + histogram[4];
	for (int i = 2; i < 254; i++) {
		reshistogram[i] = sum / 5;
		sum = sum - histogram[i - 2] + histogram[i + 2];
	}
	//����ƽ�����ֱ��ͼ
	RmwDrawHistogram2Img(reshistogram, 256, 1.0, pResImg, width, height);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "./res_data/histogram_5.bmp");
	// step.3-------------��ֵ��----------------------------//
	//�õ���ֵ
	thre1 = my_RmwGetOtsuThreshold(histogram, 256, gmin, gmax);
	thre2 =RmwGetOtsuThreshold(reshistogram, 256);
	printf("\nthre1=%d\nthre2=%d\n",  thre1,thre2);
	//��ֵͼ��
	RmwThreshold(pGryImg, width, height, thre2);
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "./res_data/H0401Gry_Bin_5.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}
/*5-4�������и�*/
void Exam5_4() {
	unsigned char* pGryImg;
	unsigned char* pImg;//ͼ��ָ��
	int width, height;
	int thre, num = 0;
	int histogram[256];
	memset(histogram, 0, sizeof(histogram));

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("./data/H0501Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE* pResImg = new BYTE[width * height];
	//ֱ��ͼ��������:���ǵ�����ԭͼ
	pImg = pGryImg;
	for (; pImg < pGryImg + height * width; pImg++) {
		*pImg = (*pImg) / 4;
		histogram[*pImg]++;
	}
	//����ֱ��ͼ
	RmwDrawHistogram2Img(histogram, 256, 1.0, pResImg, width, height);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "./res_data/histogram4.bmp");
	// step.3-------------��ֵ��----------------------------//
	//�õ���ֵ
	thre = RmwGetOtsuThreshold(histogram, 256);
	printf("\nthre3=%d\n",  thre);
	//��ֵͼ��
	RmwThreshold(pGryImg, width, height, thre);
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "./res_data/H0401Gry_Bin4.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}
/*�ı���λ������ʦ���ķ���*/
void Exam4() {
	clock_t start_t, end_t;
	start_t = clock();
	/*��ȡ�ļ��������и�����*/
	class RmwLocateMark gRmwPos;
	BYTE* pGryImg;
	int width, height;
	char filename[255];
	//����
	clock_t t1, t2;
	bool isFirst = true;
	bool ret;
	//���
	BYTE* pResImg;
	int w, h;
	RMWRECT refPos;
	for (int i = 2; i <= 4; i++)
	{
		
		//��ͼ��
		sprintf_s(filename, "./data/H040%dGry.bmp", i);
		//cout << "����ִ���ļ���" << filename << "\n";
		pGryImg = Rmw_Read8BitBmpFile2Img(filename, &width, &height);
		if (!pGryImg)
		{
			printf("*file open err!\n");
			getchar();
			continue;
		}
		else {
			//printf("%s,width=%d,height=%d\n", filename, width, height);
		}
		//��ʼ��
		if (isFirst)
		{
			isFirst = false;
			ret = gRmwPos.Initialize(1024, //֧�ֵ����ͼ����
				1024, //֧�ֵ����ͼ��߶�
				false, //�Ҷȷ���
				2, //����ģʽ,1-ʹ�ûҶ�,2-ʹ���ݶ�
				3, //����,0-������,1-����4��,2-����16��(Ŀ��ܴ�ʱ)
				false //�Ƿ����
			);
			if (!ret)
			{
				printf("********* gRmwPos.Initialize() Err! ************* \n");
				getchar();
				getchar();
				delete pGryImg;
				return;
			}
		}
		//��λ
		t1 = clock();
		for (int k = 0; k < 100; k++)
			gRmwPos.DoNext(pGryImg, //��ǰͼ��
				width, //��ǰͼ��Ŀ��  
				height, //��ǰͼ��ĸ߶�
				100, //Ŀ����
				100, //Ŀ��߶�
				50, //ȥ����ߵĿ��,����Ϊ0
				50, //ȥ���ұߵĿ��,����Ϊ0
				15, //ȥ���ϱߵĵĸ߶�,����Ϊ0
				15, //ȥ���ϱߵĵĸ߶�,����Ϊ0
				i //֡��
			);
		t2 = clock();
		//���
		pResImg = gRmwPos.GetTargetImg(16, &w, &h, &refPos);
		//sprintf_s(filename, "./res_data/H040%dGry_detect%d.bmp", i, t2 - t1);
		//Rmw_Write8BitImg2BmpFileMark(pResImg, w, h, filename);
		//��һ��
		delete pGryImg;
	}
	end_t = clock();
	cout << "ִ�н�������"<<(end_t - start_t)/1000.0<<"����";
	getchar();
	getchar();
	return;
}
/*���¸�д*/
void searchMaxPos(
	int* pSumImg, int width, int height,
	int w, int h,
	int delLRUk,
	int delLRUz,
	int* x1, int* x2, int* y1,
	int* y2
) {
	int* pLine, * pCur, * pL, * pR;
	int x, y, k, z;
	int maxV, bstX, bstY, sumC, sumL, sumR, sumU = 0;
	k = delLRUk;
	z = delLRUz;
	maxV = -INT_MAX;
	bstX = bstY = 0;
	for (y = z, pLine = pSumImg + z * width; y < height - h; y++, pLine += width) {
		for (x = 1 + k, pCur = pLine + x; x < width - w - k; x++, pCur++) {
			sumC = *(pCur + h * width + w) - *(pCur + h * width) - *(pCur + w) + *pCur;
			//���
			pL = pCur - k;
			sumL = *(pL + h * width + k) - *(pL + h * width - 1) - *(pL + k) + *pL;
			//�ұ�
			pR = pCur + w;
			sumR = *(pR + h * width + k) - *(pR + h * width - 1) - *(pR + k) + *pR;
			//�ϱ�
			sumU = *(pCur + w - width) - *(pCur - width) - *(pCur + w - (z - 1) * width) + *(pCur - (z - 1) * width);
			if (sumC - sumL-sumU -sumR> maxV) {
				maxV = sumC  -sumL-sumU-sumR;
				bstX = x;
				bstY = y;
			}
		}
	}
	*x1 = bstX; 
	*x2 = *x1 + w;
	*y1 = bstY;
	*y2 = *y1 + h;
}
//ѹ��ͼ��
void toSmall(BYTE* pGryImg, int width, int height, BYTE* pResImg, int* pSumImg, int nSize) {
	//��ͼ��ѹ��:nSizeȡ2���ݴ�
	int* pImg;
	BYTE* pRes;
	int* pLine;
	int x, y;
	int sq = sqrt(nSize);
	
	int temp;
	/*
	for (y = 1, pLine = pSumImg + width, pRes = pResImg; y < height - sq; y += sq, pLine += sq * width) {
		for (x = 1, pImg = pLine + x; x < width - sq; x += sq, pImg += sq) {
			temp = *(pImg + sq * width + sq) + *(pImg - width) - *(pImg + sq * width) - *(pImg - width + sq);
			*(pRes++) = temp / nSize;
		}
		pRes++;
	}
	*/
	for (y = 1, pLine = pSumImg + width, pRes = pResImg; y < height - sq; y += sq, pLine += sq * width) {
		for (x = 1, pImg = pLine + x; x < width - sq; x += sq, pImg += sq) {
			temp = *(pImg + (width << 2) + sq) + *(pImg - width) - *(pImg + (width << 2) )- *(pImg - width + sq);
			*(pRes++) = temp / nSize;
		}
		pRes++;
	}

}
//��Ե���
void Sobel(BYTE* pGryImg, int width, int height, BYTE* pResImg) {
	BYTE* pImg, * pRes;//ָ�룺ͼ�񣬽��
	int dx, dy, x, y;
	memset(pResImg, 0, sizeof(pResImg));
	for (y = 1, pImg = pGryImg + width, pRes = pResImg + width; y < height - 1; y++) {
		*(pRes++) = 0;//�߲߱�������ֵ0
		pImg++;
		for (x = 1; x < width - 1; x++, pImg++) {
			//��dx
			dx = *(pImg - 1 - width) + (*(pImg - 1) << 1) + *(pImg - 1 + width);
			dx -= *(pImg + 1 - width) + (*(pImg + 1) << 1) + *(pImg + 1 + width);
			//��dy
			dy = *(pImg - 1 - width) + (*(pImg - width) << 1) + *(pImg + 1 - width);
			dy -= *(pImg - 1 + width) + (*(pImg + width) << 1) + *(pImg + 1 + width);
			//���
			*(pRes++) = min(255, abs(dx) + abs(dy));
		}
		*(pRes++) = 0;//�߲߱�������ֵ0
		pImg++;
	}
}
//�������ͼ
void doSumImg_SSE(BYTE* pGryImg, int width, int height, int* pSumImg) {
	//����SSE���ٵõ���ͼ
	_declspec(align(16)) int sumCol[4096];//Լ��ͼ���Ȳ�����4096��16�ֽڶ���
	__m128i* pSumSSE, A;
	BYTE* pImg;
	int* pRes;
	int x, y;
	memset(sumCol, 0, sizeof(sumCol));
	for (y = 0, pImg = pGryImg, pRes = pSumImg; y < height; y++) {
		//0
		sumCol[0] += *(pImg++);
		*(pRes++) = sumCol[0];
		//1
		sumCol[1] += *(pImg++);
		*(pRes++) = *(pRes - 1) + sumCol[1];
		//2
		sumCol[2] += *(pImg++);
		*(pRes++) = *(pRes - 1) + sumCol[2];
		//3
		sumCol[3] += *(pImg++);
		*(pRes++) = *(pRes - 1) + sumCol[3];
		//������������ļ���
		for (x = 4, pSumSSE = (__m128i*)(sumCol + 4); x < width; x += 4, pImg += 4) {
			A = _mm_cvtepu8_epi32(_mm_loadl_epi64((__m128i*)pImg));
			//4��32λ���������
			*(pSumSSE++) = _mm_add_epi32(*pSumSSE, A);
			for (int i = 0; i < 4; i++) {
				*(pRes++) = *(pRes - 1) + sumCol[x + i];
			}
		}
	}

}
void Draw(BYTE* pGryImg, int width, int w, int h, int* left, int* right, int* up, int* down) {
	//�����������ԭͼ��
	BYTE* pImg, * pCur;
	int x, y;

	//�Ϸ�
	y = *up;
	x = *left;
	pCur = pGryImg + y * width + *left;
	x = 0;
	while (x < w) {
		*(pCur++) = 255;
		x++;
	}
	//�·�
	y = *down;
	x = *left;
	pCur = pGryImg + y * width + x;
	x = 0;
	while (x < w) {
		*(pCur++) = 255;
		x++;
	}
	//��
	y = *up, x = *left;
	pCur = pGryImg + y * width + x;
	y = 0;
	while (y < h) {
		*pCur = 255;
		pCur += width;
		y++;
	}
	//�ҷ�
	y = *up, x = *right;
	pCur = pGryImg + y * width + x;
	y = 0;
	while (y < h) {
		*pCur = 255;
		pCur += width;
		y++;
	}
	return;
}
//ͼ����
void myCopyGryRect(BYTE* pGryImg, int width, int height, int x1, int x2, int y1, int y2, BYTE* pRectImg)
{   //ͼ��鸴��
	BYTE* pRow, * pRect;
	int y, w;
	pRectImg = new unsigned char[(y2 - y1) * (x2 - x1)];
	w = x2 - x1 + 1;
	pRect = pRectImg;
	for (y = y1, pRow = pGryImg + y * width + x1; y <= y2; y++, pRow += width)
	{
		memcpy(pRect, pRow, w);
		pRect += w;
	}
	return;
}
void myExam4() {
	clock_t start_t, end_t;
	
	char filename[255];
	unsigned char* pGryImg;
	int height, width;

	int w = 22, h = 26;//Ŀ���С
	int shrink_a = 16;//���ű���
	int shrink_h, shrink_w;//���ź��Ŀ���С
	int sq = sqrt(shrink_a);
	int* x1 = new int;
	int* x2 = new int;
	int* y1 = new int;
	int* y2 = new int;
	unsigned char* pResImg=NULL ;
	start_t = clock();
	for (int i = 2; i <= 4; i++) {
		//��ͼ��
		sprintf_s(filename, "./data/H040%dGry.bmp", i);
		//std::cout << "����ִ���ļ���" << filename << "\n";
		pGryImg = Rmw_Read8BitBmpFile2Img(filename, &width, &height);
		//������ͼƬ��С�̶��Ļ�������д�����Ŵ�С��ͼƬ��С
		shrink_h = height / sq - (height / sq) % sq;
		shrink_w = width / sq - (width / sq) % sq;
		unsigned char* pShrinkImg = new unsigned char[shrink_h * shrink_w];
		int* pSumImg = new int[width * height];//����ͼ
		int* pSumShrinkImg = new int[shrink_h * shrink_w];//ѹ����Ļ���ͼ
		unsigned char* pSbImg = new unsigned char[shrink_h * shrink_w];
		if (!pGryImg)
		{
			printf("*file open err!\n");
			getchar();
			continue;
		}
		else {
			//printf("%s,width=%d,height=%d\n", filename, width, height);
		}
		//��ԭʼͼ����д���
		
		//1������ԭʼͼ��Ļ���ͼ
		doSumImg_SSE(pGryImg, width, height, pSumImg);
		//2��ѹ��ԭʼͼ��
		toSmall(pGryImg, width, height, pShrinkImg, pSumImg, shrink_a);//ѹ��ͼ��
		//sprintf_s(filename, "./res_data/my_dect_H040%dGryShrink.bmp", i);
		//Rmw_Write8BitImg2BmpFile(pShrinkImg, shrink_w, shrink_h, filename);
		//3����Եǿ�ȼ��ѹ�����ͼ��
		Sobel(pShrinkImg, shrink_w, shrink_h, pSbImg);
		//sprintf_s(filename, "./res_data/my_dect_H040%dGrySb.bmp", i);
		//Rmw_Write8BitImg2BmpFile(pSbImg, shrink_w, shrink_h, filename);
		//4������ѹ�����ͼ��Ļ���ͼ
		doSumImg_SSE(pShrinkImg, shrink_w, shrink_h, pSumShrinkImg);
		//5��������
		//��x������ ��y������
		searchMaxPos(pSumShrinkImg,shrink_w,shrink_h,w,h,20,25,x1,x2,y1,y2);
		*x1 = (*x1) * sq;
		*x2 = (*x1) +w*sq;
		*y1 = (*y1) * sq;
		*y2 = (*y1) + h * sq;
		//std::cout << *x1 << " " << *x2 << " " << *y1 << " " << *y2 << endl;
		//6�����ػ��ƵĽ��
		//Draw(pGryImg,width,w*sq,h*sq,x1,x2,y1,y2);
		//sprintf_s(filename, "./res_data/my_dect_H040%dGry.bmp", i);
		//Rmw_Write8BitImg2BmpFile(pGryImg, width, height,filename);
		
	}
	end_t = clock();
	std::cout << "ִ�н�������" << (end_t - start_t) / 1000.0 << "����";
}
int main() {
	//testEdgeSharpen();
	//testExtractRiceEdge();/*4.6�����*/
	//Exam5_2();
	//Exam5_3();
	//Exam5_4();
	


	//��������Ϊ���ֺ������ص���������������ʵ������ؿ�һ����Ŀ������Exam4(),ע��ͷ�ļ�����Ӻ�ɾ������Ȼһ��bug~
	//�������ʦ�ķ���
	Exam4();
	//�ҵķ���
	//myExam4();
	return 0;
}