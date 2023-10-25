#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

using namespace std;

void err_display(const char*);
void PrintIP(LPVOID);

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
	SOCKET gate_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (gate_sock == INVALID_SOCKET)
	{
		err_display("socket error");
		return -1;
	}

	//���� ip+port ����
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//sin_addr : SOCKADDR_IN ���� IP �ּҸ� �����ϴ� �ʵ�
	//s_addr : in_addr ����ü���� ���� IP�ּҸ� �����ϴ� �ʵ�
	//htonl(INADDR_ANY) : INADDR_ANY ��� Ư���� IP�ּҸ� host byte order���� network byte order�� ��ȯ
	//INADDR_ANT : ���� ������ ��� ��Ʈ��ũ �������̽����� ������ ������ �����ϱ� ���� Ư���� IP�ּ�

	if (bind(gate_sock, (SOCKADDR*)&saddr, sizeof(saddr)))
	{
		err_display("bind error");
		return -1;
	}

	//bind : ������ IP�� ��Ʈ ��ȣ�� ���ε��Ͽ� �ش� �ּҷ� ������ ������ ���� �� �� �ְ� ����
	//SOCKET s : ���ε� �� ����
	//const struct sockaddr *name : ���ε� �� �ּ�����, SOCKADDR ����ü�� ����Ͽ� IP�� ��Ʈ��ȣ ����
	//int namelen : �ּ� ������ ũ�� 
	//���� 0, ���� SOCKET_ERROR


	if (listen(gate_sock, SOMAXCONN))
	{
		err_display("listen error");
		return -1;
	}

	//listen : ���� ������ ��� ���·� ����� Ŭ���̾�Ʈ�� ���� ��û�� ������ �غ� ��
	//SOCKET s : �����·� ��ȯ �� ������ �ڵ�
	//int backlog : ��⿭�� ũ�� SOMAXCONN�� ���� ����� �����Ͽ� �ý����� �����ϴ� �ִ� ���� ���� ���

	SOCKADDR_IN caddr; //Ŭ���̾�Ʈ ������ �ּ������� ������ ����ü
	int addrlen = sizeof(caddr);
	SOCKET c_sock = accept(gate_sock, (SOCKADDR*)&caddr, &addrlen);

	//SOCKET s : ���� ��� ������ ����, Ŭ���̾�Ʈ�� ���� ��û�� �޾Ƶ��̱� ���� ����
	//struct sockaddr *addr : Ŭ���̾�Ʈ�� �ּ������� ������ ����ü ������
	//int *addrlen : Ŭ���̾�Ʈ �ּ������� ũ��
	//���� SOCKETŸ�Թ�ȯ, ���� INVALID_SOCKET

	if (c_sock == INVALID_SOCKET)
	{
		err_display("accept error");
		return -1;
	}

	//������ Ŭ���̾�Ʈ�� ���� : IP, port
	cout << "Client connected : ";
	PrintIP(&caddr.sin_addr);
	cout << "(" << ntohs(caddr.sin_port) << ")" << endl;

	//echo service

	char buf[80];
	int ret;
	while (true)
	{
		//Ŭ���̾�Ʈ ������ ����
		ret = recv(c_sock, buf, 79, 0); //�� �������� null �־���ؼ� 79
		//SOCKET s : �����͸� ������ ����, Ŭ���̾�Ʈ�� ����� �� ���
		//char *buf : ������ �����͸� ������ ����
		//int len : ������ ũ��
		//int flags : ���ŵ��ۿ� ���� �ɼ� 
		//���� ���ŵ� ����Ʈ �� ��ȯ, ���� 0

		if (ret == 0) //Ŭ���̾�Ʈ�� closesocket() ȣ���� ���̽�
		{
			cout << "graceful closing case" << endl;
			err_display("recv");
			break;
		}
		if (ret == SOCKET_ERROR) //Ŭ���̾�Ʈ�� rst(reset signal) ����
		{
			cout << "abnormal closing case" << endl;
			err_display("recv");
			break;
		}

		//�������� ���� ����
		buf[ret] = '\0';
		cout << "From client : " << buf << endl;

		send(c_sock, buf, ret, 0);
		//SOCKET s : �����͸� ���� ����, Ŭ���̾�Ʈ�� ����� �� ���
		//const char *buf : ���� �����Ͱ� ����� ����
		//int len : ���� �������� ũ��
		//int flags : ������ �۽ŵ��� �ɼ�

	}
	closesocket(c_sock);
	closesocket(gate_sock);
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

void PrintIP(LPVOID addr)
{
	char IPbuf[20];
	if (!InetNtop(AF_INET, addr, IPbuf, 20))
	{
		err_display("Inettop");
		return;
	}
	cout << IPbuf;
}