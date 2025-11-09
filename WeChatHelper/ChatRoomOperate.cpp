#include "stdafx.h"
#include "ChatRoomOperate.h"
// #include <atlconv.h>  // 暂时注释掉，使用 Windows API 转换函数

//************************************************************
// ��������: SetWxRoomAnnouncement
// ����˵��: ����Ⱥ����
// ��    ��: GuiShou
// ʱ    ��: 2019/7/4
// ��    ��: chartoomwxid ȺID Announcement Ⱥ����
// �� �� ֵ: void 
//************************************************************
void SetWxRoomAnnouncement(wchar_t* chatroomwxid, wchar_t* Announcement)
{
	//��ȡWeChatWin�Ļ�ַ
	DWORD callAdrress = GetWeChatWinBase()+ WxSetRoomAnnouncement;
	//��װ����
	CHAR bufferA[0xD0] = { 0 };
	DWORD* bufA = (DWORD*)& bufferA;

	CHAR buffer[0xD0] = { 0 };
	DWORD* buf = (DWORD*)& buffer;

	buf[0] = (DWORD)chatroomwxid;
	buf[1] = wcslen(chatroomwxid);
	buf[2] = wcslen(chatroomwxid) * 2;
	buf[3] = 0;
	buf[4] = 0;

	buf[0 + 5] = (DWORD)Announcement;
	buf[1 + 5] = wcslen(Announcement);
	buf[2 + 5] = wcslen(Announcement) * 2;
	buf[3 + 5] = 0;
	buf[4 + 5] = 0;

	bufA[0] = (DWORD)& buffer;
	bufA[1] = bufA[0] + 0x60;
	bufA[2] = bufA[0] + 0x60;

	DWORD r_esp = 0;
	__asm
	{
		//�����ջ�Ĵ���
		mov r_esp, esp;
		lea eax, bufferA;
		push eax;
		call callAdrress;

		//�ָ���ջ�Ĵ���
		mov eax, r_esp;
		mov esp, eax;
	}
}



//************************************************************
// ��������: QuitChatRoom
// ����˵��: �˳�Ⱥ��
// ��    ��: GuiShou
// ʱ    ��: 2019/7/9
// ��    ��: chartoomwxid ȺID 
// �� �� ֵ: void 
//************************************************************
void QuitChatRoom(wchar_t* chatroomwxid)
{
	DWORD dwBaseAddress = GetWeChatWinBase();
	DWORD dwCallAddr = dwBaseAddress + WxQuitChatRoom;

	//��������
	GeneralStruct structWxid(chatroomwxid);
	DWORD* asmMsg = (DWORD*)&structWxid.pstr;

	__asm 
	{
		mov ebx, asmMsg;
		push ebx;
		call dwCallAddr;
	}
}


//************************************************************
// ��������: AddGroupMember
// ����˵��: ����Ⱥ��Ա
// ��    ��: GuiShou
// ʱ    ��: 2019/7/9
// ��    ��: chartoomwxid ȺID  wxid ΢��ID
// �� �� ֵ: void 
//************************************************************
void AddGroupMember(wchar_t* chatroomwxid, wchar_t* wxid)
{
	DWORD dwBase = GetWeChatWinBase();
	DWORD dwCallAddr1 = dwBase + WxAddGroupMemberCall1;
	DWORD dwCallAddr2 = dwBase + WxAddGroupMemberCall2;
	DWORD dwCallAddr3 = dwBase + WxAddGroupMemberCall3;
	DWORD dwCallAddr4 = dwBase + WxAddGroupMemberCall4;

	DWORD dwParam = dwBase + WxAddGroupMemberParam1;
	DWORD dwParam2 = dwBase + WxAddGroupMemberParam2;

	//������Ҫ������
	GeneralStruct wxidStruct(wxid);
	RoomIdStruct chatroomStruct = { 0 };
	chatroomStruct.str = chatroomwxid;
	chatroomStruct.strLen = wcslen(chatroomwxid)*2;
	chatroomStruct.maxLen = wcslen(chatroomwxid) * 4;

	char wxidBuff[0xC] = { 0 };
	char tempWxid[0x14] = { 0 };
	char tempBuff[0x14] = { 0 };

	char* pWxid = (char*)&wxidStruct.pstr;
	char* pChatRoom = (char*)&chatroomStruct.fill2;

	__asm {
		pushad;
		lea esi, wxidBuff;
		mov ecx, esi;
		mov eax, pWxid;
		push eax;
		call dwCallAddr1;

		push 0;
		push dwParam;
		lea ecx, tempWxid;
		call dwCallAddr2;

		sub esp, 0x14;
		mov ecx, pChatRoom;
		mov eax, esp;
		push eax;
		call dwCallAddr3;

		push esi;
		mov ecx, dwParam2;
		call dwCallAddr4;
		popad;
	}
}


