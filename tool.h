#pragma once
#ifndef TOOL_H
#define TOOL_H

#include <string>
#include <vector>
#include <wtypes.h>

std::wstring string2wstring(std::string str);
std::string wstring2string(std::wstring wstr);
wchar_t* StringToWchar_t(std::string& str);
void Wchar_tToString(std::string& szDst, wchar_t* wchar);

BOOL ���Ƶ����а�(const char* data);

//--------------------------
// ��pattern�ָ�str
//--------------------------
std::vector<std::string> split(std::string str, std::string pattern);

//--------------------------
// ��ȡ����ת��Ϊ������ʾ
//--------------------------
std::string get_last_error(DWORD errCode = GetLastError());

//---------------------------------------------------------------
//function:  ��ȡע�����ָ���ļ�ֵ
//Parameter: [in] int nKeyType - ע���������ͣ�����Ĳ���ֻ������������ֵ��
//	0:HKEY_CLASSES_ROOT           1:HKEY_CURRENT_USER
//	2:HKEY_LOCAL_MACHINE          3:HKEY_USERS
//	4:HKEY_PERFORMANCE_DATA       5:HKEY_CURRENT_CONFIG
//	6:HKEY_DYN_DATA               7:HKEY_CURRENT_USER_LOCAL_SETTINGS
//	8:HKEY_PERFORMANCE_TEXT       9:HKEY_PERFORMANCE_NLSTEXT
//          [in] const std::string & strUrl - Ҫ���� �ļ���·��
//          [in] const std::string & strKey - ָ���ļ�
//���ӣ�GetRegValue(2, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString");
//---------------------------------------------------------------


enum ע������� {
	E_HKEY_CLASSES_ROOT = 0,
	E_HKEY_CURRENT_USER,
	E_HKEY_LOCAL_MACHINE,
	E_HKEY_USERS,
	E_HKEY_PERFORMANCE_DATA,
	E_HKEY_CURRENT_CONFIG,
	E_HKEY_DYN_DATA,
	E_HKEY_CURRENT_USER_LOCAL_SETTINGS,
	E_HKEY_PERFORMANCE_TEXT,
	E_HKEY_PERFORMANCE_NLSTEXT,
};

/// ע������ͣ�
/// 0:HKEY_CLASSES_ROOT
/// 1:HKEY_CURRENT_USER
/// 2:HKEY_LOCAL_MACHINE
/// 3:HKEY_USERS
/// 4:HKEY_PERFORMANCE_DATA
/// 5:HKEY_CURRENT_CONFIG
/// 6:HKEY_DYN_DATA
/// 7:HKEY_CURRENT_USER_LOCAL_SETTINGS
/// 8:HKEY_PERFORMANCE_TEXT
/// 9:HKEY_PERFORMANCE_NLSTEXT

/// <summary>
///	ע�����������E_HKEY_��ͷ
/// </summary>
/// <param name="nKeyType">ע�������</param>
/// <param name="strUrl">����·��</param>
/// <param name="strKey">��ֵ</param>
/// <returns></returns>
std::string GetRegValue(int nKeyType, const std::string& strUrl, const std::string& strKey);
#endif // !TOOL_H
