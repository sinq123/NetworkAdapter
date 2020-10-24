
// NetworkAdapterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NetworkAdapter.h"
#include "NetworkAdapterDlg.h"
#include "afxdialogex.h"
#include "AdapterInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetworkAdapterDlg 对话框

CString ToStatus(const int nStatus)
{
	switch(nStatus)
	{
	case 0:
		{
			return L"无法连接 ncs";
		}
		break;
	case 1:
		{
			return L"网络控制系统连接";
		}
		break;
	case 2:
		{
			return L"Ncs 连接";
		}
		break;
	case 3:
		{
			return L"Ncs 断开";
		}
		break;
	case 4:
		{
			return L"Ncs 硬件不存在";
		}
		break;
	case 5:
		{
			return L"Ncs 硬件失效";
		}
		break;
	case 6:
		{
			return L"Ncs 硬件故障";
		}
		break;
	case 7:
		{
			return L"Ncs 媒体断开连接";
		}
		break;
	case 8:
		{
			return L"Ncs 认证";
		}
		break;
	case 9:
		{
			return L"Ncs 认证成功";
		}
		break;
	case 10:
		{
			return L"Ncs 认证失败";
		}
		break;
	case 11:
		{
			return L"Ncs 无效地址";
		}
		break;
	case 12:
		{
			return L"需要国家安全局的证件";
		}
		break;
	default:
		{
			return L"未知类型";
		}
		break;
	}
}


CNetworkAdapterDlg::CNetworkAdapterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNetworkAdapterDlg::IDD, pParent)
	, m_nCheckNetOK(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetworkAdapterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNetworkAdapterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNetworkAdapterDlg 消息处理程序

BOOL CNetworkAdapterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	GetPcNetInfo();
	OnNetTest();
	ShowInfo();
	SetTimer(1, 5000, NULL);
	//CheckNetIsOK(L"192.168.200.201");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNetworkAdapterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNetworkAdapterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNetworkAdapterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);

	switch(nIDEvent)
	{
	case 1:
		{
			KillTimer(1);
			if (CheckNetIsOK())
			{
				GetDlgItem(IDC_ED_CHECKIPOK)->SetWindowTextW(L"拼车管所IP 192.168.5.9 正常");
				m_nCheckNetOK = 0;
			}
			else
			{
				CString strMsg;
				strMsg.Format(L"拼车管所IP 192.168.5.9 失败 是否断网？？次数：%d", m_nCheckNetOK);
				GetDlgItem(IDC_ED_CHECKIPOK)->SetWindowTextW(strMsg);
				m_nCheckNetOK +=1;

				if (m_nCheckNetOK > 10)
				{
					CString strTemp;
					strTemp.Format(L"光钎以断开，正在尝试接入4G网络");
					GetDlgItem(IDC_ST_GQDK)->SetWindowTextW(strTemp);
					for (std::list<SAdapter>::iterator ir=m_lsAdapter.begin(); ir!=m_lsAdapter.end(); ++ir)
					{
						if (ir->strZT.Find(L"无法连接 ncs") == -1)
						{
							continue;
						}
						NetConnect(ir->strConnectName);
						GetPcNetInfo();
						OnNetTest();
						break;
					}
				}
			}
			ShowInfo();
			SetTimer(1, 5000, NULL);
		}
		break;
	}
}


