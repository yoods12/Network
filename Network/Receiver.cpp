#pragma comment (lib,"ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <process.h>
#include <iostream>
#include <locale>

using namespace std;

void err_display(const char*);
unsigned __stdcall echo_serv(LPVOID);

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
	SOCKET udpsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udpsock == INVALID_SOCKET)
	{
		err_display("socket");
		return -1;
	}

	//receiver ip+port 예약
	SOCKADDR_IN raddr;
	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(8000);
	raddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(udpsock, (SOCKADDR*)&raddr, sizeof(raddr)))
	{
		err_display("bind");
		return -1;
	}

	setlocale(LC_ALL, ".utf8");

	//UDP를 이용하여 데이터 수신
	char buf[512]; //수신한 데이터 저장
	SOCKADDR_IN saddr; //송신자의 주소정보
	int addrlen = sizeof(saddr); 
	int retval; //수신한 데이터의 바이트 수를 저장

	while (true)
	{
		retval = recvfrom(udpsock, buf, 511, 0, (SOCKADDR*)&saddr, &addrlen);
		//recvfrom : 소켓을 통해 데이터를 수신할 때 사용
		//SOCKET s : 데이터를 수신할 소켓의 핸들
		//char *buf : 수신한 데이터를 저장할 버퍼
		//int len : 버퍼의 크기, 수신한 데이터가 저장됨
		//int flags : 옵션 플래그, 일반적으로 0 또는 특정 플래그 값
		//struct sockaddr *from : 송신자의 주소정보를 저장할 구조체의 포인터, 송신자의 주소정보를 얻음
		//int *fromlen : 송신자의 주소정보 구조체의 크기
		//성공 받은 바이트 수 반환, 실패 SOCKET_ERROR

		if (retval == 0 || retval == SOCKET_ERROR)
		{
			break;
		}
		buf[retval] = '\0';
		cout << buf;
	}



	closesocket(udpsock);
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