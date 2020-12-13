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
	    Note��
	    ������Ƥ����������һ��ʱֱ�������ɫLineTo����ֱ�ӱ���ƻ���ǰ�����ߣ�������Ҫ��ÿ�λ���ʱ��¼ÿ�����RGBֵ��Ȼ���ڲ���ʱ�ָ������������queue<COLORREF>
		
		˼·
		1. ÿ��LineTo�����ж�SP(x1,y1)��EP(x2,y2)�Ƿ���prev_state�����Ϊ����ģʽ������Ϊ����ģʽ
		2. ����ģʽ
		2.1 ����ģʽ���Ƚ�ԭ�����ض��д���prev_state�еĶ�Ӧ����(x1,y1,x2,y2)��Ȼ�󻭵�
		2.2 ����ģʽ���ȴ�prev_stateȡ����Ӧ����(x1,y1,x2,y2)�����ض��У�Ȼ��Ӷ���ȡ��������RGB����
	*/
	int x1 = P0.x, y1 = P0.y;
	int x2 = ep.x, y2 = ep.y;
	int dx = x2 - x1, dy = y2 - y1;
	int steps, k;
	float xIncrement, yIncrement, x, y;
	if (P0 == ep)exit;

	// Ԫ���¼����(x1,y1,x2,y2)
	auto tp = std::make_tuple(x1, x2, y1, y2);

	// ԭ�����ض��У�NULLΪ����ģʽ������Ϊ����ģʽ
	std::queue<COLORREF> * dots = NULL;

	if (prev_state.find(tp) != prev_state.end()) {
		// ��prev_state���ҵ���ǰ��ŵ�������ȡ�����ض��д浽dots��
		dots = &prev_state[tp];
		if (dots->empty()) {
			// ����ģʽ�£�ԭ�����ض���Ϊ����ɾ����ֱ�ӷ���
			prev_state.erase(tp);
			return;
		}
	}

	COLORREF color = RGB(0, 255, 0);

	x = x1;
	y = y1;
	if (abs(dx) > abs(dy))steps = abs(dx);
	else steps = abs(dy);//��������

	xIncrement = dx / float(steps); //��һ��Ϊ1 
	yIncrement = dy / float(steps);

	P1.SetPoint(round(x), round(y));
	pDC->SetPixel(P1,color);

	// �µ����ض���
	std::queue<COLORREF> new_dots;

	for (k = 0; k< steps; k++)
	{
		x += xIncrement;
		y += yIncrement;

		P1.SetPoint(round(x), round(y));

		if (dots) {
			// ����ģʽ
			// ��ԭ�����ض���ȡ��RGB
			color = dots->front();
			dots->pop();
		}
		else {
			// ����ģʽ
			// ��RGB�����µ����ض���
			new_dots.push(pDC->GetPixel(P1));
		}
		pDC->SetPixel(P1, color);
	}
	if (dots) {
		// ����ģʽ��������prev_stateɾ����Ӧ����(x1,y1,x2,y2)�����ض��С�
		prev_state.erase(tp);
	}
	else {
		// ����ģʽ���������µ����ض��д���prev_state�еĶ�Ӧ����(x1,y1,x2,y2)
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

	// λ������¼����(x1,y1,x2,y2)
	auto tp = std::make_tuple(x, x2, y, y2);

	// ԭ�����ض��У�NULLΪ����ģʽ������Ϊ����ģʽ
	std::queue<COLORREF> * dots = NULL;

	if (prev_state.find(tp) != prev_state.end()) {
		// ��prev_state���ҵ���ǰ��ŵ�������ȡ�����ض��д浽dots��
		dots = &prev_state[tp];
		if (dots->empty()) {
			// ����ģʽ�£�ԭ�����ض���Ϊ����ɾ����ֱ�ӷ���
			prev_state.erase(tp);
			return;
		}
	}

	std::queue<COLORREF> new_dots;
	COLORREF color = RGB(0, 0, 255);
	
	//��չ���ĸ�������㲻�䣬ͳһ�����Ϸ�������Ӧ�Գ�
	//���·���ˮƽ�Գ�
	if (dy < 0){
		ySymIndex = 1; 
	}
	//���Ϸ��򣺴�ֱ�Գ�
	//���·��򣺴�ֱ+ˮƽ
	if (dx < 0){
		xSymIndex = 1;
	}

	dy = abs(dy);
	dx = abs(dx);

	//��б��>1ʱ��XY��������������y=x�Գ�
	if (dy>dx){
		m = dx / double(dy);
		index = dy; //����Ҫ��
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

	//����
	for (i = 0; i < index; i++){
		if (e >= 0){
			y++;
			e = e + m - 1;
		}
		else{
			e = e + m;
		}

		//��ǰ�������XY������������
		if (kIndex == 1){
			opx = y;
			opy = x;
		}
		else{
			opx = x;
			opy = y;
		}
		x++;

		//������ϣ��ԳƱ任
		if (ySymIndex) opy = 2 * y0 - opy;
		if (xSymIndex) opx = 2 * x0 - opx;

		//���
		P1.SetPoint(opx, opy); 
		if (dots) {
			// ����ģʽ
			// ��ԭ�����ض���ȡ��ԭRGB
			color = dots->front();
			dots->pop();
		}
		else {
			// ����ģʽ
			// ��RGB�����µ����ض���
			new_dots.push(pDC->GetPixel(P1));
		}
		pDC->SetPixel(P1, color);
	}
	if (dots) {
		// ����ģʽ��������prev_stateɾ����Ӧ����(x1,y1,x2,y2)�����ض��С�
		prev_state.erase(tp);
	}
	else {
		// ����ģʽ���������µ����ض��д���prev_state�еĶ�Ӧ����(x1,y1,x2,y2)
		prev_state[tp] = new_dots;
	}
}
