#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

using namespace std;

void err_display(const char*);
void PrintIP(LPVOID);

int main()
{
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) // != 0
	{
		err_display("WSAStartup error");
		return -1;
	}

	//소켓 생성
	SOCKET gate_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (gate_sock == INVALID_SOCKET)
	{
		err_display("socket error");
		return -1;
	}

	//소켓 ip+port 지정
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//sin_addr : SOCKADDR_IN 에서 IP 주소를 저장하는 필드
	//s_addr : in_addr 구조체에서 실제 IP주소를 저장하는 필드
	//htonl(INADDR_ANY) : INADDR_ANY 라는 특별한 IP주소를 host byte order에서 network byte order로 변환
	//INADDR_ANT : 서버 소켓이 모든 네트워크 인터페이스에서 들어오는 연결을 수락하기 위한 특별한 IP주소

	if (bind(gate_sock, (SOCKADDR*)&saddr, sizeof(saddr)))
	{
		err_display("bind error");
		return -1;
	}

	//bind : 소켓을 IP와 포트 번호에 바인딩하여 해당 주소로 들어오는 연결을 수신 할 수 있게 만듬
	//SOCKET s : 바인딩 할 소켓
	//const struct sockaddr *name : 바인딩 할 주소정보, SOCKADDR 구조체를 사용하여 IP와 포트번호 지정
	//int namelen : 주소 정보의 크기 
	//성공 0, 실패 SOCKET_ERROR


	if (listen(gate_sock, SOMAXCONN))
	{
		err_display("listen error");
		return -1;
	}

	//listen : 서버 소켓을 대기 상태로 만들어 클라이언트의 연결 요청을 수락할 준비를 함
	//SOCKET s : 대기상태로 전환 할 소켓의 핸들
	//int backlog : 대기열의 크기 SOMAXCONN와 같은 상수로 설정하여 시스템이 지원하는 최대 연결 수를 사용

	SOCKADDR_IN caddr; //클라이언트 소켓의 주소정보를 저장할 구조체
	int addrlen = sizeof(caddr);
	SOCKET c_sock = accept(gate_sock, (SOCKADDR*)&caddr, &addrlen);

	//SOCKET s : 수신 대기 상태의 소켓, 클라이언트의 연결 요청을 받아들이기 위한 소켓
	//struct sockaddr *addr : 클라이언트의 주소정보를 저장할 구조체 포인터
	//int *addrlen : 클라이언트 주소정보의 크기
	//성공 SOCKET타입반환, 실패 INVALID_SOCKET

	if (c_sock == INVALID_SOCKET)
	{
		err_display("accept error");
		return -1;
	}

	//서비스할 클라이언트의 정보 : IP, port
	cout << "Client connected : ";
	PrintIP(&caddr.sin_addr);
	cout << "(" << ntohs(caddr.sin_port) << ")" << endl;

	//echo service

	char buf[80];
	int ret;
	while (true)
	{
		//클라이언트 데이터 수신
		ret = recv(c_sock, buf, 79, 0); //맨 마지막에 null 넣어야해서 79
		//SOCKET s : 데이터를 수신할 소켓, 클라이언트와 통신할 때 사용
		//char *buf : 수신한 데이터를 저장할 버퍼
		//int len : 버퍼의 크기
		//int flags : 수신동작에 대한 옵션 
		//성공 수신된 바이트 수 반환, 실패 0

		if (ret == 0) //클라이언트가 closesocket() 호출한 케이스
		{
			cout << "graceful closing case" << endl;
			err_display("recv");
			break;
		}
		if (ret == SOCKET_ERROR) //클라이언트가 rst(reset signal) 송출
		{
			cout << "abnormal closing case" << endl;
			err_display("recv");
			break;
		}

		//정상적인 에코 서비스
		buf[ret] = '\0';
		cout << "From client : " << buf << endl;

		send(c_sock, buf, ret, 0);
		//SOCKET s : 데이터를 보낼 소켓, 클라이언트와 통신할 때 사용
		//const char *buf : 보낼 데이터가 저장된 버퍼
		//int len : 보낼 데이터의 크기
		//int flags : 데이터 송신동작 옵션

	}
	closesocket(c_sock);
	closesocket(gate_sock);
	WSACleanup();

	return 0;
}

void err_display(const char* mes)
{
	LPVOID lpbuffer = nullptr; //void * lpbuffer

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, 10054, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpbuffer, 0, nullptr);

	cout << mes << ":" << (char*)lpbuffer << endl;
	LocalFree(lpbuffer);

}

void PrintIP(LPVOID addr)
{
	char IPbuf[20];
	if (!InetNtop(AF_INET, addr, IPbuf, 20))
	{
		err_display("Inettop");
		return;
	}
	cout << IPbuf;
}