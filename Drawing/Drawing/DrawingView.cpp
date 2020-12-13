
// DrawingView.cpp : CDrawingView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
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

// CDrawingView 构造/析构

CDrawingView::CDrawingView()
{
	// TODO:  在此处添加构造代码

}

CDrawingView::~CDrawingView()
{
}

BOOL CDrawingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDrawingView 绘制

void CDrawingView::OnDraw(CDC* /*pDC*/)
{
	CDrawingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
}


// CDrawingView 打印

BOOL CDrawingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDrawingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CDrawingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// CDrawingView 诊断

#ifdef _DEBUG
void CDrawingView::AssertValid() const
{
	CView::AssertValid();
}

void CDrawingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDrawingDoc* CDrawingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawingDoc)));
	return (CDrawingDoc*)m_pDocument;
}
#endif //_DEBUG


// CDrawingView 消息处理程序




void CDrawingView::OnDrawLine()
{
	// TODO:  在此添加命令处理程序代码
	MenuDrawType = 1;
	PressNum = 0;
	AfxMessageBox(_T("开始绘线段！"));
}


void CDrawingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
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
			// 这里不用
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
			//画矩形
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
	// TODO:  在此添加命令处理程序代码
	MenuDrawType = 2;
	PressNum = 0;
	AfxMessageBox(_T("开始DDA绘线段！"));
}


void CDrawingView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//橡皮筋
	CClientDC pDC(this);
	CDC* pCDC = GetDC();
	//pDC.SetROP2(R2_NOTXORPEN);//异或=得到背景色
	//原生LineTo
	if (MenuDrawType == 1 && PressNum == 1){
		if (endP != point){ //判断：指针不静止则画线
			pDC.MoveTo(startP);//起点不变
			pDC.LineTo(endP); //擦去上一时刻（通过

			pDC.MoveTo(startP);//画现在
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
	// TODO:  在此添加命令处理程序代码
	MenuDrawType = 3;
	PressNum = 0;
	AfxMessageBox(_T("开始绘线段！"));
}


void CDrawingView::OnDrawCircle()
{
	// TODO:  在此添加命令处理程序代码
	//刷新画布
	CRect rectCtrl;
	CClientDC pDC(this);
	this->GetClientRect(rectCtrl);
	pDC.FillSolidRect(rectCtrl, RGB(255, 255, 255));

	MenuDrawType = 4;
	PressNum = 0;
	AfxMessageBox(_T("开始画圆！"));
}


void CDrawingView::OnDrawInspoly()
{
	// TODO:  在此添加命令处理程序代码
	CClientDC pDC(this);
	CDC* pCDC = GetDC();

	C.DrawInsPoly(pCDC,5); //误差设5
}


void CDrawingView::OnClip()
{
	// TODO:  在此添加命令处理程序代码
	// TODO:  在此添加命令处理程序代码
	MenuDrawType = 5;
	PressNum = 0;
	AfxMessageBox(_T("注意：仅支持裁剪使用DDA画出的线"));
}


void CDrawingView::OnDrawPoly()
{
	// TODO:  在此添加命令处理程序代码
	CRect rectCtrl;
	CClientDC pDC(this);
	CDC* pCDC = GetDC();

	//刷新画布
	this->GetClientRect(rectCtrl);
	pDC.FillSolidRect(rectCtrl,RGB(255, 255, 255));

	MenuDrawType = 6;
	PressNum = 0;
	Fl.buildPoly(pCDC);

	//种子点为（170，170）
	Fl.scanSeedFillUpward(pCDC, 170, 170);
	Fl.scanSeedFillDownward(pCDC,170.,170);
}


void CDrawingView::Onabx()
{
	// TODO:  在此添加命令处理程序代码

}
