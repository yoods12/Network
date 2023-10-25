#include <WinSock2.h>
#include <process.h>
#include <iostream>

using namespace std;

unsigned _stdcall f(LPVOID);

CRITICAL_SECTION cs; //application ������ ����ȭ ���� Ű
//CRITICAL_SECTION : ���� ������ �� ����ȭ�� ���� ����ȭ ��ü, �Ӱ迵�� ����
//�Ӱ迵�� : �� �̻��� �����尡 ���� �ڿ��� �����ؾ��� �� ���� �� �����常 ������ ����ؾ� �ϴ� ��� ���

int a;

int main()
{
	//Ű �ʱ�ȭ
	InitializeCriticalSection(&cs); //�Ӱ迵�� ����

	SYSTEM_INFO sys; //����ü�� �����ϰ� �ý��� ������ ������ ����ü ����
	GetSystemInfo(&sys); //�ý��� ������ ������, SYSTEM_INFO�� ����
	cout << "Number of core : " << sys.dwNumberOfProcessors << endl;
	//sys.dwNumberOfProccessors : �ý��ۿ� �ִ� ���μ���(�ھ�)�� ���� ��Ÿ��

	HANDLE th[30]; //30���� ������ �ڵ��� �����ϴ� �迭
	//HANDLE : �����쿡�� �ڵ��� ��Ÿ���� ������ ����, �ڵ��� �����带 �ĺ��ϰ� �����ϴµ� ���

	for (int i = 0; i < sys.dwNumberOfProcessors; i++)
	{
		th[i] = (HANDLE)_beginthreadex(NULL, 0, &f, NULL, 0, NULL);
	}//�����带 �����ϰ� �迭 th�� �� �������� �ڵ��� ����

	WaitForMultipleObjects(sys.dwNumberOfProcessors, th, true, INFINITE);
	//WaitForMultipleObjects : �������� �ڵ��� ����ϰ�, �� �� ��ڵ��� ���� ���º�ȭ �ϸ� �ش� �ڵ��� �ε����� ��ȯ
	//DWORD nCount : ������� �ڵ��� �� (�ý����� ���μ���(�ھ�)�� ��ŭ �Ǿ������Ƿ� ��� �������� �ϷḦ �����)
	//HANDLE* lpHandles : ������� �ڵ�, �ھ� ����ŭ �Ǿ��ִ�, �� �������� �ϷḦ ���
	//BOOL bWaitAll : true�̸� ��� �ڵ��� ���� ��ȭ�� ���, false�̸� ���� �ڵ��� �ϳ��� ���º�ȭ�� ���
	//DWORD dwMilliseconds : ���ð� �и���, 0���� �ϸ� ��ù�ȯ INFINITE�� ���Ѵ��

	//sleep(1000);

	cout << "main end a : " << a << endl;
	DeleteCriticalSection(&cs); //�Ӱ迵�� ����

	return 0;
}

unsigned __stdcall f(LPVOID arg)
{
	//cout << __FUNCTION__ << endl;
	for (int i = 0; i < 100000;  i++)
	{
		//Ű ȹ��
		EnterCriticalSection(&cs); //�Ӱ迵�� ����
		a++; //�����ڿ� ����
		LeaveCriticalSection(&cs); //�Ӱ迵������ ��������
	}
	return 0;
}