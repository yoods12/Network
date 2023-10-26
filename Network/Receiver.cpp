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
	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		err_display("WSAStartup");
		return -1;
	}

	//socket ����
	SOCKET udpsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udpsock == INVALID_SOCKET)
	{
		err_display("socket");
		return -1;
	}

	//receiver ip+port ����
	SOCKADDR_IN raddr;
	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(8000);
	raddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(udpsock, (SOCKADDR*)&raddr, sizeof(raddr)))
	{
		err_display("bind");
		return -1;
	}

	setlocale(LC_ALL, ".utf8");

	//UDP�� �̿��Ͽ� ������ ����
	char buf[512]; //������ ������ ����
	SOCKADDR_IN saddr; //�۽����� �ּ�����
	int addrlen = sizeof(saddr); 
	int retval; //������ �������� ����Ʈ ���� ����

	while (true)
	{
		retval = recvfrom(udpsock, buf, 511, 0, (SOCKADDR*)&saddr, &addrlen);
		//recvfrom : ������ ���� �����͸� ������ �� ���
		//SOCKET s : �����͸� ������ ������ �ڵ�
		//char *buf : ������ �����͸� ������ ����
		//int len : ������ ũ��, ������ �����Ͱ� �����
		//int flags : �ɼ� �÷���, �Ϲ������� 0 �Ǵ� Ư�� �÷��� ��
		//struct sockaddr *from : �۽����� �ּ������� ������ ����ü�� ������, �۽����� �ּ������� ����
		//int *fromlen : �۽����� �ּ����� ����ü�� ũ��
		//���� ���� ����Ʈ �� ��ȯ, ���� SOCKET_ERROR

		if (retval == 0 || retval == SOCKET_ERROR)
		{
			break;
		}
		buf[retval] = '\0';
		cout << buf;
	}



	closesocket(udpsock);
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