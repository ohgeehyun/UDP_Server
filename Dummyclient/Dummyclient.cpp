#include <iostream>
#include <WinSock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <thread>
#pragma comment(lib,"ws2_32.lib")

using namespace std;


void HandleError(const char* cause)
{
	int32_t errCode = ::WSAGetLastError();
	cout << "ErrorCode : " << errCode << endl;
}


int main()
{


	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		cout << "start up 에러" << endl;


	SOCKET clientSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}


	SOCKADDR_IN serverAddr; // IPv4
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(5252);


	//Connected UDP 
	//Tcp처럼 CONNECT한다는 뜻이 아닌 소켓에 즐겨찾기처럼 보낼장소를 등록해둘수있음.
	//UDP에서 connect시 sendto 가아닌 send로 보내야함.  
	::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));


	while (true)
	{
		//TODO 
		char sendBuffer[100] = "Hello World!";


		//UDP는 나의 IP주소 + 포트 번호가 sendto할떄 자동으로 설정된다

		//Unconnected UDP
		/*int32_t resultCode = ::sendto(clientSocket, sendBuffer, sizeof(sendBuffer), 0,
			(SOCKADDR*)&serverAddr,sizeof(serverAddr));*/

			//Counnected UDP
		int32_t resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);


		if (resultCode == SOCKET_ERROR)
		{
			HandleError("SendTo");
		}
		cout << "Send Data! Len = " << sizeof(sendBuffer) << endl;


		SOCKADDR_IN recvAddr; // IPv4
		::memset(&recvAddr, 0, sizeof(recvAddr));
		int32_t addrLen = sizeof(recvAddr);

		char recvBuffer[1000];

		//Unconnected UDP
		/*int32_t  recvLen = ::recvfrom(clientSocket, recvBuffer, sizeof(recvBuffer), 0,
			(SOCKADDR*)&recvAddr,&addrLen);*/

			//connected UDP
		int32_t  recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);

		if (recvLen <= 0)
		{
			HandleError("RecvFrom");
			return 0;
		}
		cout << "Recv Dadta! Len = " << recvBuffer << endl;
		cout << "Recv Dadta! Len = " << recvLen << endl;

		this_thread::sleep_for(1s);
	}



	::closesocket(clientSocket);
	::WSACleanup();
}
