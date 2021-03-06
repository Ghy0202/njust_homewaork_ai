///////////////////////////////////////////////////////////////////////////////////////////
//
//第五章-二维直方图
//
///////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RMW_ALG_HISTOGRAM_H
#define _RMW_ALG_HISTOGRAM_H

///////////////////////////////////////////////////////////////////////////////////////////
//
//二值化
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwThreshold(BYTE *pGryImg, int width, int height, int thre);
///////////////////////////////////////////////////////////////////////////////////////////
//
//Otsu阈值
//
///////////////////////////////////////////////////////////////////////////////////////////
int RmwGetOtsuThreshold(int *histogram, int nSize);
int my_RmwGetOtsuThreshold(int* histogram, int nSize,int gmin,int gmax);
///////////////////////////////////////////////////////////////////////////////////////////
//
//画直方图
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwDrawHistogram2Img( int *histogram, int histSize, 
	                       double maxScale,
	                       BYTE *pGryImg, int width, int height
                         );
///////////////////////////////////////////////////////////////////////////////////////////
//
//二维直方图
//
///////////////////////////////////////////////////////////////////////////////////////////
void Rmw2DHistogram(BYTE *pGryImg, BYTE *pAvrImg, int width, int height, int *histogram2D);
int RmwHistogramBy2D( BYTE *pGryImg, BYTE *pAvrImg, 
	                  int width, int height, 
	                  int dist, //与均值的偏差
	                  int *histogram
                    );
 
#endif

