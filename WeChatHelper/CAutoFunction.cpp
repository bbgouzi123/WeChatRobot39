#include "stdafx.h"
#include "CAutoFunction.h"
#include "Function.h"
// #include <atlconv.h>  // 暂时注释掉，如果后续需要 ATL 功能再启用
#include <direct.h> //_mkdirͷļ
#include <io.h>     //_accessͷļ
#include <iostream>
#include <cstdio>
#include <ctime>
#include "ChatRecord.h"
#include <Shlobj.h>
#include <Shlwapi.h>
// #include <atlconv.h>  // 暂时注释掉，如果后续需要 ATL 功能再启用
#include <wininet.h>
#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"Wininet.lib")


//------------------------------------------------------------------------------------------------
BYTE bjmpcode[5] = { 0 };
DWORD dwReternAddress = (DWORD)GetModuleHandle(L"WeChatWin.dll") + WxGetExpressionsAddr + 5;	//صַ
DWORD dwCallAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll") + WxGetExpressionsCallAddr;


//************************************************************
// : AgreeUserRequest
// ˵: ͬ
//     : GuiShou
// ʱ    : 2019/7/17
//     : v1 v2
//   ֵ: void
//************************************************************
void AgreeUserRequest(wchar_t* v1, wchar_t* v2)
{
	DWORD base = GetWeChatWinBase();
	DWORD callAdd1 = base + WxAgreeUserRequestCall1;
	DWORD callAdd2 = base + WxAgreeUserRequestCall2;
	DWORD callAdd3 = base + WxAgreeUserRequestCall3;
	DWORD callAdd4 = base + WxAgreeUserRequestCall4;
	DWORD params = base + WxAgreeUserRequestParam;

	DWORD* asmP = (DWORD*)params;

	v1Info userInfoV1 = { 0 };
	v2Info userInfoV2 = { 0 };
	userInfoV1.v2 = (DWORD)&userInfoV2.fill;
	userInfoV1.v1 = v1;
	userInfoV1.v1Len = wcslen(v1);
	userInfoV1.maxV1Len = wcslen(v1) * 2;
	userInfoV2.v2 = v2;
	userInfoV2.v2Len = wcslen(v2);
	userInfoV2.maxV2Len = wcslen(v2) * 2;

	char* asmUser = (char*)&userInfoV1.fill;
	char buff[0x14] = { 0 };
	char buff2[0x48] = { 0 };
	char* asmBuff = buff2;

	__asm
	{
		mov ecx, asmUser;
		push 0x11;
		sub esp, 0x14;
		push esp;
		call callAdd1;
		mov ecx, asmUser;
		lea eax, buff;
		push eax;
		call callAdd2;
		mov esi, eax;
		sub esp, 0x8;
		mov ecx, asmP;
		call callAdd3;
		mov ecx, asmBuff;
		mov edx, ecx;
		push edx;
		push eax;
		push esi;
		call callAdd4;
	}
}


//************************************************************
// : AutoAgreeUserRequest
// ˵: Զͬ
//     : GuiShou
// ʱ    : 2019/7/17
//     : msg Ϣ
//   ֵ: void
//************************************************************
void AutoAgreeUserRequest(wstring msg)
{
	int v1strat = msg.find(L"v3_");
	int v1end = msg.find(L"@stranger");
	wstring v1;
	v1 = msg.substr(v1strat, v1end - v1strat + 9);
	//ҵv2
	int v2strat = msg.find(L"v4_");
	int v2end = msg.rfind(L"@stranger");
	wstring v2;
	v2 = msg.substr(v2strat, v2end - v2strat + 9);

	//ͬcall
	AgreeUserRequest((wchar_t*)v1.c_str(), (wchar_t*)v2.c_str());
}


//************************************************************
// : CllectMoney
// ˵: տ
//     : GuiShou
// ʱ    : 2019/7/15
//     : transferid תID wxid ΢ID
//   ֵ: void 
//************************************************************
void CllectMoney(wchar_t* transferid, wchar_t* wxid)
{
	CllectMoneyStruct cllect;
	cllect.ptransferid = transferid;
	cllect.transferidLen = wcslen(transferid) + 1;
	cllect.transferidMaxLen = (wcslen(transferid) + 1) * 2;
	cllect.pwxid = wxid;
	cllect.wxidLen = wcslen(wxid) + 1;
	cllect.wxidMaxLen = (wcslen(wxid) + 1) * 2;

	char* asmBuff = (char*)&cllect.ptransferid;

	DWORD dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	DWORD dwCall1 = dwWeChatWinAddr + WxCllectMoneyCall1;
	DWORD dwCall2 = dwWeChatWinAddr + WxCllectMoneyCall2;

	__asm
	{
		sub esp, 0x30;
		mov ecx, esp;
		mov eax, asmBuff;
		push eax;
		call dwCall1;
		call dwCall2;
		add esp, 0x30;
	}
}


