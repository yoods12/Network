#include <WinSock2.h>
#include <process.h>
#include <iostream>

using namespace std;

unsigned _stdcall f(LPVOID);

CRITICAL_SECTION cs; //application 레벨의 동기화 위한 키
//CRITICAL_SECTION : 다중 스레드 간 동기화를 위한 동기화 객체, 임계영역 설정
//임계영역 : 둘 이상의 스레드가 공유 자원에 접근해야할 때 오직 한 스레드만 접근을 허용해야 하는 경우 사용

int a;

int main()
{
	//키 초기화
	InitializeCriticalSection(&cs); //임계영역 설정

	SYSTEM_INFO sys; //구조체를 선언하고 시스템 정보를 저장할 구조체 생성
	GetSystemInfo(&sys); //시스템 정보를 가져옴, SYSTEM_INFO에 저장
	cout << "Number of core : " << sys.dwNumberOfProcessors << endl;
	//sys.dwNumberOfProccessors : 시스템에 있는 프로세서(코어)의 수를 나타냄

	HANDLE th[30]; //30개의 스레드 핸들을 저장하는 배열
	//HANDLE : 윈도우에서 핸들을 나타내는 데이터 형식, 핸들은 스레드를 식별하고 관리하는데 사용

	for (int i = 0; i < sys.dwNumberOfProcessors; i++)
	{
		th[i] = (HANDLE)_beginthreadex(NULL, 0, &f, NULL, 0, NULL);
	}//스레드를 생성하고 배열 th에 각 스레드의 핸들을 저장

	WaitForMultipleObjects(sys.dwNumberOfProcessors, th, true, INFINITE);
	//WaitForMultipleObjects : 여러개의 핸들을 대기하고, 그 중 어떤핸들이 먼저 상태변화 하면 해당 핸들의 인덱스를 반환
	//DWORD nCount : 대기중인 핸들의 수 (시스템의 프로세스(코어)수 만큼 되어있으므로 모든 스레드의 완료를 대기함)
	//HANDLE* lpHandles : 대기중인 핸들, 코어 수만큼 되어있다, 각 스레드의 완료를 대기
	//BOOL bWaitAll : true이면 모든 핸들의 상태 변화를 대기, false이면 여러 핸들중 하나의 상태변화만 대기
	//DWORD dwMilliseconds : 대기시간 밀리초, 0으로 하면 즉시반환 INFINITE는 무한대기

	//sleep(1000);

	cout << "main end a : " << a << endl;
	DeleteCriticalSection(&cs); //임계영역 해제

	return 0;
}

unsigned __stdcall f(LPVOID arg)
{
	//cout << __FUNCTION__ << endl;
	for (int i = 0; i < 100000;  i++)
	{
		//키 획득
		EnterCriticalSection(&cs); //임계영억 진입
		a++; //공유자원 접근
		LeaveCriticalSection(&cs); //임계영역에서 빠져나옴
	}
	return 0;
}