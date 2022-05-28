///////////////////////////////////////////////////////////////////////////////////////////
//
//����-��1D���ݵ�ͼ��
//
///////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
///////////////////////////////////////////////////////////////////////////////////
//
//��1D���ݵ��У����£���ֱ��
//
///////////////////////////////////////////////////////////////////////////////////
void RmwDrawData2ImgCol( int *pData, int nData, 
	                     double maxScale,
	                     BYTE *pGryImg, int width, int height,
	                     int color
                       )
{   //����ÿ����
	int x1, x2, y1;
	int maxV, i;
	int x, y;

	// step.1-------------Ѱ�����ֵ------------------------//
	maxV = 0;
	for (i = 0; i<nData; i++)
	{
		maxV = max(maxV, pData[i]);
	}
	maxV = (int)(maxV*maxScale);
	// step.2-------------�������--------------------------//
	for (i = 0; i<nData; i++)
	{
		y1 = (height-1)-pData[i]*(height-1)/(maxV+1);
		y1 = max(0, y1);
		x1 = i*width/nData;
		x2 = (i+1)*width/nData;
		for (x = x1; x<x2; x++)
		{
			for (y = height-1; y>y1; y--) *(pGryImg+y*width+x) = color;
		}
	}
	// step.3-------------����------------------------------//
	return;
}
///////////////////////////////////////////////////////////////////////////////////
//
//��1D���ݵ��У����ң�ˮƽ��
//
///////////////////////////////////////////////////////////////////////////////////
void RmwDrawData2ImgRow( int *pData, int nData, 
	                     double maxScale,
	                     BYTE *pGryImg, int width, int height,
	                     int color
                       )
{   //����ÿ����
	int y1, y2, x1;
	int maxV, i;
	int x, y;

	// step.1-------------Ѱ�����ֵ------------------------//
	maxV = 0;
	for (i = 0; i<nData; i++)
	{
		maxV = max(maxV, pData[i]);
	}
	maxV = (int)(maxV*maxScale);
	// step.2-------------�������--------------------------//
	for (i = 0; i<nData; i++)
	{
		x1 = (width-1)-pData[i]*(width-1)/(maxV+1);
		x1 = max(0, x1);
		y1 = i*height/nData;
		y2 = (i+1)*height/nData;
		for (y = y1; y<y2; y++)
		{
			for (x = width-1; x>x1; x--) *(pGryImg+y*width+x) = color;
		}
	}
	// step.3-------------����------------------------------//
	return;
}
