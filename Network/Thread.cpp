#include <WinSock2.h>
#include <process.h>
#include <iostream>

using namespace std;

void err_display(const char*);
unsigned __stdcall echo_serc(LPVOID);

int main()
{
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		err_display("WSAStartup");
		return -1;
	}

	//socket 생성
	SOCKET s_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s_sock == INVALID_SOCKET)
	{
		err_display("socket");
		return -1;
	}

	//bind
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s_sock, (SOCKADDR*)&saddr, sizeof(saddr)))
	{
		err_display("bind");
		return -1;
	}

	//listen
	if (listen(s_sock, SOMAXCONN))
	{
		err_display("listen");
		return -1;
	}

	SOCKADDR_IN caddr;
	int addrlen = sizeof(caddr);
	SOCKET c_sock;
	HANDLE th;
	while (true)
	{
		c_sock = accept(s_sock, (SOCKADDR*)&caddr, &addrlen);
		if(c_sock==INVALID_SOCKET)
		{
			err_display("accept");
			break;
		}
		//thread 생성
		th = (HANDLE)_beginthreadex(NULL, 0, &echo_serv, (void*)c_sock, 0, NULL);

		//_beginthreadex : 스레드를 생성하고 시작하는 함수
		//void* security : 스레드 보안 속성 (보통 NULL)
		//unsigned stack_size : 스레드 스택 크기 (0이면 기본 스택크기)
		//__stdcall start_address : 스레드가 시작될 함수의 포인터
		//void* arglist : 스레드 실행함수에 전달될 인수를 지정하는 포인터
		//initflag : 스레드 초기화 플레그 (0이면 기본값)
		//threadaddr : 스레드 ID를 저장할 변수의 포인터, 스레드 ID를 얻을 필요가 없으면 NULL

	}

	WaitForSingleObject(th, INFINITE);

	//WaitForSingleObject : 스레드 또는 다른 윈도우의 개체의 상태를 모니터링 하고, 해당 개체의 상태가 원하는 상태로 변화할 때 까지 기다리는데 사용
	//HANDLE hHandle : 모니터링할 개체의 핸들 (스레드, 이벤트, 뮤텍스, 세마포어, 파일 핸들등 다양한 윈도우 개체를 나타낼 수 있음)
	//DWORD dwMilliseconds : 대기시간 밀리초, 0보다 큰 정수 또는 INFINITE(무한대기)

	closesocket(s_sock);
	WSACleanup();

	return 0;
}


void err_display(const char* mes)
{
	char* outmes = nullptr;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&outmes, 0, NULL);
	cout << mes << " : " << outmes << endl;
}

unsigned __stdcall echo_serv(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;

	char buf[80];
	int retval;

	while (true)
	{
		retval = recv(sock, buf, 79, 0);
		if (retval == 0 || retval == SOCKET_ERROR)
		{
			err_display("recv");
			break;
		}
		buf[retval] = '\0';
		cout << "From client : " << buf << endl;
		send(sock, buf, retval, 0);
	}
	closesocket(sock);
	return 0;
}