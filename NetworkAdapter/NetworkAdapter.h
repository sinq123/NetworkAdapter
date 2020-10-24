
// NetworkAdapter.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <NetCon.h>

// CNetworkAdapterApp:
// 有关此类的实现，请参阅 NetworkAdapter.cpp
//

class CNetworkAdapterApp : public CWinApp
{
public:
	CNetworkAdapterApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CNetworkAdapterApp theApp;