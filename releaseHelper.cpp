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
		throw std::exception("Error:��ȡ��ַʧ��");
	}
	//��ȡĿ¼
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
	//���������ͷ��ļ�·��
	char strFullPath[MAX_DLL_PATH] = { 0 };
	sprintf_s(strFullPath, "%s\\%s", this->m_filePath, m_strFileName);
	//������Դ
	HRSRC hResID = ::FindResourceA(this->m_hModule, MAKEINTRESOURCEA(m_lResourceID), m_strResourceType);
	if (hResID == NULL) {
		std::cout << "ERROR:\thResID\t" << get_last_error() << std::endl;
		return FALSE;
	}
	//������Դ  
	HGLOBAL hRes = ::LoadResource(this->m_hModule, hResID);
	if (hRes == NULL) {
		std::cout << "ERROR:\thRes\t" << get_last_error() << std::endl;
		return FALSE;
	}
	//������Դ
	LPVOID pRes = ::LockResource(hRes);
	if (pRes == NULL) {
		std::cout << "ERROR:\tpRes\t" << get_last_error() << std::endl;
		return FALSE;
	}
	//�õ����ͷ���Դ�ļ���С 
	unsigned long dwResSize = ::SizeofResource(this->m_hModule, hResID);
	//�����ļ� 
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
		//����ͷŵİ����ඨ����DLL�У�����������ķ�ʽ��ȡ��ַ
		MEMORY_BASIC_INFORMATION mbi;
		return ((::VirtualQuery((LPCVOID)&CReleaseDLL::GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0)
			? (HMODULE)mbi.AllocationBase : NULL);
#else
		//���ֱ�Ӷ�����exe����Ĵ�����
		return ::GetModuleHandle(NULL);
#endif
	} catch (...) {
		return NULL;
	}

}
