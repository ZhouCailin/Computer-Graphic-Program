
// Drawing.h : Drawing Ӧ�ó������ͷ�ļ�
#include "Line.h" //2020.5.21
#include "Circle.h"//2020.6.19
#include "Clip.h"//2020.6.19
#include "Fill.h" //2020.6.20
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CDrawingApp:
// �йش����ʵ�֣������ Drawing.cpp
//

class CDrawingApp : public CWinApp
{
public:
	CDrawingApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDrawingApp theApp;
