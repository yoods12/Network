#pragma comment (lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

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

	//IP+PORT지정
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	
	//SOCKADDR_IN 구조체 : IPv4 주소와 포트번호를 저장하는데 사용
	//sin_family : 주소체계를 지정 AF_INET는 IPv4 주소체계를 나타냄
	//sin_port : 포트번호 지정
	//htons : host byte order에서 network byte order로 변환 후 8000번 포트 설정
	//네트워크 프로토콜에서 사용되는 byte order로 일치 시킴
	//서로 다른 엔디안 시스템 간에 데이터를 정확하게 해석하기 위함
	
	//IP+PORT지정
	if (!InetPton(AF_INET, "1.2.3.4", &addr.sin_addr)) // InetPton == 0
	{
		err_display("InetPton error");
		return -1;
	}

	//InetPton : 문자열 형태의 ip주소를 이진형태의 ip주소로 변환
	//INT Family : 주소체계 AF_INET는 IPv4
	//LPCTSTR pszAddr : 변환할 IP주소 문자열("1.2.3.4")
	//PVOID pAddr : 변환된 이진 형태의 IP주소를 저장할 변수
	// 성공 1 , 실패 0

	//IP출력
	char IPaddr[20];
	if (!InetNtop(AF_INET, &addr.sin_addr, IPaddr, 20))
	{
		err_display("InetNtop error");
		return -1;
	}

	//InetNtoP : 이진형태의 ip주소를 문자열 형태의 ip주소로 변환
	//INT Family : 주소체계 AF_INET는 IPv4
	//pAddr : 문자열로 변환할 network byte의 ip주소에 대한 포인터
	//pStringBuf : 변환된 IP주소를 저장하는 문자열 버퍼, 변환에 성공하면 이 버퍼에 IP주소 문자열이 저장
	//StringBufSize : 버퍼의 크기
	//성공 문자열포인터, 실패 NULL

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