//************************************************************
// ��������: ShowChatRoomUser
// ����˵��: ��ʾȺ��Ա
// ��    ��: GuiShou
// ʱ    ��: 2019/7/13
// ��    ��: void
// �� �� ֵ: void 
//************************************************************
void ShowChatRoomUser(wchar_t* chatroomwxid)
{
	//׼��������
	DWORD dwWxidArr = 0;	//����΢��ID���ݵĵ�ַ
	char buff[0x164] = { 0 };
	char userListBuff[0x174] = { 0 };
	//��������
	GeneralStruct pWxid(chatroomwxid);
	char* asmWxid = (char *)&pWxid.pstr;

	//����call
	DWORD dwCall1 = GetWeChatWinBase() + WxGetRoomUserWxidCall1;
	DWORD dwCall2 = GetWeChatWinBase()  + WxGetRoomUserWxidCall2;
	DWORD dwCall3 = GetWeChatWinBase()  + WxGetRoomUserWxidCall3;
	DWORD dwCall4 = GetWeChatWinBase()  + WxGetRoomUserWxidCall4;

	//��ȡȺ��Ա
	__asm
	{
		lea ecx, buff[16];
		call dwCall1;
		lea eax, buff[16];
		push eax;
		mov ebx, asmWxid;
		push ebx;
		call dwCall2;
		mov ecx, eax;
		call dwCall3;
		lea eax, buff;
		push eax;
		lea ecx, buff[16];
		call dwCall4;
		mov dwWxidArr, eax;
	}

	//�õ�΢��ID
	wchar_t test[0x100] = { 0 };
	wchar_t tempWxid[0x100] = { 0 };
	char tempWxidA[0x100] = { 0 };
	DWORD userList = *((DWORD *)dwWxidArr);		//userList��������΢��ID�б� 3.1��ASCII��ʽ��΢��ID
	DWORD testTmp = dwWxidArr + 0xB4;
	int Len = *((int *)testTmp);				//ȡ��΢��ID�ĸ���



	for (int i = 0; i < Len; i++)
	{
		DWORD temWxidAdd = userList + (i * 0x18);		//0x18��ÿ��΢��ID�ļ��
		int flags = (int)(*((LPVOID*)(temWxidAdd + 0x14)));
		if (flags == 0xF)
		{
			sprintf_s(tempWxidA, "%s", (char*)temWxidAdd);
		}
		else
		{
			sprintf_s(tempWxidA, "%s", (char*)*((LPVOID *)temWxidAdd));
		}

				// 使用 Windows API 转换函数替代 ATL 转换宏
		int len = MultiByteToWideChar(CP_ACP, 0, tempWxidA, -1, NULL, 0);
		wchar_t* tempWxidW = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, tempWxidA, -1, tempWxidW, len);
		//2.通过微信ID获取群成员信息
		GetUserInfoByWxid(tempWxidW);
		delete[] tempWxidW;
	}


	//���֮������Ϣ
	HWND hWnd = FindWindow(NULL, TEXT("ChatRoomMember"));
	if (hWnd == NULL)
	{
		OutputDebugStringA("δ���ҵ�ChatRoomMember����");
		return;
	}

	COPYDATASTRUCT userinfodata;
	userinfodata.dwData = WM_ShowChatRoomMembersDone;//����һ����ֵ, ������������־��
	userinfodata.cbData = 0;// strlen(szSendBuf);//�����͵����ݵĳ�
	userinfodata.lpData = NULL;// szSendBuf;//�����͵����ݵ���ʼ��ַ(����ΪNULL)
	SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&userinfodata);
}



