// example.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "example.h"
#include "obj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

dmsoft * g_dm = NULL;

// 唯一的应用程序对象

CWinApp theApp;

using namespace std;


void TestMain(void)
{
	dmsoft dm;

	::MessageBox(NULL,dm.Ver(),_T("主线程"),MB_OK);
}



dmsoft * g_thread_dm = NULL;
HANDLE   g_thread_handle = NULL;

unsigned WINAPI ThreadEntry(LPVOID pParam)
{
	dmsoft & dm = *(dmsoft *)pParam;

	// 初始化COM(mta)
	CoInitializeEx(NULL,0);

	while (1)
	{
		_tprintf(_T("子线程,版本号:%s\n"),dm.Ver());
		dm.delay(500);
	}


	return 0;
}

void TestThread(void)
{
	g_thread_dm = new dmsoft;

	if (g_thread_dm == NULL)
	{
		return;
	}

	g_thread_handle = (HANDLE)_beginthreadex(NULL, 0, ThreadEntry, (PVOID)g_thread_dm, 0, 0);

	if (g_thread_handle == NULL)
	{
		delete g_thread_dm;
		g_thread_dm = NULL;

		return;
	}
	
	// 等待4秒
	g_thread_dm->delay(4000);


	// 先解绑,避免结束线程时影响到目标窗口
	g_thread_dm->UnBindWindow();
	TerminateThread(g_thread_handle,0);
	CloseHandle(g_thread_handle);
	g_thread_handle = NULL;
	delete g_thread_dm;
	g_thread_dm = NULL;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
	
	// 初始化COM(mta)
	CoInitializeEx(NULL,0);
	
	// 设置本地字符集为gbk
	setlocale(LC_ALL, "chs");

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。

		// 创建对象
		g_dm = new dmsoft;
		if (g_dm == NULL)
		{
			return 1;
		}
		
		// 注册
		long dm_ret = g_dm->Reg(_T("注册码"),_T(""));
		if (dm_ret != 1)
		{
			_tprintf(_T("注册失败:%d\n"),dm_ret);
			delete g_dm;
			return 1;
		}

		// 接下来可以做一些全局性的设置,比如加载保护盾，设置共享字库等等

		
		// 主线程调用
		TestMain();

		// 子线程调用
		TestThread();

		delete g_dm;
	}

	return nRetCode;
}
