#include <WinSock2.h>
#include <iostream>
#include < process.h>

using namespace std;

#define NumOfThread 4
HANDLE hevent; //�̺�Ʈ ��ü, �����尣 ��� �� ����ȭ�� ���� ��� 
//ex)������� �̺�Ʈ ��ü�� ����ϰ�, �ٸ� ������� �ش� �̺�Ʈ�� �ñ׳� ���·� �����Ͽ� ������� �ٸ� �����带 ���� �� ����
HANDLE th[4]; //������ �ڵ�, �����带 �����ϰ� �����ϱ� ���� ���

void CreateEventandThread();
unsigned __stdcall ThreadProc(LPVOID);

int main()
{
	//���ν������� ������Id ���
	cout << "main thread id : " << GetCurrentThreadId() << "start" << endl;

	//Create Event & Thread : Event(manual reset, non-signalled)
	//Thread 4, arg NULL
	CreateEventandThread(); //�̺�Ʈ ����, ������ ����

	Sleep(2000); //2�� ���� ���

	SetEvent(hevent);
	//SetEvent : �̺�Ʈ ��ü�� �ñ׳� ���·� �����Ͽ� �ٸ� �����峪 ���μ����� �̺�Ʈ�� �߻������� �˸�
	//WaitForSingleObject �Լ��� ����Ͽ� �ش� �̺�Ʈ�� ��ٸ��� �ִ� ��� ������ ������ �簳�ϵ��� ��
	//HANDLE hEvent : �ñ׳� ���·� ������ �̺�Ʈ ��ü

	WaitForMultipleObjects(4, th, true, INFINITE);
	//4���� �����尡 ��� ����� ������ ��ٸ�, true�� ����Ͽ� ��� �����尡 ����Ǿ���ϰ�, INFINITE�� ����Ͽ� ������ ���

	cout << "main thread id : " << GetCurrentThreadId() << " end" << endl;

	return 0;
}

void CreateEventandThread() //�̺�Ʈ ��ü�� �����ϰ� �����带 �����ϴ� ����
{
	//CreateEvent (manual reset, non-sig
	hevent = CreateEvent(NULL, true, false, NULL);
	//CreateEvent : �̺�Ʈ ��ü ����
	//LPSECURITY_ATTRIBUTES lpEventAttributes : ���ȼӼ� (NULL �⺻��)
	//BOOL bManualReset	: �̺�Ʈ ��ü�� ������������ �ڵ��������� ���� (true ��������, false �ڵ�����)
	//BOOL bInitialState : �̺�Ʈ ��ü�� �ʱ���� ���� (true �ñ׳λ��·� ����, false ��ñ׳λ��·� ����)
	//LPCTSTR lpName : �̺�Ʈ ��ü�� �̸� (NULL �̸����� �̺�Ʈ ��ü ����)

	for (int i = 0; i < 4; i++) //4���� ������ ����, �� �����尡 ThreadProc�Լ��� �����ϰ� ����
	{
		th[i] = (HANDLE)_beginthreadex(NULL, 0, &ThreadProc, NULL, 0, NULL);
	}//���Ŀ� SetEvent(hevent)�� ȣ���ϸ� �̺�Ʈ ��ü�� �ñ׳� ���·� �����ϸ� ����ϰ� �ִ� �����尡 ����
}

unsigned __stdcall ThreadProc(LPVOID arg) //�����尡 �����ϴ� �Լ�, �� �����尡 ����� �� �� �Լ��� ȣ��
{
	cout << "New Thread : " << GetCurrentThreadId() << endl;
	//GetCurrentThreadId : ���� �������� �������� �ĺ��ڸ� ��ȯ
	cout << "Wait until Event Object becom signaled" << endl;
	WaitForSingleObject(hevent, INFINITE);
	cout << "Thread : " << GetCurrentThreadId() << "will ve finished" << endl;

	return 0;
}