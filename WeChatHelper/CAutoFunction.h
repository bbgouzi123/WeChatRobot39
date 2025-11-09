#pragma once
// #include <atlstr.h>  // 暂时注释掉，如果后续需要 ATL 功能再启用

void AgreeUserRequest(wchar_t* v1, wchar_t* v2);	//ͬ���������
void AutoAgreeUserRequest(wstring msg);	//�Զ�ͬ���������
void CllectMoney(wchar_t* transferid, wchar_t* wxid);	//�տ�
void AutoCllectMoney(wstring msg, wchar_t* wxid);	//�Զ��տ�
void AddCardUser(wchar_t* v1, wchar_t* msg);	//������Ƭ����
void AutoAddCardUser(wstring msg);			//�Զ�������Ƭ����
void CreateDir(const char *dir);		//����Ŀ¼
void ExtractExpression();				//��ȡ����
void HookExtractExpression();//HOOK��ȡ����
void __stdcall OutputExpression(DWORD dwExpressionAddr);	//�������
void CreateFileWithCurrentTime(char* filedir, char* filepostfix, DWORD filedata, DWORD filedatalen);	//�õ�ǰʱ�䴴���ļ�
wstring GetXmlContent(wstring fullxmlcontent, wstring str1, wstring str2);	//��ȡ��Ҫ��XML����
