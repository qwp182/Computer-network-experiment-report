#ifndef PUBLIC_H
#define PUBLIC_H

#include<conio.h>
#include<vector>
#include<iostream>
#include<thread>
#include<winsock2.h>  
#include<stdio.h>
#include<string>
#include<sstream>
#include<ws2tcpip.h>//����socklen_t
#pragma comment(lib, "WS2_32")  // ���ӵ�WS2_32.lib 
#include <thread>
using namespace std;

#define SERVER_IP "192.168.0.101"// Ĭ�Ϸ�������IP��ַ
#define SERVER_PORT 8307// �������˿ں�

class client
{
public:
	client();
	int init();
	void process();
	bool isNum(string str);
	vector<string> split(char *str);

private:
	int user;
	int writing;
	sockaddr_in  serverAddr;//IPV4�ĵ�ַ��ʽ��������˵�ַ�塢�����IP��ַ������˶˿ں�
	void sendata();
};

#endif
