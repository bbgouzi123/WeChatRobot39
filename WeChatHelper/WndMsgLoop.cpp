#include "stdafx.h"
#include "WndMsgLoop.h"
#include "InitWeChat.h"
#include "Login.h"
#include "MainWindow.h"
#include "FriendList.h"
#include "ChatRecord.h"
#include "Function.h"
#include "ChatRoomOperate.h"
#include "CAutoFunction.h"
#include "SelfInformation.h"
#include <stdio.h>

extern BOOL g_AutoChat;					//Զ



//************************************************************
// : RegisterWindow
// ˵: ʼ 
//     : GuiShou
// ʱ    : 2019/6/30
//     : HMODULE hModule 
//   ֵ: void 
//************************************************************
void InitWindow(HMODULE hModule)
{
	//鵱ǰ΢Ű汾
	if (IsWxVersionValid())
	{
		//ȡWeChatWinĻַ
		DWORD dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	
		//΢Ƿ½
		// 检测微信是否已登录 (3.9版本使用账户服务管理器)
		UINT64 service_addr = GetAccountService();
		UINT64 login_flag = 0;
		if (service_addr != 0)
		{
			login_flag = *(UINT64*)(service_addr + LoginSign_Offset);
		}
		if (login_flag == 0)	//0˵΢δ¼
		{
			//̳߳΢ŵ½״̬
			HANDLE hThread= CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckIsLogin, 0, 0, NULL);
			CloseHandle(hThread);

			//HOOKȡбcall
			HookGetFriendList();
			
			//HOOKϢ
			HookChatRecord();
			
			//
			AntiRevoke();
			
			//HOOKȡ 
			//HookExtractExpression();

			//עᴰ
			RegisterWindow(hModule);
		}
		else
		{
			//΢Ѿ½ Ϣͻ
			HWND hLogin = FindWindow(NULL, L"Login");
			if (hLogin == NULL)
			{
				OutputDebugStringA("δҵLogin");
				return;
			}
			COPYDATASTRUCT login_msg;
			login_msg.dwData = WM_AlreadyLogin;
			login_msg.lpData = NULL;
			login_msg.cbData = 0;
			//Ϣƶ
			SendMessage(hLogin, WM_COPYDATA, (WPARAM)hLogin, (LPARAM)&login_msg);
		}
	}
	else
	{
		MessageBoxA(NULL, "ǰ΢Ű汾ƥ䣬WeChat 3.2.1.154", "", MB_OK);
	}

}


//************************************************************
// : RegisterWindow
// ˵: עᴰ
//     : GuiShou
// ʱ    : 2019/6/30
//     : HMODULE hModule ھ
//   ֵ: void 
//************************************************************

void RegisterWindow(HMODULE hModule)
{
	//1  һ
	WNDCLASS wnd;
	wnd.style = CS_VREDRAW | CS_HREDRAW;//
	wnd.lpfnWndProc = WndProc;//ڻصָ.
	wnd.cbClsExtra = NULL;
	wnd.cbWndExtra = NULL;
	wnd.hInstance = hModule;
	wnd.hIcon = NULL;
	wnd.hCursor = NULL;
	wnd.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = TEXT("WeChatHelper");
	//2  עᴰ
	RegisterClass(&wnd);
	//3  
	HWND hWnd = CreateWindow(
		TEXT("WeChatHelper"),  //
		TEXT("WeChatHelper"),//
		WS_OVERLAPPEDWINDOW,//ڷ
		10, 10, 500, 300, //λ
		NULL,             //ھ
		NULL,             //˵
		hModule,        //ʵ
		NULL              //WM_CREATEϢʱĸӲ
	);
	//4  ʾ
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
	//5  ϢѭϢã
	MSG  msg = {};
	//   5.1ȡϢ
	while (GetMessage(&msg, 0, 0, 0))
	{
		//   5.2Ϣ
		TranslateMessage(&msg);
		//   5.3תϢص
		DispatchMessage(&msg);
	}
}


