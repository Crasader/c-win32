#include "stdafx.h"
#include "EasyQQSocket.h"
#include <WinSock2.h>
#include <Windows.h>
#include <exception>
#include <fcntl.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")


HANDLE g_hmutex = CreateMutex(NULL, FALSE, NULL);
HANDLE g_bufferMutex ;

CEasyQQSocket::CEasyQQSocket(SocketType type) :_type(type), _server(false), _socket(SOCKET_ERROR)
{
	WSAData wsaData;
	int err_no = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err_no != 0)
		throw SocketException("WSAStartup Error,Error Code:", err_no);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		throw SocketException("Socket Ver Error:", wsaData.wVersion);

	int sock_af = AF_INET, sock_type = SOCK_DGRAM, sock_pro = IPPROTO_UDP;
	if (_type == SOCKETTYPE_TCP)
	{
		sock_type = SOCK_STREAM;
		sock_pro = IPPROTO_TCP;
	}

	_socket = socket(sock_af, sock_type, sock_pro);
	if (_socket == INVALID_SOCKET)
	{
		WSACleanup();
		throw SocketException("Socket Error", WSAGetLastError());
	}
}

CEasyQQSocket::~CEasyQQSocket()
{
	closesocket(_socket);
	WSACleanup();
}

bool CEasyQQSocket::StartServe(unsigned port, unsigned int listen_num)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	//return false;
	return StartServe(addr, listen_num);
}

bool CEasyQQSocket::StartServe(sockaddr_in & addr, unsigned int listen_num)
{
	bool ret = false;
	do
	{
		if (bind(_socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
			break;
		if (_type == SOCKETTYPE_TCP)
			if (::listen(_socket, listen_num) == SOCKET_ERROR)
				break;
		ret = true;
		_server = true;
	} while (false);
	return ret;
}


bool CEasyQQSocket::Connect(const char * ip, unsigned int port)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ip);

	return Connect(addr);
}


