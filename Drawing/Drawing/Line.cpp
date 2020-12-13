#include "stdafx.h"
#include "Line.h"


CLine::CLine()
	: P0(0)
	, P1(0)
{
}

CLine::~CLine()
{
}

void CLine::MoveTo(CDC* pDC, CPoint sp)
{
	P0.SetPoint(sp.x, sp.y);
}

void CLine::LineToDDA(CDC* pDC, CPoint ep)
{
	/*
	    Note：
	    对于橡皮筋：如果擦除上一线时直接用异或色LineTo，会直接变白破坏以前画的线，所以需要在每次画线时记录每个点的RGB值，然后在擦除时恢复，解决方法：queue<COLORREF>
		
		思路
		1. 每次LineTo都先判断SP(x1,y1)和EP(x2,y2)是否在prev_state里，是则为擦除模式，否则为画线模式
		2. 两种模式
		2.1 画线模式：先将原有像素队列存入prev_state中的对应线条(x1,y1,x2,y2)，然后画点
		2.2 擦除模式：先从prev_state取出对应线条(x1,y1,x2,y2)的像素队列，然后从队列取出各像素RGB画点
	*/
	int x1 = P0.x, y1 = P0.y;
	int x2 = ep.x, y2 = ep.y;
	int dx = x2 - x1, dy = y2 - y1;
	int steps, k;
	float xIncrement, yIncrement, x, y;
	if (P0 == ep)exit;

	// 元组记录线条(x1,y1,x2,y2)
	auto tp = std::make_tuple(x1, x2, y1, y2);

	// 原有像素队列，NULL为画线模式，否则为擦除模式
	std::queue<COLORREF> * dots = NULL;

	if (prev_state.find(tp) != prev_state.end()) {
		// 在prev_state中找到先前存放的线条，取出像素队列存到dots里
		dots = &prev_state[tp];
		if (dots->empty()) {
			// 擦除模式下，原有像素队列为空则删除并直接返回
			prev_state.erase(tp);
			return;
		}
	}

	COLORREF color = RGB(0, 255, 0);

	x = x1;
	y = y1;
	if (abs(dx) > abs(dy))steps = abs(dx);
	else steps = abs(dy);//步长恒正

	xIncrement = dx / float(steps); //有一个为1 
	yIncrement = dy / float(steps);

	P1.SetPoint(round(x), round(y));
	pDC->SetPixel(P1,color);

	// 新的像素队列
	std::queue<COLORREF> new_dots;

	for (k = 0; k< steps; k++)
	{
		x += xIncrement;
		y += yIncrement;

		P1.SetPoint(round(x), round(y));

		if (dots) {
			// 擦除模式
			// 从原有像素队列取出RGB
			color = dots->front();
			dots->pop();
		}
		else {
			// 画线模式
			// 将RGB存入新的像素队列
			new_dots.push(pDC->GetPixel(P1));
		}
		pDC->SetPixel(P1, color);
	}
	if (dots) {
		// 擦除模式结束，从prev_state删掉对应线条(x1,y1,x2,y2)的像素队列。
		prev_state.erase(tp);
	}
	else {
		// 画线模式结束，将新的像素队列存入prev_state中的对应线条(x1,y1,x2,y2)
		prev_state[tp] = new_dots;
	}
}

void CLine::LineToBre(CDC* pDC, CPoint ep)
{
	double m, e;
	int dx, dy, x, y,x2,y2,i,y0,x0,index,temp,opx,opy;
	int kIndex = 0;
	int ySymIndex = 0;
	int xSymIndex = 0;
	
	x = P0.x; y = P0.y;
	x2 = ep.x; y2 = ep.y;
	dx = x2 - x; dy = y2 - y;
	x0 = x; y0=y;

	// 位操作记录线条(x1,y1,x2,y2)
	auto tp = std::make_tuple(x, x2, y, y2);

	// 原有像素队列，NULL为画线模式，否则为擦除模式
	std::queue<COLORREF> * dots = NULL;

	if (prev_state.find(tp) != prev_state.end()) {
		// 在prev_state中找到先前存放的线条，取出像素队列存到dots里
		dots = &prev_state[tp];
		if (dots->empty()) {
			// 擦除模式下，原有像素队列为空则删除并直接返回
			prev_state.erase(tp);
			return;
		}
	}

	std::queue<COLORREF> new_dots;
	COLORREF color = RGB(0, 0, 255);
	
	//拓展到四个方向：起点不变，统一做右上方向，再相应对称
	//右下方向：水平对称
	if (dy < 0){
		ySymIndex = 1; 
	}
	//左上方向：垂直对称
	//左下方向：垂直+水平
	if (dx < 0){
		xSymIndex = 1;
	}

	dy = abs(dy);
	dx = abs(dx);

	//当斜率>1时，XY调换处理，最后关于y=x对称
	if (dy>dx){
		m = dx / double(dy);
		index = dy; //步长要改
		temp=x;
		x = y;
		y = temp;
		kIndex = 1;
	}
	else{
		m = dy / double(dx);
		index = dx;
	}
	e = m - 0.5;

	//核心
	for (i = 0; i < index; i++){
		if (e >= 0){
			y++;
			e = e + m - 1;
		}
		else{
			e = e + m;
		}

		//如前面进行了XY调换，反调换
		if (kIndex == 1){
			opx = y;
			opy = x;
		}
		else{
			opx = x;
			opy = y;
		}
		x++;

		//如非右上，对称变换
		if (ySymIndex) opy = 2 * y0 - opy;
		if (xSymIndex) opx = 2 * x0 - opx;

		//输出
		P1.SetPoint(opx, opy); 
		if (dots) {
			// 擦除模式
			// 从原有像素队列取出原RGB
			color = dots->front();
			dots->pop();
		}
		else {
			// 画线模式
			// 将RGB存入新的像素队列
			new_dots.push(pDC->GetPixel(P1));
		}
		pDC->SetPixel(P1, color);
	}
	if (dots) {
		// 擦除模式结束，从prev_state删掉对应线条(x1,y1,x2,y2)的像素队列。
		prev_state.erase(tp);
	}
	else {
		// 画线模式结束，将新的像素队列存入prev_state中的对应线条(x1,y1,x2,y2)
		prev_state[tp] = new_dots;
	}
}
