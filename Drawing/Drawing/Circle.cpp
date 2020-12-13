#include "stdafx.h"
#include "Circle.h"
#include <math.h>
#include "Line.h"

#define PI 3.1415926 

Circle::Circle()
{
	CPoint p(0, 0);
	centre = p;
	radius = 0;
}


Circle::~Circle()
{
}

void Circle::SetCircle(CPoint sp,CPoint ep)
{
	centre = sp;
	radius = int(pow(pow(double(sp.x - ep.x), 2) + pow(double(sp.y - ep.y), 2),0.5));
}

void Circle::DrawCircle(CDC* pDC)
{
	COLORREF color = RGB(255, 0, 0);

	int e,x,y,x0,y0,opx,opy,realx,realy;
	int index = 0;
	x0 = centre.x;
	y0 = centre.y;
	x = 0; y = radius; e = 1 - radius;

	//只画1/8圆，然后对称处理
	while (x <= y)
	{
		if (e<0)
			e += 2 * x + 3;
		else
		{
			e += 2 * (x - y) + 5;
			y--;
		}
		x++;
		//扩展到四个象限
		while (!index){
			//元点平移到圆心
			realx = x + x0;
			realy = y + y0;
			//计算XY的对称
			opx = 2 * x0 - realx;
			opy = 2 * y0 - realy;
			pDC->SetPixel(realx, realy, color);
			if (opy > 0){
				pDC->SetPixel(realx, opy, color);
			}
			if (opx > 0){
				pDC->SetPixel(opx, realy, color);
			}
			if (opx > 0 && opy > 0){
				pDC->SetPixel(opx, opy, color);
			}
			index = 1;
		}
		//沿y=x对称
			while (index){
				realx = y + x0; //注意是加X0！因为已经Y->X
				realy = x + y0;
				opx = 2 * x0 - realx;
				opy = 2 * y0 - realy;
				pDC->SetPixel(realx, realy, color);
				if (opy > 0){
					pDC->SetPixel(realx, opy, color);
				}
				if (opx > 0){
					pDC->SetPixel(opx, realy, color);
				}
				if (opx > 0 && opy > 0){
					pDC->SetPixel(opx, opy, color);
				}
				index = 0;
		}
	}
}

void Circle::DrawInsPoly(CDC* pDC, double d)
{
	CLine L;
	CPoint P1,P2;
	int n,x,y,x2,y2,realx,realy,realx2,realy2;
	double alfa,xita;
	alfa = 2 * acos((radius - d) / radius);
	n = int(PI* 2 / alfa);
	xita = 0;

	x = radius; y = 0;

	while (xita<2*PI)
	{
		//参数方程
		x2 = round(radius*cos(xita));
		y2 = round(radius*sin(xita));
		
		//坐标系平移
		realx = x + centre.x;
		realy = y + centre.y;
		realx2 = x2 + centre.x;
		realy2 = y2 + centre.y;

		//画边
		P1.SetPoint(realx, realy);
		P2.SetPoint(realx2, realy2);
		L.MoveTo(pDC, P1);
		L.LineToDDA(pDC, P2);

		//循环
		x = x2;
		y = y2;
		xita = xita + alfa;
	}
	//连接最后一边
	P1.SetPoint(radius+centre.x, centre.y);
	L.MoveTo(pDC,P1);
	L.LineToDDA(pDC, P2);

}