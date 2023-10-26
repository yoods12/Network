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
		err_display("WSAStartup Error");
		return -1;
	}

	//socket ����
	SOCKET s_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s_sock == INVALID_SOCKET)
	{
		err_display("Socket Error");
		return -1;
	}

	//broadcast socket ip+port ����
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	bool flag = true;
	if (setsockopt(s_sock, SOL_SOCKET, SO_BROADCAST, (char*)&flag, sizeof(flag)))
	{
		//setsockopt : ���� �ɼ� ����, ���Ͽ� ���� �ɼ��� �����Ͽ� ��Ʈ��ũ ��� ������ �����ϰų� ����
		//SOCET s : ������ ������ �ڵ�
		//int level : �ɼ��� ������ ���� (SOL_SOCKET�� �Ϲ� ���� �ɼ��� ����, IPPROTO_TCP IPPROTO_UDP ���� �ش� �������� ���� �ɼ��� �����Ҷ� ���)
		//int optname : ������ �ɼ��� �̸�, �ɼ��� ������ ��Ÿ�� ex) SO_RCVBUF, SO_BROADCAST���� ����
		//const char* optval : �ɼǿ� ���� ������ ������, ������� SO_RCVBUF�� ��� ������ ũ�⸦ �����ϴµ� ���
		//int optlen : optval�� ����Ű�� �������� ũ��

		err_display("Setsockopt Error");
		return -1;
	}
	
	setlocale(LC_ALL, ".utf8");
	//setlocale : ���������� �����ϱ� ���� ��� ��¥ �� �ð�, ����, ��ȭ, ��ȣ �� �پ��� ���� ���� ������ ����
	//int category : ������ ���� ���� (LC_ALL �����������)
	//const char* locale : ������ ���� ("en_US.UTF-8")

	//file open
	FILE* fp = nullptr; //FILE�� ������ �ٷ�� �� ���Ǵ� ������ ����
	fopen_s(&fp, "news.txt", "r");
	//fpoen_s : ������ ���� ���
	//FILE** restrict streamprt : ���� �����͸� ������ ������, �Լ��� ������ ���� ���� �����Ͱ� ���� ������ �߻��ϸ� NULL ������ ����
	//const char* restrict mode : ������ �� �� ����� ��� ("r" �б���, "w" ������, "a" �߰����, "rb""wb""ab" �������Ϸ� ����)
	if (fp == NULL)
	{
		err_display("fopen_s");
		return -1;
	}//.cpp������ �ִ� ������ �־����

	//���Ͽ��� �����͸� �а�, ���� �����͸� UDP������ ���� ��ε�ĳ��Ʈ�� ����
	char buf[512]; //���Ͽ��� ���� ������ ����
	int retval;
	while (true)
	{
		fgets(buf, 511, fp);
		//fgets : ���Ͽ��� ���ڿ��� �дµ� ��� 
		//char *_Buffer : ���� ���ڿ��� ������ ���ڿ� �迭�� ������
		//int MaxCount : ���� �ִ� ���ڼ�, ���ڿ��� ���̴� count -1������
		//FILE *_STREAM : ���ڿ��� �о�� ������ ������

		if (feof(fp)) //������ ���� ������
		{
			//feof : ���� ��Ʈ���� ���� �˻��ϴ� �� ���
			//���̸� true ��ȯ, �׷��� ������ false ��ȯ
			break;
		}
		sendto(s_sock, buf, strlen(buf), 0, (SOCKADDR*)&saddr, sizeof(saddr));
		//sendto : �����͸� ������ ���� ����
		//SOCKET s : �����͸� ������ ������ �ڵ�, �����͸� ���� ����
		//const char *buf : ������ �����Ͱ� ����� buf�� ������, �� ���ۿ� �ִ� �����Ͱ� ������ ���� ����
		//int len : ������ �������� ����, ���ۿ� �ִ� �������� ũ��
		//int flags : �۽� �ɼ�, �Ϲ������� 0 �Ǵ� Ư�� ���
		//const struct sockaddr *to : ������ �ּ� 
		//int tolen : ������ �ּ� ����ü�� ũ��

		Sleep(2000); //2�� ���
	}	

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