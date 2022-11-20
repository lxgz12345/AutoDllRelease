#include <Windows.h>
#include <iostream>
#include "mysql.h"
#include "tool.h"
#include "releaseHelper.h"
#include "resource.h"

//typedef ����ֵ���� (WINAPI* ���������)(����);
//��������� ������;
typedef MYSQL* (WINAPI* _mysql_init)(MYSQL* mysql);
_mysql_init MY_mysql_init;
typedef MYSQL* (WINAPI* _mysql_real_connect)(MYSQL* mysql, const char* host, const char* user, const char* passwd, const char* db, unsigned int port, const char* unix_socket, unsigned long clientflag);
_mysql_real_connect MY_mysql_real_connect;
typedef unsigned int (WINAPI* _mysql_errno)(MYSQL* mysql);
_mysql_errno MY_mysql_errno;
typedef int (WINAPI* _mysql_real_query)(MYSQL* mysql, const char* q, unsigned long length);
_mysql_real_query MY_mysql_real_query;
typedef MYSQL_RES* (WINAPI* _mysql_store_result)(MYSQL* mysql);
_mysql_store_result MY_mysql_store_result;
typedef MYSQL_ROW(WINAPI* _mysql_fetch_row)(MYSQL_RES* result);
_mysql_fetch_row MY_mysql_fetch_row;
typedef void (WINAPI* _mysql_free_result)(MYSQL_RES* result);
_mysql_free_result MY_mysql_free_result;
typedef void (WINAPI* _mysql_close)(MYSQL* sock);
_mysql_close MY_mysql_close;

HMODULE g_hDll;

int MYSQL_() {

	///< �������ݿ���
	MYSQL mysql;
	//MYSQL *mysql = mysql_init(nullptr);

	///< ��ʼ�����
	MY_mysql_init(&mysql);

	///< ���ӵ����ݿ⣨��������������û��������롢���ݿ������˿ںš�socketָ�롢��ǣ�
	if (!MY_mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test1", 3306, nullptr, 0)) {
		std::cout << "���ݿ�����ʧ��" << MY_mysql_errno(&mysql) << std::endl;
		return -1;
	}
	std::cout << "���ݿ����ӳɹ�" << std::endl << std::endl;

	///< �������ݿ��Ӧ�ṹ��
	MYSQL_RES* res = nullptr;
	///< ������Ž���Ľṹ��
	MYSQL_ROW row;

	char sql[1024] = { 0 };
	sprintf_s(sql, 1024, "SELECT * FROM `data`");

	///< ���ò�ѯ�ӿ�
	if (MY_mysql_real_query(&mysql, sql, (unsigned int)strlen(sql))) {
		std::cout << "��ѯʧ��" << ": " << MY_mysql_errno(&mysql) << std::endl;
	} else {
		std::cout << "��ѯ�ɹ�" << std::endl << std::endl;

		///< װ�ؽ����
		res = MY_mysql_store_result(&mysql);

		if (nullptr == res) {
			std::cout << "װ������ʧ��" << ": " << MY_mysql_errno(&mysql) << std::endl;
		} else {
			///< ȡ�������������
			while (row = MY_mysql_fetch_row(res)) {
				std::cout << row[0] << "  " << row[1] << std::endl;
			}
		}
	}

	///< �ͷŽ����
	MY_mysql_free_result(res);

	///< �ر����ݿ�����
	MY_mysql_close(&mysql);

	system("pause");

	return 0;
}

int main() {
	//�ͷ�dll
	CReleaseDLL releasehelper;
	bool blRes = releasehelper.FreeResFile(IDR_DLL1, "DLL", "libmysql.dll");
	if (!blRes) {
		std::cout << "ERROR:\t \"libmysql.dll\"�ͷ�ʧ��\t" << get_last_error() << std::endl;
		return 1;
	}
	//����dll
	g_hDll = LoadLibrary(L"libmysql.dll");
	if (!g_hDll) {
		std::cout << "ERROR:\tLoadLibrary\"libmysql.dll\"\t" << get_last_error() << std::endl;
		return 1;
	}
	//��ȡdll�еĺ�����ַ����ֵ���Զ���ĺ�����
	MY_mysql_close			= (_mysql_close)		GetProcAddress(g_hDll, "mysql_close"		);
	MY_mysql_fetch_row		= (_mysql_fetch_row)	GetProcAddress(g_hDll, "mysql_fetch_row"	);
	MY_mysql_free_result	= (_mysql_free_result)	GetProcAddress(g_hDll, "mysql_free_result"	);
	MY_mysql_store_result	= (_mysql_store_result)	GetProcAddress(g_hDll, "mysql_store_result"	);
	MY_mysql_real_query		= (_mysql_real_query)	GetProcAddress(g_hDll, "mysql_real_query"	);
	MY_mysql_real_connect	= (_mysql_real_connect)	GetProcAddress(g_hDll, "mysql_real_connect"	);
	MY_mysql_init			= (_mysql_init)			GetProcAddress(g_hDll, "mysql_init"			);
	MY_mysql_errno			= (_mysql_errno)		GetProcAddress(g_hDll, "mysql_errno"		);

	//����һЩmysql����
	int ret = MYSQL_();

	//�ͷ��ں˾���������ڴ�й¶
	FreeLibrary(g_hDll);
	return ret;


}