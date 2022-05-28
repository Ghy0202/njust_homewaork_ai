#include<stdio.h>
#include<string>
#include<string.h>
#include<math.h>
#include<windows.h>
#include<time.h>
#include<vector>
#include<iostream>
#include<algorithm>

#include "bmpFile.h"
#include "RmwALG_HoughLine.h"
#include "RmwALG_HoughCircle.h"
#include"RmwDraw1Data2Img.h"
#include "RmwALG_ContourTrace.h"
#include "RmwALG_ContourFill.h"

using namespace std;

//ȫ�ֱ���
#define  MAX_POINT_NUM   (512*512)
#define MAX_CONOUR_LENGTH  (512*512)
#define PI 3.1415926
BYTE gChainCode[MAX_CONOUR_LENGTH];

static int gN;
static int gX[MAX_POINT_NUM];
static int gY[MAX_POINT_NUM];
/*�ʼ�����ͼ��ռ�*/
void Test(int t) {
	int width = 256;
	int height = 256;
	int x[] = { 128,0,255,255 };
	int y[] = { 128,255,0,255 };
	int cosV, sinV, maxThro;
	int  thro;
	int theta;
	int i=t;
	int maxCount ,bstTheta , bstThro ;
	int* pCenter, * pCur;
	maxThro = int(sqrt(1.0 * width + width + height * height + 0.5) + 1);
	int* pCount = new int[(maxThro *2* 180)];
	memset(pCount, 0, sizeof(int) * (maxThro * 2 * 180));
	//����1
	for (theta = 0; theta < 180; theta++) {
		cosV = int(cos(theta * PI / 180) * 8912);
		sinV = int(sin(theta * PI / 180) * 8912);
		pCenter = pCount + int((maxThro * 2) * theta) + maxThro;
		thro = ((x[i] * cosV + y[i] * sinV) >> 13);
		*(pCenter + thro) += 1;
		//std::cout << "!" << *(pCenter + thro) << endl;
	}
	maxCount = bstTheta = bstThro = 0;
	for (theta = 0, pCur = pCount; theta < 180; theta++)
	{
		for (thro = 0; thro < maxThro * 2; thro++, pCur++)
		{
			if (*pCur > maxCount)
			{
				maxCount = *pCur;
				bstTheta = int(theta);
				bstThro = thro;
			}
		}
	}
	bstThro -= maxThro; //ȥ��ƫ��maxThro
	printf("theta=%d,thro=%d\n", bstTheta, bstThro);
	unsigned char* pCountImg = new unsigned char[maxThro * 2 * 180];
	for (int k = 0; k < maxThro * 2 * 180; k++) {
		if (pCount[k] != 0) {
			pCountImg[k] = 255;
		}
		else {
			pCountImg[k] = 0;
		}
			
	}
	char tname[90];
	sprintf_s(tname, "./res_data/%d.bmp", i);
	Rmw_Write8BitImg2BmpFile(pCountImg, maxThro, 2 * 180,tname);

}
/*����һ����ߣ���ʾ������*/
struct Ans{
	int theta;
	int thro;
	int count;
};
vector<Ans>ans;
bool cmp(Ans a, Ans b) {
	return a.count > b.count;
}
int HoughLine(int width, int height,
	int* x, int* y, int N,
	double* A, double* B, double* C,int* pCount
)
{
	int maxCount, bstTheta, bstThro;
	int* pCenter, * pCur;
	int maxThro, cosV, sinV;
	int theta, thro, i;
	// step.1-------------����������ռ�---------------------//
	maxThro = (int)sqrt(1.0 * width * width + height * height + 0.5) + 1;
	//pCount = new int[(maxThro*2)*180]; //(-maxThro,maxThro)
	if (!pCount) return 0; //����pCount�ڸú���������
	// step.2-------------����任---------------------------//
	memset(pCount, 0, sizeof(int) * (maxThro * 2) * 180);
	for (theta = 0; theta < 180; theta++) //����Ϊ1��
	{
		cosV = (int)(cos(theta * 3.1415926 / 180) * 8192); //�Ŵ�8192��
		sinV = (int)(sin(theta * 3.1415926 / 180) * 8192);
		pCenter = pCount + (maxThro * 2) * theta + maxThro; //����ƫ��maxThro
		for (i = 0; i < N; i++)
		{
			thro = ((x[i] * cosV + y[i] * sinV) >> 13); //��С8192��,thro�Ĳ���Ϊ1
			*(pCenter + thro) += 1;
		}
	}
	// step.3-------------���ֵ����-------------------------//
	maxCount = bstTheta = bstThro = 0;
	for (theta = 0, pCur = pCount; theta < 180; theta++)
	{
		for (thro = 0; thro < maxThro * 2; thro++, pCur++)
		{
			if (*pCur > 0) {
				Ans temp;
				temp.theta = theta;
				temp.thro = thro;
				temp.count = *pCur;
				ans.push_back(temp);
			}
			if (*pCur > maxCount)
			{
				maxCount = *pCur;
				bstTheta = theta;
				bstThro = thro;
			}
		}
	}
	sort(ans.begin(), ans.end(), cmp);
	for (int k = 0; k < 4; k++) {
		std::cout << " theta:" << ans[k].theta << ",thro: " << ans[k].thro << endl;
	}
	bstThro -= maxThro; //ȥ��ƫ��maxThro
	// step.4-------------��ֱ��Ax+By+C��ֵ-----------------//
	//x*cos(bstTheta)+y*sin(bstTheta)=bstThro => Ax+By+C=0
	*A = cos(bstTheta * 3.1415926 / 180);
	*B = sin(bstTheta * 3.1415926 / 180);
	*C = -bstThro;
	//printf("%lfX+%lfY+%lf\n", *A, *B, *C);
	//printf("theta=%d,thro=%d\n", bstTheta, bstThro);
	unsigned char* pCountImg = new unsigned char[maxThro * 2 * 180];
	for (int k = 0; k < maxThro * 2 * 180; k++) {
		if (pCount[k] != 0) {
			pCountImg[k] = 255;
		}
		else {
			pCountImg[k] = 0;
		}

	}
	char tname[90];

	Rmw_Write8BitImg2BmpFile(pCountImg, maxThro, 2 * 180, "ccb.bmp");
	// step.5-------------���ؾ����ĵ���---------------------//
	//delete pCount; //�ͷ��Լ�������ڴ�,�ú����ڶ�̬������ͷŻ�����ڴ���Ƭ
	return maxCount;
}

