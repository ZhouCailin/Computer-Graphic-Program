#include "stdafx.h"
#include "Clip.h"
#include"Line.h"
#include <unordered_map>
#include <unordered_set>

//转为二进制就是00001,000010...
#define LEFT_EDGE 0x1
#define RIGHT_EDGE 0x2
#define BOTTOM_EDGE 0x4
#define TOP_EDGE 0x8

#define INSIDE(a)(!a) //编码为0000则点在win内部——点完全可见
#define REJECT(a,b)(a&b) //一旦两点编码有同一位都1，说明两点同某边外，则不可能穿过win——线完全不可见
#define ACCEPT(a,b)(!(a|b)) //两个点都是0000（因为两端点都不在四边外）——线完全可见

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
	*P1 = *P2;//之前写反！
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
	//Note:如果等于，即处在边上，则可判断在内，否则Clip函数一直draw不了
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
	outPair.Pt1.SetPoint(-1,-1);//如果没draw，则持续标记为-1

	//标记P1==P2时的奇怪现象（iter里常有这种）
	if (P1 == P2){
		outPair.Pt1.SetPoint(-2, -2);
		return outPair;
	}

	while (!done)
	{
		//裁剪后的点也要重新编码
		code1 = encode(P1);
		code2 = encode(P2);
		if (ACCEPT(code1, code2)) //如or后全为0，线在内部——裁剪最终输出形态
		{
			done = TRUE;
			draw = TRUE;
		}
		else
		{
			if (REJECT(code1, code2))//两点一侧，则线不可见（有时候裁完一次才发现（比如中上与左，裁完左才发现同上），所以下面每裁一边都要判断一次REJECT,否则死循环）
			{
				done = TRUE;
			}
			else
			{
				if (INSIDE(code1))//如起点已在内部，则末点换起点，重新裁剪，否则继续跟剩下三边比较
				{
					swapPts(&P1, &P2);
					swapCodes(&code1, &code2);
				}
				if (P2.x != P1.x)//如不为垂直线，求斜率
				{
					m = (float)(P2.y - P1.y) / (P2.x - P1.x);
				}
				//开始四边的裁切
				//Note：为什么4if不能并列存在？因为每次裁完重新编码，否则用原码来做判断，将裁好的线又拉长，eg:裁了左边后y>winMin了，但是用之前的码，y又强行=winMin,x又超出左边了，造成死循环
				if (code1&LEFT_EDGE)//如在左边外
				{
					//Note：为何这里不判断m?因为在左右边外的垂直线早就REJECT了，剩下的不会进到这个if
					P1.y += (winMin.x - P1.x)*m;
					P1.x = winMin.x;
				}
				else//如果在左边内，讨论与右边情况
				{
					if (code1&RIGHT_EDGE)
					{
						P1.y += (winMax.x - P1.x)*m;
						P1.x = winMax.x;
					}
					else
					{
						if (code1&BOTTOM_EDGE)//如果不在左右外，讨论下
						{
							if (P2.x != P1.x) P1.x += (winMin.y - P1.y) / m; //讨论上下边时，注意斜率不存情况
							P1.y = winMin.y;
						}
						else
						{
							if (code1&TOP_EDGE)//非左右下，则上
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

	//遍历map
	std::map<std::tuple<USHORT, USHORT, USHORT, USHORT>, std::queue<COLORREF>> ::iterator myit;
	std::map<std::tuple<USHORT, USHORT, USHORT, USHORT>, std::queue<COLORREF>> now_state;
	now_state = onLine->prev_state;
	for (myit = now_state.begin(); myit != now_state.end(); ++myit)
	{
		//tupple取值
		t1 = myit->first;
		x1 = std::get<0>(t1);
		x2 = std::get<1>(t1);
		y1 = std::get<2>(t1);
		y2 = std::get<3>(t1);

		pts1.SetPoint(x1, y1);
		pts2.SetPoint(x2, y2);
		ptsPair=ClipLine(pDC,pts1, pts2);

		//根据标记，去掉完全框外线
		if (ptsPair.Pt1.x == -1)
		{
			onLine->MoveTo(pDC, pts1);
			onLine->LineToDDA(pDC, pts2);
			continue;
		}

		//根据标记，仅处理正常情况
		if (ptsPair.Pt1.x >=0)
		{
			onLine->MoveTo(pDC, pts1);
			onLine->LineToDDA(pDC, pts2);//因为这里用DDA来擦除旧线，所以对Bresham的结果线擦不干净。。。因为算法不同像点不一样
			onLine->MoveTo(pDC, ptsPair.Pt1);
			onLine->LineToDDA(pDC, ptsPair.Pt2);
		}
	}
}