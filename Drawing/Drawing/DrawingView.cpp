
// DrawingView.cpp : CDrawingView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Drawing.h"
#endif

#include "DrawingDoc.h"
#include "DrawingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrawingView

IMPLEMENT_DYNCREATE(CDrawingView, CView)

BEGIN_MESSAGE_MAP(CDrawingView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_DRAW_LINE, &CDrawingView::OnDrawLine)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_DRAW_LINE_DDA, &CDrawingView::OnDrawLineDda)
	ON_COMMAND(ID_DRAW_LINE_BRE, &CDrawingView::OnDrawLineBre)
	ON_COMMAND(ID_DRAW_CIRCLE, &CDrawingView::OnDrawCircle)
	ON_COMMAND(ID_DRAW_INSPOLY, &CDrawingView::OnDrawInspoly)
	ON_COMMAND(ID_CLIP, &CDrawingView::OnClip)
	ON_COMMAND(ID_DRAW_POLY, &CDrawingView::OnDrawPoly)
	ON_COMMAND(abx, &CDrawingView::Onabx)
END_MESSAGE_MAP()

// CDrawingView ����/����

CDrawingView::CDrawingView()
{
	// TODO:  �ڴ˴���ӹ������

}

CDrawingView::~CDrawingView()
{
}

BOOL CDrawingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CDrawingView ����

void CDrawingView::OnDraw(CDC* /*pDC*/)
{
	CDrawingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CDrawingView ��ӡ

BOOL CDrawingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CDrawingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CDrawingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// CDrawingView ���

#ifdef _DEBUG
void CDrawingView::AssertValid() const
{
	CView::AssertValid();
}

void CDrawingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDrawingDoc* CDrawingView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawingDoc)));
	return (CDrawingDoc*)m_pDocument;
}
#endif //_DEBUG


// CDrawingView ��Ϣ�������




void CDrawingView::OnDrawLine()
{
	// TODO:  �ڴ���������������
	MenuDrawType = 1;
	PressNum = 0;
	AfxMessageBox(_T("��ʼ���߶Σ�"));
}


void CDrawingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CClientDC dc(this);
	CDC* pCDC = GetDC();
	if (MenuDrawType == 1){
		if (PressNum == 0){
			startP = point;
			endP = point;
			PressNum++;
		}
		else{
			endP = point;
			dc.MoveTo(startP);
			dc.LineTo(endP);
			PressNum = 0;
		}
	}
	if (MenuDrawType == 2){
		if (PressNum == 0){
			startP = point;
			endP = point;
			PressNum++;
		}
		else{
			// ���ﲻ��
			 L.MoveTo(pCDC, startP);
			 L.LineToDDA(pCDC, endP);
			 endP = point;
			 L.MoveTo(pCDC, startP);
			 L.LineToDDA(pCDC, endP);
			PressNum = 0;
		}
	}
	//Myline.Bres
	if (MenuDrawType == 3){
		if (PressNum == 0){
			startP = point;
			endP = point;
			PressNum++;
		}
		else{
			//L.MoveTo(pCDC, startP);
			//L.LineToBre(pCDC, endP);
			//endP = point;
			//L.MoveTo(pCDC, startP);
			//L.LineToBre(pCDC, endP);
			PressNum = 0;
		}
	}
	//circle
	if (MenuDrawType == 4){
		if (PressNum == 0){
			startP = point;
			endP = point;
			PressNum++;
		}
		else{
			L.MoveTo(pCDC, startP);
			L.LineToDDA(pCDC, endP);
			endP = point;
			PressNum = 0;
			PressNum = 0;
			C.SetCircle(startP, endP);
			C.DrawCircle(pCDC);
		}
	}
	//Rect
	if (MenuDrawType == 5){
		if (PressNum == 0){
			startP = point;
			endP = point;
			PressNum++;
		}
		else{
			//������
			CPen pen(PS_SOLID, 1, RGB(145, 145, 45));
			CPen *pOldPen = dc.SelectObject(&pen);
			CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
			CBrush *pOldBrush = dc.SelectObject(pBrush);
			dc.Rectangle(CRect(startP, point));
			dc.SelectObject(pOldPen);
			dc.SelectObject(pOldBrush);

			endP = point;
			PressNum = 0;
			Cp.setWin(startP, endP);
			Cp.ExeClip(pCDC,&L);
		}
	}


	CView::OnLButtonDown(nFlags, point);
}

