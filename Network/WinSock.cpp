#pragma comment (lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>

using namespace std;

int main()
{
	//윈속 초기화
	WSADATA wsa; //WinSock 라이브러리의 초기화 상태 및 정보 저장
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		//초기화 실패처리
	}

	//WSAStartup : 성공하면 0 실패하면 오류코드

	//WORD wVersionRequested : 사용하는 WinSock의 버전 지정 MAKEWORD(2,2)는 Winsock 2.2버전을 지정
	
	//LPWSADATA lpWSAData : 초기화된 Winsock 라이브러리의 상태 및 정보를 저장하기 위한 구조체(WSADATA)에 대한 포인터

	//윈속 종료
	WSACleanup();

	//WSACleanup : Winsock 라이브러리를 사용한 후 라이브러리를 정리하고 사용된 자원을 반환 하는데 사용
	//라이브러리의 사용을 종료할때 반드시 호출

	return 0;
}