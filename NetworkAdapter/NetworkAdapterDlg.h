
// NetworkAdapterDlg.h : 头文件
//

#pragma once
#include "AdapterInfo.h"

#include <Mprapi.h>
#include <iphlpapi.h>
#include <Sensapi.h>
#include <string>
#include <list>

#pragma comment(lib, "Sensapi.lib")
#pragma comment( lib, "Iphlpapi.lib" )
#pragma comment( lib, "Mprapi.lib" )

struct SAdapter
{
	CStringW strConnectName; //适配器名称
	CStringW strAdapterName; //适配器名称代号
	CStringW strMACVL;//MAC地址
	CStringW strIPVL;//IP地址
	CStringW strSubnetmaskvl;//子网掩码
	CStringW strGatewayvl;//网关
	CStringW strDhcpEnabled;//是否自动获取IP信息DhcpEnabled
	CStringW strZT;
};

// CNetworkAdapterDlg 对话框
class CNetworkAdapterDlg : public CDialogEx
{
// 构造
public:
	CNetworkAdapterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NETWORKADAPTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

private:
	std::list<SAdapter> m_lsAdapter;

	void OnNetTest(void);
	_AdapterInfo m_AdapterInfo;
	bool GetPcNetInfo(void);
	void InsertConnectName(ULONG index, TCHAR *pName);
	
private:
	/****************************************************************** 
	* 函数介绍：运行程序 
	* 输入参数： 
	* 输出参数： 
	* 返回值 ： 
	*******************************************************************/
	DWORD WinExecAndWait32(LPCTSTR lpszAppPath, LPCTSTR lpParameters, LPCTSTR lpszDirectory, 
		DWORD dwMilliseconds, BOOL bIsWait=TRUE, int nShow=0);
	// 测试网络是否正常
	bool CheckNetIsOK(const CStringW& sUpdateIP=L"192.168.5.9");
	// 显示参数信息
	void ShowInfo(void);
private:
	int m_nCheckNetOK;
	void NetConnect(const CString& strIntConName);
	void NetDisconnect(const CString& strIntConName);
	
};
