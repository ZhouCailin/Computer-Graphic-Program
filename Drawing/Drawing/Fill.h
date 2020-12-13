#pragma once
#include <set>
#include <iostream>
using namespace std;


class CFill
{
public:
	CFill();
	virtual ~CFill();
	void scanSeedFillUpward(CDC* pDC,int seedX, int seedY);
	void scanSeedFillDownward(CDC* pDC, int seedX, int seedY);
	void buildPoly(CDC* pDC);
private:
	std::set<std::pair<USHORT, USHORT>>boundary;
};

