// ============================================
// 微信 3.9.5.81 版本适配补丁
// 需要手动将这些代码添加到相应文件中
// ============================================

// ============================================
// 1. 在 InitWeChat.cpp 的 CheckIsLogin() 函数之前添加：
// ============================================

// 获取账户服务管理器 (3.9版本)
typedef UINT64(*__GetAccountService)();
UINT64 GetAccountService()
{
	DWORD dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	UINT64 accout_service_addr = dwWeChatWinAddr + WxGetAccountServiceMgr;
	__GetAccountService GetSevice = (__GetAccountService)accout_service_addr;
	return GetSevice();
}

// ============================================
// 2. 替换 InitWeChat.cpp 中的 CheckIsLogin() 函数：
// ============================================

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

// ============================================
// 3. 在 InitWeChat.h 中添加函数声明：
// ============================================

UINT64 GetAccountService();	//获取账户服务管理器 (3.9版本)

// ============================================
// 4. 替换 WndMsgLoop.cpp 中 InitWindow() 函数里的登录检查代码：
// ============================================

// 将这段代码：
//		DWORD dwIsLogin = dwWeChatWinAddr + LoginSign_Offset;
//		if (*(DWORD*)dwIsLogin == 0)	//等于0说明微信未登录

// 替换为：
		// 检测微信是否已登录 (3.9版本使用账户服务管理器)
		UINT64 service_addr = GetAccountService();
		UINT64 login_flag = 0;
		if (service_addr != 0)
		{
			login_flag = *(UINT64*)(service_addr + LoginSign_Offset);
		}
		if (login_flag == 0)	//等于0说明微信未登录

// ============================================
// 5. 在 WndMsgLoop.cpp 的 else 分支中也需要更新：
// ============================================

// 将这段代码：
//		DWORD dwIsLogin = dwWeChatWinAddr + LoginSign_Offset;
//		if (*(DWORD*)dwIsLogin != 0)

// 替换为：
		// 检测微信是否已登录 (3.9版本使用账户服务管理器)
		UINT64 service_addr = GetAccountService();
		UINT64 login_flag = 0;
		if (service_addr != 0)
		{
			login_flag = *(UINT64*)(service_addr + LoginSign_Offset);
		}
		if (login_flag != 0)

