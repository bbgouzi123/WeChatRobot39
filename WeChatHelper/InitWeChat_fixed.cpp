#include "stdafx.h"
#include <strstream>
#include <iostream>
#pragma comment(lib,"Version.lib")



//************************************************************
// 函数名称: IsWxVersionValid
// 功能说明: 检查微信版本是否匹配
// 作    者: GuiShou
// 时    间: 2019/6/30
// 参    数: void
// 返 回 值: BOOL
//************************************************************
BOOL IsWxVersionValid()
{
	DWORD wxBaseAddress = (DWORD)GetModuleHandle(TEXT("WeChatWin.dll"));
	// 支持 3.9.x.x 版本系列
	const string wxVersoin = "3.9.0.28";
	const string wxVersoin2 = "3.9.2.23";
	const string wxVersoin3 = "3.9.2.26";
	const string wxVersoin4 = "3.9.5.81";

	WCHAR VersionFilePath[MAX_PATH];
	if (GetModuleFileName((HMODULE)wxBaseAddress, VersionFilePath, MAX_PATH) == 0)
	{
		return FALSE;
	}

	string asVer = "";
	VS_FIXEDFILEINFO* pVsInfo;
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
	int iVerInfoSize = GetFileVersionInfoSize(VersionFilePath, NULL);
	if (iVerInfoSize != 0) {
		char* pBuf = new char[iVerInfoSize];
		if (GetFileVersionInfo(VersionFilePath, 0, iVerInfoSize, pBuf)) {
			if (VerQueryValue(pBuf, TEXT("\\"), (void**)& pVsInfo, &iFileInfoSize)) {

				int s_major_ver = (pVsInfo->dwFileVersionMS >> 16) & 0x0000FFFF;

				int s_minor_ver = pVsInfo->dwFileVersionMS & 0x0000FFFF;

				int s_build_num = (pVsInfo->dwFileVersionLS >> 16) & 0x0000FFFF;

				int s_revision_num = pVsInfo->dwFileVersionLS & 0x0000FFFF;

				//将版本号转为字符串
				strstream wxVer;
				wxVer << s_major_ver << "." << s_minor_ver << "." << s_build_num << "." << s_revision_num;
				wxVer >> asVer;
			}
		}
		delete[] pBuf;
	}

	//版本匹配 - 支持多个 3.9 版本
	if (asVer == wxVersoin || asVer == wxVersoin2 || asVer == wxVersoin3 || asVer == wxVersoin4)
	{
		return TRUE;
	}

	//版本不匹配
	return FALSE;
}


//************************************************************
// 函数名称: GetAccountService
// 功能说明: 获取账户服务管理器 (3.9版本)
// 作    者: 适配3.9版本
// 时    间: 2024
// 参    数: void
// 返 回 值: UINT64 账户服务地址
//************************************************************
// 获取账户服务管理器 (3.9版本)
typedef UINT64(*__GetAccountService)();
UINT64 GetAccountService()
{
	DWORD dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	UINT64 accout_service_addr = dwWeChatWinAddr + WxGetAccountServiceMgr;
	__GetAccountService GetSevice = (__GetAccountService)accout_service_addr;
	return GetSevice();
}

//************************************************************
// 函数名称: CheckIsLogin
// 功能说明: 检测微信是否已登录 (3.9版本)
// 作    者: GuiShou (已适配3.9版本)
// 时    间: 2019/6/30 (2024适配)
// 参    数: void
// 返 回 值: void
//************************************************************
void CheckIsLogin()
{
	// 获取WeChatWin的基址
	DWORD  dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");

	while (true)
	{
		// 3.9版本使用账户服务管理器检查登录状态
		UINT64 service_addr = GetAccountService();
		if (service_addr != 0)
		{
			UINT64 login_flag = *(UINT64*)(service_addr + LoginSign_Offset);
			if (login_flag != 0)
			{
				// 找到登录窗口句柄
				HWND hLogin = FindWindow(NULL, L"Login");
				if (hLogin == NULL)
				{
					OutputDebugStringA("未找到Login窗口");
					return;
				}
				COPYDATASTRUCT login_msg;
				login_msg.dwData = WM_Login;
				login_msg.lpData = NULL;
				login_msg.cbData = 0;
				// 发送消息给客户端
				SendMessage(hLogin, WM_COPYDATA, (WPARAM)hLogin, (LPARAM)&login_msg);
				break;
			}
		}

		Sleep(500);
	}
}

