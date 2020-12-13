#include "stdafx.h"
#include "Fill.h"
#include <stack>


CFill::CFill()
{
}


CFill::~CFill()
{
}

void CFill::buildPoly(CDC* pDC)
{
	pair<USHORT, USHORT>tempP;
	CPoint pts;
	int i, j;
	COLORREF color = RGB(0, 255, 255);
	//������״
	i = 100;
	for (j = 100; j < 301; j++){
		pts.SetPoint(i, j);
		pDC->SetPixel(pts, color);
		tempP = make_pair(i, j);
		boundary.insert(tempP);
	}
	j = 100;
	for (i = 101; i < 300; i++){
		pts.SetPoint(i, j);
		pDC->SetPixel(pts, color);
		tempP = make_pair(i, j);
		boundary.insert(tempP);
	}
	j = 300;
	for (i = 101; i < 300; i++){
		pts.SetPoint(i, j);
		pDC->SetPixel(pts, color);
		tempP = make_pair(i, j);
		boundary.insert(tempP);
	}
	//��
	for (i = 200; i < 251; i++){
		j=i;
		pts.SetPoint(i, j);
		pDC->SetPixel(pts, color);
		tempP = make_pair(i, j);
		boundary.insert(tempP);
	}
	i = 200;
	for (j = 201; j < 250; j++){
		pts.SetPoint(i, j);
		pDC->SetPixel(pts, color);
		tempP = make_pair(i, j);
		boundary.insert(tempP);
	}
	j = 250;
	for (i = 201; i < 250;i++){
		pts.SetPoint(i, j);
		pDC->SetPixel(pts, color);
		tempP = make_pair(i, j);
		boundary.insert(tempP);
	}
	//ʵ�Ķ�
	for (i = 130; i < 151; i++)
	{
		for (j = 120; j < 151; j++)
		{
			pts.SetPoint(i, j);
			pDC->SetPixel(pts, color);
			tempP = make_pair(i, j);
			boundary.insert(tempP);
		}
	}
	//�ұ��谼��
	i = 300;
	for (j = 100; j < 151; j++){
		pts.SetPoint(i, j);
		pDC->SetPixel(pts, color);
		tempP = make_pair(i, j);
		boundary.insert(tempP);
	}
	for (j = 250; j < 301; j++){
		pts.SetPoint(i, j);
		pDC->SetPixel(pts, color);
		tempP = make_pair(i, j);
		boundary.insert(tempP);
	}
	i = 260;
	for (j = 150; j < 251; j++){
		pts.SetPoint(i, j);
		pDC->SetPixel(pts, color);
		tempP = make_pair(i, j);
		boundary.insert(tempP);
	}
	j = 150;
	for (i = 261; i< 300; i++){
		pts.SetPoint(i, j);
		pDC->SetPixel(pts, color);
		tempP = make_pair(i, j);
		boundary.insert(tempP);
	}
	j = 250;
	for (i = 261; i< 300; i++){
		pts.SetPoint(i, j);
		pDC->SetPixel(pts, color);
		tempP = make_pair(i, j);
		boundary.insert(tempP);
	}

}

//downupwardӦ��Ҫ���ϱ߿�ʼ����֮ǰд���˴����ӿ�ʼ

