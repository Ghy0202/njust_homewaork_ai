// RmwLocateMark.h: interface for the RmwLocateMark class.
//
//////////////////////////////////////////////////////////////////////
#ifndef RMW_LOCATE_MARK_H
#define RMW_LOCATE_MARK_H

//���ζ���
typedef struct tagRMWRECT
{
	int left;
	int right;
	int top;
	int bottom;
}RMWRECT;

class RmwLocateMark
{
public:
	RmwLocateMark();
	virtual ~RmwLocateMark();
	//��ʼ��
    bool Initialize( int maxWidth, //֧�ֵ����ͼ����
		             int maxHeight, //֧�ֵ����ͼ��߶�
		             bool invertGry, //�Ҷȷ���
		             int mode12, //����ģʽ,1-ʹ�ûҶ�,2-ʹ���ݶ�
		             int nFast0123, //����,0-������,1-����4��,2-����16��(Ŀ��ܴ�ʱ)
		             bool doDebug //�Ƿ����
		           );
	//ִ��
	bool DoNext( BYTE *pGryImg, //��ǰͼ��
		         int width, //��ǰͼ��Ŀ��  
		         int height, //��ǰͼ��ĸ߶�
		         int targetW, //Ŀ����
		         int targetH, //Ŀ��߶�
		         int delLeftW, //ȥ����ߵĿ��,����Ϊ0
		         int delRightW, //ȥ���ұߵĿ��,����Ϊ0
		         int delTopH, //ȥ���ϱߵĵĸ߶�,����Ϊ0
		         int delBottomH, //ȥ���ϱߵĵĸ߶�,����Ϊ0
		         int frameID //֡��
	           );
	//ȡ��ԭʼͼ���е�λ��
	void GetOrgPos(int *x1,int *x2,int *y1,int *y2); 
	//ȡ���ͼ��
	BYTE *GetTargetImg(int extWH,int *w, int *h,RMWRECT *refPos);
private:
	void Dump();
	//����
	void Debug();
private:
	bool m_isInitdOK;
	//����
	int m_maxWidth; //֧�ֵ����ͼ����
	int m_maxHeight; //֧�ֵ����ͼ��߶�
	bool m_invertGry; //�Ҷȷ���
	int m_mode12; //����ģʽ,1-ʹ�ûҶ�,2-ʹ���ݶ�,3-ʹ�ûҶ�+�ݶ�
	int m_nFast0123; //����,0-������,1-����4��,2-����16��(Ŀ��ܴ�ʱ)
	int m_nShrink;
	bool m_doDebug; //�Ƿ����
	//�ڴ�
	BYTE *m_pOrgImg; //��ָ��
	BYTE *m_pGryImg;
	BYTE *m_pGrdImg;
	int *m_pSumImg;
	int m_memSize;
	//��ǰͼ�����
	int m_orgWidth;
	int m_orgHeight;
	int m_width; //��ǰͼ��Ŀ��  
	int m_height; //��ǰͼ��ĸ߶�
	int m_targetW; //Ŀ����
	int m_targetH; //Ŀ��߶�
	int m_delLeftW; //ȥ����ߵĿ��,����Ϊ0
	int m_delRightW; //ȥ���ұߵĿ��,����Ϊ0
	int m_delTopH; //ȥ���ϱߵĵĸ߶�,����Ϊ0
	int m_delBottomH; //ȥ���ϱߵĵĸ߶�,����Ϊ0
	int m_frameID; //֡��
	//���
	int m_x1;
	int m_x2;
	int m_y1;
	int m_y2;
	//����
	char m_filename[255];
};
 
#endif
