#include <WinSock2.h>
#include <iostream>
#include < process.h>

using namespace std;

#define NumOfThread 4
HANDLE hevent; //이벤트 객체, 스레드간 통신 및 동기화를 위해 사용 
//ex)스레드는 이벤트 객체를 대기하고, 다른 스레드는 해당 이벤트를 시그널 상태로 설정하여 대기중인 다른 스레드를 깨울 수 있음
HANDLE th[4]; //스레드 핸들, 스레드를 관리하고 제어하기 위해 사용

void CreateEventandThread();
unsigned __stdcall ThreadProc(LPVOID);

int main()
{
	//메인스레드의 스레드Id 출력
	cout << "main thread id : " << GetCurrentThreadId() << "start" << endl;

	//Create Event & Thread : Event(manual reset, non-signalled)
	//Thread 4, arg NULL
	CreateEventandThread(); //이벤트 생성, 스레드 생성

	Sleep(2000); //2초 동안 대기

	SetEvent(hevent);
	//SetEvent : 이벤트 객체를 시그널 상태로 설정하여 다른 스레드나 프로세스에 이벤트가 발생했음을 알림
	//WaitForSingleObject 함수를 사용하여 해당 이벤트를 기다리고 있는 경우 깨워서 실행을 재개하도록 함
	//HANDLE hEvent : 시그널 상태로 설정할 이벤트 객체

	WaitForMultipleObjects(4, th, true, INFINITE);
	//4개의 스레드가 모두 종료될 때까지 기다림, true을 사용하여 모든 스레드가 종료되어야하고, INFINITE를 사용하여 무한히 대기

	cout << "main thread id : " << GetCurrentThreadId() << " end" << endl;

	return 0;
}

void CreateEventandThread() //이벤트 객체를 생성하고 스레드를 시작하는 역할
{
	//CreateEvent (manual reset, non-sig
	hevent = CreateEvent(NULL, true, false, NULL);
	//CreateEvent : 이벤트 객체 생성
	//LPSECURITY_ATTRIBUTES lpEventAttributes : 보안속성 (NULL 기본값)
	//BOOL bManualReset	: 이벤트 객체가 수동리셋인지 자동리셋인지 설정 (true 수동리셋, false 자동리셋)
	//BOOL bInitialState : 이벤트 객체의 초기상태 지정 (true 시그널상태로 시작, false 비시그널상태로 시작)
	//LPCTSTR lpName : 이벤트 객체의 이름 (NULL 이름없는 이벤트 객체 생성)

	for (int i = 0; i < 4; i++) //4개의 스레드 생성, 각 스레드가 ThreadProc함수를 실행하게 만듬
	{
		th[i] = (HANDLE)_beginthreadex(NULL, 0, &ThreadProc, NULL, 0, NULL);
	}//이후에 SetEvent(hevent)를 호출하면 이벤트 객체를 시그널 상태로 변경하면 대기하고 있는 스레드가 실행
}

unsigned __stdcall ThreadProc(LPVOID arg) //스레드가 실행하는 함수, 각 스레드가 실행될 떄 이 함수를 호출
{
	cout << "New Thread : " << GetCurrentThreadId() << endl;
	//GetCurrentThreadId : 현재 실행중인 스레드의 식별자를 반환
	cout << "Wait until Event Object becom signaled" << endl;
	WaitForSingleObject(hevent, INFINITE);
	cout << "Thread : " << GetCurrentThreadId() << "will ve finished" << endl;

	return 0;
}