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
	//基本形状
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
	//洞
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
	//实心洞
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
	//右边设凹槽
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

//downupward应该要从上边开始。。之前写成了从种子开始

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
		//环节1：基于种子点扫描行。获得xright,xleft
		pixel = ptsStk.top();
		ptsStk.pop();
		pDC->SetPixel(pixel, color);
		saveX = pixel.x;
		x = pixel.x;
		x = x + 1;
		y = pixel.y;
		tempPair = make_pair(x, y);

		//填至右界——Xright为边界左一个
		while (!boundary.count(tempPair))
		{
			pixel.SetPoint(x, y);
			pDC->SetPixel(pixel, color);
			x = x + 1;
			tempPair = make_pair(x, y);
		}
		Xright = x - 1;

		//填至左界——Xleft为边界右一个
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

		//环节2：找寻下一行的种子点，从xleft到xright
		x = Xleft;
		y = y - 1;
		while (x <= Xright)
		{
			pFlag = 0;
			tempPair = make_pair(x, y);
			/* Note：
			现在找下一行种子点：
				从上一行的xleft到xright,找到不为边界且未填充且小等于上一行的最右的点里的最右的点
				要么是：1 内空洞边界的左边第一个非填非边界像元；
				要么是：2 xright
			*/
			//会有很多种子点，但必须是目前的最右（孔洞的最左）
			while (!boundary.count(tempPair) && pDC->GetPixel(x,y)!=color && x<=Xright) //=?
			{
				//pFlag标记，小区间内确是有潜在种子
				if (pFlag == 0)pFlag = 1;
				x = x + 1;
				tempPair = make_pair(x, y);
			}
			//小区间存在潜在种子点
			if (pFlag == 1)
			{
				tempPair = make_pair(x, y);
				//如果这个潜在种子点在xright上，push
				if (x == Xright && !boundary.count(tempPair) && pDC->GetPixel(x, y) != color)
				{
					seedP.SetPoint(x, y);
					ptsStk.push(seedP);
				}
				else
				//否则现在的x就在孔洞的左边界上，要push左一个
				{
					seedP.SetPoint(x - 1, y);
					ptsStk.push(seedP);
				}
			}
			//跨过去：假如种子点右边的那个是内边界的第一个，找到内边界的右边第一个非边界的
			//这种情况很有可能：对应的是”\“型和内有空洞的。
			//因为上一步我们在找潜在种子点时，入栈的就是未填充非边界的最右一个
			Xenter = x;
			tempPair = make_pair(x, y);
			while (x < Xright && !boundary.count(tempPair) && pDC->GetPixel(x, y) != color)
			{
				x = x + 1;
				tempPair = make_pair(x, y);
			}
			//假如不是右边紧挨孔洞边界，那就只可能是入栈了xright，那就+1来跳出while循环，下一行
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