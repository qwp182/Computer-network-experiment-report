#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "public.h"

int main()
{
	server ser;
	ser.process();

	return 0;
}

//���캯�������÷������Ϣ
server::server()
{
	listener = 0;
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = SERVER_PORT;
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP); //���ַ�������ת��uint32_t
}

//��ʼ�����������ܴ��������׽��֣��󶨶˿ڣ������м���
void server::init()
{
	int Ret;
	WSADATA wsaData; // ���ڳ�ʼ���׽��ֻ���

	//��ʼ��WinSock����
	//ʧ��ʱ
	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		printf("WSAStartup()   failed   with   error   %d\n", Ret);
		WSACleanup();
	}

	//��ʼ��WinSock�����ɹ�
	listener = socket(AF_INET, SOCK_STREAM, 0); //����ipv4,TCP����
	if (listener == -1)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		perror("����ʧ��");
		exit(1);
	}
	cout << "�����ɹ�" << endl;

	unsigned long ul = 1;
	if (ioctlsocket(listener, FIONBIO, (unsigned long *)&ul) == -1)
	{
		perror("ioctl failed");
		exit(1);
	}

	if (bind(listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("bind error");
		exit(1);
	}
	if (listen(listener, 6) < 0)
	{
		perror("listen failed");
		exit(1);
	}
	socnum.push_back(listener); //�������׽��ּ���
}

void server::process()
{
	int mount = 0;
	fd_set fds;
	FD_ZERO(&fds); //��fds����

	//��ʼ���������˽�������
	init();

	cout << "���ڵȴ��ͻ�����Ϣ......" << endl;
	while (1)
	{
		mount = socnum.size();
		//��fdsÿ�ζ����¸�ֵ
		for (int i = 0; i < mount; i++)
		{
			FD_SET(socnum[i], &fds);
		}

		struct timeval timeout = {1, 0}; //����ÿ��1��selectһ��
		//�����ֻ���ö���������д
		switch (select(0, &fds, NULL, NULL, &timeout))
		{
		//ʧ��
		case -1:
		{
			perror("select\n");
			printf("Error at socket(): %ld\n", WSAGetLastError());
			printf("%d\n", mount);

			Sleep(1000);
			break;
		}
		//��ʱ
		case 0:
		{
			break;
		}
		//�ɹ�
		default:
		{
			//�������е�ÿһ���׽��ֶ���ʣ����׽��ֽ��бȽϵõ���ǰ������
			for (int i = 0; i < mount; i++)
			{
				//�����һ�����׽��ֿɶ�����Ϣ���ͽ�����������
				if (i == 0 && FD_ISSET(socnum[i], &fds))
				{
					struct sockaddr_in client_address;
					socklen_t client_addrLength = sizeof(struct sockaddr_in);
					//����һ���û����׽���
					int clientfd = accept(listener, (struct sockaddr *)&client_address, &client_addrLength);
					//����û�������������ʾ��Ϣ����֪ͨ�û����ӳɹ�
					socnum.push_back(clientfd);
					cout << "�ͻ��� " << clientfd << "�ɹ����ӱ�������" << endl;
					char ID[1024];
					sprintf(ID, "��ã����IDΪ��%d", clientfd);

					//����������ID�����͸��ͻ����ÿͻ���֪���Լ���ID
					send(clientfd, ID, sizeof(ID) - 1, 0); //��ȥ���һ��'/0'
				}
				if (i != 0 && FD_ISSET(socnum[i], &fds))
				{
					char buf[1024];
					memset(buf, '\0', sizeof(buf));
					int size = recv(socnum[i], buf, sizeof(buf) - 1, 0);
					//����Ƿ����
					if (size == 0 || size == -1)
					{
						cout << "�ͻ��� " << socnum[i] << "�ѵ���" << endl;

						closesocket(socnum[i]);	 //�ر�����׽���
						FD_CLR(socnum[i], &fds); //���б��б���ɾ��

						socnum.erase(socnum.begin() + i); //��vector������ɾ��
						mount--;
					}
					//��û�е���
					else
					{
						printf("�ͻ��� %d ˵: %s \n", socnum[i], buf);

						//���û����͹�������Ϣ���зָ��ȡ�������кţ����û���������
						vector<string> info = split(buf);
						string str;
						if (info.size() == 2)
						{
							info[0] = "�û�����" + info[0];
							info[1] = "���룺" + info[1];
							str = info[0] + " " + info[1];
						}
						else
						{
							info[0] = "���кţ�" + info[0];
							info[1] = "�û�����" + info[1];
							info[2] = "���룺" + info[2];
							str = info[0] + " " + info[1] + " " + info[2];
						}

						/*
						��ȡ����������������Ϣ�󣬽���У�飬У��ķ���Ӧ�ü�������
						*/

						//����Ϣ���͸�������Ϣ�Ŀͻ���
						char client[1024];
						sprintf(client, "�ͻ��� %d��", socnum[i]);
						strcat(client, str.c_str());

						send(socnum[i], client, sizeof(client) - 1, 0); //���
					}
				}
			}
			break;
		}
		}
	}
}

//���������Ϣ���ݿո���зָ�
vector<string> server::split(char *str)
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