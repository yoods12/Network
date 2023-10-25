#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

using namespace std;

void err_display(const char*);

int main()
{

	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) // != 0
	{
		err_display("WSAStartup error");
		return -1;
	}

	//���� ����
	SOCKET c_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (c_sock == INVALID_SOCKET)
	{
		err_display("socket error");
		return -1;
	}

	//������ ���� (server ip : 127.0.0.1)
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	if (!InetPton(AF_INET, "127.0.0.1", &saddr.sin_addr))
	{
		err_display("InetPton");
		return -1;
	}

	if (connect(c_sock, (SOCKADDR*)&saddr, sizeof(saddr)))
	{
		err_display("InetPton");
		return -1;
	}
	//connect : Ŭ���̾�Ʈ�� ������ ���� �� �� ��
	//SOCKET s : ������ �õ��� ����, Ŭ���̾�Ʈ ����
	//const struct sockaddr *name : ������ ������ �ּ������� ��Ÿ���� ����ü�� ������, ������ IP�� ��Ʈ��ȣ�� ����ü�� ���ԵǾ� �־����
	//int namelen : ����ü�� ũ��

	//echo service request
	char buf[80];
	int sendbyte;
	while (fgets(buf, 79, stdin))
	{
		//fgets : ���ڿ� �Է� �޴� �Լ�
		//char *str : �Էµ� ���ڿ��� ������ ���۸� ����Ű�� ������, ���ڿ��� ���ۿ� ����
		//int count : ���� ������ �ִ� ����
		//FILE *stream : ���ڿ��� �о�� ���� ��Ʈ���� ����Ű�� ������, stdin�� ����Ͽ� Ű����� �Է� ����
		sendbyte = strlen(buf);
		send(c_sock, buf, sendbyte - 1, 0);

		//������ ���� ������ ����
		sendbyte = recv(c_sock, buf, 79, 0);
		buf[sendbyte] = '\0';
		cout << "From server : " << buf << endl;

	}


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