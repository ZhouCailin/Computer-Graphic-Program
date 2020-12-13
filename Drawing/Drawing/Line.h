#pragma once
#include "atltypes.h"
#include <map>
#include <queue>
#include "Clip.h"

class CClip;

class CLine
{
	friend class CClip; //clip裁剪需要调用private:map
public:
	CLine();
	virtual ~CLine();
private:
	CPoint P0;
	CPoint P1;
	//Note：元组+队列的map，记录起末坐标+线上所有像元<画线之前>的RGB
	std::map<std::tuple<USHORT, USHORT, USHORT, USHORT>, std::queue<COLORREF>> prev_state;

public:
	void MoveTo(CDC* pDC, CPoint sp);
	void LineToDDA(CDC* pDC, CPoint ep);
	void LineToBre(CDC* pDC, CPoint ep);
};
