#pragma comment (lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>

using namespace std;

void err_display(const char*);

int main()
{
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		err_display("WSAStartup error");
		return -1;
	}

	//TCP socket 생성
	SOCKET sock1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//int af : 주소체계 AF_INET(IPv4), AF_INET6(IPv6)를 사용
	
	//int type : 소켓타입 SOCK_STREAM(TCP 소켓), SOCK_DRAM(UDP 소켓)

	//int protocol : 사용할 프로토콜 0을 사용하면 기본프로토콜 사용하도록 위임

	//성공 : 새로운 소켓 , 실패 : INVALID_SOCKET 반환

	if (sock1 == INVALID_SOCKET)
	{
		err_display("socket tcp error");
		return -1;
	}

	//UDP socket 생성
	SOCKET sock2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock2 == INVALID_SOCKET)
	{
		err_display("socket udp error");
		return -1;
	}

	//각각 생성된 소켓 핸들값을 화면에 출력
	cout << "tcp socket handle : " << sock1 << endl;
	cout << "udp socket handle : " << sock2 << endl;

	//윈속 종료
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