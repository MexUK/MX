#include "URL.h"
#include "Static/String.h"

using namespace std;
using namespace mx;

URL::URL(void) :
	m_usPort(0)
{
}

void						URL::loadFromURL(string& strURLRef)
{
	string strURL = strURLRef;
	string str;

	// protocol
	size_t uiPosition = strURL.find("://");
	if(uiPosition != string::npos)
	{
		str = strURL.substr(0, uiPosition);
		setProtocol(str);
		strURL = strURL.substr(uiPosition + 3);
	}
	else
	{
		str = "http";
		setProtocol(str);
	}
	
	// fragment
	uiPosition = strURL.find("#");
	if(uiPosition != string::npos)
	{
		str = strURL.substr(uiPosition);
		setFragment(str);
		strURL = strURL.substr(0, uiPosition);
	}
	else
	{
		str = "";
		setFragment(str);
	}
	
	// query
	uiPosition = strURL.find("?");
	if(uiPosition != string::npos)
	{
		str = strURL.substr(uiPosition);
		setQuery(str);
		strURL = strURL.substr(0, uiPosition);
	}
	else
	{
		str = "";
		setFragment(str);
	}
	
	// path
	uiPosition = strURL.find("/");
	if(uiPosition != string::npos)
	{
		str = strURL.substr(uiPosition);
		setPath(str);
		strURL = strURL.substr(0, uiPosition);
	}
	else
	{
		str = "/";
		setPath(str);
	}
	
	// port
	uiPosition = strURL.find(":");
	if(uiPosition != string::npos)
	{
		setPort(atoi(strURL.substr(uiPosition).c_str()));
		strURL = strURL.substr(0, uiPosition);
	}
	else
	{
		setPort(80);
	}
	
	// domains
	uiPosition = strURL.find("/");
	if(uiPosition != string::npos)
	{
		str = strURL.substr(0, uiPosition);
		setDomains(str);
		strURL = strURL.substr(uiPosition + 1);
	}
	else
	{
		setDomains(strURL);
		strURL = "";
	}
}

vector<string>	URL::getHTTPHeaders(void)
{
	vector<string> vecHeaders;
	vecHeaders.push_back("GET " + getPath() + " HTTP/1.1");
	vecHeaders.push_back("Host: " + getDomains());
	return vecHeaders;
}

URLComponents	URL::getURLComponents(string& strURLRef)
{
	string strURL = strURLRef;
	URLComponents url;
	size_t uiPos;

	// protocol
	uiPos = strURL.find_first_of("://");
	if (uiPos == string::npos)
	{
		url.m_strProtocol = "http";
	}
	else
	{
		url.m_strProtocol = strURL.substr(0, uiPos);
		strURL = strURL.substr(uiPos + 3);
	}

	// hash
	uiPos = strURL.find_last_of("#");
	if (uiPos == string::npos)
	{
		url.m_strHash = "";
	}
	else
	{
		url.m_strHash = strURL.substr(uiPos + 1);
		strURL = strURL.substr(0, uiPos);
	}

	// query
	uiPos = strURL.find_last_of("?");
	if (uiPos == string::npos)
	{
		url.m_strQuery = "";
	}
	else
	{
		url.m_strQuery = strURL.substr(uiPos + 1);
		strURL = strURL.substr(0, uiPos);
	}

	// path
	uiPos = strURL.find_first_of("/");
	if (uiPos == string::npos)
	{
		url.m_strPath = "/";
	}
	else
	{
		url.m_strPath = strURL.substr(uiPos);
		strURL = strURL.substr(0, uiPos);
	}

	// port
	uiPos = strURL.find_first_of(":");
	if (uiPos == string::npos)
	{
		url.m_usPort = 80;
	}
	else
	{
		string str = strURL.substr(uiPos + 1);
		url.m_usPort = String::toNumber(str);
		strURL = strURL.substr(0, uiPos);
	}

	// domains
	url.m_strDomains = strURL;

	return url;
}