//************************************************************
// ��������: GetUserInfoByWxid
// ����˵��: ͨ��΢��ID��ȡ�û���Ϣ
// ��    ��: GuiShou
// ʱ    ��: 2019/7/13
// ��    ��: userwxid ΢��ID
// �� �� ֵ: void 
//************************************************************
void GetUserInfoByWxid(wchar_t* userwxid)
{
	DWORD WechatBase = (DWORD)GetModuleHandle(L"WeChatWin.dll");

	DWORD dwCall1 = WechatBase + WxGetUserInfoWithNoNetworkCall1;
	DWORD dwCall2 = WechatBase + WxGetUserInfoWithNoNetworkCall2;
	DWORD dwCall3 = WechatBase + WxGetUserInfoWithNoNetworkCall3;

	char buff[0x508] = { 0 };		
	char * asmHeadBuff = buff;
	char * asmBuff = &buff[0x18];

	GeneralStruct pWxid(userwxid);
	char* asmWxid = (char*)&pWxid.pstr;

	__asm
	{
		pushad;
		mov edi, asmWxid;		//΢��ID�ṹ��	
		mov eax, asmBuff;		//������
		push eax;
		sub esp, 0x14;
		mov ecx, esp;
		push - 0x1;
		mov dword ptr ds : [ecx], 0x0;
		mov dword ptr ds : [ecx + 0x4], 0x0;
		mov dword ptr ds : [ecx + 0x8], 0x0;
		mov dword ptr ds : [ecx + 0xC], 0x0;
		mov dword ptr ds : [ecx + 0x10], 0x0;
		push dword ptr ds : [edi];	//΢��ID
		call dwCall1;				//call1
		call dwCall2;				//call2
		mov eax, asmHeadBuff;
		push eax;
		mov ecx, asmBuff;
		call dwCall3;
		popad
	}



	LPVOID lpWxid = *((LPVOID *)((DWORD)buff + 0x20));				//΢��ID
	LPVOID lpWxcount = *((LPVOID *)((DWORD)buff + 0x34));			//΢���˺�
	LPVOID lpNickName = *((LPVOID *)((DWORD)buff + 0x7C));			//�ǳ�




	//��װ�ṹ��
	UserInfo *userinfo = new UserInfo;
	swprintf_s(userinfo->UserId, L"%s", (wchar_t*)lpWxid);
	swprintf_s(userinfo->UserNickName, L"%s", (wchar_t*)lpNickName);
	swprintf_s(userinfo->UserNumber, L"%s", (wchar_t*)lpWxcount);



	//���͵��ͻ���
	HWND hWnd = FindWindow(NULL, TEXT("ChatRoomMember"));
	if (hWnd == NULL)
	{
		OutputDebugStringA("δ���ҵ�ChatRoomMember����");
		return;
	}


	COPYDATASTRUCT userinfodata;
	userinfodata.dwData = WM_ShowChatRoomMembers;//����һ����ֵ, ������������־��
	userinfodata.cbData = sizeof(UserInfo);// strlen(szSendBuf);//�����͵����ݵĳ�
	userinfodata.lpData = userinfo;// szSendBuf;//�����͵����ݵ���ʼ��ַ(����ΪNULL)
	SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&userinfodata);

	delete userinfo;

}



//************************************************************
// ��������: SetRoomName
// ����˵��: �޸�Ⱥ����
// ��    ��: GuiShou
// ʱ    ��: 2019/7/14
// ��    ��: roomwxid ȺID roomnameȺ����
// �� �� ֵ: void 
//************************************************************
void SetRoomName(wchar_t* roomwxid, wchar_t* roomname)
{
	GeneralStruct pRoomwxid(roomwxid);
	GeneralStruct pRoomname(roomname);
	char* asmWxid = (char*)&pRoomwxid.pstr;
	char* asmRoomname = (char*)&pRoomname.pstr;

	DWORD dwWeChatWinAddr = GetWeChatWinBase();
	DWORD dwCall1 = dwWeChatWinAddr + WxSetRoomName;

	__asm
	{
		mov edx, asmRoomname;
		mov ecx, asmWxid;
		call dwCall1;
	}
}


