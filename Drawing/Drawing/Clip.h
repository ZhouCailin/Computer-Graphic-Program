#pragma once
#include "Line.h"

class CLine;

struct ptPair{
	CPoint Pt1;
	CPoint Pt2;
};

class CClip
{
public:
	CClip();
	virtual ~CClip();

private:
	CPoint winMin;
	CPoint winMax;
public:
	void setWin(CPoint sp, CPoint ep);
	void swapPts(CPoint* P1, CPoint* P2);
	void swapCodes(unsigned char*c1, unsigned char* c2);
	unsigned char encode(CPoint p);
	ptPair ClipLine(CDC* pDC,CPoint P1, CPoint P2);
	void ExeClip(CDC* pDC,CLine* onLine);
};

