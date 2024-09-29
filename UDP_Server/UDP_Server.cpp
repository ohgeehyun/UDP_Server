#include <iostream>
#include <WinSock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>
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
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "start up 에러" << endl;
		return 0;
	}

	SOCKET serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	SOCKADDR_IN serverAddr; // IPv4
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1");<=구식
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(5252);

	if (::bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		HandleError("Bind");
		return 0;
	}

		while (true)
		{
			SOCKADDR_IN clientAddr;
			::memset(&clientAddr, 0, sizeof(clientAddr));
			int32_t addrLen = sizeof(clientAddr);

			char recvBuffer[1000];
			int32_t recvLen = ::recvfrom(serverSocket, recvBuffer, sizeof(recvBuffer), 0,
				(SOCKADDR*)&clientAddr, &addrLen);
			if (recvLen <= 0)
			{
				HandleError("RecvFrom");
				return 0;
			}

			cout << "Recv Data! data : " << recvBuffer << endl;
			cout << "Recv Data! Len  : " << recvLen << endl;

			int32_t errorCode = ::sendto(serverSocket, recvBuffer, recvLen, 0,
				(SOCKADDR*)&clientAddr, sizeof(clientAddr));

			if (errorCode == SOCKET_ERROR)
			{
				HandleError("SendTo");
				return 0;
			}
			cout << "Send Data! Len : " << recvLen << endl;
		}
		//윈속 종료
		::WSACleanup();
}