bool CEasyQQSocket::Connect(sockaddr_in & addr)
{
	bool ret = false;
	if (_type == SOCKETTYPE_TCP)
	{
		if (::connect(_socket, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR)
			ret = true;
	}
	return ret;
}


void CEasyQQClient::ThreadRecv()
{
	m_sendThread = CreateThread(NULL, 0, ReceiveMessageThread, this, 0, NULL);

}

void CEasyQQClient::WaitForRecvMessage(LoginData& logindata, _Out_ std::vector<LoginData>::iterator& firstiter)
{

	WaitForSingleObject(m_recvEvent, INFINITE);
	EnterCriticalSection(&g_cs);
	if (m_vecAnotherClientInfo.empty()==false)
	{
		auto itfirst = m_vecAnotherClientInfo.begin();
		firstiter = itfirst;
		logindata = *itfirst;
		//m_vecAnotherClientInfo.erase(itfirst);
	}
	LeaveCriticalSection(&g_cs);
	ResetEvent(m_recvEvent);
}

size_t CEasyQQClient::send(void * QQData, size_t len) const throw(SocketException)
{
	size_t sendlen = 0;
	if ((sendlen = ::send(_socket, (char*)QQData, len, 0)) == SOCKET_ERROR)
		throw SocketException("Socket Error", WSAGetLastError());
	return sendlen;
}

size_t CEasyQQClient::receive(void* QQData, size_t len) const throw(SocketException)
{
	size_t recvlen = 0;
	if ((recvlen = ::recv(_socket, (char*)QQData, len, 0)) == SOCKET_ERROR)
		throw SocketException("Socket Error", WSAGetLastError());
	return recvlen;
}

void CEasyQQClient::EraseFirstRecvMsg(std::vector<LoginData>::iterator firstiter)
{
	m_vecAnotherClientInfo.erase(firstiter);
}

DWORD CEasyQQClient::ReceiveMessageThread(LPVOID IpParameter)
{
	CEasyQQClient* pThis = (CEasyQQClient*)IpParameter;
	while (true)
	{
		static LoginData info;
		::recv(pThis->_socket, (char*)&info, sizeof(LoginData), 0);
		WaitForSingleObject(pThis->m_bufferMutex, INFINITE);     // P（资源未被占用）    
		//printf("%s Says: %s", "Server", recvBuf);       // 接收信息  
		pThis->m_vecAnotherClientInfo.push_back(info);
		SetEvent(pThis->m_recvEvent);
		ReleaseSemaphore(pThis->m_bufferMutex, 1, NULL);     // V（资源占用完毕）
	}
	return 0;
}

//DWORD CEasyQQClient::SendMessageThread(LPVOID IpParameter)
//{
//	while (true)
//	{
//		string talk;
//		getline(cin, talk);
//		//WaitForSingleObject(bufferMute)
//	}
//	return 0;
//}


//CEasyQQServer::CEasyQQServer(unsigned short int port, const char * ip, int backlog) throw(SocketException)
//{
//}

CEasyQQServer::CEasyQQServer(unsigned short int port, const char * ip, SocketType type, int backlog) throw(SocketException) :CEasyQQSocket(type)
{
	CreateIOCP();
	CEasyQQSocket::StartServe(port, 10);
	InitializeCriticalSection(&g_cs);
}

CEasyQQServer::~CEasyQQServer()
{
	DeleteCriticalSection(&g_cs);
}

bool CEasyQQServer::Init()
{
	bool ret = false;
	m_hFile = CreateFile(L"AccountList.dat", GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFilePos, dwFileSize;
		dwFileSize = GetFileSize(m_hFile, NULL);
		do
		{
			AccountInfo info;
			DWORD dwRes;
			ReadFile(m_hFile, &info, sizeof(AccountInfo), &dwRes, NULL);
			m_vecAccountInfos.push_back(info);
			dwFilePos = SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
			SetFilePointer(m_hFile, dwFilePos, NULL, FILE_BEGIN);
		} while (dwFilePos < dwFileSize);
		ret = true;
	}
	CloseHandle(m_hFile);
	return ret;
}

bool CEasyQQServer::AppendAccount(LoginData& data)
{
	m_hFile = CreateFile(L"AccountList.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwRes, dwWritenSize;
		SetFilePointer(m_hFile, 0, NULL, FILE_END);
		if (data.type == EASYQQREGISTER)
		{
			AccountInfo info;
			wcscpy_s(info.user_account, data.user_account);
			wcscpy_s(info.user_code, data.user_code);
			info.loginstate.Islogin = LOGIN_NOTLOGIN;
			wcscpy_s(info.loginstate.user_account, data.user_account);
			for (int i = 0; i < 10; i++)
			{
				memset(info.loginstate.user_friendlist[i], 0, sizeof(info.loginstate.user_friendlist[i]));
			}
			WriteFile(m_hFile, &info, sizeof(AccountInfo), &dwRes, NULL);
			m_vecAccountInfos.push_back(info);
		}
	}
	CloseHandle(m_hFile);
	return true;
}

bool CEasyQQServer::ClientConnectLoop()
{
	HANDLE sendThread = CreateThread(NULL, 0, ServerSendThread, this, 0, NULL);
	while (true)
	{
		PER_HANDLE_DATA * PerHandleData = NULL;
		SOCKADDR_IN saRemote;
		int RemoteLen;
		SOCKET acceptSocket;

		RemoteLen = sizeof(saRemote);
		acceptSocket = accept(_socket, (SOCKADDR*)&saRemote, &RemoteLen);
		if (SOCKET_ERROR == acceptSocket)
			throw SocketException("Accept Socket Error: ", GetLastError());

		PerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA));
		PerHandleData->socket = acceptSocket;
		memcpy(&PerHandleData->ClientAddr, &saRemote, RemoteLen);
		clientGroup.push_back(PerHandleData);

		CreateIoCompletionPort((HANDLE)(PerHandleData->socket), m_completionPort, (DWORD)PerHandleData, 0);

		LPPER_IO_OPERATION_DATA PerIoData = NULL;
		PerIoData = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATEION_DATA));
		ZeroMemory(&(PerIoData->overlapped), sizeof(OVERLAPPED));
		PerIoData->databuff.len = 1024;
		PerIoData->databuff.buf = PerIoData->buffer;
		PerIoData->operationType = 0;
		
		DWORD RecvBytes;
		DWORD Flags = 0;
		WSARecv(PerHandleData->socket, &(PerIoData->databuff), 1, &RecvBytes, &Flags, &(PerIoData->overlapped), NULL);
	}
	CloseHandle(sendThread);
	return 0;
}

