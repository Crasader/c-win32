#pragma once
#include <exception>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <Windows.h>
#include <exception>
#include "EasyQQData.h"
#include <map>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "Kernel32.lib") 

/**
* 结构体名称：PER_IO_DATA
* 结构体功能：重叠I/O需要用到的结构体，临时记录IO数据
**/
const int DataBuffSize = 2 * 1024;
typedef struct
{
	OVERLAPPED overlapped;
	WSABUF databuff;
	char buffer[DataBuffSize];
	int BufferLen;
	int operationType;
}PER_IO_OPERATEION_DATA, *LPPER_IO_OPERATION_DATA, *LPPER_IO_DATA, PER_IO_DATA;


/**
* 结构体名称：PER_HANDLE_DATA
* 结构体存储：记录单个套接字的数据，包括了套接字的变量及套接字的对应的客户端的地址。
* 结构体作用：当服务器连接上客户端时，信息存储到该结构体中，知道客户端的地址以便于回访。
**/
typedef struct
{
	SOCKET socket;
	SOCKADDR_STORAGE ClientAddr;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;


const int DefaultPort = 10086;


class SocketException :std::exception
{
public:
	SocketException(const char* msg, const int error_code) :std::exception()
	{
		std::string temp = msg;
		temp += std::to_string(error_code);
		size_t len = temp.length() + sizeof(char);
		_msg = new char[len];
		strcpy_s(_msg, len, temp.c_str());
	}
	~SocketException()
	{
		delete[]_msg;
	}
	SocketException(const SocketException& other)
	{
		CopyData(other._msg, other._len);
	}
	SocketException& operator=(const SocketException& other)
	{
		if (this == &other)
			return *this;
		CopyData(other._msg, other._len);
	}

	virtual char const* what()const
	{
		return _msg;
	}
private:
	void CopyData(const char* data, size_t len)
	{
		_msg = new char[_len];
		strcpy_s(_msg, _len, data);
	}
	char* _msg;
	size_t _len;
};
class CEasyQQSocket
{
public:
	enum SocketType
	{
		SOCKETTYPE_UDP,
		SOCKETTYPE_TCP
	};
public:
	CEasyQQSocket(SocketType type)throw(SocketException);
	virtual ~CEasyQQSocket();
	bool StartServe(unsigned port, unsigned int listen_num)throw();
	bool StartServe(sockaddr_in& addr, unsigned int listen_num);

	bool Connect(const char* ip, unsigned int port);
	bool Connect(sockaddr_in& addr);

protected:
	SOCKET _socket;
	SocketType _type;
	bool _server;
};

class CEasyQQClient :public CEasyQQSocket
{
public:
	CEasyQQClient(SocketType type) :CEasyQQSocket(type)
	{
		m_bufferMutex = CreateSemaphore(NULL, 1, 1, NULL);
		m_recvEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
		InitializeCriticalSection(&g_cs);
	}
	~CEasyQQClient()
	{
		DeleteCriticalSection(&g_cs);
		CloseHandle(m_bufferMutex);
		CloseHandle(m_recvEvent);
		CloseHandle(m_sendThread);
	}
	//void Set
	//void ThreadSend(LPVOID IpParameter);
	void ThreadRecv();
	void WaitForRecvMessage(_Out_ LoginData& logindata,_Out_ std::vector<LoginData>::iterator& firstiter);
	size_t send(void* message, size_t len) const throw(SocketException);
	size_t receive(void* QQData, size_t len) const throw(SocketException);
	void EraseFirstRecvMsg(std::vector<LoginData>::iterator firstiter);
private:
	//static DWORD WINAPI SendMessageThread(LPVOID IpParameter);
	static DWORD WINAPI ReceiveMessageThread(LPVOID IpParameter);
	CRITICAL_SECTION g_cs;
	std::vector<ISLOGIN> m_vecIslogin;
	HANDLE m_sendThread;
	HANDLE m_recvEvent;
	HANDLE m_bufferMutex;
	std::vector<LoginData> m_vecAnotherClientInfo;
};

class CEasyQQServer : public CEasyQQSocket
{
public:
	CEasyQQServer(unsigned short int port, const char *ip = NULL, SocketType type = SOCKETTYPE_TCP, int backlog = SOMAXCONN) throw(SocketException);
	~CEasyQQServer();
	bool Init();
	bool AppendAccount(LoginData& data);

	bool ClientConnectLoop();

	bool AcceptServe();
	bool RecvData();
	bool SendData(LPPER_HANDLE_DATA socketdata, LoginData& data);
	bool IsAccountExit(_In_ LoginData& data,_Out_ ISLOGIN & tgtinfo, _Out_ ISLOGIN &sourceinfo);
	bool IsAccount(_In_ LoginData& data, _Out_ ISLOGIN & logininfo);
	bool CloseSocket();
private:
	HANDLE m_hFile;
	HANDLE m_completionPort;


protected:
	std::vector<SOCKET> m_vecSocket;
	std::vector<LoginData> m_vecLoginData;
	std::vector<AccountInfo> m_vecAccountInfos;
	std::map<LPPER_HANDLE_DATA,LoginData> m_MapTgtWorkclient;
	std::map<std::wstring, LPPER_HANDLE_DATA> m_MapclientsocketGroup;
	CRITICAL_SECTION g_cs;
private:
	void ReNewFile();
	bool CreateIOCP();
	std::vector < PER_HANDLE_DATA* > clientGroup;
	static DWORD WINAPI ServerWorkThread(LPVOID IpParam);
	static DWORD WINAPI ServerSendThread(LPVOID IpParam);


	//CEasyQQClient accept() const throw(SocketException);
};
