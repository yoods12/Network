#pragma comment (lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>

using namespace std;

int main()
{
	//���� �ʱ�ȭ
	WSADATA wsa; //WinSock ���̺귯���� �ʱ�ȭ ���� �� ���� ����
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		//�ʱ�ȭ ����ó��
	}

	//WSAStartup : �����ϸ� 0 �����ϸ� �����ڵ�

	//WORD wVersionRequested : ����ϴ� WinSock�� ���� ���� MAKEWORD(2,2)�� Winsock 2.2������ ����
	
	//LPWSADATA lpWSAData : �ʱ�ȭ�� Winsock ���̺귯���� ���� �� ������ �����ϱ� ���� ����ü(WSADATA)�� ���� ������

	//���� ����
	WSACleanup();

	//WSACleanup : Winsock ���̺귯���� ����� �� ���̺귯���� �����ϰ� ���� �ڿ��� ��ȯ �ϴµ� ���
	//���̺귯���� ����� �����Ҷ� �ݵ�� ȣ��

	return 0;
}