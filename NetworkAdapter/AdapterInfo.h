#ifndef _ADAPTERINFO_H_
#define _ADAPTERINFO_H_

#define MAX_IP_ADDR_LEN 16              //最大IP长度
#define MAX_MAC_ADDR_LEN 13             //MAC最大长度
#define MAX_NAME_LEN 256                //最大连接名称长度

#define CONNECTNAME "CONNECTNAME"
#define IPDHCP "IPDHCP"
#define DNSDHCP "DNSDHCP"
#define IPADDRESS "IPADDRESS"
#define MASK "MASK"
#define GATEWAY "GATEWAY"
#define PRIMARYDNS "PRIMARYDNS"
#define BACKUPDNS "BACKUPDNS"
#define MACADDR   "MACADDR"

#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

#define WM_MSG_MODIFY_COMPLETE WM_USER + 100  //设置完成消息

typedef struct _AdapterInfo
{
	PIP_ADAPTER_INFO pIpAdapterInfo;          //通过iphlpapi库获取
	int iCount;                               //适配器个数
	TCHAR (*csConnectName)[MAX_NAME_LEN];     //连接名称
}AdapterInfo;

class CPlanInfo
{
public:
	char m_PlanName[MAX_NAME_LEN];          //方案名称
	char m_ConnectName[MAX_NAME_LEN];	    //连接名称
	BOOL m_IsIpDHCP;                        //IP是否自动获取
	BOOL m_IsDNSDHCP;                       //DNS是否自动获取   
	char m_IpAddress[MAX_IP_ADDR_LEN];      //IP地址
	char m_Mask[MAX_IP_ADDR_LEN];           //子网掩码
	char m_GateWay[MAX_IP_ADDR_LEN];        //网关
	char m_PrimaryDns[MAX_IP_ADDR_LEN];     //主DNS
	char m_BackupDns[MAX_IP_ADDR_LEN];      //附属DNS
	char m_MacAddr[MAX_MAC_ADDR_LEN];       //MAC地址
	CPlanInfo();
	~CPlanInfo();
	BOOL SetConnectName(char *pName);
	BOOL SetDHCPFlag(BOOL flag);
	BOOL SetIpInfo(char *pIpAddr, char *pMask, char *pGateWay);
	BOOL SetDns(char *pPrimaryDns, char *pBackupDns);
	BOOL SetMacAddr(char *pMac);
};
#endif