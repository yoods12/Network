#include <WinSock2.h>
#include <process.h>
#include <iostream>

using namespace std;

unsigned _stdcall f(LPVOID);

HANDLE hmutex; //뮤텍스 핸들값 지정
//뮤텍스를 이용한 다중 스레드간 공유 자원에 접근
//뮤텍스는 다중 스레드 환경에서 공유 자원에 대한 동기화를 제어 하는데 사용

int a;

int main()
{
	hmutex = CreateMutex(NULL, false, NULL);
	//CreateMutex : 뮤텍스를 생성하고 핸들을 반환
	//LPSECURITY_ATTRIBUTES lpMutexAttributes : 보안특성 (NULL이면 기본 보안특성)
	//BOOL bInitialOwner : 뮤텍스를 초기화한 스레드가 뮤텍스의 소유자가 되도록 할지 여부, true 이면 뮤텍스를 초기화한 스레드가 소유자 false이면 초기 소유자 없이 뮤텍스 생성
	//LPCSTR lpName : 뮤텍스의 이름을 지정, NULL로 설정하여 로컬 뮤텍스를 생성

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
		//키 획득
		WaitForSingleObject(hmutex, INFINITE); //hmutex의 소유권 획득
		a++;
		//키 반납
		ReleaseMutex(hmutex);
		//ReleaseMutex : 뮤텍스의 소유권을 반환
		//HANDLE hMutex : 뮤텍스 핸들, WaitForSingleObject 함수를 사용하여 뮤텍스를 획득한 후 해당 뮤텍스의 핸들을 받아서 사용
	}
}