void CNetworkAdapterDlg::ShowInfo(void)
{
	int nN(IDC_ST_CONNECTNAME);
	for(int i=0;i<4; i++)
	{
		GetDlgItem(nN)->SetWindowTextW(L"");
		nN +=1;
		GetDlgItem(nN)->SetWindowTextW(L"");
		nN +=1;
		GetDlgItem(nN)->SetWindowTextW(L"");
		nN +=1;
		GetDlgItem(nN)->SetWindowTextW(L"");
		nN +=1;
		GetDlgItem(IDC_ST_ZT+i)->SetWindowTextW(L"");
	}

	if (m_lsAdapter.size() > 0)
	{
		nN = IDC_ST_CONNECTNAME;
		int i(0);
		bool bIPOk(false);
		CString strConnectName;
		for (std::list<SAdapter>::iterator ir=m_lsAdapter.begin(); ir!=m_lsAdapter.end(); ++ir)
		{
			GetDlgItem(nN)->SetWindowTextW(ir->strConnectName);
			nN +=1;
			GetDlgItem(nN)->SetWindowTextW(ir->strIPVL);
			nN +=1;
			GetDlgItem(nN)->SetWindowTextW(ir->strSubnetmaskvl);
			nN +=1;
			GetDlgItem(nN)->SetWindowTextW(ir->strGatewayvl);
			nN +=1;
			GetDlgItem(IDC_ST_ZT+i)->SetWindowTextW(ir->strZT);
			i++;

			if (ir->strIPVL.Find(L"192.168.168.230") != std::string::npos)
			{
				bIPOk = true;
				strConnectName = ir->strConnectName;
			}
		}
		if (bIPOk)
		{
			NetDisconnect(strConnectName);
			GetPcNetInfo();
			OnNetTest();
		}
	}
}

void CNetworkAdapterDlg::OnNetTest(void)
{
	INetConnectionManager *pNetManager;
	INetConnection *pNetConnection;
	IEnumNetConnection *pEnum;

	do
	{
		CoInitialize(NULL);

		if (S_OK != CoCreateInstance(CLSID_ConnectionManager, NULL, CLSCTX_SERVER, IID_INetConnectionManager, (void**)&pNetManager))
		{
			break;
		}

		pNetManager->EnumConnections(NCME_DEFAULT, &pEnum);
		pNetManager->Release();
		if (NULL == pEnum)
		{
			break;
		}

		ULONG celtFetched;
		int i = 0;
		std::list<SAdapter> lsAdapter;
		while (pEnum->Next(1, &pNetConnection, &celtFetched) == S_OK)
		{
			NETCON_PROPERTIES *properties;
			pNetConnection->GetProperties(&properties);
			CString name = properties->pszwName;	//网络连接的名称
			CString guidId, strTemp;
			wchar_t sz[3] = { 0 };
			for (int n = 0; n < sizeof(properties->guidId.Data4) / sizeof(properties->guidId.Data4[0]); n++)
			{
				_stprintf(sz, TEXT("%02X"), properties->guidId.Data4[n]);
				strTemp.AppendFormat(_T("%s"), sz);
				if (strTemp.GetLength() == 4)
				{
					strTemp.AppendFormat(L"-");
				}
			}
			guidId.Format(L"%08X-%04X-%04X-%s", properties->guidId.Data1, properties->guidId.Data2, properties->guidId.Data3, strTemp); 

			//pNetConnection->Disconnect();	//禁目连接
			//pNetConnection->Connect();	//启用连接
			SAdapter sAdapter;
			sAdapter.strConnectName = name;
			sAdapter.strZT.Format(L"%s", ToStatus(properties->Status));
			for (std::list<SAdapter>::iterator ir=m_lsAdapter.begin(); ir!=m_lsAdapter.end(); ++ir)
			{

				if (ir->strAdapterName.Find(guidId) == -1)
				{
					continue;
				}

				sAdapter.strAdapterName = ir->strAdapterName;
				sAdapter.strDhcpEnabled = ir->strDhcpEnabled;
				sAdapter.strGatewayvl = ir->strGatewayvl;
				sAdapter.strIPVL = ir->strIPVL;
				sAdapter.strMACVL = ir->strMACVL;
				sAdapter.strSubnetmaskvl = ir->strSubnetmaskvl;
				break;
			}

			lsAdapter.push_back(sAdapter);
			i++;
		}
		m_lsAdapter.clear();
		m_lsAdapter = lsAdapter;

	}
	while(false);
}

