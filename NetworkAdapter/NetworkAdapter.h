
// NetworkAdapter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <NetCon.h>

// CNetworkAdapterApp:
// �йش����ʵ�֣������ NetworkAdapter.cpp
//

class CNetworkAdapterApp : public CWinApp
{
public:
	CNetworkAdapterApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNetworkAdapterApp theApp;