void CEasyQQServer::ReNewFile()
{
	m_hFile = CreateFile(L"AccountList.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		BOOL Islogin=FALSE;
		for (auto &Info : m_vecAccountInfos)
		{
			if (Info.loginstate.Islogin==LOGIN_SUCCESS)
			{
				Info.loginstate.Islogin = LOGIN_NOTLOGIN;
				Islogin = TRUE;
			}
			DWORD dwRes;
			WriteFile(m_hFile, &Info, sizeof(AccountInfo), &dwRes, NULL);
			if (Islogin==TRUE)
			{
				Info.loginstate.Islogin = LOGIN_SUCCESS;
				Islogin = FALSE;
			}
		}
	}
	CloseHandle(m_hFile);
}

bool CEasyQQServer::CreateIOCP()
{
	m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (m_completionPort == NULL)
		throw SocketException("CreateIoCompletionPort failed. Error:", GetLastError());
	SYSTEM_INFO mySysInfo;
	GetSystemInfo(&mySysInfo);

	// 基于处理器的核心数量创建线程  
	for (DWORD i = 0; i < (mySysInfo.dwNumberOfProcessors * 2); ++i) {
		// 创建服务器工作器线程，并将完成端口传递到该线程  
		static std::pair<HANDLE, CEasyQQServer*> CP_Server(m_completionPort, this);
		HANDLE ThreadHandle = CreateThread(NULL, 0, ServerWorkThread, &CP_Server, 0, NULL);
		if (NULL == ThreadHandle)
			throw SocketException("Create Thread Handle failed. Error:", GetLastError());
		CloseHandle(ThreadHandle);
	}

	return true;
}

bool CEasyQQServer::AcceptServe()
{
	if (_server&&_type == SOCKETTYPE_TCP)
	{
		m_vecSocket.push_back(::accept(_socket, nullptr, nullptr));
		return true;
	}
	return false;
}

bool CEasyQQServer::RecvData()
{
	bool ret = false;
	if (!m_vecSocket.empty())
	{
		for (auto Socket : m_vecSocket)
		{
			LoginData logindata;
			if (::recv(Socket, (char*)&logindata, sizeof(LoginData), 0) == SOCKET_ERROR)
				throw SocketException("RECV ERROR", WSAGetLastError());
			m_vecLoginData.push_back(logindata);
		}
		ret = true;
	}

	return ret;
}