bool CNetworkAdapterDlg::GetPcNetInfo(void)
{
	bool bRt = true;
	/*******************************************
	*通过Iphlpapi库获取网卡信息和个数
	********************************************/
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	ULONG stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize); //获得其大小
	if (ERROR_BUFFER_OVERFLOW == nRel) //重新申请所需要的空间
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO) new BYTE[stSize];
		nRel=GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel) //获取信息成功
	{
		m_AdapterInfo.pIpAdapterInfo = pIpAdapterInfo;
		m_AdapterInfo.iCount = 0;
		while (pIpAdapterInfo) //获取网卡个数
		{
			m_AdapterInfo.iCount++;
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
	/*******************************************
	*通过mprapi库获取连接名称
	*并通过index将网卡信息和连接名称相关联
	********************************************/
	HANDLE hMprConfig; //连接信息的句柄
	DWORD dwRet=0; //返回值
	PIP_INTERFACE_INFO plfTable = NULL; //接口信息表
	DWORD dwBufferSize=0; //接口信息表空间大小
	m_AdapterInfo.csConnectName = new TCHAR [m_AdapterInfo.iCount] [256]; //申请空间存储连接名
	dwRet = MprConfigServerConnect(NULL, &hMprConfig); //获得句柄
	dwRet = GetInterfaceInfo(NULL, &dwBufferSize); //获得接口信息表大小
	if(dwRet == ERROR_INSUFFICIENT_BUFFER) //获得接口信息
	{
		plfTable = (PIP_INTERFACE_INFO)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBufferSize);
		GetInterfaceInfo(plfTable, &dwBufferSize);
	}
	TCHAR szFriendName[256]; //接口名称
	DWORD tchSize = sizeof(TCHAR) * 256;
	ZeroMemory(&szFriendName, tchSize);
	for (UINT i = 0; i < plfTable->NumAdapters; i++)
	{
		IP_ADAPTER_INDEX_MAP AdaptMap; //接口信息
		AdaptMap = plfTable->Adapter[i];
		dwRet = MprConfigGetFriendlyName(hMprConfig, AdaptMap.Name,
			(PWCHAR)szFriendName, tchSize); //获得连接名称unicode
		USES_CONVERSION;
		//char* pName = W2A((PWCHAR)szFriendName); //转换为ansi
		InsertConnectName(AdaptMap.Index, szFriendName); //根据Index存储名字信息
	}
	HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, plfTable);
	//*********************************************************************
	//*******写网络配置相关信息到文件**************************************
	PIP_ADAPTER_INFO pIpAdapterInfoWt = m_AdapterInfo.pIpAdapterInfo;
	for (UINT i = 0;i < m_AdapterInfo.iCount;i++)
	{
		SAdapter sAdapter;
		//写网络适配器的名称
		CString str_adpname = _T("");
		str_adpname.Format(_T("AdapterName%d"),i);
		CString str_adpnamevl(pIpAdapterInfoWt->AdapterName);
		sAdapter.strAdapterName.Format(L"%s", str_adpnamevl);
		//是否自动获取IP信息DhcpEnabled
		CString str_dhcpenabled = _T("");
		str_dhcpenabled.Format(_T("DhcpEnabled%d"),i);
		CString str_dhcpenabledvl = _T("");
		str_dhcpenabledvl.Format(_T("%d"),pIpAdapterInfoWt->DhcpEnabled);
		sAdapter.strDhcpEnabled.Format(L"%s", str_dhcpenabledvl);
		//写MAC地址
		CString str_mac = _T("");
		str_mac.Format(_T("MAC%d"),i);
		CString str_macvl= _T("");
		str_macvl.Format(_T("%02X%02X%02X%02X%02X%02X"),
			pIpAdapterInfoWt->Address[0],
			pIpAdapterInfoWt->Address[1],
			pIpAdapterInfoWt->Address[2],
			pIpAdapterInfoWt->Address[3],
			pIpAdapterInfoWt->Address[4],
			pIpAdapterInfoWt->Address[5]);
		sAdapter.strMACVL.Format(L"%s", str_macvl);
		//写IP地址
		CString str_ip = _T("");
		str_ip.Format(_T("IP%d"),i);
		CString str_ipvl = _T("");
		str_ipvl.Format(_T("%s"), CStringW(pIpAdapterInfoWt->IpAddressList.IpAddress.String));
		sAdapter.strIPVL.Format(L"%s", str_ipvl);
		//子网掩码
		CString str_subnetmask = _T("");
		str_subnetmask.Format(_T("SubnetMask%d"),i);
		CString str_subnetmaskvl = _T("");
		str_subnetmaskvl.Format(_T("%s"),CStringW(pIpAdapterInfoWt->IpAddressList.IpMask.String));
		sAdapter.strSubnetmaskvl.Format(L"%s", str_subnetmaskvl);
		//网关
		CString str_gateway = _T("");
		str_gateway.Format(_T("Gateway%d"),i);
		CString str_gatewayvl = _T("");
		str_gatewayvl.Format(_T("%s"),CStringW(pIpAdapterInfoWt->GatewayList.IpAddress.String));
		sAdapter.strGatewayvl.Format(L"%s", str_gatewayvl);
		//Dns服务器
		IP_PER_ADAPTER_INFO* pPerAdapt	= NULL;
		ULONG ulLen = 0;
		int err = GetPerAdapterInfo( pIpAdapterInfoWt->Index, pPerAdapt, &ulLen);
		if( err == ERROR_BUFFER_OVERFLOW )
		{
			pPerAdapt = ( IP_PER_ADAPTER_INFO* ) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulLen);
			err = GetPerAdapterInfo( pIpAdapterInfoWt->Index, pPerAdapt, &ulLen );
			if( err == ERROR_SUCCESS )
			{
				IP_ADDR_STRING* pNext = &( pPerAdapt->DnsServerList );
				if (pNext && strcmp(pNext->IpAddress.String, "") != 0)//自动获取为false
				{
					//dsn是否自动获取
					CString str_dnsdhcp = _T("");
					str_dnsdhcp.Format(_T("DnsDhcpEnabled%d"),i);
					CString str_dnsdhcpvl = _T("0");
					//dsn-PrimaryDns
					CString str_primaryDns = _T("");
					str_primaryDns.Format(_T("PrimaryDns%d"),i);
					CString str_primaryDnsvl = _T("");
					str_primaryDnsvl.Format(_T("%s"),CStringW(pNext->IpAddress.String));
					//dns-backupdns
					CString str_backupdns = _T("");
					str_backupdns.Format(_T("BackupDns%d"),i);
					if (pNext=pNext->Next)
					{
						CString str_backupdnsvl = _T("");
						str_backupdnsvl.Format(_T("%s"),CStringW(pNext->IpAddress.String));
						//m_pIniFile->WriteString(_T("System"),str_backupdns, str_backupdnsvl);
					}

				}
				else //dns自动获取为true
				{
					//dsn是否自动获取
					CString str_dnsdhcp = _T("");
					str_dnsdhcp.Format(_T("DnsDhcpEnabled%d"),i);
					CString str_dnsdhcpvl = _T("1");

				}
			}
			HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, pPerAdapt);
		}
		pIpAdapterInfoWt = pIpAdapterInfoWt->Next;
		m_lsAdapter.push_back(sAdapter);
	}

	return bRt;
}

