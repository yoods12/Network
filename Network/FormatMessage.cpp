#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>

using namespace std;

void err_display(const char*);

int main()
{
	err_display("socket error");
	return 0;
}

void err_display(const char* mes)
{
	LPVOID lpbuffer = nullptr; //LPVOID�� ����ϸ� �ٸ� ������ �������� ����ȯ ����
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpbuffer, 0, nullptr);

	cout << mes << " : " << (char*)lpbuffer << endl;
	LocalFree(lpbuffer); //LocalAlloc �Լ��� ����Ͽ� �Ҵ��� �޸� ����

	//FormatMessage �Լ� : ���� �޽����� ���ڿ��� ��ȯ�ϴµ� ���

	//DWORD dwFlags �ɼ� : ����ȭ�� �޼����� ��� ���������� ����
	//FORMAT_MESSAGE_ALLOCATE_BUFFER : FormatMessage �Լ��� �޽����� ������ �޸� ���۸� �������� �Ҵ�, �޸� ���� �ؾ���
	//FORMAT_MESSAGE_FROM_SYSTEM : FormatMessage �Լ��� �ý��� �����ڵ忡 ���� �޽����� ������, �ý��� ���� �޽��� ���� �� ���
	
	//LPCVOID lpSource : �޽��� �ؽ�Ʈ�� ������ �ҽ��� ����, �Ϲ������� NULL�� ������ ��� FormatMessage �Լ��� �ý��ۿ��� �����ϴ� �޽��� �ҽ��� ���
	//FORMAT_MESSAGE_FROM_SYSTEM �÷��׿� �Բ� ����ϸ� �ý��� ���� �ڵ忡 ���� �⺻ �޽����� ������

	//DWORD dwMessageId : �����ڵ峪 �޽����� �ĺ� �Ҷ� ���
	//�����ð����� 10054 ����
	//WSAGetLastError() : �����ڵ� ���

	//DWORD dwLanguageId : ���
	//MAKELANGID : dwLanguageId ���� ����µ� ��� (SubLanguage ID, Primary Language ID)
	//MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)�̶�� (����, �̱�)�� ��Ÿ��
	
	//LPTSTR lpBuffer : ���� ���ڿ� �����ּ�, null�� ������ ���ڿ��� ���� �ϴ� ���ۿ� ���� ������
	//FORMAT_MESSAGE_ALLOCATE_BUFFER �÷��װ� ������ Local Alloc �Լ�(���� �޸𸮸� �������� �Ҵ�)�� ����Ͽ� ���۸� �Ҵ��ϰ� lpbuffer�� ������ �ּ��� ���ۿ� �����͸� ��ġ

	//DWORD nSize : FormatMessage �Լ����� ��� ���ڿ��� ������ ������ �ִ����
	//0���� �����Ͽ� ��¹��ڿ� ���̸� ���

	//va_list* Arguments : �����μ� ���
	//���� �ȹ���?

	//FormatMessage �����ϸ� �����޽��� ����, �����ϸ� 0
}