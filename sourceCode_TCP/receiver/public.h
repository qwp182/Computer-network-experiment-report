#ifndef PUBLIC_H
#define PUBLIC_H

#include<iostream>
#include <winsock2.h>  
#include <stdio.h>
#include <vector>
#include<ws2tcpip.h>//����socklen_t
#pragma comment(lib, "WS2_32")  // ���ӵ�WS2_32.lib

using namespace std;

#define SERVER_IP "192.168.0.101"// Ĭ�Ϸ�������IP��ַ
#define SERVER_PORT 8307// �������˿ں�

class server
{
public:
	server();
	void init();
	void process();
	vector<string> split(char *str);

private:
	int listener;//�����׽���
	sockaddr_in  serverAddr;//IPV4�ĵ�ַ��ʽ
	vector <int> socnum;//��Ŵ������׽��֣��������ͻ��˵����
};


#endif // !PUBLIC_H
