#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>

using namespace std;

void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);

	LocalFree(lpMsgBuf);
	exit(1);

	//MessageBox : 오류 메시지를 팝업 대화 상자로 표시
	//HWND hWnd : 메시지 상자의 소유자 창에 대한 핸들, NULL이면 메시지 상자에 소유자 창이 없음
	//LPCTSTR lpText : 표시할 메시지
	//LPCTSTR lpCaption : 대화 상자 제목
	//UNIT uType : 대화 상자의 내용 및 동작
}