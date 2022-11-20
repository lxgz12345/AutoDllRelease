#include "tool.h"
#include <atlconv.h>

std::wstring string2wstring(std::string str) {
	std::wstring result;
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), NULL, 0);
	wchar_t* buffer = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), buffer, len);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

std::string wstring2string(std::wstring wstr) {
	std::string result;
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

std::vector<std::string> split(std::string str, std::string pattern) {
	int pos;
	std::vector<std::string> result;
	str += pattern;
	int size = (int)str.size();
	for (int i = 0; i < size; i++) {
		pos = (int)str.find(pattern, i);
		if (pos < size) {
			std::string s = str.substr(i, static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(pos) - i);
			result.push_back(s);
			i = pos + (int)pattern.size() - 1;
		}
	}
	return result;
}

void Wchar_tToString(std::string& szDst, wchar_t* wchar) {
	wchar_t* wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);
	char* psText;
	psText = new char[dwNum];
	WideCharToMultiByte(CP_ACP, NULL, wText, -1, psText, dwNum, NULL, FALSE);
	szDst = psText;
	delete[]psText;
}

wchar_t* StringToWchar_t(std::string& str) {
	wchar_t* text1 = new wchar_t[str.size() + 1];
	swprintf(text1, str.size() + 1, L"%S ", str.c_str());
	return text1;
}

BOOL 复制到剪切板(const char* data) {
	int contentSize = (int)strlen(data) + 1;
	HGLOBAL hMemory;
	LPTSTR lpMemory;
	if (!OpenClipboard(NULL)) return false;
	if (!EmptyClipboard()) return false;
	if (!(hMemory = GlobalAlloc(GMEM_MOVEABLE, contentSize))) return false;
	if (!(lpMemory = (LPTSTR)GlobalLock(hMemory))) return false;
	memcpy_s(lpMemory, contentSize, data, contentSize);
	GlobalUnlock(hMemory);
	if (!SetClipboardData(CF_TEXT, hMemory)) return false;
	CloseClipboard();
	//printf("已复制结果！\n");
	//MyMessageBox("已复制结果！");
	return true;
}

std::string get_last_error(DWORD errCode) {
	std::string err("");
	if (errCode == 0) errCode = GetLastError();
	LPTSTR lpBuffer = NULL;
	//失败
	if (0 == FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpBuffer, 0, NULL)) {
		/*FormatMessage
		标志位，决定如何说明lpSource参数，dwFlags的低位指定如何处理换行功能在输出缓冲区，也决定最大宽度的格式化输出行,可选参数。
		根据dwFlags标志而定。
		请求的消息的标识符。当dwFlags标志为FORMAT_MESSAGE_FROM_STRING时会被忽略。
		请求的消息的语言标识符。
		接收错误信息描述的缓冲区指针。
		如果FORMAT_MESSAGE_ALLOCATE_BUFFER标志没有被指定，这个参数必须指定为输出缓冲区的大小，如果指定值为0，这个参数指定为分配给输出缓冲区的最小数。
		保存格式化信息中的插入值的一个数组。*/
		char tmp[100] = { 0 };
		sprintf_s(tmp, "{未定义错误描述(%d)}", errCode);
		err = tmp;
	}
	else {
		//成功
		USES_CONVERSION;
		if (lpBuffer != NULL) err = wstring2string(lpBuffer);
		LocalFree(lpBuffer);
	}
	return err;
}


std::string GetRegValue(int nKeyType, const std::string& strUrl, const std::string& strKey) {
	std::string strValue("");
	HKEY hKey = NULL;
	HKEY  hKeyResult = NULL;
	DWORD dwSize = 0;
	DWORD dwDataType = 0;
	std::wstring wstrUrl = string2wstring(strUrl);
	std::wstring wstrKey = string2wstring(strKey);

	switch (nKeyType) {
	case 0: {
		hKey = HKEY_CLASSES_ROOT;
		break;
	}
	case 1: {
		hKey = HKEY_CURRENT_USER;
		break;
	}
	case 2: {
		hKey = HKEY_LOCAL_MACHINE;
		break;
	}
	case 3: {
		hKey = HKEY_USERS;
		break;
	}
	case 4: {
		hKey = HKEY_PERFORMANCE_DATA;
		break;
	}
	case 5: {
		hKey = HKEY_CURRENT_CONFIG;
		break;
	}
	case 6: {
		hKey = HKEY_DYN_DATA;
		break;
	}
	case 7: {
		hKey = HKEY_CURRENT_USER_LOCAL_SETTINGS;
		break;
	}
	case 8: {
		hKey = HKEY_PERFORMANCE_TEXT;
		break;
	}
	case 9: {
		hKey = HKEY_PERFORMANCE_NLSTEXT;
		break;
	}
	default:
		return strValue;
	}
	if (ERROR_SUCCESS == ::RegOpenKeyEx(hKey, wstrUrl.c_str(), 0, KEY_QUERY_VALUE, &hKeyResult)) {
		::RegQueryValueEx(hKeyResult, wstrKey.c_str(), 0, &dwDataType, NULL, &dwSize);
		switch (dwDataType) {
		case REG_MULTI_SZ: {
			BYTE* lpValue = new BYTE[dwSize];
			LONG lRet = ::RegQueryValueEx(hKeyResult, wstrKey.c_str(), 0, &dwDataType, lpValue, &dwSize);
			delete[] lpValue;
			break;
		}
		case REG_SZ: {
			wchar_t* lpValue = new wchar_t[dwSize];
			memset(lpValue, 0, dwSize * sizeof(wchar_t));
			if (ERROR_SUCCESS == ::RegQueryValueEx(hKeyResult, wstrKey.c_str(), 0, &dwDataType, (LPBYTE)lpValue, &dwSize)) {
				std::wstring wstrValue(lpValue);
				strValue = wstring2string(wstrValue);
			}
			delete[] lpValue;
			break;
		}
		default:
			break;
		}
	}
	::RegCloseKey(hKeyResult);
	return strValue;
}
