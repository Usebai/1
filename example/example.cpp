// example.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "example.h"
#include "obj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

dmsoft * g_dm = NULL;

// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;


void TestMain(void)
{
	dmsoft dm;

	::MessageBox(NULL,dm.Ver(),_T("���߳�"),MB_OK);
}



dmsoft * g_thread_dm = NULL;
HANDLE   g_thread_handle = NULL;

unsigned WINAPI ThreadEntry(LPVOID pParam)
{
	dmsoft & dm = *(dmsoft *)pParam;

	// ��ʼ��COM(mta)
	CoInitializeEx(NULL,0);

	while (1)
	{
		_tprintf(_T("���߳�,�汾��:%s\n"),dm.Ver());
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
	
	// �ȴ�4��
	g_thread_dm->delay(4000);


	// �Ƚ��,��������߳�ʱӰ�쵽Ŀ�괰��
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
	
	// ��ʼ��COM(mta)
	CoInitializeEx(NULL,0);
	
	// ���ñ����ַ���Ϊgbk
	setlocale(LC_ALL, "chs");

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣

		// ��������
		g_dm = new dmsoft;
		if (g_dm == NULL)
		{
			return 1;
		}
		
		// ע��
		long dm_ret = g_dm->Reg(_T("ע����"),_T(""));
		if (dm_ret != 1)
		{
			_tprintf(_T("ע��ʧ��:%d\n"),dm_ret);
			delete g_dm;
			return 1;
		}

		// ������������һЩȫ���Ե�����,������ر����ܣ����ù����ֿ�ȵ�

		
		// ���̵߳���
		TestMain();

		// ���̵߳���
		TestThread();

		delete g_dm;
	}

	return nRetCode;
}