void CFill::scanSeedFillUpward(CDC* pDC, int seedX, int seedY)
{
	int x,y,saveX,Xright,Xleft,pFlag,Xenter;
	CPoint pixel;
	CPoint seedP;
	pair<USHORT, USHORT>tempPair;
	std::stack<CPoint>ptsStk;
	COLORREF color = RGB(255, 0, 255);

	seedP.SetPoint(seedX, seedY);
	ptsStk.push(seedP);
	while (!ptsStk.empty())
	{
		//����1���������ӵ�ɨ���С����xright,xleft
		pixel = ptsStk.top();
		ptsStk.pop();
		pDC->SetPixel(pixel, color);
		saveX = pixel.x;
		x = pixel.x;
		x = x + 1;
		y = pixel.y;
		tempPair = make_pair(x, y);

		//�����ҽ硪��XrightΪ�߽���һ��
		while (!boundary.count(tempPair))
		{
			pixel.SetPoint(x, y);
			pDC->SetPixel(pixel, color);
			x = x + 1;
			tempPair = make_pair(x, y);
		}
		Xright = x - 1;

		//������硪��XleftΪ�߽���һ��
		x = saveX;
		x = x - 1;
		tempPair = make_pair(x, y);
		while (!boundary.count(tempPair))
		{
			pixel.SetPoint(x, y);
			pDC->SetPixel(pixel, color);
			x = x - 1;
			tempPair = make_pair(x, y);
		}
		Xleft = x + 1;
		x = saveX;

		//����2����Ѱ��һ�е����ӵ㣬��xleft��xright
		x = Xleft;
		y = y - 1;
		while (x <= Xright)
		{
			pFlag = 0;
			tempPair = make_pair(x, y);
			/* Note��
			��������һ�����ӵ㣺
				����һ�е�xleft��xright,�ҵ���Ϊ�߽���δ�����С������һ�е����ҵĵ�������ҵĵ�
				Ҫô�ǣ�1 �ڿն��߽����ߵ�һ������Ǳ߽���Ԫ��
				Ҫô�ǣ�2 xright
			*/
			//���кܶ����ӵ㣬��������Ŀǰ�����ң��׶�������
			while (!boundary.count(tempPair) && pDC->GetPixel(x,y)!=color && x<=Xright) //=?
			{
				//pFlag��ǣ�С������ȷ����Ǳ������
				if (pFlag == 0)pFlag = 1;
				x = x + 1;
				tempPair = make_pair(x, y);
			}
			//С�������Ǳ�����ӵ�
			if (pFlag == 1)
			{
				tempPair = make_pair(x, y);
				//������Ǳ�����ӵ���xright�ϣ�push
				if (x == Xright && !boundary.count(tempPair) && pDC->GetPixel(x, y) != color)
				{
					seedP.SetPoint(x, y);
					ptsStk.push(seedP);
				}
				else
				//�������ڵ�x���ڿ׶�����߽��ϣ�Ҫpush��һ��
				{
					seedP.SetPoint(x - 1, y);
					ptsStk.push(seedP);
				}
			}
			//���ȥ���������ӵ��ұߵ��Ǹ����ڱ߽�ĵ�һ�����ҵ��ڱ߽���ұߵ�һ���Ǳ߽��
			//����������п��ܣ���Ӧ���ǡ�\���ͺ����пն��ġ�
			//��Ϊ��һ����������Ǳ�����ӵ�ʱ����ջ�ľ���δ���Ǳ߽������һ��
			Xenter = x;
			tempPair = make_pair(x, y);
			while (x < Xright && !boundary.count(tempPair) && pDC->GetPixel(x, y) != color)
			{
				x = x + 1;
				tempPair = make_pair(x, y);
			}
			//���粻���ұ߽����׶��߽磬�Ǿ�ֻ��������ջ��xright���Ǿ�+1������whileѭ������һ��
			if (x == Xenter)x = x + 1;
			tempPair = make_pair(x, y);
		}
	}
}

void CFill::scanSeedFillDownward(CDC* pDC, int seedX, int seedY)
{
	int x, y, saveX, Xright, Xleft, pFlag, Xenter;
	CPoint pixel;
	CPoint seedP;
	pair<USHORT, USHORT>tempPair;
	std::stack<CPoint>ptsStk;
	COLORREF color = RGB(255, 0, 255);

	seedP.SetPoint(seedX, seedY);
	ptsStk.push(seedP);
	while (!ptsStk.empty())
	{
		pixel = ptsStk.top();
		ptsStk.pop();
		pDC->SetPixel(pixel, color);
		saveX = pixel.x;
		x = pixel.x;
		x = x + 1;
		y = pixel.y;
		tempPair = make_pair(x, y);
		while (!boundary.count(tempPair))
		{
			pixel.SetPoint(x, y);
			pDC->SetPixel(pixel, color);
			x = x + 1;
			tempPair = make_pair(x, y);
		}
		Xright = x - 1;
		x = saveX;
		x = x - 1;
		tempPair = make_pair(x, y);
		while (!boundary.count(tempPair))
		{
			pixel.SetPoint(x, y);
			pDC->SetPixel(pixel, color);
			x = x - 1;
			tempPair = make_pair(x, y);
		}
		Xleft = x + 1;
		x = saveX;
		x = Xleft;
		y = y + 1;
		while (x <= Xright)
		{
			pFlag = 0;
			tempPair = make_pair(x, y);
			while (!boundary.count(tempPair) && pDC->GetPixel(x, y) != color && x <= Xright) //=?
			{
				if (pFlag == 0)pFlag = 1;
				x = x + 1;
				tempPair = make_pair(x, y);
			}
			if (pFlag == 1)
			{
				tempPair = make_pair(x, y);
				if (x == Xright && !boundary.count(tempPair) && pDC->GetPixel(x, y) != color)
				{
					seedP.SetPoint(x, y);
					ptsStk.push(seedP);
				}
				else
				{
					seedP.SetPoint(x - 1, y);
					ptsStk.push(seedP);
				}
			}
			Xenter = x;
			tempPair = make_pair(x, y);
			while (x < Xright && !boundary.count(tempPair) && pDC->GetPixel(x, y) != color)
			{
				x = x + 1;
				tempPair = make_pair(x, y);
			}

			if (x == Xenter)x = x + 1;
			tempPair = make_pair(x, y);
		}
	}
}