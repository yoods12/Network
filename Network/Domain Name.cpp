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
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) // !=0
	{
		err_display("WSAStartup error");
		return -1;
	}

	ADDRINFOA hint;
	ZeroMemory(&hint, sizeof(hint)); // 0으로 초기화

	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;

	//ai_family : 주소체계 AF_INET는 IPv4 주소를 검색 
	//ai_socktype : 소켓타입 SOCK_STREAM는 TCP소켓을 검색
	//ai_protocol : 프로토콜 IPPROTO_TCP는 TCP프로토콜을 지칭, TCP를 사용하는 소켓을 검색

	PADDRINFOA result = nullptr; //ADDRINFOA *result;

	//PADDRINFOA : getaddrinfo에서 반환되는 주소(ADDRINFOA)에 대한 포인터 유형

	if (getaddrinfo("www.naver.com", "http", &hint, &result)) //성공하면 0
	{
		cout << "getaddrinfo error" << endl;
		return -1;
	}

	//getaddrinfo : 도메인 이름 및 포트 번호를 사용하여 주소 정보를 가져오는 함수
	//pNodeName : 검색하려는 호스트명 또는 IP주소 지정
	//pServiceName : 호스트에 연결할 때 사용할 포트번호 또는 서비스 이름 지정
	//*pHints : 소켓 유형에 대한 힌트 제공, 검색 옵션 및 제한을 설정, 구조체에 주소체계 소켓타입 프로토콜 등을 설정
	//*ppResult : 검색결과를 저장하는 변수, addrinfo 구조체에 대한 포인터에 대한 포인터

	SOCKADDR_IN* inaddr = nullptr; //구조체를 가리킬 포인터 inaddr 선언 및 초기화
	for (ADDRINFO* ptr = result; ptr; ptr = ptr->ai_next)
	{
		inaddr = (SOCKADDR_IN*)ptr->ai_addr; 
		//현재 주소정보에 대한 포인터를 SOCKADDR_IN 구조체로 형변환 후 inaddr 변수에 저장
		//ai_addr은 주소정보의 소켓주소를 나타냄, 해당 소켓주소도 형변환 됨
		
		PrintIP(&inaddr->sin_addr);
		//inaddr 변수에서 sin_addr 필드를 추출하여 IP주소를 얻고 출력
	}
	//getaddrinfo 함수의 결과인 result에 저장된 정보 목록을 순회하는 반복문
	//ptr변수는 주소를 가리킴
	//ptr->ai_next를 통해 다음 주소 정보로 이동

	freeaddrinfo(result);
	//getaddrinfo 함수로부터 할당된 메모리 해제

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

void PrintIP(LPVOID addr) // PrintIP(IN_ADDR*) : PrintIP(&addr.sin_addr)
{
	char IPaddr[20];
	if (!InetNtop(AF_INET, addr, IPaddr, 20))
	{
		err_display("InetNtop error");
		return;
	}

	cout << "IP addr : " << IPaddr << endl;

}