bool CEasyQQServer::SendData(LPPER_HANDLE_DATA socketdata, LoginData& data)
{
	bool ret = false;
	switch (data.type)
	{
	case EASYQQLOGIN:
	{
		ISLOGIN info;
		if(IsAccount(data, info))
			ret = true;
		::send(socketdata->socket, (char*)&info, sizeof(info), NULL);
	}
	break;
	case EASYQQCHAT:
	{
		//接收到之后，LoginData数据中有AddFriend，
		//1、首先要从AddFriend中搜索用户的Socket还有一个用户的状态，
			//如果用户状态不在线则发送用户不在线
			//如果用户在线则通过这个Socket发送

		LoginData ActiveSenddata,PassiveSenddata;
		if (wcslen(data.Addfriend_account)!=0)
		{
			ActiveSenddata.type = EASYQQCHAT;
			if (IsAccountExit(data,PassiveSenddata.loginstate,ActiveSenddata.loginstate ))
			{
				if (ActiveSenddata.loginstate.Islogin== LOGIN_SUCCESS)
				{
					auto tgtiter = m_MapclientsocketGroup.find(data.Addfriend_account);
					if (tgtiter!=m_MapclientsocketGroup.end())
					{
						wcscpy_s(ActiveSenddata.Message, data.Message);
						wcscpy_s(ActiveSenddata.Addfriend_account, data.loginstate.user_account);
						::send(tgtiter->second->socket, (char*)&ActiveSenddata, sizeof(ActiveSenddata),NULL);
					}
				}
				else
				{
					auto iter = m_MapclientsocketGroup.find(data.user_account);
					if (iter!=m_MapclientsocketGroup.end())
					{
						wcscpy_s(PassiveSenddata.Message, _T("该用户不在线未能及时发送消息"));
						::send(iter->second->socket, (char*)&PassiveSenddata, sizeof(PassiveSenddata),NULL);
					}
					
				}
			}
		}
		//2、接着发送给被发消息其他用户AddFriend消息
	}
		break;
	case EASYQQACCOUNTINFO:
	{
		LoginData info,sourceInfo;
		if (wcslen(data.Addfriend_account) != 0)
		{
			if (IsAccountExit(data, info.loginstate , sourceInfo.loginstate))
			{
				auto Tgtiter = m_MapclientsocketGroup.find(data.Addfriend_account);
				if (Tgtiter != m_MapclientsocketGroup.end())
				{
					if (sourceInfo.loginstate.Islogin== LOGIN_SUCCESS)
					{
						//传输发送给被添加好友的账号
						sourceInfo.type = EASYQQACCOUNTINFO;
						::send(Tgtiter->second->socket, (char*)&sourceInfo, sizeof(sourceInfo), NULL);
					}
				}
				//传输发送给发送好友的账号
				::send(socketdata->socket, (char*)&info, sizeof(info), NULL);
			}
			else
			{
				//info.Islogin = LOGIN_NOTLOGIN;
				::send(socketdata->socket, (char*)&info, sizeof(info), NULL);
			}
		}
	}
		break;
	case EASYQQADDFRIENDS:
	{
		for (auto &Info:m_vecAccountInfos)
		{
			if (wcscmp(Info.user_account,data.loginstate.user_account)==0)
			{
				//更新数据
				Info.loginstate = data.loginstate;
			}
			if (wcscmp(Info.user_account,data.Addfriend_account)==0)
			{
				for (int i = 0; i < sizeof(Info.loginstate.user_friendlist) / sizeof(Info.loginstate.user_friendlist[0]); i++)
				{
					if (wcslen(Info.loginstate.user_friendlist[i]) == 0)
					{
						//添加好友
						wcscpy_s(Info.loginstate.user_friendlist[i], data.loginstate.user_account);
						break;
					}
				}
				LoginData AddInfo;
				AddInfo.loginstate = Info.loginstate;
				AddInfo.type = EASYQQADDFRIENDS;
				auto Tgtiter = m_MapclientsocketGroup.find(data.Addfriend_account);
				if (Tgtiter != m_MapclientsocketGroup.end())
				{
					//传输发送给被添加好友的账号
					::send(Tgtiter->second->socket, (char*)&AddInfo, sizeof(AddInfo), NULL);
				}
			}
		}
		ReNewFile();
	}
		break;
	case EASYQQREGISTER:
	{
		AppendAccount(data);
	}
		break;
	case EASYQQNULL:
		break;
	default:
		break;
	}
	return ret;
}

bool CEasyQQServer::IsAccountExit(LoginData & data, ISLOGIN & tgtinfo, ISLOGIN &sourceinfo)
{
	bool ret = false;
	for (auto & info : m_vecAccountInfos)
	{
		if (wcscmp(info.user_account, data.Addfriend_account)==0)
		{
			if (info.loginstate.Islogin == LOGIN_SUCCESS)
			{
				auto Tgtiter = m_MapclientsocketGroup.find(info.user_account);
				if (Tgtiter != m_MapclientsocketGroup.end())
				{
					tgtinfo = info.loginstate;
					ret = true;
				}
				break;
			}
			else
			{
				ret = false;
				break;
			}
		}
	}
	for (auto& info : m_vecAccountInfos)
	{
		if (wcscmp(info.user_account,data.user_account)==0)
		{
			if (info.loginstate.Islogin==LOGIN_SUCCESS)
			{
				auto Tgtiter = m_MapclientsocketGroup.find(info.user_account);
				if (Tgtiter != m_MapclientsocketGroup.end())
				{
					sourceinfo = info.loginstate;
					ret = true;
				}
				break;
			}
			else
			{
				ret = false;
			}
		}
	}
	return ret;
}
	


bool CEasyQQServer::IsAccount(LoginData & data, ISLOGIN & logininfo)
{
	bool ret = false;
	for (auto &info : m_vecAccountInfos)
	{
		if (wcscmp(info.user_account, data.user_account) == 0)
		{
			if (wcscmp(info.user_code, data.user_code) == 0)
			{
				if (info.loginstate.Islogin == LOGIN_NOTLOGIN)
				{
					info.loginstate.Islogin = LOGIN_SUCCESS;
					logininfo = info.loginstate;
					ret = true;
					break;
				}
				if (info.loginstate.Islogin == LOGIN_SUCCESS)
				{
					logininfo = info.loginstate;
					logininfo.Islogin = LOGIN_ERR_HASLOGIN;
					break;
				}
			}
			else
			{
				logininfo = info.loginstate;
				logininfo.Islogin = LOGIN_ERR_WRONGPASSWORD;
				break;
			}
		}

	}

	return ret;
}

