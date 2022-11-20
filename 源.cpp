#include <Windows.h>
#include <iostream>
#include "mysql.h"
#include "tool.h"
#include "releaseHelper.h"
#include "resource.h"

//typedef 返回值类型 (WINAPI* 定义的名称)(参数);
//定义的名称 函数名;
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

	///< 创建数据库句柄
	MYSQL mysql;
	//MYSQL *mysql = mysql_init(nullptr);

	///< 初始化句柄
	MY_mysql_init(&mysql);

	///< 连接的数据库（句柄、主机名、用户名、密码、数据库名、端口号、socket指针、标记）
	if (!MY_mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test1", 3306, nullptr, 0)) {
		std::cout << "数据库连接失败" << MY_mysql_errno(&mysql) << std::endl;
		return -1;
	}
	std::cout << "数据库连接成功" << std::endl << std::endl;

	///< 创建数据库回应结构体
	MYSQL_RES* res = nullptr;
	///< 创建存放结果的结构体
	MYSQL_ROW row;

	char sql[1024] = { 0 };
	sprintf_s(sql, 1024, "SELECT * FROM `data`");

	///< 调用查询接口
	if (MY_mysql_real_query(&mysql, sql, (unsigned int)strlen(sql))) {
		std::cout << "查询失败" << ": " << MY_mysql_errno(&mysql) << std::endl;
	} else {
		std::cout << "查询成功" << std::endl << std::endl;

		///< 装载结果集
		res = MY_mysql_store_result(&mysql);

		if (nullptr == res) {
			std::cout << "装载数据失败" << ": " << MY_mysql_errno(&mysql) << std::endl;
		} else {
			///< 取出结果集中内容
			while (row = MY_mysql_fetch_row(res)) {
				std::cout << row[0] << "  " << row[1] << std::endl;
			}
		}
	}

	///< 释放结果集
	MY_mysql_free_result(res);

	///< 关闭数据库连接
	MY_mysql_close(&mysql);

	system("pause");

	return 0;
}

int main() {
	//释放dll
	CReleaseDLL releasehelper;
	bool blRes = releasehelper.FreeResFile(IDR_DLL1, "DLL", "libmysql.dll");
	if (!blRes) {
		std::cout << "ERROR:\t \"libmysql.dll\"释放失败\t" << get_last_error() << std::endl;
		return 1;
	}
	//加载dll
	g_hDll = LoadLibrary(L"libmysql.dll");
	if (!g_hDll) {
		std::cout << "ERROR:\tLoadLibrary\"libmysql.dll\"\t" << get_last_error() << std::endl;
		return 1;
	}
	//获取dll中的函数地址，赋值给自定义的函数名
	MY_mysql_close			= (_mysql_close)		GetProcAddress(g_hDll, "mysql_close"		);
	MY_mysql_fetch_row		= (_mysql_fetch_row)	GetProcAddress(g_hDll, "mysql_fetch_row"	);
	MY_mysql_free_result	= (_mysql_free_result)	GetProcAddress(g_hDll, "mysql_free_result"	);
	MY_mysql_store_result	= (_mysql_store_result)	GetProcAddress(g_hDll, "mysql_store_result"	);
	MY_mysql_real_query		= (_mysql_real_query)	GetProcAddress(g_hDll, "mysql_real_query"	);
	MY_mysql_real_connect	= (_mysql_real_connect)	GetProcAddress(g_hDll, "mysql_real_connect"	);
	MY_mysql_init			= (_mysql_init)			GetProcAddress(g_hDll, "mysql_init"			);
	MY_mysql_errno			= (_mysql_errno)		GetProcAddress(g_hDll, "mysql_errno"		);

	//运行一些mysql试试
	int ret = MYSQL_();

	//释放内核句柄，避免内存泄露
	FreeLibrary(g_hDll);
	return ret;


}