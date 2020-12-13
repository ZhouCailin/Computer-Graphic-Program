#include "stdafx.h"
#include "Clip.h"
#include"Line.h"
#include <unordered_map>
#include <unordered_set>

//תΪ�����ƾ���00001,000010...
#define LEFT_EDGE 0x1
#define RIGHT_EDGE 0x2
#define BOTTOM_EDGE 0x4
#define TOP_EDGE 0x8

#define INSIDE(a)(!a) //����Ϊ0000�����win�ڲ���������ȫ�ɼ�
#define REJECT(a,b)(a&b) //һ�����������ͬһλ��1��˵������ͬĳ���⣬�򲻿��ܴ���win��������ȫ���ɼ�
#define ACCEPT(a,b)(!(a|b)) //�����㶼��0000����Ϊ���˵㶼�����ı��⣩��������ȫ�ɼ�

#define FALSE 0
#define TRUE 1

CClip::CClip()
{
	winMax.SetPoint(0, 0);
	winMin.SetPoint(0, 0);
}


CClip::~CClip()
{
}

void CClip::setWin(CPoint sp, CPoint ep){
	int tempx, tempy;
	tempx= sp.x > ep.x ? ep.x : sp.x;
	tempy = sp.y > ep.y ? ep.y : sp.y;
	winMin.SetPoint(tempx, tempy);
	tempx = sp.x < ep.x ? ep.x : sp.x;
	tempy = sp.y < ep.y ? ep.y : sp.y;
	winMax.SetPoint(tempx, tempy);
}

void CClip::swapPts(CPoint* P1, CPoint* P2)
{
	CPoint temp;
	temp = *P1;
	*P1 = *P2;//֮ǰд����
	*P2 = temp;
}

void CClip::swapCodes(unsigned char*c1, unsigned char* c2)
{
	unsigned char temp;
	temp = *c1;
	*c1 = *c2;
	*c2 = temp;
}

unsigned char CClip::encode(CPoint pt)
{
	unsigned char code = 0x00;
	if (pt.x < winMin.x) code = code | LEFT_EDGE;
	if (pt.x > winMax.x)code = code | RIGHT_EDGE;
	if (pt.y > winMax.y)code = code | TOP_EDGE;
	if (pt.y < winMin.y)code = code | BOTTOM_EDGE;
	//Note:������ڣ������ڱ��ϣ�����ж����ڣ�����Clip����һֱdraw����
	return code;
}

ptPair CClip::ClipLine(CDC* pDC,CPoint P1, CPoint P2) 

{
	unsigned char code1, code2;
	int done = FALSE, draw = FALSE;
	float m;
	CLine line;
	CPoint initP1;
	CPoint initP2;
	ptPair outPair;

	initP1 = P1;
	initP2 = P2;
	outPair.Pt1.SetPoint(-1,-1);//���ûdraw����������Ϊ-1

	//���P1==P2ʱ���������iter�ﳣ�����֣�
	if (P1 == P2){
		outPair.Pt1.SetPoint(-2, -2);
		return outPair;
	}

	while (!done)
	{
		//�ü���ĵ�ҲҪ���±���
		code1 = encode(P1);
		code2 = encode(P2);
		if (ACCEPT(code1, code2)) //��or��ȫΪ0�������ڲ������ü����������̬
		{
			done = TRUE;
			draw = TRUE;
		}
		else
		{
			if (REJECT(code1, code2))//����һ�࣬���߲��ɼ�����ʱ�����һ�βŷ��֣������������󣬲�����ŷ���ͬ�ϣ�����������ÿ��һ�߶�Ҫ�ж�һ��REJECT,������ѭ����
			{
				done = TRUE;
			}
			else
			{
				if (INSIDE(code1))//����������ڲ�����ĩ�㻻��㣬���²ü������������ʣ�����߱Ƚ�
				{
					swapPts(&P1, &P2);
					swapCodes(&code1, &code2);
				}
				if (P2.x != P1.x)//�粻Ϊ��ֱ�ߣ���б��
				{
					m = (float)(P2.y - P1.y) / (P2.x - P1.x);
				}
				//��ʼ�ıߵĲ���
				//Note��Ϊʲô4if���ܲ��д��ڣ���Ϊÿ�β������±��룬������ԭ�������жϣ����úõ�����������eg:������ߺ�y>winMin�ˣ�������֮ǰ���룬y��ǿ��=winMin,x�ֳ�������ˣ������ѭ��
				if (code1&LEFT_EDGE)//���������
				{
					//Note��Ϊ�����ﲻ�ж�m?��Ϊ�����ұ���Ĵ�ֱ�����REJECT�ˣ�ʣ�µĲ���������if
					P1.y += (winMin.x - P1.x)*m;
					P1.x = winMin.x;
				}
				else//���������ڣ��������ұ����
				{
					if (code1&RIGHT_EDGE)
					{
						P1.y += (winMax.x - P1.x)*m;
						P1.x = winMax.x;
					}
					else
					{
						if (code1&BOTTOM_EDGE)//������������⣬������
						{
							if (P2.x != P1.x) P1.x += (winMin.y - P1.y) / m; //�������±�ʱ��ע��б�ʲ������
							P1.y = winMin.y;
						}
						else
						{
							if (code1&TOP_EDGE)//�������£�����
							{
								if (P2.x != P1.x) P1.x += (winMax.y - P1.y) / m;
								P1.y = winMax.y;
							}
						}
					}
				}
			}
		}
		if (draw)
		{
			outPair.Pt1 = P1;
			outPair.Pt2 = P2;
		}
	}
	return outPair;
}

void CClip::ExeClip(CDC* pDC, CLine* onLine)
{
	int x1,x2,y1,y2;
	CPoint pts1, pts2;
	ptPair ptsPair;
	std::tuple<USHORT, USHORT, USHORT, USHORT>t1;

	//����map
	std::map<std::tuple<USHORT, USHORT, USHORT, USHORT>, std::queue<COLORREF>> ::iterator myit;
	std::map<std::tuple<USHORT, USHORT, USHORT, USHORT>, std::queue<COLORREF>> now_state;
	now_state = onLine->prev_state;
	for (myit = now_state.begin(); myit != now_state.end(); ++myit)
	{
		//tuppleȡֵ
		t1 = myit->first;
		x1 = std::get<0>(t1);
		x2 = std::get<1>(t1);
		y1 = std::get<2>(t1);
		y2 = std::get<3>(t1);

		pts1.SetPoint(x1, y1);
		pts2.SetPoint(x2, y2);
		ptsPair=ClipLine(pDC,pts1, pts2);

		//���ݱ�ǣ�ȥ����ȫ������
		if (ptsPair.Pt1.x == -1)
		{
			onLine->MoveTo(pDC, pts1);
			onLine->LineToDDA(pDC, pts2);
			continue;
		}

		//���ݱ�ǣ��������������
		if (ptsPair.Pt1.x >=0)
		{
			onLine->MoveTo(pDC, pts1);
			onLine->LineToDDA(pDC, pts2);//��Ϊ������DDA���������ߣ����Զ�Bresham�Ľ���߲����ɾ���������Ϊ�㷨��ͬ��㲻һ��
			onLine->MoveTo(pDC, ptsPair.Pt1);
			onLine->LineToDDA(pDC, ptsPair.Pt2);
		}
	}
}