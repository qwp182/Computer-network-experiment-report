#include"public.h"
#include"client.h"
client myClient;

bool login(std::string username, std::string password, std::string seqNum) {
	//�������������Ϣ��¼
	myClient.sendToServer("login " + username + " " + password + " " + seqNum);
	std::string reply = myClient.receieveFromServer();
	if (reply == "permit")
		return true;
	else return false;
}

void check() {
	//ÿ����һ��ʱ��Է�������һ��check
}

int main()
{
	std::string username, password, seqNum;
	printf("�밴�գ�[�û���] [����] [���к�]�ĸ�ʽ����\n");
	std::cin >> username >> password >> seqNum;
	while (!login(username, password, seqNum)) {
		printf("��½ʧ�ܣ�����������\n���գ�[�û���] [����] [���к�]�ĸ�ʽ����\n");
		std::cin >> username >> password >> seqNum;
	}
	printf("��¼�ɹ�\n");
	//std::thread check_thread(check);
	while (true) {
		printf("����quit�˳�\n");
		std::string INS;
		std::cin >> INS;
		if (INS == "quit") {
			myClient.sendToServer(INS);
			break;
		}
	}

	system("pause");
	return 0;
}