#pragma comment (lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>

using namespace std;

void err_display(const char*);

int main()
{
	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		err_display("WSAStartup error");
		return -1;
	}

	//TCP socket ����
	SOCKET sock1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//int af : �ּ�ü�� AF_INET(IPv4), AF_INET6(IPv6)�� ���
	
	//int type : ����Ÿ�� SOCK_STREAM(TCP ����), SOCK_DRAM(UDP ����)

	//int protocol : ����� �������� 0�� ����ϸ� �⺻�������� ����ϵ��� ����

	//���� : ���ο� ���� , ���� : INVALID_SOCKET ��ȯ

	if (sock1 == INVALID_SOCKET)
	{
		err_display("socket tcp error");
		return -1;
	}

	//UDP socket ����
	SOCKET sock2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock2 == INVALID_SOCKET)
	{
		err_display("socket udp error");
		return -1;
	}

	//���� ������ ���� �ڵ鰪�� ȭ�鿡 ���
	cout << "tcp socket handle : " << sock1 << endl;
	cout << "udp socket handle : " << sock2 << endl;

	//���� ����
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