//************************************************************
// ��������: SendRoomAtMsg
// ����˵��: ���Ͱ�����Ϣ
// ��    ��: GuiShou
// ʱ    ��: 2019/7/26
// ��    ��: chatroomid ȺID memberwxid Ⱥ��Ա΢��ID membernicknameȺ��Ա�ǳ� msg ��Ϣ����
// �� �� ֵ: void 
//************************************************************
void SendRoomAtMsg(wchar_t* chatroomid, wchar_t* memberwxid, wchar_t* membernickname, wchar_t* msg)
{
	//����call��װ���ݸ�ʽ
	DWORD dwDllBase = GetWeChatWinBase();
	DWORD dwPackageMsgDataCall = dwDllBase + WxPackageMsgData;
	DWORD dwSendTextCall = dwDllBase + WxSendMessage;

	//��װ�ı�����
	wchar_t tempmsg[500] = { 0 };
	swprintf_s(tempmsg, L"@%s %s", membernickname, msg);

	//��װ΢��ID���ݽṹ��
	GeneralStruct wxid(memberwxid);
	//��װȺID���ݽṹ��
	GeneralStruct roomid(chatroomid);
	//��װ��Ϣ�ı����ݽṹ��
	GeneralStruct textcontent(tempmsg);
	//0x81C������
	BYTE buff[0x81C] = { 0 };

	//΢��ID���ݽṹָ��
	wchar_t* pWxid = (wchar_t*)&wxid.pstr;
	//ȺID���ݽṹָ��
	wchar_t* pRoomId = (wchar_t*)&roomid.pstr;
	//��Ϣ�ı����ݽṹָ��
	wchar_t* pTextContent = (wchar_t*)&textcontent.pstr;

	//װ���ݽṹ�建����
	char databuff[16] = { 0 };
	//������װ���ݸ�ʽcall
	__asm
	{
		mov eax, pWxid;			//΢��ID�ṹ��
		push eax;
		lea ecx, databuff;
		call dwPackageMsgDataCall;
	}


	//����call���Ͱ�����Ϣ
	__asm
	{
		mov edx, pRoomId;		//ȺID�ṹ��
		lea eax, databuff;		//��װ�����ݽṹ
		push 0x1;
		push eax;
		mov ebx, pTextContent;	//��Ϣ�ı���ָ��
		push ebx;
		lea ecx, buff;			//0x81C������
		call dwSendTextCall;	//������Ϣcall
		add esp, 0xC;
	}
}


//************************************************************
// ��������: DelRoomMember
// ����˵��: ɾ��Ⱥ��Ա
// ��    ��: GuiShou
// ʱ    ��: 2019/7/26
// ��    ��: roomid ȺID memberwxid Ⱥ��Ա΢��ID 
// �� �� ֵ: void 
//************************************************************
void DelRoomMember(wchar_t* roomid, wchar_t* memberwxid)
{
	//�õ�call�����ݵ�ַ
	DWORD dwCall1 = GetWeChatWinBase() + WxDelRoomMemberCall1;
	DWORD dwCall2 = GetWeChatWinBase() + WxDelRoomMemberCall2;
	DWORD dwCall3 = GetWeChatWinBase() + WxDelRoomMemberCall3;

	//��װȺID�ṹ��
	GeneralStruct roomiddata(roomid);

	//��װ΢��ID�ṹ��
	wchar_t wxidbuff[0xD0] = { 0 };
	DWORD* dwBuff = (DWORD*)&wxidbuff;
	dwBuff[0] = (DWORD)memberwxid;
	dwBuff[1] = wcslen(memberwxid);
	dwBuff[2] = wcslen(memberwxid) * 2;
	dwBuff[3] = 0;
	dwBuff[4] = 0;


	wchar_t datatbuffer[0xD0] = { 0 };
	DWORD* dwDatabuf = (DWORD*)&datatbuffer;
	dwDatabuf[0] = (DWORD)& wxidbuff;
	dwDatabuf[1] = dwDatabuf[0] + 0x14;
	dwDatabuf[2] = dwDatabuf[0] + 0x14;


	__asm
	{
		pushad;
		sub esp, 0x14;
		mov ecx, esp;
		lea ebx, roomiddata.pstr;
		push ebx;
		call dwCall1;
		lea edi, datatbuffer;
		push edi;
		call dwCall2;
		mov ecx, eax;
		call dwCall3;
		popad;
	}
}