void Exam1(string filename,string targetname) {
	unsigned char* pGryImg,*pResImg,* gDbgImg;
	int width, height;
	int* pCount = new int[MAX_POINT_NUM];
	double A, B, C;
	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img(filename.c_str(), &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	//TODO:�������Ҫ��ȫ�Լ�д��
	pResImg = new unsigned char[width * height];
	/*---------�Ѽ���Ե��---------*/
	int x, y;
	gN = 0;
	for (y = 0; y < height; y++)
	{
		//������
		for (x = 0; x < width; x++) {
			if (*(pGryImg + y * width + x) == 0) {
				gX[gN] = x;
				gY[gN] = y;
				gN++;
				pResImg[y * width + x] = 0;
				break;
			}
		}
	}
	int h=RmwHoughLine(width, height, gX, gY, gN, &A, &B, &C,pCount);
	RmwDrawABCLine(pResImg, width, height, A, B, C, 254);
	gN = 0;
	for (y = 0; y < height; y++) {
		//���ҵ���
		for (x = width - 1; x >= 0; x--) {
			if (*(pGryImg + y * width + x) == 0) {
				gX[gN] = x;
				gY[gN] = y;
				gN++;
				pResImg[y * width + x] = 0;
				break;
			}
		}
	}
	h = RmwHoughLine(width, height, gX, gY, gN, &A, &B, &C, pCount);
	RmwDrawABCLine(pResImg, width, height, A, B, C, 254);
	gN = 0;
	for (x = 0; x < width; x++)
	{
		//���ϵ���
		for (y = 0; y < height; y++) {
			if (*(pGryImg + y * width + x) == 0) {
				gX[gN] = x;
				gY[gN] = y;
				gN++;
				pResImg[y * width + x] = 0;
				break;
			}
		}
	}
	h = RmwHoughLine(width, height, gX, gY, gN, &A, &B, &C, pCount);
	RmwDrawABCLine(pResImg, width, height, A, B, C, 254);
	gN = 0;
	for (x = 0; x < width; x++) {
		//���µ���
		for (y = height - 1; y >= 0; y--) {
			if (*(pGryImg + y * width + x) == 0) {
				gX[gN] = x;
				gY[gN] = y;
				gN++;
				pResImg[y * width + x] = 0;
				break;
			}
		}
	}
	h = RmwHoughLine(width, height, gX, gY, gN, &A, &B, &C, pCount);
	RmwDrawABCLine(pResImg, width, height, A, B, C, 254);
	/*����һ��һ���ؼ��*/
	
	printf("%d ", gN);
	//Rmw_Write8BitImg2BmpFile(pResImg, width, height, "debug.bmp");
	//Rmw_Write8BitImg2BmpFileMark(pResImg, width, height, (targetname + "_one2one.bmp").c_str());

	/*ֱ�Ӳ�һ����*/

	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}
void Exam1_1(string filename,string targetname) {
	//���μ��
	unsigned char* pGryImg, * gDbgImg,*pResImg;
	int width, height;
	int* pCount = new int[MAX_POINT_NUM];
	double A, B, C;
	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img(filename.c_str(), &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	pResImg = new unsigned char[width * height];
	//���μ�⣺Ԥ�����ͼ���м�ĵ�
	//����ĸ���ֵ
	int x, y;
	gN = 0;
	for (y = 0; y < height; y++)
	{
		//������
		for (x = 0; x < width; x++) {
			if (*(pGryImg + y * width + x) == 0) {
				gX[gN] = x;
				gY[gN] = y;
				gN++;
				pResImg[y * width + x] = 0;
				break;
			}
		}
		//���ҵ���
		for (x = width - 1; x >= 0; x--) {
			if (*(pGryImg + y * width + x) == 0) {
				gX[gN] = x;
				gY[gN] = y;
				gN++;
				pResImg[y * width + x] = 0;
				break;
			}
		}
	}
	for (x = 0; x < width; x++)
	{
		//���ϵ���
		for (y = 0; y < height; y++) {
			if (*(pGryImg + y * width + x) == 0) {
				gX[gN] = x;
				gY[gN] = y;
				gN++;
				pResImg[y * width + x] = 0;
				break;
			}
		}
		//���µ���
		for (y = height - 1; y >= 0; y--) {
			if (*(pGryImg + y * width + x) == 0) {
				gX[gN] = x;
				gY[gN] = y;
				gN++;
				pResImg[y * width + x] = 0;
				break;
			}
		}
	}
	int maxThro = (int)sqrt(1.0 * width * width + height * height + 0.5) + 1;
	int h = HoughLine(width, height, gX, gY, gN, &A, &B, &C, pCount);
	for (int i = 0; i < 4; i++) {
		A = cos(ans[i].theta * 3.1415926 / 180);
		B = sin(ans[i].theta * 3.1415926 / 180);
		C = -(ans[i].thro-maxThro);
		cout << A << " " << B << " " << C << endl;
		RmwDrawABCLine(pResImg, width, height, A, B, C, 254);
	}
	Rmw_Write8BitImg2BmpFileMark(pResImg, width, height, (targetname + "_four.bmp").c_str());
}
/*����������η�����Բ*/
void Exam2(string filename,string targetfile) {
	unsigned char* pGryImg,* gDbgImg;
	int width, height;
	int i=0;
	int R = 0;
	int x0=-1, y0=-1;
	int maxCountx = 0;
	int maxCounty = 0;
	int maxCountr = 0;
	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img(filename.c_str(), &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	else {
		printf("success open file\n");
		gDbgImg = new unsigned char[width * height];
	}
	// step.2-----------���η���������Բ��---------------------//
	int cnt = 0;
	int* pCountX=new int[width + 1];
	int* pCountY = new int[height + 1];
	int* pCountR = new int[int(sqrt(width * width + height * height * 1.0)) + 1];
	memset(pCountX, 0, sizeof(int)*(width+1));
	memset(pCountY, 0, sizeof(int)*(height+1));
	memset(pCountR, 0, sizeof(int)*(int(sqrt(width * width + height * height * 1.0)) + 1));
	vector<int>posx;
	vector<int>posy;
	//�Ժ����꣬��Ե���ּ��
	int xl, xr,xc;
	for (int y = 0; y < height; y++) {
		//�����ң�����ߵı�Ե��
		for (xl = 0; xl < width; xl++) {
			if (!pGryImg[y * height + xl]) {
				posx.push_back(xl);
				posy.push_back(y);
				cnt++;
				break;
			}
		}
		//���ҵ������ұߵı�Ե��
		for (xr = width - 1; xr >= 0; xr--) {
			if (!pGryImg[y * height + xr]) {
				posx.push_back(xr);
				posy.push_back(y);
				cnt++;
				break;
			}
		}
		//�е����
		if (xr - xl > 0)
		{
			xc = (xr + xl + 1) >> 1;
			pCountX[xc]++;
			if (pCountX[xc] > maxCountx) {
				maxCountx = pCountX[xc];
				x0 = xc;
			}
		}

	}
	//�������꣬��Ե���ּ��
	int yl, yr,yc;
	for (int x = 0; x < width; x++) {
		//���ϵ��£����ϱߵı�Ե��
		for (yl = 0; yl < height; yl++) {
			if (!pGryImg[yl * height + x]) {
				posx.push_back(x);
				posy.push_back(yl);
				cnt++;
				break;
			}
		}
		//���µ��ϣ����±ߵı�Ե��
		for (yr = height-1; yr >=0; yr--) {
			if (!pGryImg[yr * height + x]) {
				posx.push_back(x);
				posy.push_back(yr);
				cnt++;
				break;
			}
		}
		//�е����
		if (yr-yl > 0)
		{
			yc = (yr + yl + 1) >> 1;
			pCountY[yc]++;
			if (pCountY[yc] > maxCounty) {
				maxCounty = pCountY[yc];
				y0 = yc;
			}
		}
	}
	//������������R
	int r;
	for (int i = 0; i < cnt; i++) {
		r = int(sqrt((posx[i]-x0)* (posx[i] - x0)+(posy[i]-y0)* (posy[i] - y0)));
		pCountR[r]++;
		if (pCountR[r] > maxCountr) {
			maxCountr = pCountR[r];
			R = r;
		}
	}

	printf("����õ������꣨%d,%d��,�뾶%d\n", x0, y0, R);
	for (int i = 0; i < width * height; i++) {
		if (pGryImg[i] == 255) {
			pGryImg[i] = 250;
		}
	}
	//��Բ
	RmwDrawCircle(pGryImg, width, height, x0,y0, R, 255);//��255�����
	Rmw_Write8BitImg2BmpFileMark(pGryImg, width, height, (targetfile+"_circle.bmp").c_str());
	//��ֱ��ͼX
	memcpy(gDbgImg, pGryImg, width* height);
	RmwDrawData2ImgCol(pCountX, height, 1.0, gDbgImg, width, height, 254);
	Rmw_Write8BitImg2BmpFileMark(gDbgImg, width, height, (targetfile + "_circle_col.bmp").c_str());
	//��ֱ��ͼY
	memcpy(gDbgImg, pGryImg, width * height);
	RmwDrawData2ImgRow(pCountY, width, 1.0, gDbgImg, width, height, 254);
	Rmw_Write8BitImg2BmpFileMark(gDbgImg, width, height, (targetfile + "_circle_row.bmp").c_str());
	//�뾶��ֱ��ͼ
	int w, h;
	w = int(sqrt(width * width + height * height * 1.0)) + 1;
	h = R + 20;
	gDbgImg = new unsigned char[w * h];
	RmwDrawData2ImgCol(pCountR, width, 1.0, gDbgImg, w, h, 253);
	Rmw_Write8BitImg2BmpFileMark(gDbgImg, w,h, (targetfile + "_circle_r.bmp").c_str());
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete gDbgImg;
	delete pCountR;
	delete pCountX;
	delete pCountY;
	printf("Press any key to exit!\n");
	getchar();
	return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//	�����5-6����õĺ���
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//���߿�����Ϊcolorֵ
void setBoundary(BYTE*pBinImg,int width,int height,BYTE color) {
	BYTE* pRow;
	int y;

	memset(pBinImg, color, width);//��һ��
	memset(pBinImg + (height - 1) * width, color, width);
	for (y = 0, pRow = pBinImg; y < height-1; y++, pRow += width)
	{
		*pRow = 0;
		*(pRow + width - 1) = 0;
	}
	memset(pBinImg, color, width);//���һ��
	return;
}
static int dx[8] = { 1, 1, 0,-1,-1,-1, 0, 1 };
static int dy[8] = { 0,-1,-1,-1, 0, 1, 1, 1 };
//�����㷨
int TraceContour(BYTE* pBinImg, int width, int height,//��ֵͼ��
	int x0, int y0, //�������
	bool isOuter, //�Ƿ���������
	BYTE* pChainCode, //�ⲿ����õ�һ���ڴ�ռ䣬�����������
	int maxChainCodeNum //���Դ�ŵ�����������
)
{
	static int initCode[8] = { 7, 7, 1, 1, 3, 3, 5, 5 };
	int dADD[8]; //��ַƫ����
	BYTE* pBegin, * pCur, * pTst; //�������,��ǰ��,����
	int code, beginCode, returnCode, i;
	int N;

	// step.1-----��ʼ��----------------------------------------//
	//��ͬ�����Ӧ�ĵ�ַƫ����
	for (code = 0; code < 8; code++) dADD[code] = dy[code] * width + dx[code];
	pBegin = pBinImg + y0 * width + x0; //�������ĵ�ַ
	pCur = pBegin; //��ǰ�����õ��������
	if (isOuter) //������ʱ�ĳ�ʼ��
	{
		*(pCur - 1) = 1;  //����Ǳ�����,���Ϊ�Ҷ�ֵ1
		code = 7; //��ʼ��Ϊ7
	}
	else //������ʱ�ĳ�ʼ��
	{
		*(pCur + 1) = 1; //�Ҳ��Ǳ�����,���Ϊ�Ҷ�ֵ1
		code = 3; //��ʼ��Ϊ3
	}
	beginCode = initCode[code]; //�������ĵ�һ������
	// step.2-----��������--------------------------------------//
	N = 0; //���������ʼ��Ϊ0
	do {
		*pCur = 254; //��������,���Ϊ�Ҷ�ֵ254
		for (i = 0, code = initCode[code]; i < 7; i++, code = (code + 1) % 8)
		{
			pTst = pCur + dADD[code]; //�õ�Ҫ����������ĵ�ַ
			if (*pTst < 2) *pTst = 1; //�Ǳ�����,���Ϊ�Ҷ�ֵ1
			else //��������
			{
				if (N < maxChainCodeNum) pChainCode[N++] = code; //��������
				if (pTst == pBegin) //�ص����Ĵ���
				{
					//�ҳ�ʣ���������ʼ���
					returnCode = (code + 4) % 8; //ת��Ϊ�ڴ��������Ѿ�����������
					for (i = 0, code = beginCode; i < 7; i++, code = (code + 1) % 8)
					{
						if (code == returnCode)
						{
							i++; //ʣ���������ʼ���
							code = (code + 1) % 8; //ʣ���������ʼֵ
							break;
						}
					}
					//���ʣ������
					for (; i < 7; i++, code = (code + 1) % 8)
					{
						pTst = pBegin + dADD[code];
						if (*pTst < 2) *pTst = 1; //�Ǳ�����,���Ϊ�Ҷ�ֵ1
						else
						{   //��������
							if (N < maxChainCodeNum) pChainCode[N++] = code;
							break; //����㿪ʼ,�ҵ����µ�������pTst
						}
					}
				}
				break; //�ҵ����µ�������pTst
			}
		}
		pCur = pTst; //��ǰ���ƶ����µ�������pTst
	} while (i < 7); //�ҵ�������ʱһ����i<7
	// step.3-----����-----------------------------------------//
	return N; //�����������
}
//��Ӿ���
void ContourRect(int x0, int y0, BYTE* pCode, int N, int* x1, int* x2, int* y1, int* y2)
{
	int i;

	*y1 = *y2 = y0;
	*x1 = *x2 = x0;
	for (i = 0; i < N - 1; i++)
	{
		x0 = x0 + dx[pCode[i]];
		y0 = y0 + dy[pCode[i]];

		if (x0 > *x2) *x2 = x0;
		else if (x0 < *x1) *x1 = x0;

		if (y0 > *y2) *y2 = y0;
		else if (y0 < *y1) *y1 = y0;
	}
	return;
}
//���������
void FillContour(BYTE* pGryImg, int width, int height,//�����ͼ��
	int x0, int y0, //�������
	bool isOuter, //�Ƿ���������
	BYTE* pCode, //����
	int N, //�������
	int regionC, //�����ڲ�����ɫ
	int contourC, //��������ɫ
	int nonC //�����ڵ���ɫ
)
{
	
	int dADD[8]; //��ַƫ����
	BYTE* pBegin, * pCur, * pTst; //�������,��ǰ��,����
	int inCode, outCode, i;
	int sumC, sumDy, direction;

	// step.1-----��ʼ��----------------------------------------//
	pBegin = pGryImg + y0 * width + x0; //�������ĵ�ַ
	//��ͬ�����Ӧ�ĵ�ַƫ����
	for (i = 0; i < 8; i++) dADD[i] = dy[i] * width + dx[i];
	// step.2-----���������������������仯��֮�͸���ֵ----------//
	for (sumC = 2; sumC <= 253; sumC++) //��sumC
	{
		if ((regionC >= sumC - 2) && (regionC <= sumC + 2)) continue;
		if ((nonC >= sumC - 2) && (nonC <= sumC + 2)) continue;
		break;
	}
	for (i = 0, pCur = pBegin; i < N; pCur += dADD[pCode[i]], i++) *pCur = sumC;
	// step.3-----�������������������������仯��֮��------------//
	inCode = pCode[N - 1]; //����������������
	for (i = 0, pCur = pBegin; i < N; i++)
	{
		outCode = pCode[i]; //�Ӹ����������������
		*pCur += dy[inCode] + dy[outCode]; //���صĻҶ�ֵ���Ǳ仯��֮�� 
		inCode = outCode; //��ǰ������ĳ����������һ��������Ľ�����
		pCur += dADD[outCode]; //ָ����һ��������ĵ�ַ
	}
	// step.4-----�������������յ���б��--------------------//
	for (i = 0, pCur = pBegin; i < N; pCur += dADD[pCode[i]], i++)
	{
		sumDy = *pCur;
		if ((sumDy == sumC + 1) || (sumDy == sumC + 2)) *pCur = regionC; //���Ϊ������
		else if ((sumDy == sumC - 1) || (sumDy == sumC - 2)) *pCur = nonC; //���Ϊ����յ�
	}
	// step.5-----�����������������յ�֮��������------------//
	direction = isOuter ? 1 : -1; //�������Ǵ�������,������+1;��������֮
	for (i = 0, pCur = pBegin; i < N; pCur += dADD[pCode[i]], i++)
	{
		if (*pCur == regionC) //�ҵ�һ��������
		{
			pTst = pCur;
			while (*pTst != nonC) //һֱ��䵽�յ�
			{
				*pTst = regionC;
				pTst += direction;
			}
			*pCur = nonC; //��ˮƽ���Ѿ�������,�����ظ����
		}
	}
	// step.6-----�����������ɫ���и�ֵ-------------------------//
	for (i = 0, pCur = pBegin; i < N; pCur += dADD[pCode[i]], i++) *pCur = contourC;
	// step.7-----����-----------------------------------------//
	return;
}
//����������
void DrawContour(BYTE* pGryImg, int width, int x, int y, BYTE* pCode, int N, BYTE color)
{
	int i, dADD[8];
	BYTE* pCur;

	*(pGryImg + y * width + x) = color; // Ϊ���ܹ������պϵ�����
	for (i = 0; i < 8; i++) dADD[i] = dy[i] * width + dx[i];
	for (pCur = pGryImg + y * width + x, i = 0; i < N; i++)
	{
		pCur += dADD[pCode[i]];
		*pCur = color;
	}
	return;
}
/*�����������ٺ����*/
void Exam3() {
	unsigned char* pGryImg,*pCur;
	int width, height;
	int x, y;
	int N, S;
	int x1, x2, y1, y2;
	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("./data/H0605Bin.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE* pDbgImg = new BYTE[width * height];//��Ŵ�ͼ��
	memset(pDbgImg, 0, width * height);
	// step.2 -------------�������----------------------//
	//���Ƚ��߿����ó�0
	setBoundary(pGryImg, width, height, 0);
	//�����Ǳ��������ϵ��£���������ɨ��ͼ��Ѱ���µ�����
	for (y = 1, pCur = pGryImg + y * width; y < height - 1; y++) {
		pCur++;//��������ߵ�����
		for (x = 1; x < width - 1; x++, pCur++) {
			if ((*pCur == 255) && (*(pCur - 1) <= 1)) {
				//������һ��������
				N = TraceContour(pGryImg, width, height,
					x, y, //�������
					true, //��������
					gChainCode, //����������������
					MAX_CONOUR_LENGTH //����Ĵ�С
				);
				//�������
				S = RmwContourPixels(gChainCode, N);
				//��Ӿ���
				ContourRect(x, y, gChainCode, N, &x1, &x2, &y1, &y2);
				if ((x1 > 1) && (y1 > 1) && (x2 < width - 2) && (y2 < height - 2) && //ȥ��������
					(S > 200) && //ȥ����ȱ�� 
					(S < 400) //ȥ��ճ����
					)
				{
					FillContour(pDbgImg, width, height,//�����ͼ��
						x, y, //�������
						true, //�Ƿ���������
						gChainCode, //����
						N, //�������
						255, //�����ڲ�����ɫ
						255, //��������ɫ
						250 //�����ڵ���ɫ
					);
					DrawContour(pDbgImg, width, x, y, gChainCode, N, 254);
				}
				else {
					DrawContour(pDbgImg, width, x, y, gChainCode, N, 253);
				}
			}
			else if ((*pCur == 0) && (*(pCur - 1) >= 254)) {
				//������һ��������
				N = RmwTraceContour(pGryImg, width, height,
					x - 1, y,  //�������,ע����x-1
					false, //��������
					gChainCode, //����������������
					MAX_CONOUR_LENGTH //����Ĵ�С
				);
				//����
				DrawContour(pDbgImg, width, x - 1, y, gChainCode, N, 253);

			}
		}
		pCur++;//�������Ҳ�ĵ�
	}
	// step.3  -------------������----------------//
	Rmw_Write8BitImg2BmpFileMark(pDbgImg, width, height, "./res_data/H0605Bin_trace_fill.bmp");

	//�ټ������ͺ͸�ʴ��Ч��
	//����
	for (int i = 0; i < width * height; i++) {
		pDbgImg[i] = (pGryImg[i] > 200) * 255;//��������ǳ�Ϊ253��254�����ó�255����ʵ������
	}
	Rmw_Write8BitImg2BmpFile(pDbgImg, width, height, "./res_data/H0605Bin_expand.bmp");
	//����
	for (int i = 0; i < width * height; i++) {
		pDbgImg[i] = (pGryImg[i] ==255) * 255;//��������ǳ�Ϊ253��254�����ó�0����ʵ������
	}
	Rmw_Write8BitImg2BmpFile(pDbgImg, width, height, "./res_data/H0605Bin_shrink.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}
/*�����ģ��ж�˳ʱ�뻹����ʱ��*/

void reverseArr(int arr[],int len) {
	//��ת����
	int t;
	for (int i = 0; i < len / 2; i++)
	{
		t = arr[i];
		arr[i] = arr[len - 1 - i];
		arr[len - 1 - i] = t;
	}
}
void Exam4(int chainCodes[],int len) {
	//�������룬����Ƿ�Ϊ˳ʱ
	//������ʼλ��Ϊ(0,0)
	int dx[8] = { 1, 1, 0,-1,-1,-1, 0, 1 };
	int dy[8] = { 0,-1,-1,-1, 0, 1, 1, 1 };
	int sum = 0;
	int x=0, y=0;
	int nextx, nexty;
	printf("�������룺{ ");
	for (int i = 0; i < len; i++) {
		//չʾ����
		printf("%d", chainCodes[i]);
		if (i != len - 1) {
			printf(",");
		}
		nextx = x + dx[chainCodes[i]];
		nexty = y + dy[chainCodes[i]];
		sum += x * nexty - y * nextx;
		x = nextx;
		y = nexty;
	}
	printf(" }");
	if (sum > 0) {
		printf("\n˳ʱ��\n");
	}
	else {
		printf("\n��ʱ��\n");
	}
}

int main() {
	string filename1="./data/H0602Bin.bmp";
	string targetname1 = "./res_data/H0602Bin";
	//Test(0);//(0,139);(0,0);(1,-237);(1,-237)
	//Exam1(filename1,targetname1);//���Բο�Test1д��ͼ�Ĳ���
	Exam1_1(filename1, targetname1);
	//string filename = "./data/H0604Bin.bmp";
	//string targetname = "./res_data/H0604Bin";
	//Exam2(filename,targetname);
	//Exam3();
	//int arr[]{ 5,5,5,5,4,4,4,6,6,6,1,1,1,1,0,0,0,2,2,2 };
	//int len = sizeof(arr) / sizeof(arr[0]);
	//Exam4(arr,len);
	//reverseArr(arr,len);
	//Exam4(arr,len);
	return 0;
}