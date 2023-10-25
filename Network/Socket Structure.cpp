#pragma comment (lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

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

	//IP+PORT����
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	
	//SOCKADDR_IN ����ü : IPv4 �ּҿ� ��Ʈ��ȣ�� �����ϴµ� ���
	//sin_family : �ּ�ü�踦 ���� AF_INET�� IPv4 �ּ�ü�踦 ��Ÿ��
	//sin_port : ��Ʈ��ȣ ����
	//htons : host byte order���� network byte order�� ��ȯ �� 8000�� ��Ʈ ����
	//��Ʈ��ũ �������ݿ��� ���Ǵ� byte order�� ��ġ ��Ŵ
	//���� �ٸ� ����� �ý��� ���� �����͸� ��Ȯ�ϰ� �ؼ��ϱ� ����
	
	//IP+PORT����
	if (!InetPton(AF_INET, "1.2.3.4", &addr.sin_addr)) // InetPton == 0
	{
		err_display("InetPton error");
		return -1;
	}

	//InetPton : ���ڿ� ������ ip�ּҸ� ���������� ip�ּҷ� ��ȯ
	//INT Family : �ּ�ü�� AF_INET�� IPv4
	//LPCTSTR pszAddr : ��ȯ�� IP�ּ� ���ڿ�("1.2.3.4")
	//PVOID pAddr : ��ȯ�� ���� ������ IP�ּҸ� ������ ����
	// ���� 1 , ���� 0

	//IP���
	char IPaddr[20];
	if (!InetNtop(AF_INET, &addr.sin_addr, IPaddr, 20))
	{
		err_display("InetNtop error");
		return -1;
	}

	//InetNtoP : ���������� ip�ּҸ� ���ڿ� ������ ip�ּҷ� ��ȯ
	//INT Family : �ּ�ü�� AF_INET�� IPv4
	//pAddr : ���ڿ��� ��ȯ�� network byte�� ip�ּҿ� ���� ������
	//pStringBuf : ��ȯ�� IP�ּҸ� �����ϴ� ���ڿ� ����, ��ȯ�� �����ϸ� �� ���ۿ� IP�ּ� ���ڿ��� ����
	//StringBufSize : ������ ũ��
	//���� ���ڿ�������, ���� NULL

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