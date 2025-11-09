# -*- coding: utf-8 -*-
"""
微信 3.9.5.81 版本适配补丁脚本
自动修改必要的文件以支持微信 3.9.5.81 版本
"""

import os
import re

def apply_patch():
    """应用补丁到相关文件"""
    
    # 1. 修改 InitWeChat.cpp
    init_wechat_cpp = "WeChatHelper/InitWeChat.cpp"
    if os.path.exists(init_wechat_cpp):
        with open(init_wechat_cpp, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        # 检查是否已经添加了 GetAccountService 函数
        if 'GetAccountService' not in content:
            # 在 CheckIsLogin 函数之前添加 GetAccountService 函数
            new_function = """
// 获取账户服务管理器 (3.9版本)
typedef UINT64(*__GetAccountService)();
UINT64 GetAccountService()
{
	DWORD dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	UINT64 accout_service_addr = dwWeChatWinAddr + WxGetAccountServiceMgr;
	__GetAccountService GetSevice = (__GetAccountService)accout_service_addr;
	return GetSevice();
}

"""
            # 在 CheckIsLogin 函数之前插入
            content = content.replace("void CheckIsLogin()", new_function + "void CheckIsLogin()")
            
            # 替换 CheckIsLogin 函数体
            old_check = """void CheckIsLogin()
{
	//ȡWeChatWinĻַ
	DWORD  dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");

	while (true)
	{
		DWORD dwIsLogin = dwWeChatWinAddr + LoginSign_Offset;
		if (*(DWORD*)dwIsLogin != 0)
		{
			//ҵ½ھ
			HWND hLogin = FindWindow(NULL, L"Login");
			if (hLogin == NULL)
			{
				OutputDebugStringA("δҵLogin");
				return;
			}
			COPYDATASTRUCT login_msg;
			login_msg.dwData = WM_Login;
			login_msg.lpData = NULL;
			login_msg.cbData = 0;
			//Ϣƶ
			SendMessage(hLogin, WM_COPYDATA, (WPARAM)hLogin, (LPARAM)&login_msg);
			break;
		}

		Sleep(500);
	}
}"""
            
            new_check = """void CheckIsLogin()
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
}"""
            
            if old_check in content:
                content = content.replace(old_check, new_check)
            else:
                # 使用正则表达式替换
                pattern = r'void CheckIsLogin\(\)\s*\{[^}]*DWORD\s+dwIsLogin\s*=\s*dwWeChatWinAddr\s*\+\s*LoginSign_Offset;[^}]*\}'
                content = re.sub(pattern, new_check, content, flags=re.DOTALL)
            
            with open(init_wechat_cpp, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"[OK] 已更新 {init_wechat_cpp}")
        else:
            print(f"[OK] {init_wechat_cpp} 已经包含 GetAccountService 函数")
    
    # 2. 修改 InitWeChat.h
    init_wechat_h = "WeChatHelper/InitWeChat.h"
    if os.path.exists(init_wechat_h):
        with open(init_wechat_h, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        if 'GetAccountService' not in content:
            # 在文件末尾添加函数声明
            if not content.endswith('\n'):
                content += '\n'
            content += 'UINT64 GetAccountService();\t//获取账户服务管理器 (3.9版本)\n'
            
            with open(init_wechat_h, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"[OK] 已更新 {init_wechat_h}")
        else:
            print(f"[OK] {init_wechat_h} 已经包含 GetAccountService 声明")
    
    # 3. 修改 WndMsgLoop.cpp
    wnd_msg_loop_cpp = "WeChatHelper/WndMsgLoop.cpp"
    if os.path.exists(wnd_msg_loop_cpp):
        with open(wnd_msg_loop_cpp, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        # 替换登录检查代码
        old_login_check = """DWORD dwIsLogin = dwWeChatWinAddr + LoginSign_Offset;
		if (*(DWORD*)dwIsLogin == 0)"""
        
        new_login_check = """// 检测微信是否已登录 (3.9版本使用账户服务管理器)
		UINT64 service_addr = GetAccountService();
		UINT64 login_flag = 0;
		if (service_addr != 0)
		{
			login_flag = *(UINT64*)(service_addr + LoginSign_Offset);
		}
		if (login_flag == 0)"""
        
        if old_login_check in content:
            content = content.replace(old_login_check, new_login_check)
            with open(wnd_msg_loop_cpp, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"[OK] 已更新 {wnd_msg_loop_cpp}")
        else:
            print(f"⚠ {wnd_msg_loop_cpp} 中的登录检查代码可能需要手动更新")
    
    print("\n补丁应用完成！")
    print("请检查编译错误，如有问题请手动修改。")

if __name__ == "__main__":
    apply_patch()