void CNetworkAdapterDlg::InsertConnectName(ULONG index, TCHAR *pName)
{
	PIP_ADAPTER_INFO pIpAdapterInfo = m_AdapterInfo.pIpAdapterInfo;
	for (int i = 0; i < m_AdapterInfo.iCount; i++)
	{
		if (pIpAdapterInfo->Index == index)
		{
			wcscpy(m_AdapterInfo.csConnectName[i], pName);//适配器名称
		}
		pIpAdapterInfo = pIpAdapterInfo->Next;
	}
}

bool CNetworkAdapterDlg::CheckNetIsOK(const CStringW& sUpdateIP/*=L"192.168.5.9"*/)
{
	//Judge Network is Connected
	int nCount = 1;
	do 
	{
		DWORD dw;
		if( IsNetworkAlive(&dw))
		{
			break;
		}
		else
		{
			Sleep(10000);
			CString sNetWorkConnect;
			sNetWorkConnect.Format(L"第%d次网络未成功连接, 10秒后重试", nCount);
			nCount++ ;
		}
	} while (nCount <4);

	if (nCount == 4)
	{
		return false;
	}

	DWORD n = WinExecAndWait32(_T("ping.exe"), sUpdateIP + " -n 2"/*sCmdPara*/, NULL, 10000);
	if (n == 0)
	{
		return true;
	}
	else
	{
		CString sNetWorkConnect;
		sNetWorkConnect.Format(L"网络连接正常, Ping:%s 失败, 请检测此IP对应的服务器是否正常工作", sUpdateIP);
		//MessageBox(sNetWorkConnect);
		return false;
	}
}