void CDrawingView::OnDrawLineDda()
{
	// TODO:  �ڴ���������������
	MenuDrawType = 2;
	PressNum = 0;
	AfxMessageBox(_T("��ʼDDA���߶Σ�"));
}


void CDrawingView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��Ƥ��
	CClientDC pDC(this);
	CDC* pCDC = GetDC();
	//pDC.SetROP2(R2_NOTXORPEN);//���=�õ�����ɫ
	//ԭ��LineTo
	if (MenuDrawType == 1 && PressNum == 1){
		if (endP != point){ //�жϣ�ָ�벻��ֹ����
			pDC.MoveTo(startP);//��㲻��
			pDC.LineTo(endP); //��ȥ��һʱ�̣�ͨ��

			pDC.MoveTo(startP);//������
			pDC.LineTo(point);

			endP = point;
		}
	}
	//MyLine.DDA
	if (MenuDrawType == 2 && PressNum == 1){
		if (endP != point){ 
			L.MoveTo(pCDC,startP);
			L.LineToDDA(pCDC, endP); 

			L.MoveTo(pCDC,startP);
			L.LineToDDA(pCDC,point);

			endP = point;
		}
	}
	//Myline.Bres
	if (MenuDrawType == 3 && PressNum == 1){
		if (endP != point){
			L.MoveTo(pCDC, startP);
			L.LineToBre(pCDC, endP);

			L.MoveTo(pCDC, startP);
			L.LineToBre(pCDC, point);

			endP = point;
		}
	}
	//circle
	if (MenuDrawType == 4 && PressNum == 1){
		if (endP != point){ 
			L.MoveTo(pCDC, startP);
			L.LineToDDA(pCDC, endP);

			L.MoveTo(pCDC, startP);
			L.LineToDDA(pCDC, point);

			endP = point;		
		}
	}


	CView::OnMouseMove(nFlags, point);
}


void CDrawingView::OnDrawLineBre()
{
	// TODO:  �ڴ���������������
	MenuDrawType = 3;
	PressNum = 0;
	AfxMessageBox(_T("��ʼ���߶Σ�"));
}


void CDrawingView::OnDrawCircle()
{
	// TODO:  �ڴ���������������
	//ˢ�»���
	CRect rectCtrl;
	CClientDC pDC(this);
	this->GetClientRect(rectCtrl);
	pDC.FillSolidRect(rectCtrl, RGB(255, 255, 255));

	MenuDrawType = 4;
	PressNum = 0;
	AfxMessageBox(_T("��ʼ��Բ��"));
}


void CDrawingView::OnDrawInspoly()
{
	// TODO:  �ڴ���������������
	CClientDC pDC(this);
	CDC* pCDC = GetDC();

	C.DrawInsPoly(pCDC,5); //�����5
}


void CDrawingView::OnClip()
{
	// TODO:  �ڴ���������������
	// TODO:  �ڴ���������������
	MenuDrawType = 5;
	PressNum = 0;
	AfxMessageBox(_T("ע�⣺��֧�ֲü�ʹ��DDA��������"));
}


void CDrawingView::OnDrawPoly()
{
	// TODO:  �ڴ���������������
	CRect rectCtrl;
	CClientDC pDC(this);
	CDC* pCDC = GetDC();

	//ˢ�»���
	this->GetClientRect(rectCtrl);
	pDC.FillSolidRect(rectCtrl,RGB(255, 255, 255));

	MenuDrawType = 6;
	PressNum = 0;
	Fl.buildPoly(pCDC);

	//���ӵ�Ϊ��170��170��
	Fl.scanSeedFillUpward(pCDC, 170, 170);
	Fl.scanSeedFillDownward(pCDC,170.,170);
}


void CDrawingView::Onabx()
{
	// TODO:  �ڴ���������������

}