bool CEasyQQServer::CloseSocket()
{
	for (auto newsocket : m_vecSocket)
	{
		closesocket(newsocket);
	}
	return false;
}

DWORD CEasyQQServer::ServerWorkThread(LPVOID IpParam)
{
	std::pair<HANDLE, CEasyQQServer*>* CP_server = (std::pair<HANDLE, CEasyQQServer*>*)IpParam;
	HANDLE CompletionPort = (HANDLE)CP_server->first;
	CEasyQQServer* pThis = CP_server->second;
	DWORD BytesTransferred;
	LPOVERLAPPED IpOverlapped = NULL;
	LPPER_HANDLE_DATA PerHandleData = NULL;
	LPPER_IO_DATA PerIoData = NULL;
	DWORD RecvBytes;
	DWORD Flags = 0;
	BOOL bRet = false;

	while (true)
	{
		bRet = GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, (PULONG_PTR)&PerHandleData, (LPOVERLAPPED*)&IpOverlapped, INFINITE);
		if (bRet == 0)
			/*throw SocketException("GetQueuedCompletionStatus Error: ", GetLastError())*/
		{
			std::cerr << "GetQueuedCompletionStatus Error:" << GetLastError() << std::endl;
			//return -1;
		}

		PerIoData = (LPPER_IO_DATA)CONTAINING_RECORD(IpOverlapped, PER_IO_DATA, overlapped);
		if (0 == BytesTransferred)
		{
			closesocket(PerHandleData->socket);
			GlobalFree(PerHandleData);
			GlobalFree(PerIoData);
			continue;
		}
		WaitForSingleObject(g_hmutex, INFINITE);
		LoginData recvdata;
		int i=sizeof(LoginData);
		recvdata = *(LoginData*)PerIoData->buffer;
		memcpy(&recvdata, PerIoData->buffer, strlen(PerIoData->buffer));
		EnterCriticalSection(&pThis->g_cs);
		if (recvdata.type<= EASYQQNULL&&recvdata.type>= EASYQQLOGIN)
		{
			for (auto client : pThis->clientGroup)
			{
				if (client->socket == PerHandleData->socket)
				{
					pThis->m_MapTgtWorkclient.insert(std::pair<LPPER_HANDLE_DATA, LoginData>(client, recvdata));
				}
			}
		}
	
		LeaveCriticalSection(&pThis->g_cs);
		ReleaseMutex(g_hmutex);

		ZeroMemory(&(PerIoData)->overlapped, sizeof(OVERLAPPED));
		PerIoData->databuff.len = 1024;
		PerIoData->databuff.buf = PerIoData->buffer;
		PerIoData->operationType = 0;
		WSARecv(PerHandleData->socket, &(PerIoData->databuff), 1, &RecvBytes, &Flags, &(PerIoData->overlapped), NULL);

	}
	return 0;
}

DWORD CEasyQQServer::ServerSendThread(LPVOID IpParam)
{
	CEasyQQServer* pThis = (CEasyQQServer*)IpParam;
	while (true)
	{
		EnterCriticalSection(&pThis->g_cs);
		if (!pThis->m_MapTgtWorkclient.empty())
		{
			for (auto client : pThis->m_MapTgtWorkclient)
			{
				bool Islogin;
				std::wstring clientname = client.second.user_account;
				Islogin= pThis->SendData(client.first, client.second);
				auto  it= pThis->m_MapclientsocketGroup.find(client.second.user_account);
				if (it == pThis->m_MapclientsocketGroup.end()
					&&Islogin==true)
				{
					pThis->m_MapclientsocketGroup.insert(std::pair<std::wstring, LPPER_HANDLE_DATA>
						(clientname, client.first));
				}
				for (auto iter : pThis->m_MapclientsocketGroup)
				{
					wprintf_s(L"%ws\n", iter.first.c_str());
				}
			}
			pThis->m_MapTgtWorkclient.clear();
		}
		LeaveCriticalSection(&pThis->g_cs);
		WaitForSingleObject(g_hmutex, INFINITE);
		ReleaseMutex(g_hmutex);
	}
	return 0;
}




//CEasyQQClient CEasyQQServer::accept() const throw(SocketException)
//{
//	return CEasyQQClient();
//}