//************************************************************
// : AutoCllectMoney
// ˵: Զտ
//     : GuiShou
// ʱ    : 2019/7/17
//     : msg Ϣ
//   ֵ: void
//************************************************************
void AutoCllectMoney(wstring msg,wchar_t* wxid)
{
	// ҵ<transferid>ַλ
	int pos1 = msg.find(L"<transferid>");
	//ҵ]]></transferid>ַλ
	int pos2 = msg.find(L"]]></transferid>");
	//ȡַ
	wstring noneed = L"<transferid><![CDATA[";
	int noneedLen = noneed.length();
	//ȡתID
	wstring transferid;
	transferid = msg.substr(pos1 + noneedLen, (pos2 - pos1) - noneedLen);

	//տcall ʵԶտ
	CllectMoney((wchar_t*)transferid.c_str(), wxid);
}


//************************************************************
// : AddCardUser
// ˵: Ƭ
//     : GuiShou
// ʱ    : 2019/7/18
//     : v1 msg Ϣ
//   ֵ: void
//************************************************************
void AddCardUser(wchar_t* v1, wchar_t* msg)
{
	DWORD dwWeChatWinAddr = GetWeChatWinBase();
	DWORD dwParam1 = dwWeChatWinAddr + WxAddWxUserParam1;
	DWORD dwCall1 = dwWeChatWinAddr + WxAddWxUserCall1;
	DWORD dwCall2 = dwWeChatWinAddr + WxAddWxUserCall2;
	DWORD dwCall3 = dwWeChatWinAddr + WxAddWxUserCall3;
	DWORD dwCall4 = dwWeChatWinAddr + WxAddWxUserCall4;
	DWORD dwCall5 = dwWeChatWinAddr + WxAddWxUserCall5;

	GeneralStruct pV1(v1);
	char* asmV1 = (char*)&pV1.pstr;
	char buff3[0x100] = { 0 };
	char* buff = buff3;
	__asm
	{
		sub esp, 0x18;
		mov ecx, esp;
		push  dwParam1;
		call dwCall1;
		sub esp, 0x18;
		mov eax, buff;
		mov ecx, esp;
		push eax;
		call dwCall2;
		push 0x11;
		sub esp, 0x14;
		mov ecx, esp;
		push - 0x1;
		mov edi, msg;
		push edi;
		call dwCall3;
		push 0x2;
		sub esp, 0x14;
		mov ecx, esp;
		mov ebx, asmV1;
		push ebx;
		call dwCall4;
		mov ecx, eax;
		call dwCall5;
	}
}



//************************************************************
// : AutoAddCardUser
// ˵: ԶƬ
//     : GuiShou
// ʱ    : 2019/7/18
//     : msg Ϣ
//   ֵ: void
//************************************************************
void AutoAddCardUser(wstring msg)
{
	//õV1
	int v1strat = msg.find(L"v1_");
	int v1end = msg.find(L"@stranger");
	wstring v1;
	v1 = msg.substr(v1strat, v1end - v1strat + 9);

	//ƬѺ
	AddCardUser((wchar_t*)v1.c_str(), (wchar_t*)L"ͨ~ͨ~ ");
}


//************************************************************
// : HookExtractExpression
// ˵: HOOKȡCall
//     : GuiShou
// ʱ    : 2019/7/21
//     : void
//   ֵ: void
//************************************************************
void HookExtractExpression()
{
	HookAnyAddress(GetWeChatWinBase() + WxGetExpressionsAddr, ExtractExpression);
}



//************************************************************
// : ExtractExpression
// ˵: ȡ
//     : GuiShou
// ʱ    : 2019/7/21
//     : void
//   ֵ: void
//************************************************************
void  __declspec(naked) ExtractExpression()
{
	__asm
	{
		pushad;
		push eax;
		call OutputExpression;
		popad;
		//ָǵĴ
		call dwCallAddr;
		//طصַ
		jmp dwReternAddress;
	}
}


