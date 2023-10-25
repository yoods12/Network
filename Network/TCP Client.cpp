#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

using namespace std;

void err_display(const char*);

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
	SOCKET c_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (c_sock == INVALID_SOCKET)
	{
		err_display("socket error");
		return -1;
	}

	//서버와 연결 (server ip : 127.0.0.1)
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	if (!InetPton(AF_INET, "127.0.0.1", &saddr.sin_addr))
	{
		err_display("InetPton");
		return -1;
	}

	if (connect(c_sock, (SOCKADDR*)&saddr, sizeof(saddr)))
	{
		err_display("InetPton");
		return -1;
	}
	//connect : 클라이언트가 서버에 연결 할 때 씀
	//SOCKET s : 연결을 시도할 소켓, 클라이언트 소켓
	//const struct sockaddr *name : 연결할 서버의 주소정보를 나타내는 구조체의 포인터, 서버의 IP와 포트번호가 구조체에 포함되어 있어야함
	//int namelen : 구조체의 크기

	//echo service request
	char buf[80];
	int sendbyte;
	while (fgets(buf, 79, stdin))
	{
		//fgets : 문자열 입력 받는 함수
		//char *str : 입력된 문자열을 저장할 버퍼를 가리키는 포인터, 문자열을 버퍼에 저장
		//int count : 읽을 문자의 최대 개수
		//FILE *stream : 문자열을 읽어올 파일 스트림을 가리키는 포인터, stdin을 사용하여 키보드로 입력 받음
		sendbyte = strlen(buf);
		send(c_sock, buf, sendbyte - 1, 0);

		//서버로 부터 데이터 수신
		sendbyte = recv(c_sock, buf, 79, 0);
		buf[sendbyte] = '\0';
		cout << "From server : " << buf << endl;

	}


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