
// DrawingView.h : CDrawingView 类的接口
//

#pragma once




class CDrawingView : public CView
{
protected: // 仅从序列化创建
	CDrawingView();
	DECLARE_DYNCREATE(CDrawingView)

// 特性
public:
	CDrawingDoc* GetDocument() const;

// 操作
public:
	int PressNum, MenuDrawType;
	CPoint startP, endP;
	CLine L; 
	Circle C;
	CClip Cp;
	CFill Fl;

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CDrawingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawLine();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDrawLineDda();
	afx_msg void OnDrawLineBre();
	afx_msg void OnDrawCircle();
	afx_msg void OnDrawInspoly();
	afx_msg void OnClip();
	afx_msg void OnDrawPoly();
	afx_msg void Onabx();
};

#ifndef _DEBUG  // DrawingView.cpp 中的调试版本
inline CDrawingDoc* CDrawingView::GetDocument() const
   { return reinterpret_cast<CDrawingDoc*>(m_pDocument); }
#endif

