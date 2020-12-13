#pragma once
class Circle
{
public:
	Circle();
	virtual ~Circle();
	void DrawCircle(CDC* pDC);
	void SetCircle(CPoint sp, CPoint ep);
	void DrawInsPoly(CDC* pDC,double d);
private:
	int radius;
	CPoint centre;
};