DWORD CNetworkAdapterDlg::WinExecAndWait32(LPCTSTR lpszAppPath, LPCTSTR lpParameters, LPCTSTR lpszDirectory, 
	DWORD dwMilliseconds, BOOL bIsWait/*=TRUE*/, int nShow/*=0*/)
{
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = lpszAppPath;
	ShExecInfo.lpParameters = lpParameters;
	ShExecInfo.lpDirectory = lpszDirectory;
	ShExecInfo.nShow = nShow; //SW_SHOW
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);

	if ( ShExecInfo.hProcess == NULL)
	{
		return 1;
	}
	if ( !bIsWait )
	{
		return 0;
	}
	if (WaitForSingleObject(ShExecInfo.hProcess, dwMilliseconds) == WAIT_TIMEOUT)
	{
		TerminateProcess(ShExecInfo.hProcess, 0);
		return 1;
	}

	DWORD dwExitCode;
	BOOL bOK = GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);
	ASSERT(bOK);

	return dwExitCode;
}

void CNetworkAdapterDlg::NetConnect(const CString& strIntConName)
{
	INetConnectionManager *pNetManager;
	INetConnection *pNetConnection;
	IEnumNetConnection *pEnum;

	do
	{
		CoInitialize(NULL);

		if (S_OK != CoCreateInstance(CLSID_ConnectionManager, NULL, CLSCTX_SERVER, IID_INetConnectionManager, (void**)&pNetManager))
		{
			break;
		}

		pNetManager->EnumConnections(NCME_DEFAULT, &pEnum);
		pNetManager->Release();
		if (NULL == pEnum)
		{
			break;
		}

		ULONG celtFetched;
		int i = 0;
		while (pEnum->Next(1, &pNetConnection, &celtFetched) == S_OK)
		{
			NETCON_PROPERTIES *properties;
			pNetConnection->GetProperties(&properties);
			CString name = properties->pszwName;	//网络连接的名称

			if (name.Find(strIntConName) != std::string::npos)
			{
				//pNetConnection->Disconnect();	//禁目连接
				pNetConnection->Connect();	//启用连接
				break;
			}
			i++;
		}

	}
	while(false);
}

void CNetworkAdapterDlg::NetDisconnect(const CString& strIntConName)
{
	INetConnectionManager *pNetManager;
	INetConnection *pNetConnection;
	IEnumNetConnection *pEnum;

	do
	{
		CoInitialize(NULL);

		if (S_OK != CoCreateInstance(CLSID_ConnectionManager, NULL, CLSCTX_SERVER, IID_INetConnectionManager, (void**)&pNetManager))
		{
			break;
		}

		pNetManager->EnumConnections(NCME_DEFAULT, &pEnum);
		pNetManager->Release();
		if (NULL == pEnum)
		{
			break;
		}

		ULONG celtFetched;
		int i = 0;
		while (pEnum->Next(1, &pNetConnection, &celtFetched) == S_OK)
		{
			NETCON_PROPERTIES *properties;
			pNetConnection->GetProperties(&properties);
			CString name = properties->pszwName;	//网络连接的名称

			if (name.Find(strIntConName) != std::string::npos)
			{
				pNetConnection->Disconnect();	//禁目连接
				//pNetConnection->Connect();	//启用连接
				break;
			}
			i++;
		}

	}
	while(false);
}

