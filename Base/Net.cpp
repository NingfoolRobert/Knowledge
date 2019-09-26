#include "Net.h"

CNet::CNet()
{
	m_sock = nullptr;
	m_ctx = zmq_ctx_new();
	assert(m_ctx);
}

CNet::~CNet()
{

}

void* CNet::Create(int nType)
{
	m_nType = nType;
	m_sock = zmq_socket(m_ctx, m_nType);
	
	return m_sock;
	
}

bool CNet::Bind (const char* pszHostAddr, int nPort/* = 0*/) //IP   无法做域名解析 注意pszHostAddr 使用
{

	return true;
}

bool CNet::Connect(const char* pszHostAddr, int nPort/* = 0*/) //IP:nPort 
{
	return true;
}

bool CNet::Send(const char* pszBuffer, int nLength)
{
	return true;
}

bool CNet::Recv(char* pszBuffer, int nLength)
{
	return true;
}

