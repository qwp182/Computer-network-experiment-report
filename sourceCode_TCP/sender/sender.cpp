#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "public.h"
string SerialNumber; //�û���������к�
int stats = 0;		 //��Ϊ0����ʾ������û�б�������Ϊ1����ʾ����������

int main()
{
	client user;
	while (1)
		user.process();

	return 0;
}

//���ÿͻ�����Ϣ
client::client()
{
	user = 0;
	writing = 0;
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = SERVER_PORT;
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP); //���ַ�������ת��uint32_t
}

//���г�ʼ��,����0��ʾ��ʼ���ɹ�������1��ʾ��ʼ��ʧ�ܣ���Ҫ�������ӷ�����
int client::init()
{
	int Ret;
	WSADATA wsaData; // ���ڳ�ʼ���׽��ֻ���
					 // ��ʼ��WinSock����
	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		cout << "WSAStartup()   failed   with   error" << Ret << endl;
		WSACleanup();
	}

	user = socket(AF_INET, SOCK_STREAM, 0); //����ipv4,TCP����
	if (user <= 0)
	{
		perror("���ӽ���ʧ��");
		cout << "Error at socket():" << WSAGetLastError() << endl;
		exit(1);
	};
	cout << "�ɹ���������" << endl; //�����ɹ�
	//����ʽ�ĵȴ�����������
	if (connect(user, (const sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("���ӷ�����ʧ��");
		cout << "Error at socket():" << WSAGetLastError() << endl;
		cout << "���ڳ�������" << endl;
		return 1;
	}
	cout << "���� IP:" << SERVER_IP << " Port:" << SERVER_PORT << " �ɹ�" << endl; //�����ɹ�
	return 0;
}

void client::process()
{
	char recvbuf[1024];
	fd_set fdread, fedwrite;
	FD_ZERO(&fdread);	//��fds����
	FD_ZERO(&fedwrite); //��fds����

	int flag = 0;
	flag = init();
	if (flag == 1)
	{
		Sleep(5000);
		cout << endl; //5������������ӷ�����
		return;
	} //���ӷ�����ʧ�ܣ���Ҫ�����ٴ�����

	while (1)
	{
		FD_SET(user, &fdread);
		if (writing == 0)
			FD_SET(user, &fedwrite);

		struct timeval timeout = {1, 0}; //����ÿ��1��selectһ��
		switch (select(0, &fdread, &fedwrite, NULL, &timeout))
		{
		case -1:
		{
			cout << "Error at socket():" << WSAGetLastError() << endl;
			break;
		}
		case 0:
		{
			break;
		}
		default:
		{
			//���������Ϣ���ͻ���ʱ��������Ϣ
			if (FD_ISSET(user, &fdread))
			{
				int size = recv(user, recvbuf, sizeof(recvbuf) - 1, 0);
				if (size > 0)
				{
					cout << "����˻ظ�:" << recvbuf << endl;
					memset(recvbuf, '\0', sizeof(recvbuf));
				}
				else
				{
					cout << "����˹رգ����ڵȴ�����˿���" << endl;
					Sleep(10000); //�ȴ�10���������
					cout << endl;
					cout << "��ʼ�����������ӷ�����" << endl
						 << endl;
					stats = 1; //����������������
					return;
				}
			}
			if (FD_ISSET(user, &fedwrite))
			{
				FD_ZERO(&fedwrite); //��fedwrite����
				writing = 1;		//����Ϊд״̬

				sendata();
			}
			break;
		}
		}
	}
}

void client::sendata()
{
	string input;
	vector<string> info;

	getline(cin, input);

	//���û��������Ϣ�н�ȡ�����кţ��û���������
	info.clear();
	info = split((char *)input.c_str());

	//���������Ϣ����У�飬����������˷�����Ϣ
	while (1)
	{
		//���ж�������Ǽ���
		if (info.size() < 2 || info.size() > 3)
		{
			cout << "Ӧ���������û���������" << endl;
			cout << "���Ұ������кţ��û�������������û����������˳�����룬�м��ÿո����" << endl;
			getline(cin, input);
			info.clear();
			info = split((char *)input.c_str());
			continue;
		}
		//��������Ϊ�����������������кţ���Ҫ�����кŽ��м���
		else if (info.size() == 3)
		{
			SerialNumber = info[0];
			//�ж��������к��Ƿ�Ϊ10
			if (SerialNumber.size() != 10)
			{
				cout << "���кŵĳ���Ӧ��Ϊ10������������������Ϣ��" << endl;
				getline(cin, input);
				info.clear();
				info = split((char *)input.c_str());
				continue;
			}
			//���ж���������к��Ƿ�ȫ��Ϊ����
			else if (!isNum(SerialNumber))
			{
				cout << "���кŵĳ���Ӧ��Ϊ10������������������Ϣ��" << endl;
				getline(cin, input);
				info.clear();
				info = split((char *)input.c_str());
				continue;
			}
			else
				break;
		}
		else
			break; //��������Ϊ�������û�����кţ�����Ҫ���м���
	}

	//���ͻ�����������кŷ��͸������������֤
	string str;
	if (info.size() == 3)
		str = info[0] + " " + info[1] + " " + info[2];
	else
		str = info[0] + " " + info[1];

	send(user, str.c_str(), sizeof(str), 0); //������Ϣ

	writing = 0; //������֮��д״̬����
}

//���������Ϣ���ݿո���зָ�
vector<string> client::split(char *str)
{
	vector<string> elems;
	const char *delim = " ";
	char *s = strtok(str, delim);
	while (s != NULL)
	{
		elems.push_back(s);
		s = strtok(NULL, delim);
	}
	return elems;
}

//�ж�������ַ����Ƿ�ȫ����������ɣ�����ǣ�����true�����򷵻�false
bool client::isNum(string str)
{
	stringstream sin(str);
	double d;
	char c;
	if (!(sin >> d))
		return false;
	if (sin >> c)
		return false;
	return true;
}