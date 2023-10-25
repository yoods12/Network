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

	//MessageBox : ���� �޽����� �˾� ��ȭ ���ڷ� ǥ��
	//HWND hWnd : �޽��� ������ ������ â�� ���� �ڵ�, NULL�̸� �޽��� ���ڿ� ������ â�� ����
	//LPCTSTR lpText : ǥ���� �޽���
	//LPCTSTR lpCaption : ��ȭ ���� ����
	//UNIT uType : ��ȭ ������ ���� �� ����
}