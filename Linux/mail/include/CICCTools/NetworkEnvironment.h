#pragma once



#pragma warning(disable:4251)

#include <string>
#include <atomic>
using namespace std;

namespace CICCTools {
	class  NetworkEnvironment
	{
	private:
		NetworkEnvironment();
		~NetworkEnvironment();

	public:
		static string& GetIp();
		static string& GetMac();

	private: 
		static string m_strIp;
		static string m_strMac;
		static atomic<int> m_lock;
	};
}

