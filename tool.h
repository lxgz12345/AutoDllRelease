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

BOOL 复制到剪切板(const char* data);

//--------------------------
// 用pattern分割str
//--------------------------
std::vector<std::string> split(std::string str, std::string pattern);

//--------------------------
// 获取错误并转换为中文提示
//--------------------------
std::string get_last_error(DWORD errCode = GetLastError());

//---------------------------------------------------------------
//function:  获取注册表中指定的键值
//Parameter: [in] int nKeyType - 注册表项的类型，传入的参数只可能是以下数值：
//	0:HKEY_CLASSES_ROOT           1:HKEY_CURRENT_USER
//	2:HKEY_LOCAL_MACHINE          3:HKEY_USERS
//	4:HKEY_PERFORMANCE_DATA       5:HKEY_CURRENT_CONFIG
//	6:HKEY_DYN_DATA               7:HKEY_CURRENT_USER_LOCAL_SETTINGS
//	8:HKEY_PERFORMANCE_TEXT       9:HKEY_PERFORMANCE_NLSTEXT
//          [in] const std::string & strUrl - 要查找 的键的路径
//          [in] const std::string & strKey - 指定的键
//例子：GetRegValue(2, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString");
//---------------------------------------------------------------


enum 注册表类型 {
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

/// 注册表类型：
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
///	注册表类型是以E_HKEY_开头
/// </summary>
/// <param name="nKeyType">注册表类型</param>
/// <param name="strUrl">键的路径</param>
/// <param name="strKey">键值</param>
/// <returns></returns>
std::string GetRegValue(int nKeyType, const std::string& strUrl, const std::string& strKey);
#endif // !TOOL_H
