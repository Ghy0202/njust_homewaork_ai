#include<stdio.h>
#include<iostream>
#include<Windows.h>
#include<time.h>
#include<vector>
#include <nmmintrin.h>
#include"bmpFile.h"
#include"RmwALG_AverageFilter.h"
using namespace std;
/*����һ����ɫת�Ҷ�*/
//��ɫת��ɫ�������˳���
void Color2Gry_Count1(unsigned char* pImg, int width, int height) {
	//���ü���ķ�ʽ
	unsigned char* p = pImg, * pEnd = pImg + height * width * 3;
	for (; p != pEnd; p++) {
		int r = *p;
		int g = *(p + 1);
		int b = *(p + 2);
		int gry = (299 * r + 587 * g + 114 * b) / 1000;
		*p = gry;
		*(++p) = gry;
		*(++p) = gry;
	}

}
//��ɫת��ɫ����������
void Color2Gry_Count2(unsigned char* pImg, int width, int height) {
	//���ü���ķ�ʽ��R*0.299 + G*0.587 + B*0.114
	unsigned char* p = pImg, * pEnd = pImg + height * width * 3;
	for (; p != pEnd; p++) {
		int r = *p;
		int g = *(p + 1);
		int b = *(p + 2);
		int gry = r * 0.299 + g * 0.587 + b * 0.114;
		*p = gry;
		*(++p) = gry;
		*(++p) = gry;
	}

}
//��ɫת��ɫ��λ��
void Color2Gry_Update(unsigned char* pImg, int width, int height) {
	//����λ�Ƶķ�ʽ
	unsigned char* p = pImg, * pEnd = pImg + height * width * 3;
	//Gray = (R*19595 + G*38469 + B*7472) >> 16
	for (; p != pEnd; p++) {
		int r = *p;
		int g = *(p + 1);
		int b = *(p + 2);
		int gry = (r * 19595 + g * 38469 + b * 7472) >> 16;
		*p = gry;
		*(++p) = gry;
		*(++p) = gry;
	}

}
void Exam3_1() {
	unsigned char* pRgbImg;
	unsigned char* pRgbImg_copy;
	unsigned char* pRgbImg_copy2;
	int width, height;
	pRgbImg = Rmw_Read24BitBmpFile2Img("./data/H0301Rgb.bmp", &width, &height);
	pRgbImg_copy = pRgbImg;
	pRgbImg_copy2 = pRgbImg;
	if (!pRgbImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	/*----------------1.�������������ķ���------------------------------------*/
	clock_t start_t = clock();
	unsigned char* pImg;
	for (int i = 0; i < 1000; i++) {
		pImg = pRgbImg_copy;
		Color2Gry_Count1(pImg, width, height);
	}
	Rmw_Write24BitImg2BmpFile(pImg, width, height, "./res_data/H0301Gry_Count.bmp");
	clock_t end_t = clock();
	printf("����������������ĵķ�����ʱ:%d\n", end_t - start_t);
	/*----------------2.���ڸ������ķ���------------------------------------*/
	start_t = clock();
	for (int i = 0; i < 1000; i++) {
		pImg = pRgbImg_copy2;
		Color2Gry_Count2(pImg, width, height);
	}
	Rmw_Write24BitImg2BmpFile(pImg, width, height, "./res_data/H0301Gry_Count1.bmp");
	end_t = clock();
	printf("���ڸ���������ĵķ�����ʱ:%d\n", end_t - start_t);
	/*--------------3.����λ�Ƶķ�ʽ-----------------------------------------*/
	start_t = clock();
	for (int i = 0; i < 1000; i++) {
		pImg = pRgbImg;
		Color2Gry_Update(pImg, width, height);
	}
	Rmw_Write24BitImg2BmpFile(pImg, width, height, "./res_data/H0301Gry_Update.bmp");
	end_t = clock();
	printf("����λ�Ƽ���ĵķ�����ʱ:%d\n", end_t - start_t);
}
/*����� �о�ֵ�˲�*/
void RmwAvrFilterByCol(BYTE* pGryImg,  //ԭʼ�Ҷ�ͼ��
	int width, int height,//ͼ��Ŀ�Ⱥ͸߶�
	int M,//�˲�����M��
	BYTE* pResImg
)
{   
	BYTE* pCur, * pRes;
	int halfx;
	int x, y;
	int sum, c;
	//��ʼ��
	M = M / 2 * 2 + 1;//������
	halfx = M / 2;
	c = (1 << 23) / M;//�˷�����
	for (y = 0, pRes = pResImg, pCur = pGryImg; y < height; y++) {
		//��ֵ
		for (sum = 0, x = 0; x < M, x++;) {
			sum += *(pCur + x);
		}
		//�˲�
		pRes += halfx;//������಻��������
		pCur += halfx;
		for (x = halfx; x < width - halfx; x++) {
			//��ҶȾ�ֵ
			*(pRes++) = ((sum * c) >> 23);//�������˷���λ�ƴ������
			sum -= *(pCur - halfx);
			sum += *(pCur + halfx + 1);
			pCur++;
		}
		pRes += halfx;
		pCur += halfx;
	}
	return;
}
void Exam3_2() {
	//���ڻҶ�ͼ��ÿһ�еľ�ֵ�˲�21ά��
	unsigned char* pGryImg;
	
	int width, height;
	pGryImg = Rmw_Read8BitBmpFile2Img("./data/H0302Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	unsigned char* presImg= Rmw_Read8BitBmpFile2Img("./data/H0302Gry.bmp", &width, &height);
	//��д�Ĳ�̫��ȷ
	//RmwAvrFilterByCol(pGryImg, width, height, 21, presImg);
	//�����
	RmwAvrFilterBySumCol(pGryImg, width, height, 21,1, presImg);
	Rmw_Write8BitImg2BmpFile(presImg, width, height, "./res_data/H0302Gry_myAvgCol.bmp");
	printf("Avg Col success!\n");
}
/*�����������û���ͼ�ľ�ֵ�˲�*/
//�������ͼ
int* Count_pSumImg(unsigned char* pGryImg, int height,int width) {
	unsigned char*pCur = pGryImg;
	unsigned char* pEnd = pCur + width * height;
	vector<int>v;
	v.resize(height * width);
	fill(v.begin(), v.end(), 0);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (x == 0 && y == 0) {
				v[y * width + x] = *pCur;
			}
			else if (x == 0) {
				v[y * width + x] = *pCur + v[(y - 1) * width + x];
			}
			else if (y == 0) {
				v[y * width + x] = *pCur + v[y * width + x-1];
			}
			else {
				v[y * width + x] = *pCur + v[(y - 1) * width + x] + v[y * width + x - 1] - v[(y-1) * width + x - 1];
			}
			pCur++;
			if (pCur == pEnd) {
				break;
			}
		}
		if (pCur == pEnd) {
			break;
		}
	}
	int* ans = new int[height * width];
	int* p = ans;
	for (int i = 0; i < width * height; i++) {
		*p = v[i];
		p++;
	}
	return ans;
}
void Exam3_3() {
	unsigned char* pGryImg;

	int width, height;
	pGryImg = Rmw_Read8BitBmpFile2Img("./data/H0302Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	unsigned char* presImg = Rmw_Read8BitBmpFile2Img("./data/H0302Gry.bmp", &width, &height);
	//�������ͼ
	int* pSumImg;
	pSumImg=Count_pSumImg(pGryImg, height, width);
	printf("!");
	//���ڻ���ͼ���˲�
	RmwAvrFilterBySumImg(pSumImg, width, height, 21, 1, presImg);
	Rmw_Write8BitImg2BmpFile(presImg, width, height, "./res_data/H0302Gry_SumImg_AvgCol.bmp");
	printf("Avg Col success!\n");
}
/*--------------SSEʵ��ͼƬ�ķ���---------*/
void RmwSSE_Invert(BYTE* pGryImg,  //ԭʼ�Ҷ�ͼ��
	int width, //ͼ��Ŀ��,������4�ı���
	int height
)
{
	BYTE* pCur = pGryImg;
	BYTE* pEnd = pGryImg + height * width;
	__m128i* pL, * pRes, temp_Cur;
	int L[] = { 255,255,255,255 };
	int res[4];
	pL = (__m128i*)(L);
	pRes = (__m128i*)(res);
	for (; pCur < pEnd; pCur += 4) {
		//�����ĵ�32λת����4�����صĻҶ�ֵ
		temp_Cur = _mm_cvtepu8_epi32(_mm_loadl_epi64((__m128i*)pCur));
		*pRes = _mm_sub_epi32(*pL, temp_Cur);//255-��������ָ��
		*pCur = res[0];//��ֵ��ȥ
		*(pCur + 1) = res[1];
		*(pCur + 2) = res[2];
		*(pCur + 3) = res[3];
	}
	//pCur -= 3;
	//while (pCur < pEnd) {
	//	*(pCur++) = 255 - *(pCur++);
	//}
	return;
}
void Exam3_4() {
	unsigned char* pGryImg;

	int width, height;
	pGryImg = Rmw_Read8BitBmpFile2Img("./data/H0302Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	//�������ͼ
	int* pSumImg;
	pSumImg = Count_pSumImg(pGryImg, height, width);
	printf("������\n");
	RmwSSE_Invert(pGryImg, width, height);
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "./res_data/H0302Gry_Invert.bmp");
	printf("Invert success!\n");

}
int main() {
	//Exam3_1();
	//Exam3_2();
	//Exam3_3();
	Exam3_4();




	return 0;
}