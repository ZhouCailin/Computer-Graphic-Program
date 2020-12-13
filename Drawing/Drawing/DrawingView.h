
// DrawingView.h : CDrawingView ��Ľӿ�
//

#pragma once




class CDrawingView : public CView
{
protected: // �������л�����
	CDrawingView();
	DECLARE_DYNCREATE(CDrawingView)

// ����
public:
	CDrawingDoc* GetDocument() const;

// ����
public:
	int PressNum, MenuDrawType;
	CPoint startP, endP;
	CLine L; 
	Circle C;
	CClip Cp;
	CFill Fl;

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CDrawingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // DrawingView.cpp �еĵ��԰汾
inline CDrawingDoc* CDrawingView::GetDocument() const
   { return reinterpret_cast<CDrawingDoc*>(m_pDocument); }
#endif

