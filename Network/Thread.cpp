#include <WinSock2.h>
#include <process.h>
#include <iostream>

using namespace std;

void err_display(const char*);
unsigned __stdcall echo_serc(LPVOID);

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
	SOCKET s_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s_sock == INVALID_SOCKET)
	{
		err_display("socket");
		return -1;
	}

	//bind
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s_sock, (SOCKADDR*)&saddr, sizeof(saddr)))
	{
		err_display("bind");
		return -1;
	}

	//listen
	if (listen(s_sock, SOMAXCONN))
	{
		err_display("listen");
		return -1;
	}

	SOCKADDR_IN caddr;
	int addrlen = sizeof(caddr);
	SOCKET c_sock;
	HANDLE th;
	while (true)
	{
		c_sock = accept(s_sock, (SOCKADDR*)&caddr, &addrlen);
		if(c_sock==INVALID_SOCKET)
		{
			err_display("accept");
			break;
		}
		//thread ����
		th = (HANDLE)_beginthreadex(NULL, 0, &echo_serv, (void*)c_sock, 0, NULL);

		//_beginthreadex : �����带 �����ϰ� �����ϴ� �Լ�
		//void* security : ������ ���� �Ӽ� (���� NULL)
		//unsigned stack_size : ������ ���� ũ�� (0�̸� �⺻ ����ũ��)
		//__stdcall start_address : �����尡 ���۵� �Լ��� ������
		//void* arglist : ������ �����Լ��� ���޵� �μ��� �����ϴ� ������
		//initflag : ������ �ʱ�ȭ �÷��� (0�̸� �⺻��)
		//threadaddr : ������ ID�� ������ ������ ������, ������ ID�� ���� �ʿ䰡 ������ NULL

	}

	WaitForSingleObject(th, INFINITE);

	//WaitForSingleObject : ������ �Ǵ� �ٸ� �������� ��ü�� ���¸� ����͸� �ϰ�, �ش� ��ü�� ���°� ���ϴ� ���·� ��ȭ�� �� ���� ��ٸ��µ� ���
	//HANDLE hHandle : ����͸��� ��ü�� �ڵ� (������, �̺�Ʈ, ���ؽ�, ��������, ���� �ڵ�� �پ��� ������ ��ü�� ��Ÿ�� �� ����)
	//DWORD dwMilliseconds : ���ð� �и���, 0���� ū ���� �Ǵ� INFINITE(���Ѵ��)

	closesocket(s_sock);
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