//************************************************************
// : OutputExpression
// ˵: 
//     : GuiShou
// ʱ    : 2019/7/21
//     : dwExpressionAddr ַ
//   ֵ: void
//************************************************************
void __stdcall OutputExpression(DWORD dwExpressionAddr)
{
	//ȡͼƬ
	DWORD dwImageLen = *((DWORD*)(dwExpressionAddr + 4));
	//ȡͼƬ
	DWORD dwImage = *((DWORD*)(dwExpressionAddr));
	unsigned char magic_head[4] = { 0 };
	char postfix[5] = { 0 };
	memcpy(magic_head, (void*)dwImage, 3);
	//MAGICͷжϺ׺
	if (magic_head[0] == 137 && magic_head[1] == 80 && magic_head[2] == 78)
	{
		strcpy_s(postfix, 5, ".png");
	}
	else if (magic_head[0] == 71 && magic_head[1] == 73 && magic_head[2] == 70)
	{
		strcpy_s(postfix, 5, ".gif");
	}
	else if (magic_head[0] == 255 && magic_head[1] == 216 && magic_head[2] == 255)
	{
		strcpy_s(postfix, 5, ".jpg");
	}

	//ȡʱļĿ¼
	char temppath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, temppath);
	char imagedir[25] = { "WeChatRecordExpressions" };

	//ƴӴ΢űĿ¼
	char WeChatExpressionsPath[MAX_PATH] = { 0 };
	sprintf_s(WeChatExpressionsPath, "%s%s\\", temppath, imagedir);
	//Ŀ¼ͼƬ
	CreateDir(WeChatExpressionsPath);

	//ͼƬ
	CreateFileWithCurrentTime(WeChatExpressionsPath, postfix, dwImage, dwImageLen);
}






//************************************************************
// : CreateFileWithCurrentTime
// ˵: ݵǰʱ䴴ļ
//     : GuiShou
// ʱ    : 2019/9/24
//     : filedir ļĿ¼ filepostfixļ׺ filedata ļʼַ filedatalenļ 
//   ֵ: void 
//************************************************************
void CreateFileWithCurrentTime(char* filedir, char* filepostfix, DWORD filedata, DWORD filedatalen)
{
	//ȡǰʱΪļ
	time_t rawtime;
	struct tm *ptminfo = new struct tm;
	time(&rawtime);
	localtime_s(ptminfo, &rawtime);
	char currenttime[30] = { 0 };
	sprintf_s(currenttime, "%02d%02d%02d%02d%02d%02d", ptminfo->tm_year + 1900,
		ptminfo->tm_mon + 1, ptminfo->tm_mday, ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);

	//ƴ·
	char filepath[MAX_PATH] = { 0 };
	sprintf_s(filepath, "%s%s%s", filedir, currenttime, filepostfix);
	//ļ
	HANDLE hFile = CreateFileA(filepath, GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL, "ļʧ", "", 0);
		return;
	}
	//дļ
	DWORD dwWrite = 0;
	WriteFile(hFile, (LPCVOID)filedata, filedatalen, &dwWrite, 0);
	//رվ
	CloseHandle(hFile);
}


//************************************************************
// : CreateDir
// ˵: Ŀ¼
//     : GuiShou
// ʱ    : 2019/7/21
//     : dir Ŀ¼
//   ֵ: void
//************************************************************
void CreateDir(const char *dir)
{
	int m = 0, n;
	string str1, str2;

	str1 = dir;
	str2 = str1.substr(0, 2);
	str1 = str1.substr(3, str1.size());

	while (m >= 0)
	{
		m = str1.find('\\');
		str2 += '\\' + str1.substr(0, m);
		n = _access(str2.c_str(), 0); //жϸĿ¼Ƿ
		if (n == -1)
		{
			_mkdir(str2.c_str());     //Ŀ¼
		}
		str1 = str1.substr(m + 1, str1.size());
	}
}



//************************************************************
// : GetXmlContent
// ˵: ȡҪXML
//     : GuiShou
// ʱ    : 2019/11/15
//     : fullxmlcontent XML  str1 ҪȡXMLߵַ str2 ҪȡXMLұߵַ
//   ֵ: wstring ҪȡXML
//************************************************************
wstring GetXmlContent(wstring fullxmlcontent, wstring str1, wstring str2)
{
	wstring targetstring;
	// ҵ<title>ַλ
	int pos1 = fullxmlcontent.find(str1);
	//ҵ</title>ַλ
	int pos2 = fullxmlcontent.find(str2);

	if (pos1==wstring::npos|| pos2== wstring::npos)
	{
		return L"NULL";
	}

	//ȡַ
	wstring noneedString1 = str1;
	int noneedLen1 = noneedString1.length();
	targetstring = fullxmlcontent.substr(pos1 + noneedLen1, pos2 - pos1 - noneedLen1);

	return targetstring;
}
