#include <WinSock2.h>
#include <process.h>
#include <iostream>

using namespace std;

unsigned _stdcall f(LPVOID);

HANDLE hmutex; //���ؽ� �ڵ鰪 ����
//���ؽ��� �̿��� ���� �����尣 ���� �ڿ��� ����
//���ؽ��� ���� ������ ȯ�濡�� ���� �ڿ��� ���� ����ȭ�� ���� �ϴµ� ���

int a;

int main()
{
	hmutex = CreateMutex(NULL, false, NULL);
	//CreateMutex : ���ؽ��� �����ϰ� �ڵ��� ��ȯ
	//LPSECURITY_ATTRIBUTES lpMutexAttributes : ����Ư�� (NULL�̸� �⺻ ����Ư��)
	//BOOL bInitialOwner : ���ؽ��� �ʱ�ȭ�� �����尡 ���ؽ��� �����ڰ� �ǵ��� ���� ����, true �̸� ���ؽ��� �ʱ�ȭ�� �����尡 ������ false�̸� �ʱ� ������ ���� ���ؽ� ����
	//LPCSTR lpName : ���ؽ��� �̸��� ����, NULL�� �����Ͽ� ���� ���ؽ��� ����

	SYSTEM_INFO sys;
	GetSystemInfo(&sys);
	cout << "Number of core : " << sys.dwNumberOfProcessors << endl;

	HANDLE th[30];

	for (int i = 0; i < sys.dwNumberOfProcessors; i++)
	{
		th[i] = (HANDLE)_beginthreadex(NULL, 0, &f, NULL, 0, NULL);
	}

	WaitForMultipleObjects(sys.dwNumberOfProcessors, th, true, INFINITE);

	//Sleep(1000);
	cout << "main end a : " << a << endl;

	return 0;
}

unsigned _stdcall f(LPVOID arg)
{
	//cout << __FUNCTION__ << endl;
	for (int i = 0; i < 100000; i++)
	{
		//Ű ȹ��
		WaitForSingleObject(hmutex, INFINITE); //hmutex�� ������ ȹ��
		a++;
		//Ű �ݳ�
		ReleaseMutex(hmutex);
		//ReleaseMutex : ���ؽ��� �������� ��ȯ
		//HANDLE hMutex : ���ؽ� �ڵ�, WaitForSingleObject �Լ��� ����Ͽ� ���ؽ��� ȹ���� �� �ش� ���ؽ��� �ڵ��� �޾Ƽ� ���
	}
}