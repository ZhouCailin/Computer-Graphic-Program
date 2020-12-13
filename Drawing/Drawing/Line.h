#pragma once
#include "atltypes.h"
#include <map>
#include <queue>
#include "Clip.h"

class CClip;

class CLine
{
	friend class CClip; //clip�ü���Ҫ����private:map
public:
	CLine();
	virtual ~CLine();
private:
	CPoint P0;
	CPoint P1;
	//Note��Ԫ��+���е�map����¼��ĩ����+����������Ԫ<����֮ǰ>��RGB
	std::map<std::tuple<USHORT, USHORT, USHORT, USHORT>, std::queue<COLORREF>> prev_state;

public:
	void MoveTo(CDC* pDC, CPoint sp);
	void LineToDDA(CDC* pDC, CPoint ep);
	void LineToBre(CDC* pDC, CPoint ep);
};
