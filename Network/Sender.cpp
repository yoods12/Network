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
		err_display("WSAStartup Error");
		return -1;
	}

	//socket 생성
	SOCKET s_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s_sock == INVALID_SOCKET)
	{
		err_display("Socket Error");
		return -1;
	}

	//broadcast socket ip+port 지정
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	bool flag = true;
	if (setsockopt(s_sock, SOL_SOCKET, SO_BROADCAST, (char*)&flag, sizeof(flag)))
	{
		//setsockopt : 소켓 옵션 설정, 소켓에 대한 옵션을 설정하여 네트워크 통신 동작을 변경하거나 제어
		//SOCET s : 설정할 소켓의 핸들
		//int level : 옵션을 설정할 레벨 (SOL_SOCKET는 일반 소켓 옵션을 설정, IPPROTO_TCP IPPROTO_UDP 등은 해당 프로토콜 관련 옵션을 설정할때 사용)
		//int optname : 설정할 옵션의 이름, 옵션의 종류를 나타냄 ex) SO_RCVBUF, SO_BROADCAST등이 있음
		//const char* optval : 옵션에 대한 데이터 포인터, 예를들어 SO_RCVBUF의 경우 버퍼의 크기를 지정하는데 사용
		//int optlen : optval이 가리키는 데이터의 크기

		err_display("Setsockopt Error");
		return -1;
	}
	
	setlocale(LC_ALL, ".utf8");
	//setlocale : 지역설정을 변경하기 위해 사용 날짜 및 시간, 형식, 통화, 기호 등 다양한 지역 관련 설정을 조절
	//int category : 설정할 지역 범주 (LC_ALL 모든지역설정)
	//const char* locale : 적용할 지역 ("en_US.UTF-8")

	//file open
	FILE* fp = nullptr; //FILE은 파일을 다루는 데 사용되는 데이터 구조
	fopen_s(&fp, "news.txt", "r");
	//fpoen_s : 파일을 열때 사용
	//FILE** restrict streamprt : 파일 포인터를 저장할 포인터, 함수가 파일을 열면 파일 포인터가 저장 에러가 발생하면 NULL 포인터 저장
	//const char* restrict mode : 파일을 열 때 사용할 모드 ("r" 읽기모드, "w" 쓰기모드, "a" 추가모드, "rb""wb""ab" 이진파일로 열기)
	if (fp == NULL)
	{
		err_display("fopen_s");
		return -1;
	}//.cpp파일이 있는 폴더에 있어야함

	//파일에서 데이터를 읽고, 읽은 데이터를 UDP소켓을 통해 브로드캐스트로 전달
	char buf[512]; //파일에서 읽은 데이터 저장
	int retval;
	while (true)
	{
		fgets(buf, 511, fp);
		//fgets : 파일에서 문자열을 읽는데 사용 
		//char *_Buffer : 읽은 문자열을 저장할 문자열 배열의 포인터
		//int MaxCount : 읽을 최대 문자수, 문자열의 길이는 count -1까지임
		//FILE *_STREAM : 문자열을 읽어올 파일의 포인터

		if (feof(fp)) //파일의 끝을 만나면
		{
			//feof : 파일 스트림의 끝을 검사하는 데 사용
			//끝이면 true 반환, 그렇지 않으면 false 반환
			break;
		}
		sendto(s_sock, buf, strlen(buf), 0, (SOCKADDR*)&saddr, sizeof(saddr));
		//sendto : 데이터를 소켓을 통해 전송
		//SOCKET s : 데이터를 전송할 소켓의 핸들, 데이터를 보낼 소켓
		//const char *buf : 전송할 데이터가 저장된 buf의 포인터, 이 버퍼에 있는 데이터가 소켓을 통해 전송
		//int len : 전송할 데이터의 길이, 버퍼에 있는 데이터의 크기
		//int flags : 송신 옵션, 일반적으로 0 또는 특정 상수
		//const struct sockaddr *to : 목적지 주소 
		//int tolen : 목적지 주소 구조체의 크기

		Sleep(2000); //2초 대기
	}	

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