#include "NetIO.h"
#include "Log.h"


CNetIO::CNetIO()
{

}

CNetIO::~CNetIO()
{

}


int CNetIO::Send(const char* pBuf, int nLength)
{
	if(nullptr == pBuf || 0 == nLength)
	{
		return 0;
	}

	int nSended  = 0;
	int nSendLen = 0;
	while(true)
	{
		nSendLen = CSocket::Send(pBuf + nSended, nLength);	
		if(nSendLen == -1)
		{
			if(errno == EAGAIN)
			{
				//对于非阻塞(NonBlocking)的socket而言,数据已经全部发送成功
				break;
			}
			else if(errno == ECONNRESET)
			{
				LogWarn("CounterPart Send out RST.");
				break;
			}
			else if(errno == EINTR)
			{
				continue;
			}
			else 
			{
				char szTmp[32] = { 0 };
				int  nPort = 0;
				//TODO 获取Socket IP：Port
				LogWarn("%s(%d) Socket Error: %d, IP:Port = %s:%d", __FILE__, __LINE__,errno, szTmp, nPort);
			}
		}
		else if(nSendLen == 0 )
		{
			//表示对端已经正常关闭 
			char szTmp[32] = { 0 };
			int  nPort = 0;
			//TODO 获取Socket IP:Port 
			LogInfo("client has closed. IP:Port = %s:%d", szTmp, nPort);
			//TODO 执行Socket 清理 步骤
			
			return 0;
		}
		else
		{
			nSended += nSendLen;//
			if(nSended == nLength)
			{
				break;
			}
			else 
			{
				continue;
			}
		}


	}
	return nSended;
}
	

int CNetIO::Recv(char* pBuf, int nLength)
{
	int nRecved = 0;
	int nRecvLen = 0;
	while(true)
	{
		nRecvLen = CSocket::Recv(pBuf + nRecved, nLength - nRecved);
		if(nRecvLen < 0)
		{
			if(errno == EAGAIN)
			{
				//由于非阻塞模式，所以当errno 为EAGIN时， 表示当前缓冲区已无数据可读
				//即该次数据读取已完成
				return nRecved;
			}
			else if(errno == ECONNRESET)
			{
				//对方发送了RST
				//TODO 关闭对端socket 
				LogInfo("Client Send out RST.");
				return nRecved;
			}
			else if(errno == EINTR)
			{
				//被信号中断
				continue;
			}
			else 
			{
				//出现其他不可弥补的错误
				char szTmp[32] = { 0 };
				int  nPort = 0;
				//TODO 获取Socket IP:Port 
				LogFatal("%s(%d) Socket occure fatal error. IP:Port = %s:%d", __FILE__, __LINE__, szTmp, nPort);
				//TODO 关闭套接字 并清理该event

			}
		}
		else 
		{
			nRecved += nRecvLen;
			if(nRecved < nLength)
			{
				continue;
			}
			else 
			{
				break;
			}
		}
		
	}

	return nRecved;
}
