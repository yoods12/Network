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
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) // !=0
	{
		err_display("WSAStartup error");
		return -1;
	}

	ADDRINFOA hint;
	ZeroMemory(&hint, sizeof(hint)); // 0���� �ʱ�ȭ

	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;

	//ai_family : �ּ�ü�� AF_INET�� IPv4 �ּҸ� �˻� 
	//ai_socktype : ����Ÿ�� SOCK_STREAM�� TCP������ �˻�
	//ai_protocol : �������� IPPROTO_TCP�� TCP���������� ��Ī, TCP�� ����ϴ� ������ �˻�

	PADDRINFOA result = nullptr; //ADDRINFOA *result;

	//PADDRINFOA : getaddrinfo���� ��ȯ�Ǵ� �ּ�(ADDRINFOA)�� ���� ������ ����

	if (getaddrinfo("www.naver.com", "http", &hint, &result)) //�����ϸ� 0
	{
		cout << "getaddrinfo error" << endl;
		return -1;
	}

	//getaddrinfo : ������ �̸� �� ��Ʈ ��ȣ�� ����Ͽ� �ּ� ������ �������� �Լ�
	//pNodeName : �˻��Ϸ��� ȣ��Ʈ�� �Ǵ� IP�ּ� ����
	//pServiceName : ȣ��Ʈ�� ������ �� ����� ��Ʈ��ȣ �Ǵ� ���� �̸� ����
	//*pHints : ���� ������ ���� ��Ʈ ����, �˻� �ɼ� �� ������ ����, ����ü�� �ּ�ü�� ����Ÿ�� �������� ���� ����
	//*ppResult : �˻������ �����ϴ� ����, addrinfo ����ü�� ���� �����Ϳ� ���� ������

	SOCKADDR_IN* inaddr = nullptr; //����ü�� ����ų ������ inaddr ���� �� �ʱ�ȭ
	for (ADDRINFO* ptr = result; ptr; ptr = ptr->ai_next)
	{
		inaddr = (SOCKADDR_IN*)ptr->ai_addr; 
		//���� �ּ������� ���� �����͸� SOCKADDR_IN ����ü�� ����ȯ �� inaddr ������ ����
		//ai_addr�� �ּ������� �����ּҸ� ��Ÿ��, �ش� �����ּҵ� ����ȯ ��
		
		PrintIP(&inaddr->sin_addr);
		//inaddr �������� sin_addr �ʵ带 �����Ͽ� IP�ּҸ� ��� ���
	}
	//getaddrinfo �Լ��� ����� result�� ����� ���� ����� ��ȸ�ϴ� �ݺ���
	//ptr������ �ּҸ� ����Ŵ
	//ptr->ai_next�� ���� ���� �ּ� ������ �̵�

	freeaddrinfo(result);
	//getaddrinfo �Լ��κ��� �Ҵ�� �޸� ����

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

void PrintIP(LPVOID addr) // PrintIP(IN_ADDR*) : PrintIP(&addr.sin_addr)
{
	char IPaddr[20];
	if (!InetNtop(AF_INET, addr, IPaddr, 20))
	{
		err_display("InetNtop error");
		return;
	}

	cout << "IP addr : " << IPaddr << endl;

}