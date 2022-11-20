//#include "stdafx.h"
#include "releaseHelper.h"
#include <cstdio>
#include <string.h>
#include <direct.h>
#include <exception>
#include <iostream>
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

CReleaseDLL::CReleaseDLL() {
	this->m_hModule = GetSelfModuleHandle();
	if (m_hModule == NULL) {
		throw std::exception("Error:获取基址失败");
	}
	//获取目录
	memset(this->m_filePath, 0, MAX_DLL_PATH);
	auto a = _getcwd(this->m_filePath, MAX_DLL_PATH);
}

CReleaseDLL::~CReleaseDLL() {
	for (size_t i = 0; i < files.size(); i++) {
		if (::PathFileExistsA(files[i].c_str())) {
			if (::DeleteFileA(files[i].c_str())) {
				std::cout << "SUCCESS:\tDeleteFile\t" << files[i].c_str() << std::endl;
			} else {
				std::cout << "ERROR:\tDeleteFile\t" << files[i].c_str() << std::endl;
			}
		}
	}
}

bool CReleaseDLL::FreeResFile(unsigned long m_lResourceID, const char* m_strResourceType, const char* m_strFileName) {
	//构造完整释放文件路径
	char strFullPath[MAX_DLL_PATH] = { 0 };
	sprintf_s(strFullPath, "%s\\%s", this->m_filePath, m_strFileName);
	//查找资源
	HRSRC hResID = ::FindResourceA(this->m_hModule, MAKEINTRESOURCEA(m_lResourceID), m_strResourceType);
	if (hResID == NULL) {
		std::cout << "ERROR:\thResID\t" << get_last_error() << std::endl;
		return FALSE;
	}
	//加载资源  
	HGLOBAL hRes = ::LoadResource(this->m_hModule, hResID);
	if (hRes == NULL) {
		std::cout << "ERROR:\thRes\t" << get_last_error() << std::endl;
		return FALSE;
	}
	//锁定资源
	LPVOID pRes = ::LockResource(hRes);
	if (pRes == NULL) {
		std::cout << "ERROR:\tpRes\t" << get_last_error() << std::endl;
		return FALSE;
	}
	//得到待释放资源文件大小 
	unsigned long dwResSize = ::SizeofResource(this->m_hModule, hResID);
	//创建文件 
	HANDLE hResFile = CreateFileA(strFullPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hResFile) {
		std::cout << "ERROR:\thResFile\t" << get_last_error() << std::endl;
		return FALSE;
	}
	DWORD dwWrited = 0;
	if (FALSE == WriteFile(hResFile, pRes, dwResSize, &dwWrited, NULL)) {
		std::cout << "ERROR:\tWriteFile\t" << get_last_error() << std::endl;
		return FALSE;
	}

	CloseHandle(hResFile);
	UnlockResource(hRes);
	FreeResource(hRes);
	SetFileAttributesA(strFullPath, FILE_ATTRIBUTE_HIDDEN);
	files.push_back(strFullPath);
	return TRUE;
}

HMODULE CReleaseDLL::GetSelfModuleHandle() {
	try {
#ifdef _USER_RELEASEDLL_
		//如果释放的帮助类定义在DLL中，将调用下面的方式获取基址
		MEMORY_BASIC_INFORMATION mbi;
		return ((::VirtualQuery((LPCVOID)&CReleaseDLL::GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0)
			? (HMODULE)mbi.AllocationBase : NULL);
#else
		//如果直接定义在exe本身的代码中
		return ::GetModuleHandle(NULL);
#endif
	} catch (...) {
		return NULL;
	}

}