//************************************************************
// : WndProc
// ˵: ص ںͿƶͨ 
//     : GuiShou
// ʱ    : 2019/6/30
//     : HWND hWnd,UINT Message,WPARAM wParam,LPARAM lParam
//   ֵ: LRESULT 
//************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (Message == WM_COPYDATA)
	{
		COPYDATASTRUCT *pCopyData = (COPYDATASTRUCT*)lParam;
		//ͨϢṹ
		MessageUnion *msg = (MessageUnion*)pCopyData->lpData;
		switch (pCopyData->dwData)
		{
		//ʾά
		case WM_ShowQrPicture:
		{
			GotoQrCode();
			HookQrCode();
		}
		break;
		//˳΢
		case WM_Logout:
		{
			LogoutWeChat();
		}
		break;
		//ıϢ
		case WM_SendTextMessage:
		{
			SendTextMessage(msg->genericmsg.msgdata1, msg->genericmsg.msgdata2);
			
		}
		break;
		//ļϢ
		case WM_SendFileMessage:
		{
			SendFileMessage(msg->genericmsg.msgdata1, msg->genericmsg.msgdata2);
		}
		break;
		//ͼƬϢ
		case WM_SendImageMessage:
		{
			SendImageMessage(msg->genericmsg.msgdata1, msg->genericmsg.msgdata2);
		}
		break;
		//ȡϢ
		case WM_GetInformation:
		{
			GetInformation();
		}
		break;
		//Ⱥ
		case WM_SetRoomAnnouncement:
		{
			SetWxRoomAnnouncement(msg->genericmsg.msgdata1, msg->genericmsg.msgdata2);
		}
		break;
		//ɾ
		case WM_DeleteUser:
		{
			DeleteUser((wchar_t*)pCopyData->lpData);
		}
		break;
		//˳Ⱥ
		case WM_QuitChatRoom:
		{
			QuitChatRoom((wchar_t*)pCopyData->lpData);
		}
		break;
		//ȺԱ
		case WM_AddGroupMember:
		{
			AddGroupMember(msg->genericmsg.msgdata1, msg->genericmsg.msgdata2);
		}
		break;
		//Ƭ
		case WM_SendXmlCard:
		{
			SendXmlCard(msg->xmlcardmsg.RecverWxid, msg->xmlcardmsg.SendWxid, msg->xmlcardmsg.NickName);
		}
		break;
		//ʾȺԱ
		case WM_ShowChatRoomMembers:
		{
			ShowChatRoomUser((wchar_t*)pCopyData->lpData);
		}
		break;
		//Ӻ
		case WM_AddUser:
		{
			AddWxUser(msg->genericmsg.msgdata1, msg->genericmsg.msgdata2);
		}
		break;
		//޸Ⱥ
		case WM_SetRoomName:
		{
			SetRoomName(msg->genericmsg.msgdata1, msg->genericmsg.msgdata2);

		}
		break;
		//Զ
		case WM_AutoChat:
		{
			g_AutoChat = TRUE;
		}
		break;
		//ȡԶ
		case WM_CancleAutoChat:
		{
			g_AutoChat = FALSE;
		}
		break;
		//ͰϢ
		case WM_SendAtMsg:
		{
			SendRoomAtMsg(msg->atmsg.chatroomid, msg->atmsg.membermsgdata1, msg->atmsg.membernickname, msg->atmsg.msgmsgdata2);
		}
		break;
		//ɾȺԱ
		case WM_DelRoomMember:
		{
			DelRoomMember(msg->genericmsg.msgdata1, msg->genericmsg.msgdata2);
		}
		break;
		//URL
		case WM_OpenUrl:
		{
			OpenUrl((wchar_t*)pCopyData->lpData);
		}
		break;
		default:
			break;
		}
	}

	return DefWindowProc(hWnd, Message, wParam, lParam);
}




