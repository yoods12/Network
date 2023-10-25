#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>

using namespace std;

void err_display(const char*);

int main()
{
	err_display("socket error");
	return 0;
}

void err_display(const char* mes)
{
	LPVOID lpbuffer = nullptr; //LPVOID를 사용하면 다른 데이터 형식으로 형변환 가능
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpbuffer, 0, nullptr);

	cout << mes << " : " << (char*)lpbuffer << endl;
	LocalFree(lpbuffer); //LocalAlloc 함수를 사용하여 할당한 메모리 해제

	//FormatMessage 함수 : 오류 메시지를 문자열로 반환하는데 사용

	//DWORD dwFlags 옵션 : 형식화된 메세지를 어떻게 가져올지를 지정
	//FORMAT_MESSAGE_ALLOCATE_BUFFER : FormatMessage 함수가 메시지를 저장할 메모리 버퍼를 동적으로 할당, 메모리 해제 해야함
	//FORMAT_MESSAGE_FROM_SYSTEM : FormatMessage 함수가 시스템 오류코드에 대한 메시지를 가져옴, 시스템 오류 메시지 얻을 때 사용
	
	//LPCVOID lpSource : 메시지 텍스트를 가져올 소스를 지정, 일반적으로 NULL로 지정될 경우 FormatMessage 함수는 시스템에서 제공하는 메시지 소스를 사용
	//FORMAT_MESSAGE_FROM_SYSTEM 플래그와 함께 사용하면 시스템 오류 코드에 대한 기본 메시지를 가져옴

	//DWORD dwMessageId : 오류코드나 메시지를 식별 할때 사용
	//수업시간에는 10054 넣음
	//WSAGetLastError() : 오류코드 얻기

	//DWORD dwLanguageId : 언어
	//MAKELANGID : dwLanguageId 값을 만드는데 사용 (SubLanguage ID, Primary Language ID)
	//MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)이라면 (영어, 미국)을 나타냄
	
	//LPTSTR lpBuffer : 오류 문자열 시작주소, null로 끝나는 문자열을 수신 하는 버퍼에 대한 포인터
	//FORMAT_MESSAGE_ALLOCATE_BUFFER 플래그가 있으면 Local Alloc 함수(로컬 메모리를 동적으로 할당)를 사용하여 버퍼를 할당하고 lpbuffer에 지정된 주소의 버퍼에 포인터를 배치

	//DWORD nSize : FormatMessage 함수에서 출력 문자열을 저장할 버퍼의 최대길이
	//0으로 설정하여 출력문자열 길이만 계산

	//va_list* Arguments : 가변인수 목록
	//아직 안배운듯?

	//FormatMessage 성공하면 오류메시지 길이, 실패하면 0
}